#include "BaseApi.hpp"
#include "../Monitors/CaixaMonitor.hpp"

class CaixaApi : public BaseApi
{

public:
    CaixaApi(WaterMonitor *caixa);
    void configuraRotas() override;
};
