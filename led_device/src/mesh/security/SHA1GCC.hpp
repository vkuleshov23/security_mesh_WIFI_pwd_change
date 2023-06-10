#pragma once
#include <Hash.h>
#include <string>
#include <Arduino.h>

class SHA1GCC {
public:
    static std::string hash(std::string msg) {
        return (std::string)sha1((String)msg.c_str()).c_str();
    }
};