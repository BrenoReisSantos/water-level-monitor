#include "WaterMonitorFactory.hpp"

#include "../Monitors/CisternaMonitor.hpp"
#include "../Monitors/CaixaMonitor.hpp"

WaterMonitor *WaterMonitorFactory::criaInstancia(MonitorConfig *monitorConfig)
{
    CaixaClient *caixaClient = NULL;
    CaixaMonitor *caixaMonitor = NULL;

    CisternaClient *cisternaClient = NULL;
    CisternaMonitor *cisternaMonitor = NULL;

    switch (monitorConfig->getTipoReservatorio())
    {
    case MonitorConfig::Caixa:
        caixaClient = new CaixaClient(monitorConfig->getIpOutroReservatorio());
        caixaMonitor = new CaixaMonitor(
            monitorConfig->getAlturaDoSendor(),
            monitorConfig->getalturaQuandoCheio(),
            monitorConfig->getalturaQuandoVazio(),
            caixaClient);
        return caixaMonitor;
        break;
    case MonitorConfig::Cisterna:
        cisternaClient = new CisternaClient(monitorConfig->getIpOutroReservatorio());
        cisternaMonitor = new CisternaMonitor(
            monitorConfig->getAlturaDoSendor(),
            monitorConfig->getalturaQuandoCheio(),
            monitorConfig->getalturaQuandoVazio(),
            cisternaClient);
        return cisternaMonitor;
        break;
    };
};
