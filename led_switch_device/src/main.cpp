#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "painlessMesh.h"
#include "mesh/SecMesh.hpp"
#include "mesh/commands/basic/PingCommand.hpp"

painlessMesh mesh;
SecMesh sec_mesh(&mesh);
Timer t;

void setup() {
    Serial.begin(115200);
    sec_mesh.setup();
}

int i = 1;

void loop() {
    sec_mesh.update();
    if(t == 0) {
        PingCommand command(3665705184);
        sec_mesh.send(&command);
        t.start(15000);
    }
}