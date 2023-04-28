#include <vector>
#include <functional>
#include <string>
#include "painlessMesh.h"
#include "WiFiConsts.h"

class FileWiFi{
public:
    FileWiFi() {
        srand((int)millis());
    }

    void readMeshWiFi(){
        Serial.println("---WiFI SETTINGS---");
        File file = SPIFFS.open("/wifi.json", "r");
        
        if (!file)
        {
            Serial.println("There was an error opening  the WiFI file");
            return;
        }else{
            Serial.println("File opened!");
            StaticJsonDocument<512> mesh_wifi;
            DeserializationError error = deserializeJson(mesh_wifi, file);
            file.close();
            if (error)
            {
                Serial.println("error...");
            }else{
                Serial.println("deserialize mesh_wifi");
                mesh_ssid = mesh_wifi["ssid"].as<String>();
                mesh_password = mesh_wifi["password"].as<String>();
            }
        }

        Serial.println("---WiFI LOADED---\n");
    }

    void writeMeshWiFi(String password){
        Serial.println("---WiFI SETTINGS---");
        File file = SPIFFS.open("/wifi.json", "w");
        
        if (!file)
        {
            Serial.println("There was an error opening  the WiFI file");
            return;
        }else{
            StaticJsonDocument<512> mesh_wifi;
            mesh_password = password;
            mesh_wifi["ssid"] = mesh_ssid;
            mesh_wifi["password"] = mesh_password;
            serializeJson(mesh_wifi, file);
            Serial.println("NEW PASSWORD: " + password);
            Serial.println("---WiFI SAVED---\n");
        }
        file.close();
    }

    String generatePassword(){
        char ch[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '.', ',', '-', '_', '&', '?', '(', ')', '!', '*', '#', '|'};
        String psw = "";
        for(int i = 0; i < 12; i++) {
            psw += ch[rand()%sizeof(ch)];
        }
        return psw;
    }
};