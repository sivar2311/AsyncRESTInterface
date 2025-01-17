#include "AsyncRESTInterface.h"

#include <ArduinoJson.h>

extern const char* webPage;

static String settings2json(RESTSettings& settings) {
    JsonDocument doc;
    for (auto& setting : settings) {
        JsonObject jsonSetting     = doc.add<JsonObject>();
        jsonSetting["key"]         = setting.key;
        jsonSetting["description"] = setting.description;
        jsonSetting["value"]       = setting.value;
    }
    String json;
    serializeJson(doc, json);
    return json;
}

static void json2settings(JsonDocument& doc, RESTSettings& settings) {
    JsonArray arr = doc.as<JsonArray>();
    size_t    max = min(arr.size(), settings.size());
    for (size_t i = 0; i < max; i++) {
        settings[i].value = arr[i]["value"] | settings[i].value;
    }
}

AsyncRESTInterface::AsyncRESTInterface(AsyncWebServer* server)
    : server(server) {
}

void AsyncRESTInterface::begin(const String& page_route, const String& api_route, RESTSettings& settings, const String& page_name) {
    if (!server) return;
    this->settings   = &settings;
    this->api_route  = api_route;
    this->page_route = page_route;
    this->page_name  = page_name;

    // GET (html page)

    server->on(String(page_route).c_str(), HTTP_GET, [this](AsyncWebServerRequest* req) {
        req->send(200, "text/html", webPage, [this](const String& param) {
            String value = "";
            if (param == "SETTINGS_URL") value = this->api_route;
            if (param == "PAGE_NAME") value = this->page_name;
            return value;
        });
    });

    // PUT (json)

    server->on(api_route.c_str(), HTTP_PUT, [&settings](AsyncWebServerRequest* req) {
        String json = settings2json(settings);
        req->send(200, "application/json", json); 
    }, 
    nullptr, 
    [this, &settings](AsyncWebServerRequest* req, uint8_t* data, size_t len, size_t index, size_t total) {
        JsonDocument doc;
        deserializeJson(doc, data);
        json2settings(doc, settings);
        if (changeHandler) changeHandler(settings);
    });

    // GET (json)
    server->on(api_route.c_str(), HTTP_GET, [&settings](AsyncWebServerRequest* req) {
        String json = settings2json(settings);
        req->send(200, "application/json", json);
    });
}

void AsyncRESTInterface::onChange(ChangeHandler handler) {
    this->changeHandler = handler;
}