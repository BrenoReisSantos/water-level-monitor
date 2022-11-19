#include "../Monitors/WaterMonitor.hpp"
#include <ESPAsyncWebServer.h>

class BaseApi
{
protected:
    BaseApi(WaterMonitor *monitor);

    AsyncWebServer webServer = AsyncWebServer(80);
    WaterMonitor *monitor;

    void getEstado();
    void getNivel();
    void postTravaSistema();
    void getEstadoDaTrava();
    void ping();

public:
    virtual void configuraRotas();
};
