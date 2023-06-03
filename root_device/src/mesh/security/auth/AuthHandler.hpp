#pragma once
#include <string>
#include <map>
#include <list>
#include <set>
#include <Arduino.h>
#include "painlessMesh.h"
#include "utils/Timer.h"
#include "utils/Generator.hpp"

class AuthHandler {
protected:
    std::set<uint32_t> authBase;
    std::map<uint32_t, std::string> authKeys;
    std::map<uint32_t, unsigned long> authTimers;
    Timer timeOutChecker;

    std::string getKey(uint32_t target) {
        File file = SPIFFS.open("/auth_data.json", "r");
        StaticJsonDocument<1024> auth_data;
        std::string res;
        DeserializationError error = deserializeJson(auth_data, file);
        if(error) {Serial.println(error.f_str()); return "";} 
        for (JsonObject key_item : auth_data["keys"].as<JsonArray>()) {
            if(key_item["target"] == target) {
                res = (std::string)key_item["password"];
                break;
            }
        }
        return res;
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

    std::string addGamma(uint32_t target, std::string key) {
        std::string public_data = this->authKeys[target];
        std::string with_gamma = "";
        int key_len = key.length();
        if(key_len != 0 && public_data.length() != key_len) {
            for(int i = 0; i < key_len; i++) {
                with_gamma.push_back(key.at(i) + public_data.at(i));
            }
        }
        return with_gamma;
    }

    void update_timer(uint32_t target) {
        this->authTimers[target] = millis();
    }

public:

    boolean check(uint32_t target, std::string data, uint8_t repeat) {
        std::string key_with_gamma = this->addGamma(target, repeat);
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
        authKeys[target] = key;
        authTimers[target] = millis();
    }

    std::string genKeyAndStartAuth(uint32_t target) {
        std::string key = generatePassword(AUTH_KEY_LEN);
        authKeys[target] = key;
        authTimers[target] = millis();
        return key;
    }

    bool isAuth(uint32_t target) {
        return authBase.find(target) != authBase.end();
    }

    bool auth(uint32_t target) {
        if(!isTimeOut(target)) {
            this->authBase.insert(target);
            authKeys.erase(target);
            authTimers.erase(target);
            return true;
        }
        return false;
    }

    void disable(uint32_t target) {
        this->authBase.erase(target);
        authKeys.erase(target);
        authTimers.erase(target);
    }

    std::string addGamma(uint32_t target) {
        return this->addGamma(target, this->getKey(target));
    }

    std::string addGamma(uint32_t target, uint8_t repeat) {
        std::string key = this->getKey(target);
        for(int i = 0; i < repeat; i++) {
            key = addGamma(target, key);
        }
        return key;
    }

    void update() {
        if(timeOutChecker == 0) {
            deleteIfTimeOut();
            timeOutChecker.start(AUTH_MILLIS_CHECKER_TIME);
        }
    }

};