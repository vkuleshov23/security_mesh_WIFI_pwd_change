#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <LittleFS.h>
#include "mesh/SecMesh.hpp"
#include "mesh/commands/basic/PingCommand.hpp"
#include "mesh/security/RSAAdapter.hpp"
#include <Hash.h>


SecMesh sec_mesh;
Timer t;
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println();
    Serial.println();
    sec_mesh.setup();
}

void loop() {
    sec_mesh.update();
    if(t == 0) {
        // sec_mesh.send(shared_ptr<IMeshCommand>(new PingCommand(MAIN_DEVICE)));
        sec_mesh.send_encrypt(RSA_PRINT, MAIN_DEVICE, "KEK RSA PRINT");
        t.start(10000);
    }
}