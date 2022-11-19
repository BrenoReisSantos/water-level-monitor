#include "../Monitors/WaterMonitor.hpp"

#include "../Config/MonitorConfig.hpp"

class WaterMonitorFactory
{
public:
    WaterMonitor *criaInstancia(MonitorConfig *monitorConfig);
};
