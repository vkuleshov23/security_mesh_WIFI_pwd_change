#pragma once
#include <Arduino.h>
#include <string>

class ICryptoAdapter {
protected:
    virtual void generate_keys();
public:
    virtual uint8_t* get_private_key();
    virtual uint8_t* get_public_key();
    virtual void encrypt(char* msg);
    virtual void decrypt(char* msg);
    virtual String encrypt(String msg);
    virtual String decrypt(String msg);
};