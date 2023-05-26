#pragma once
#include <Arduino.h>
#include "../interfaces/ICryptoAdapter.hpp"
#include "rsa/RSA.h"
#include "../utils/HexConverter.hpp"

class RSAAdatper : ICryptoAdapter {
protected:
    void generate_keys() override {}
    uint8_t public_key[RSA_SIZE] = {};
    uint8_t private_key[RSA_SIZE] = {};

public:
    RSAAdatper() {
        bool generate = rsa_generate_keys(public_key, private_key);
        while (!generate) {
            generate = rsa_generate_keys(public_key, private_key);
        }
    }

    uint8_t* get_private_key() override {
        return this->private_key;
    }

    uint8_t* get_public_key() override {
         return this->public_key;
    }

    static void print_data(const uint8_t *data, uint8_t size) {
        for (uint8_t i = 0; i < size; i++) {
            // Serial.printf("%02X ", data[i]);
            Serial.printf("%u ", data[i]);
        }
        Serial.println();
    }

    String encrypt(String msg) override {
        char buffer[RSA_SIZE] = {};
        char* c_msg = (char*)msg.c_str();
        int data_len = msg.length();
        uint8_t times = (data_len/RSA_BLOCK_SIZE);
        String res;
        for(uint8_t i = 0; i < times; i++) {
            int offset = (i * RSA_BLOCK_SIZE);
            for(uint8_t j = 0; j < RSA_BLOCK_SIZE; j++) {
                buffer[j] = c_msg[offset + j];
            }
            this->encrypt(buffer);
            res += HexConverter::toHex(buffer, RSA_SIZE);
        }
        for(uint16_t i = 0; i < data_len % RSA_SIZE; i++) {
            buffer[i] = c_msg[i];
        }
        for(uint16_t i = data_len % RSA_SIZE; i < RSA_SIZE; i++) {
            buffer[i] = 0;
        }
        this->encrypt(buffer);
        res += HexConverter::toHex(buffer, RSA_SIZE);
        return res;
    }

    String decrypt(String msg) override {
        char buffer[RSA_SIZE] = {};
        char write_data[RSA_SIZE] = {};
        int data_len = msg.length()/2;
        char c_msg[data_len] = {};
        HexConverter::hexToCharArr(msg.c_str(), c_msg);
        uint8_t times = (data_len/RSA_SIZE);
        String res;
        for(uint8_t i = 0; i < times; i++) {
            int offset = (i * RSA_SIZE);
            for(uint8_t j = 0; j < RSA_SIZE; j++) {
                buffer[j] = c_msg[offset + j];
            }
            this->decrypt(buffer);
            memcpy(write_data, buffer, RSA_BLOCK_SIZE);
            res += write_data;
        }
        return res;
    }

    void encrypt(char* msg) override {
        rsa_public_encrypt((uint8_t*)msg, RSA_BLOCK_SIZE, public_key, (uint8_t*)msg);
    }
   
    void decrypt(char* msg) override {
        rsa_private_decrypt((uint8_t*)msg, public_key, private_key, (uint8_t*)msg);
    }
    
};