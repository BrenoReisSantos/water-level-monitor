#include "../Monitors/WaterMonitor.hpp"
#include <ESPAsyncWebServer.h>

class BaseApi
{
protected:
    AsyncWebServer webServer = AsyncWebServer(80);
    WaterMonitor *monitor;

    void getEstado();
    void getNivel();
    void postTravaSistema();
    void getEstadoDaTrava();
    void ping();

public:
    void configuraRotas();
};
