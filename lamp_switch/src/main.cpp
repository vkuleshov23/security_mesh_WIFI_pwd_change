//************************************************************
// this is a simple example that uses the painlessMesh library
//
// 1. sends a silly message to every node on the mesh at a random time between 1 and 5 seconds
// 2. prints anything it receives to Serial.print
//
//
//************************************************************
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

auto button1 = SO.makeSmartActivator("button1.click");

auto passwordChanger = SO.makeSmartValue("device",
[](const String& event, String& value) {
  value = event;
},
[](const String& value) {
    filewifi.writeMeshWiFi(value);
    restarter.needReboot(10000);
});

int button_pin = D1;
int led_pin = D2;

//при вызове этой переменной будут обработаны сценарии с активатором button1.click

void setup() {
  Serial.begin(115200);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, HIGH);
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
    digitalWrite(led_pin, state);
  }
}

void loop() {
  
  mesh.update();
  if (millis() - t > 15000){
    t = millis();
    Serial.println(mesh.asNodeTree().toString());
  }
  send();
  restarter.update();
  
}
