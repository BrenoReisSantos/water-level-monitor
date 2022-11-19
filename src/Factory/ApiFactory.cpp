#include "ApiFactory.hpp"

#include "../Api/CaixaApi.hpp"
#include "../Api/CisternaApi.hpp"

ApiFactory::ApiFactory(){};

BaseApi *ApiFactory::criaInstancia(MonitorConfig::TipoReservatorio tipo, WaterMonitor *monitor)
{
    switch (tipo)
    {
    case MonitorConfig::Caixa:
        return new CaixaApi(monitor);
        break;
    case MonitorConfig::Cisterna:
        return new CisternaApi(monitor);
        break;
    };
};
