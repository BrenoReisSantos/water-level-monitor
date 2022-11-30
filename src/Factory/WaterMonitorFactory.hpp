#ifndef WATERMONITORFACTORY_H
#define WATERMONITORFACTORY_H

#include "../Monitors/WaterMonitor.hpp"

#include "../Config/MonitorConfig.hpp"

class WaterMonitorFactory
{
public:
    WaterMonitor *criaInstancia(MonitorConfig *monitorConfig);
};

#endif
