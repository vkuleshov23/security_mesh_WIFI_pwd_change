#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

AsyncWebServer server(80);

void setup() {
    Serial.begin(115200);
    LittleFS.begin();
}

void loop() {
}