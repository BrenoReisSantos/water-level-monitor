#include "WaterMonitorFactory.hpp"

#include "../Monitors/WaterMonitor.hpp"
#include "../Monitors/CaixaMonitor.hpp"

WaterMonitor *WaterMonitorFactory::criaInstancia(MonitorConfig *monitorConfig)
{
    switch (monitorConfig->getTipoReservatorio())
    {
    case Caixa:
        CaixaClient *caixaClient = new CaixaClient(monitorConfig->getIpOutroReservatorio());
        CaixaMonitor *caixaMonitor = new CaixaMonitor(
            monitorConfig->getAlturaDoSendor(),
            monitorConfig->getalturaQuandoCheio(),
            monitorConfig->getalturaQuandoVazio(),
            caixaClient);
        return caixaMonitor;
        break;
    case Cisterna:
        CisternaClient *cisternaClient = new CisternaClient(monitorConfig->getIpOutroReservatorio());
        CisternaMonitor *cisternaMonitor = new CisternaMonitor(
            monitorConfig->getAlturaDoSendor(),
            monitorConfig->getalturaQuandoCheio(),
            monitorConfig->getalturaQuandoVazio(),
            cisternaClient);
        return cisternaMonitor;
        break;
    };
};
