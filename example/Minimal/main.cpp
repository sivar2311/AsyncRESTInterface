#include <Arduino.h>
#include <AsyncRESTInterface.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

const char* WIFI_SSID    = "";
const char* WIFI_PASS    = "";

const int   server_port = 80;
const char* page_route  = "/settings.html";
const char* page_name   = "Settings";
const char* api_route   = "/api/v1/settings";

AsyncWebServer server(server_port);
AsyncRESTInterface interface(&server);

RESTSettings settings = {
//   key,         description, value
    {"firstname", "Vorname", "Hans"},
    {"lastname", "Nachname", "Dampf"},
    {"wifi_ssid", "WiFi-SSID", "MyWiFiNetwork"},
    {"wifi_pass", "WiFi-Password", "12345678"}};

void setupWiFi() {
    Serial.print("Connecting Wifi");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println("connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void setupWebServer() {
    server.begin();
}

void setupRESTInterface() {
    interface.begin(page_route, api_route, settings, page_name);
}

void setup() {
    Serial.begin(115200);
    setupWiFi();
    setupWebServer();
    setupRESTInterface();
}

void loop() {
}