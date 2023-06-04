#pragma once
#include <Arduino.h>
#include "rsa/RSA.h"
#include "../utils/HexConverter.hpp"

class RSAAdatper {
protected:
    std::map<uint32_t, std::string> rsa_pub_keys;
    void generate_keys() {
        bool generate = rsa_generate_keys(public_key, private_key);
        while (!generate) {
            generate = rsa_generate_keys(public_key, private_key);
            yield();
        }
    }
    uint8_t tmp_pub_key[RSA_SIZE];
    uint8_t public_key[RSA_SIZE];
    uint8_t private_key[RSA_SIZE];

    void clear_buffer(char* buffer, int len) {
        for(int i = 0; i < len; i++) {
            buffer[i] = 0;
        }
    }

    string fill(string res, char* buffer, int len) {
        for(int i = 0; i < len; i++) {
            res.push_back(buffer[i]);
        }
        return res;
    }

    string pad_data(string data, uint8_t len) {
        uint8_t p = data.length()%len;
        if(p) {
            for(int i = 0; i < len - p; i++) {
                data += " ";
            }
        }
        return data;
    }
    string trimString(string str) {
        const string whiteSpaces = " \t\n\r\f\v";
        size_t first_non_space = str.find_first_not_of(whiteSpaces);
        str.erase(0, first_non_space);
        size_t last_non_space = str.find_last_not_of(whiteSpaces);
        str.erase(last_non_space + 1);
    return str;
}

public:
    RSAAdatper() {
        this->generate_keys();
    }

    string get_target_pub_key(uint32_t target) {
        if (rsa_pub_keys.find(target) != rsa_pub_keys.end()) {
            return rsa_pub_keys[target];
        } else {
            return "";
        }
    }

    string set_target_pub_key(uint32_t target, string key) {
        return rsa_pub_keys[target] = key;
    }

    string get_public_key() {
         return HexConverter::toHex((const char*)this->public_key, RSA_SIZE);
    }

    string encrypt(string msg, uint8_t* pub_key) {
        msg = pad_data(msg, RSA_BLOCK_SIZE);
        string res;
        char c_buffer[RSA_SIZE];
        for(int i = 0; i < msg.length(); i+= RSA_BLOCK_SIZE) {
            string buffer = msg.substr(i, RSA_BLOCK_SIZE);
            this->encrypt((char*)buffer.c_str(), c_buffer, pub_key);
            res += HexConverter::toHex(c_buffer, sizeof(c_buffer));
        }
        return res;
    }

    string encrypt_for_target(string msg, uint32_t target) {
        string key = get_target_pub_key(target);
        if (!key.empty()) {
            return encrypt(msg, key);
        } else {
            return " ";
        }
    }

    string encrypt(string msg) {
        return this->encrypt(msg, this->public_key);
    }

    string encrypt(string msg, string str_pub_key) {
        HexConverter::hexToCharArr(str_pub_key.c_str(), (char*)tmp_pub_key);
        return this->encrypt(msg, this->tmp_pub_key);
    }

    string decrypt(string msg) {
        // msg = HexConverter::hexToCharArr(msg);
        string res;
        char c_buffer[RSA_SIZE];
        for(size_t  i = 0; i < msg.length(); i+=(RSA_SIZE*2)) {
            // string buffer = msg.substr(i, i+(RSA_SIZE*2));
            HexConverter::hexToCharArr(msg.substr(i, (RSA_SIZE*2)).c_str(), c_buffer);
            this->decrypt(c_buffer, c_buffer);
            res = fill(res, c_buffer, RSA_BLOCK_SIZE);
            yield();
        }
        res = trimString(res);
        return res;
    }

    void encrypt(char* msg) {
        rsa_public_encrypt((uint8_t*)msg, RSA_BLOCK_SIZE, public_key, (uint8_t*)msg);
    }

    void encrypt(char* msg, char* res) {
        rsa_public_encrypt((uint8_t*)msg, RSA_BLOCK_SIZE, public_key, (uint8_t*)res);
    }

    void encrypt(char* msg, char* res, uint8_t* pub_key ) {
        rsa_public_encrypt((uint8_t*)msg, RSA_BLOCK_SIZE, pub_key, (uint8_t*)res);
    }
   
    void decrypt(char* msg) {
        rsa_private_decrypt((uint8_t*)msg, public_key, private_key, (uint8_t*)msg);
    }

    void decrypt(char* msg, char* res)  {
        rsa_private_decrypt((uint8_t*)msg, public_key, private_key, (uint8_t*)res);
    }
    
};