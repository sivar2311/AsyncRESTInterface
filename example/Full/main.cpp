#include <Arduino.h>
#include <AsyncRESTInterface.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

const char* WIFI_SSID    = "";
const char* WIFI_PASS    = "";

const int   server_port = 80;
const char* page_name   = "Settings";
const char* page_route  = "/settings.html";
const char* api_route   = "/api/v1/settings";

AsyncWebServer server(server_port);
AsyncRESTInterface interface(&server);

RESTSettings settings = {
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

    server.onNotFound([](AsyncWebServerRequest* req) { req->send(200, "text/html", "<a href=\""+String(page_route)+"\">Settings</a>"); });
    Serial.printf("Server started. Open your brorwser and navigate to http://%s:%d\r\n", WiFi.localIP().toString().c_str(), server_port);
}

void setupRESTInterface() {
    interface.begin(page_route, api_route, settings, page_name);
    interface.onChange([](RESTSettings& settings){ Serial.println("Settings have changed!"); });
}

void setup() {
    Serial.begin(115200);
    setupWiFi();
    setupWebServer();
    setupRESTInterface();
}

void loop() {
}