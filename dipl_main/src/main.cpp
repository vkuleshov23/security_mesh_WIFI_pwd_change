//************************************************************
// this is a simple example that uses the painlessMesh library to
// connect to a another network and broadcast message from a webpage to the edges of the mesh network.
// This sketch can be extended further using all the abilities of the AsyncWebserver library (WS, events, ...)
// for more details
// https://gitlab.com/painlessMesh/painlessMesh/wikis/bridge-between-mesh-and-another-network
// for more details about my version
// https://gitlab.com/Assassynv__V/painlessMesh
// and for more details about the AsyncWebserver library
// https://github.com/me-no-dev/ESPAsyncWebServer
//************************************************************

#include "IPAddress.h"
#include "painlessMesh.h"
#include "Timer.h"
#include "WiFiConsts.h"
#include "FileWiFi.hpp"
#include "Restarter.hpp"
#include <Adafruit_NeoPixel.h>

#ifdef ESP8266
#include "Hash.h"
#include "FS.h"
#include <ESPAsyncTCP.h>
#else
#include "SPIFFS.h"
#include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

#include "SmartObjectMain.hpp"

// Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, MATRIX_PIN, NEO_RGB + NEO_KHZ800);
// uint8_t r__ = 128;
// uint8_t g__ = 128;
// uint8_t b__ = 128;

// Prototype
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback();
void nodeTimeAdjustedCallback(int32_t offset);
String getContentType(String);
bool handleFileRead(AsyncWebServerRequest*);
IPAddress getlocalIP();

painlessMesh  mesh;
AsyncWebServer server(80);
IPAddress myIP(0,0,0,0);
IPAddress myAPIP(0,0,0,0);

SmartObjectMain SO(&mesh);
FileWiFi filewifi;
Restarter restarter;

bool ignoreMode = true;
uint32_t nodeToIgnore1 = 3662020501;
uint32_t nodeToIgnore2 = 3662027117;

void setup() {
  Serial.begin(115200);
  pinMode(D0, OUTPUT);
  // strip.begin();

    // Initialize SPIFFS
  if (!SPIFFS.begin()){ Serial.println("Failed to mount SPIFFS");}
  else{
      filewifi.readMeshWiFi();
      SO.loadSettings();
    }

  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION | DEBUG);
  mesh.init( mesh_ssid, mesh_password, MESH_PORT, WIFI_AP_STA, 1);
  mesh.stationManual(STATION_SSID, STATION_PASSWORD);
  mesh.setHostname(HOSTNAME);
  mesh.setRoot(true);
  mesh.setContainsRoot(true);

  SO.initMesh();

  myAPIP = IPAddress(mesh.getAPIP());
  Serial.println("My AP IP is " + myAPIP.toString());
  Serial.println("My MESH WIFI is " + mesh_ssid + " | " + mesh_password);



//---------------------------------------------------------------------------
  //Async webserver
  server.on("/buf", HTTP_GET, [](AsyncWebServerRequest *request){
    String text = mesh.asNodeTree().toString();
    request->send(200, "text/html", text.c_str());
  });

  server.on("/api/set/systemMode", HTTP_GET, [](AsyncWebServerRequest *request){
    SO.systemMode(request->arg("systemMode"));;
    request->send(200, "text/plain", "OK");
    if(SO.getSystemMode() == "local") {
      // r__ = 255; g__ = 0; b__ = 0;
    } else {
      // r__ = 0; g__ = 0; b__ = 255;
    }
  });

  server.on("/api/set/ignoreMode", HTTP_GET, [](AsyncWebServerRequest *request){
    ignoreMode =  request->arg("ignoreMode").toInt();
    SO.ignoreConnectionState(nodeToIgnore1, nodeToIgnore2, ignoreMode);
    request->send(200, "text/plain", "OK");
    Serial.printf("IGNORE MODE RECIEVE: %u\n", ignoreMode);
  });

  server.on("/api/get/systemMode", HTTP_GET, [](AsyncWebServerRequest *request){
    DynamicJsonDocument data(64);
    data["ignoreMode"] = SO.getSystemMode();
    String out;
    serializeJson(data, out);
    request->send(200, "text/plain", out);
  });

  server.on("/api/get/ignoreMode", HTTP_GET, [](AsyncWebServerRequest *request){
    DynamicJsonDocument data(64);
    data["ignoreMode"] = ignoreMode;
    String out;
    serializeJson(data, out);
    request->send(200, "text/plain", out);
  });


  server.on("/api/set/WiFiChange", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "OK");
    Serial.printf("RECIEVE WIFI CHANGE\n");
    filewifi.writeMeshWiFi(filewifi.generatePassword());
    SO.sendUpdatePasswordToAll();
    // -- delete if not work
    restarter.needReboot((mesh.getNodeList().size()+5)*1000);
  });

  server.on("/api/get/WiFiChange", HTTP_GET, [](AsyncWebServerRequest *request){
    DynamicJsonDocument data(64);
    data["ssid"] = mesh_ssid;
    data["password"] = mesh_password;
    String out;
    serializeJson(data, out);
    request->send(200, "text/plain", out);
  });


  server.onNotFound([](AsyncWebServerRequest *request){
    if (!handleFileRead(request))
          request->send(404, "text/plain", "FileNotFound");
  });

  server.begin();

}

void loop() {
  mesh.update();
  if(myIP != getlocalIP()){
    myIP = getlocalIP();
    Serial.println("My IP is " + myIP.toString());
  }
  restarter.update();
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

IPAddress getlocalIP() {
  return IPAddress(mesh.getStationIP());
}

//Web
String getContentType(String filename) {
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(AsyncWebServerRequest *request) {
  String path = request->url();
  if (path.endsWith("/")) path += "index.html";
  if(path[0] != '/')path = String("/") + path;
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    request->send(SPIFFS, path, contentType);
    return true;
  }
  return false;
}