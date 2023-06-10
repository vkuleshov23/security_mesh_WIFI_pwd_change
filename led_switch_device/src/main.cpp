#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <LittleFS.h>
#include "mesh/SecMesh.hpp"
#include "mesh/commands/basic/PingCommand.hpp"
#include "mesh/commands/test/RSASerialCommand.hpp"

SecMesh sec_mesh;
Timer t;

void setup() {
    Serial.begin(115200);
    delay(1000);
    sec_mesh.setup();
}

void loop() {
    sec_mesh.update();
    if(t == 0) {
        // sec_mesh.send(shared_ptr<IMeshCommand>(new PingCommand(MAIN_DEVICE)));
        sec_mesh.send(shared_ptr<IMeshCommand>(new RSASerialCommand(MAIN_DEVICE, "KEK RSA PRINT")));
        t.start(30000);
    }
}