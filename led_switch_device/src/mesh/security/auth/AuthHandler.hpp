#pragma once
#include <string>
#include <map>
#include <list>
#include <set>
#include <utility>
#include <Arduino.h>
#include "painlessMesh.h"
#include "utils/Timer.h"
#include "utils/Generator.hpp"
#include "mesh/security/auth/ModularExp.hpp"

class AuthHandler {
protected:
    std::set<uint32_t> authBase;
    std::map<uint32_t, uint16_t> authKeys;
    std::map<uint32_t, unsigned long> authTimers;
    Timer timeOutChecker;

    void getKey(uint32_t target, std::pair<uint16_t, uint16_t>* p) {
        File file = SPIFFS.open("/auth_data.json", "r");
        StaticJsonDocument<1024> auth_data;
        std::string res;
        DeserializationError error = deserializeJson(auth_data, file);
        if(error) {Serial.println("error: "); Serial.println(error.f_str());} 
        for (JsonObject key_item : auth_data["keys"].as<JsonArray>()) {
            if(key_item["target"] == target) {
                *p = std::make_pair((uint16_t)key_item["degree"], (uint16_t)key_item["module"]);
                break;
            }
        }
    }

    bool isTimeOut(uint32_t target) {
        return (millis() > (AUTH_MILLIS_CONFIRM_TIME + authTimers[target]));
    }

    void deleteIfTimeOut() {
        std::list<uint32_t> targets_for_delete;
        for(const auto& kv : authTimers) {
            if(isTimeOut(kv.first)) {
                targets_for_delete.push_back(kv.first);
            }
        }
        while (!targets_for_delete.empty()) {
            uint32_t target = targets_for_delete.front();
            authKeys.erase(target);
            authTimers.erase(target);
            targets_for_delete.pop_front();
        }
    }

    void update_timer(uint32_t target) {
        this->authTimers[target] = millis();
    }

public:

    std::set<uint32_t> get_auth_devices() {
        return this->authBase;
    }

    boolean check(uint32_t target, std::string data, uint8_t repeat) {
        std::string key_with_gamma = this->addGammaThenHash(target, repeat);
        if(key_with_gamma == data) {
            this->update_timer(target);
            return true;
        }
        return false;
    }

    boolean check_and_try_auth(uint32_t target, std::string data, uint8_t repeat) {
        return check(target, data, repeat) ? this->auth(target) : false;
    }

    void startAuth(uint32_t target, std::string key) {
        authKeys[target] = (uint16_t)std::stol(key);
        authTimers[target] = millis();
    }

    std::string genKeyAndStartAuth(uint32_t target) {
        uint16_t key = generateUintPassword(millis());
        authKeys[target] = key;
        authTimers[target] = millis();
        return std::to_string(key);
    }

    bool isAuth(uint32_t target) {
        return authBase.find(target) != authBase.end();
    }

    bool auth(uint32_t target) {
        if(!isTimeOut(target)) {
            this->authBase.insert(target);
            return true;
        }
        return false;
    }

    void disable(uint32_t target) {
        this->authBase.erase(target);
        authKeys.erase(target);
        authTimers.erase(target);
    }

    std::string addGammaThenHash(uint32_t target, uint8_t repeat) {
        std::pair<uint16_t, uint16_t> p;
        this->getKey(target, &p);
        uint32_t aKey = authKeys[target];
        for(int i = 0; i < repeat; i++) {
            aKey = ModularExp::mod_exp(aKey, p.first, p.second);
        }
        std::string res = std::to_string(aKey).c_str();
        return std::string(sha1(res.c_str(), res.length()).c_str());
    }

    void update() {
        if(timeOutChecker == 0) {
            deleteIfTimeOut();
            timeOutChecker.start(AUTH_MILLIS_CHECKER_TIME);
        }
    }

};