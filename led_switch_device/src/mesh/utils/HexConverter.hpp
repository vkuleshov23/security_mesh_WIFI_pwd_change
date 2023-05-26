#pragma once
#include <Arduino.h>


static const char symbols[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C','D','E','F'};
class HexConverter{
public:
    static String toHex(char* arr, int len){
        String res;
        for(int i = 0; i < len; i++) {
            char const byte = arr[i];
            res += symbols[ ( byte & 0xF0 ) >> 4 ];
            res += symbols[ ( byte & 0x0F ) >> 0 ];
        }
        return res;
    }

    static void hexToCharArr(const char* hex, char* dst) {
        for(int i = 0; i < strlen(hex); i ++) {
            dst[i] = hexToChar(hexParser(hex[i*2]), hexParser(hex[(i*2)+1]));
        }
    }

    static char hexToChar(char highByte, char lowByte){
        return ((highByte * 16) + (lowByte));
    }

    static char hexParser(char hex){
        if(hex >= 48 && hex <= 57){
            return hex%48;
        } else if(hex >= 97 && hex <= 102){
            return (hex%97) + 10;
        }
        return hex;
    }
};