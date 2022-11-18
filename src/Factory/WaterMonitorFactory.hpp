#include "../Monitors/CisternaMonitor.hpp"

#include "../Config/MonitorConfig.hpp"

class WaterMonitorFactory
{
public:
    WaterMonitor *criaInstancia(MonitorConfig *monitorConfig);
};
