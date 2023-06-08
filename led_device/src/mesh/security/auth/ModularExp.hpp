#pragma once
#include <Arduino.h>

class ModularExp {
private:
public:
    static uint32_t mod_exp(uint32_t a, uint16_t degree, uint16_t mod) {
        uint32_t c = 1;
        for(int i = 1; i <= degree; i++) {
            c = (c*a)%mod;
        }
        return c;
    }
};