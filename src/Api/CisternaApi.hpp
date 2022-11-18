#include "BaseApi.hpp"
#include "../Monitors/CisternaMonitor.hpp"

class CisternaApi : BaseApi
{
    void controlaBomba(bool deveLigar);

public:
    CisternaApi(CisternaMonitor *cisterna);
};
