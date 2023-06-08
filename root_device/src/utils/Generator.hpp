#include <string>
#include <Arduino.h>

std::string generatePassword(){
    char ch[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', ',', '-', '_', '&', '?', '(', ')', '!', '*', '#', '|'};
    std::string psw = "";
    for(int i = 0; i < 12; i++) {
        psw += ch[rand()%sizeof(ch)];
    }
    return psw;
}

std::string generatePassword(uint8_t length, unsigned long seed){
    srand(seed);
    char ch[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', ',', '-', '_', '&', '?', '(', ')', '!', '*', '#', '|'};
    std::string psw = "";
    for(int i = 0; i < length; i++) {
        psw += ch[rand()%sizeof(ch)];
    }
    return psw;
}

uint16_t generateUintPassword(unsigned long seed){
    srand(seed);
    return (uint16_t)rand();
}