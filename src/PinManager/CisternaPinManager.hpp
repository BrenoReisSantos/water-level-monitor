#include "../Monitors/CisternaMonitor.hpp"

class CisternaPinManager
{
    int releBombaPin;
    CisternaMonitor *monitor;

public:
    CisternaPinManager(int releBombaPin, CisternaMonitor *monitor);
    void atualizaPinoDoRele();
    virtual ~CisternaPinManager(){};
};
