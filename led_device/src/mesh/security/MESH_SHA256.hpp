#pragma once
#include <string>
#include <Arduino.h>
#include <SHA256.h>
#include "mesh/utils/HexConverter.hpp"

class MESH_SHA256 {
private:
    const static uint16_t _hash_len = 32; 
public:
    static std::string hashing(std::string msg) {
        uint8_t _hash[_hash_len];
        SHA256 _sha256;
        _sha256.reset();
        _sha256.update(msg.c_str(), msg.length());
        _sha256.finalize(_hash, _hash_len);
        return HexConverter::toHex((const char*)_hash, _hash_len);
    }
};