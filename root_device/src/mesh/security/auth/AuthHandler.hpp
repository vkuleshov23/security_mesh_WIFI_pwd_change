#pragma once
#include <string>
#include <map>
#include <list>
#include <set>
#include <Arduino.h>
#include "painlessMesh.h"
#include "utils/Timer.h"
#include "utils/Generator.hpp"
#include "mesh/security/SHA1.hpp"

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
        if(error) {Serial.println("error: "); Serial.println(error.f_str()); return "";} 
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

    void addGamma(uint32_t target, std::string key, char* current_data) {
        int key_len = key.length();
        if(key_len != 0) {
            for(int i = 0; i < key_len; i++) {
                current_data[i] = key.at(i) + current_data[i];
            }
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
        authKeys[target] = key;
        authTimers[target] = millis();
    }

    std::string genKeyAndStartAuth(uint32_t target) {
        std::string key = generatePassword(AUTH_KEY_LEN, millis());
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
        std::string key = this->getKey(target);
        std::string aKey = authKeys[target];
        char* res = (char*)aKey.c_str();
        for(int i = 0; i < repeat; i++) {
            addGamma(target, key, res);
        }
        return std::string(sha1(res, aKey.length()).c_str());
    }

    void update() {
        if(timeOutChecker == 0) {
            deleteIfTimeOut();
            timeOutChecker.start(AUTH_MILLIS_CHECKER_TIME);
        }
    }

};