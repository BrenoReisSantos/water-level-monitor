#include "BaseApi.hpp"
#include "../Monitors/CaixaMonitor.hpp"

class CaixaApi : BaseApi
{
    CaixaMonitor *caixa;

public:
    CaixaApi(CaixaMonitor *caixa);
};
