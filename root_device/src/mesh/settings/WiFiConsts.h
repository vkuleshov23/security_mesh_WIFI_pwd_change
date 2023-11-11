#ifndef _WiFiConsts_
#define _WiFiConsts_

#include <Arduino.h>

#define   STATION_SSID     "aaaa1234"
#define   STATION_PASSWORD "12345678"
#define   HOSTNAME "HTTP_BRIDGE"

#define   MESH_PORT  5555

unsigned long AUTH_MILLIS_CONFIRM_TIME = 2 * 60 * 1000;
unsigned long AUTH_MILLIS_CHECKER_TIME = 60 * 1000;
unsigned long AUTH_CHECK = 15 * 1000;
uint8_t AUTH_KEY_LEN = 25;

uint32_t MAIN_DEVICE = 3665705184;

String mesh_ssid = "defaultssid";
String mesh_password = "defaultpwd";

#endif