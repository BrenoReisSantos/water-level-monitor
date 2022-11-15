#include "WaterMonitor.hpp"
#include "../Clients/CisternaClient.hpp"

class CisternaMonitor : public WaterMonitor
{
    CisternaClient *client;

    bool bombaLigada;

public:
    void controlaBomba(bool ligar);
    void gerenciaBomba();
    CisternaMonitor::CisternaMonitor(float alturaDoSensor, float alturaQuandoCheio, float alturaQuandoVazio, CisternaClient *client);
};
