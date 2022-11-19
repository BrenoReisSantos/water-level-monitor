#include "../Config/MonitorConfig.hpp"
#include "../Api/BaseApi.hpp"
#include "../Monitors/WaterMonitor.hpp"

class ApiFactory
{
public:
    ApiFactory();
    BaseApi *criaInstancia(MonitorConfig::TipoReservatorio tipo, WaterMonitor *monitor);
};
