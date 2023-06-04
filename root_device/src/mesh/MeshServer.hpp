#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "painlessMesh.h"
#include "settings/WiFiConfigurer.hpp"
#include "utils/Restarter.hpp"
#include "mesh/security/auth/AuthHandler.hpp"

class MeshServer {
public:

    MeshServer(painlessMesh* mesh, AsyncWebServer* server, WiFiConfigurer* wifi_conf, Restarter* restarter, AuthHandler* auth) {
        this->mesh = mesh;
        this->server = server;
        this->wifi_conf = wifi_conf;
        this->restarter = restarter;
        this->auth = auth;
    }

    void setup() {
        this->request_connected_devices();
        this->request_change_wifi_pwd();
        this->request_get_wifi_credentials();
        this->request_authenticated_list();
        this->request_get_node_list();
        this->request_with_no_found();
        this->server->begin();
    }

protected:

    AsyncWebServer* server;
    painlessMesh* mesh;
    WiFiConfigurer* wifi_conf;
    Restarter* restarter;
    AuthHandler* auth;

    void request_connected_devices() {
        this->server->on("/connected_devices", HTTP_GET, [this](AsyncWebServerRequest *request){
            request->send(200, "text/html", this->mesh->asNodeTree().toString().c_str());
        });
    }

    void request_change_wifi_pwd() {
        this->server->on("/password_update", HTTP_GET, [this](AsyncWebServerRequest *request){
            request->send(200, "text/plain", "OK");
            Serial.println("RECIEVE WIFI CHANGE");
            // this->wifi_conf->writeMeshWiFi(this->wifi_conf->generatePassword());
            // SO.sendUpdatePasswordToAll();
            //-- delete if not work
            // this->restarter->needReboot((this->mesh->getNodeList().size()+5)*2000);
            Serial.println("END");
        });
    }

    void request_authenticated_list() {
        this->server->on("/auth_list", HTTP_GET, [this](AsyncWebServerRequest *request){
            Serial.println("RECIEVE WIFI CHANGE");
            String devices;
            for(uint32_t device : this->auth->get_auth_devices()) {
                devices += device;
                devices += ",";
            }
            devices.remove(devices.length()-1);
            request->send(200, "text/plain", devices);
        });
        
    }
    
    void request_get_wifi_credentials() {
        this->server->on("/wifi_credentials", HTTP_GET, [this](AsyncWebServerRequest *request){
            DynamicJsonDocument data(64);
            data["ssid"] = mesh_ssid;
            data["password"] = mesh_password;
            String out;
            serializeJson(data, out);
            request->send(200, "text/plain", out);
        });
    }

    void request_get_node_list() {
        this->server->on("/node_list", HTTP_GET, [this](AsyncWebServerRequest *request){
            std::list<uint32_t> nodes = this->mesh->getNodeList();
            String nodesList = "{\"nodes\" : [";
            auto itr = nodes.begin();
            for(int i = 0; i < nodes.size() ;i++, itr++) {
            nodesList += *itr;
            if (!(i+1 == nodes.size())) {
                nodesList += ',';
            }
            }
            nodesList += "]}";
            request->send(200, "text/plain", nodesList);
        });
    }

    void request_with_no_found() {
        this->server->onNotFound([this](AsyncWebServerRequest *request){
            if (!handleFileRead(request))
                request->send(404, "text/plain", "FileNotFound");
        });
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
};
