#ifndef APIFACTORY_H
#define APIFACTORY_H

#include "../Api/BaseApi.hpp"
#include "../Monitors/WaterMonitor.hpp"
#include "../Config/MonitorConfig.hpp"

class ApiFactory
{
public:
    ApiFactory();
    BaseApi *criaInstancia(MonitorConfig::TipoReservatorio tipo, WaterMonitor *monitor);
};

#endif
