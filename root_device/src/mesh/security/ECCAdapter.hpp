#pragma once
#include <Arduino.h>
#include "../utils/HexConverter.hpp"
#include <uECC.h>

static int RNG(uint8_t *dest, unsigned size) {
    uint8_t val = 0;
    for (unsigned i = 0; i < 8; ++i) {
      int init = analogRead(0);
      int count = 0;
      while (analogRead(0) == init) {
        ++count;
      }
      
      if (count == 0) {
         val = (val << 1) | (init & 0x01);
      } else {
         val = (val << 1) | (count & 0x01);
      }
    }
    *dest = val;
    ++dest;
    --size;
    yield();
    return 1;
}

class ECCAdatper {
private:
    static const uint8_t PRIV_SIZE = 21;
    uint8_t private_key[PRIV_SIZE];
    static const uint8_t PUB_SIZE = 40;
    uint8_t public_key[PUB_SIZE];
    uint8_t tmp_signature[PUB_SIZE];
    uint8_t tmp_pub_key[PUB_SIZE];
    const struct uECC_Curve_t * curve = uECC_secp160r1();

    void generate_keys() {
        uECC_set_rng(RNG);
        uECC_make_key(this->public_key, this->private_key, this->curve);
    }

public:
    ECCAdatper() {}

    void setup() {
        generate_keys();
    }

    std::string getPublicKey() {
        return HexConverter::toHex((const char*)this->public_key, PUB_SIZE);
    }

    std::string sign(String data) {
        uint8_t c_data[data.length()];
        ((String)data.c_str()).getBytes(c_data, data.length());
        uECC_sign(this->private_key, c_data, sizeof(c_data), tmp_signature, curve);
        return HexConverter::toHex((const char*)tmp_signature, PUB_SIZE);
    }

    int verify(std::string data, std::string signature, std::string pub_key) {
        HexConverter::hexToCharArr(signature.c_str(), (char*)tmp_signature);
        HexConverter::hexToCharArr(pub_key.c_str(), (char*)tmp_pub_key);
        uint8_t c_data[data.length()];
        ((String)data.c_str()).getBytes(c_data, data.length());
        int res = uECC_verify(tmp_pub_key, c_data, sizeof(c_data), tmp_signature, curve);
        return res;
    }

    int verify(std::string data, std::string signature) {
        return verify(data, signature, HexConverter::toHex((const char*)this->public_key, PUB_SIZE));
    }

    ~ECCAdatper() {}
    
};