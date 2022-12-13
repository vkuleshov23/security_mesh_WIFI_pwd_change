
#include "painlessMesh.h"
#include <Arduino.h>
#include "SmartObjectBasic.hpp"
#include "FileWiFi.hpp"
#include "WiFiConsts.h"
#include "Restarter.hpp"

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

SmartObjectBasic SO(&mesh);
FileWiFi filewifi;
Restarter restarter;

auto button1 = SO.makeSmartActivator("button2.click");

auto passwordChanger = SO.makeSmartValue("device",
[](const String& event, String& value) {
  value = event;
},
[](const String& value) {
    filewifi.writeMeshWiFi(value);
    restarter.needReboot(5000);
});

int button_pin = D1;
int leed_pin = D2;

//при вызове этой переменной будут обработаны сценарии с активатором button2.click

void setup() {
  Serial.begin(115200);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(leed_pin, OUTPUT);
  digitalWrite(leed_pin, HIGH);
  // Initialize SPIFFS
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount SPIFFS");
  }
  else
  {
    filewifi.readMeshWiFi();
    SO.loadSettings();
  }

  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION | DEBUG ); 
  mesh.init( mesh_ssid, mesh_password, &userScheduler, MESH_PORT );
  mesh.setContainsRoot(true);
  SO.initMesh();
}

auto t = millis();

bool state = digitalRead(button_pin);

void send() {
  if(state != digitalRead(button_pin)){
    state = !state;
    button1->publish();//активируем сценарий
    digitalWrite(leed_pin, state);
  }
}

void loop() {
  
  mesh.update();
  if (millis() - t > 3000){
    Serial.print(WiFi.RSSI());
    Serial.printf(WiFi.SSID().c_str());
    Serial.printf("\n");
    t = millis();
    Serial.print("\n");
  }
  send();
  restarter.update();
}
