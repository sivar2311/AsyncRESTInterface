#include <Arduino.h>
#include <AsyncRESTInterface.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

const char* WIFI_SSID    = "";
const char* WIFI_PASS    = "";

const int   server_port  = 80;

const char* page_route_1 = "/user_settings";
const char* api_route_1  = "/api/v1/user_settings";
const char* page_name_1  = "User Settings";

const char* page_route_2 = "/wifi_settings";
const char* api_route_2  = "/api/v1/wifi_settings";
const char* page_name_2  = "WiFi Settings";

AsyncWebServer server(server_port);

AsyncRESTInterface interface_1(&server);
AsyncRESTInterface interface_2(&server);

RESTSettings settings_1 = {
    //   key,         description, value
    {"firstname", "Vorname", "Hans"},
    {"lastname", "Nachname", "Dampf"}};

RESTSettings settings_2 = {
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

    server.onNotFound([](AsyncWebServerRequest* req) {
        req->send(200, "text/html", R"HTML_PAGE(
            <html>
              <body>
                <a href="/user_settings">User Settings</a><br> 
                <a href="/wifi_settings">WiFi Settings</a> 
              </body>
            </html>
        )HTML_PAGE");
    });

    Serial.printf("Server started on port http://%s:%d\r\n", WiFi.localIP().toString().c_str(), server_port);
}

void setupRESTInterface() {
    interface_1.begin(page_route_1, api_route_1, settings_1, page_name_1);
    interface_2.begin(page_route_2, api_route_2, settings_2, page_name_2);

    interface_1.onChange([](RESTSettings& settings) { Serial.println("User settings have changed!"); });
    interface_2.onChange([](RESTSettings& settings) { Serial.println("WiFi settings have changed!"); });
}

void setup() {
    Serial.begin(115200);
    setupWiFi();
    setupWebServer();
    setupRESTInterface();
}

void loop() {
    delay(10);
}