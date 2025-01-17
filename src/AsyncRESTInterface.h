#pragma once

#include <ESPAsyncWebServer.h>

#include <functional>
#include <vector>

struct RESTSetting {
    const String key;
    const String description;
    String       value;
};

using RESTSettings = std::vector<RESTSetting>;

class AsyncRESTInterface {
  public:
    using ChangeHandler = std::function<void(RESTSettings&)>;

  public:
    AsyncRESTInterface(AsyncWebServer* server);
    void begin(const String& page_route, const String& api_route, RESTSettings& settings);
    void onChange(ChangeHandler handler);

  protected:
    String          page_route    = "";
    String          api_route     = "";
    AsyncWebServer* server        = nullptr;
    RESTSettings*   settings      = nullptr;
    ChangeHandler   changeHandler = nullptr;
};