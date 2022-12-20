
#include "painlessMesh.h"
#include <Arduino.h>
#include "SmartObjectBasic.hpp"
#include "FileWiFi.hpp"
#include "WiFiConsts.h"
#include "Consts.h"
#include "Restarter.hpp"

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

SmartObjectBasic SO(&mesh);
FileWiFi filewifi;
Restarter restarter;

void led_control() {
  if(mesh.getNodeList().size()) {
    digitalWrite(CONNECT_PIN, LOW);
  } else {
    digitalWrite(CONNECT_PIN, HIGH);
  }
}

auto led = SO.makeSmartValue("led.light",
[](const String& event, String& value) {
  if(value == "on") {
    value = "off";
  } else {
    value = "on";
  }
},
[](const String& value) {
    if (value == "on"){
      digitalWrite(LED_PIN, HIGH);
    }else{
      digitalWrite(LED_PIN, LOW);
    }
});

auto passwordChanger = SO.makeSmartValue("device",
[](const String& event, String& value) {
  value = event;
},
[](const String& value) {
    filewifi.writeMeshWiFi(value);
    restarter.needReboot(5000);
});

//при вызове этой переменной будут обработаны сценарии с активатором button2.click

void setup() {
  Serial.begin(115200);
  pinMode(CONNECT_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  // Initialize SPIFFS
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount SPIFFS");
    digitalWrite(CONNECT_PIN, LOW);
  }
  else
  {
    filewifi.readMeshWiFi();
    SO.loadSettings();
    digitalWrite(CONNECT_PIN, HIGH);
  }

  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION | DEBUG ); 
  mesh.init( mesh_ssid, mesh_password, &userScheduler, MESH_PORT );
  mesh.setContainsRoot(true);
  SO.initMesh();

}

auto t = millis();

void loop() {
  
  mesh.update();
  if (millis() - t > 3000){
    Serial.print(WiFi.RSSI());
    Serial.printf(WiFi.SSID().c_str());
    Serial.printf("\n");
    t = millis();
    Serial.print("\n");
  }
  restarter.update();
  led_control();
}


