#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "painlessMesh.h"
#include "mesh/MainSecMesh.hpp"

painlessMesh mesh;
AsyncWebServer server(80);
MainSecMesh main_sec_mesh(&mesh, &server);

void setup() {
    Serial.begin(115200);
    main_sec_mesh.setup();
}

void loop() {
    main_sec_mesh.update();
}