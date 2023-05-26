#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <LittleFS.h>
#include "painlessMesh.h"
#include "mesh/SecMesh.hpp"
#include "mesh/commands/basic/PingCommand.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include "mesh/security/rsa/RSA.h"

painlessMesh mesh;
SecMesh sec_mesh(&mesh);
Timer t;
RSAAdatper crypto_adapter;

static void print_data(const uint8_t *data, uint8_t size);
void crypt_test();

void setup() {
    Serial.begin(115200);
    sec_mesh.setup();
}


void loop() {
    sec_mesh.update();
    if(t == 0) {
        sec_mesh.send(shared_ptr<IMeshCommand>(new PingCommand(3665705184)));
        t.start(10000);
        // crypt_test();
        String res = HexConverter::toHex("1234\n\t1xcz", 10);
        char c_res[4] = {};
        HexConverter::hexToCharArr(res.c_str(), c_res);
        Serial.println(res);
        Serial.println(c_res);
    }
}

void crypt_test() {
    // uint8_t message[RSA_SIZE] = {};

    // for (uint8_t i = 0; i < RSA_BLOCK_SIZE; i++)
    // {
    //     message[i] = random(0xFF);
    // }
    String message = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    Serial.println("INCOMING DATA:");
    Serial.println(message.length());
    Serial.println(message);
    // print_data(message, RSA_BLOCK_SIZE);
    message = crypto_adapter.encrypt(message);
    Serial.println("ENCRYPTED DATA:");
    Serial.println(message.length());
    Serial.println(message);
    // print_data(message, RSA_SIZE);
    message = crypto_adapter.decrypt(message);
    Serial.println("DECRYPTED DATA:");
    Serial.println(message.length());
    Serial.println(message);
    // print_data(message, RSA_BLOCK_SIZE);
}

static void print_data(const uint8_t *data, uint8_t size) {
    for (uint8_t i = 0; i < size; i++) {
        // Serial.printf("%02X ", data[i]);
        Serial.printf("%u ", data[i]);
    }
    Serial.println();
}