#include "CaixaClient.hpp"

CaixaClient::CaixaClient(char *cisternaIP)
{
    baseUrl = strcat("http://", cisternaIP);
};

void CaixaClient::controlaBomba(bool deveLigar)
{
    http.begin(strcat(baseUrl, (deveLigar ? "on" : "off")));
    http.setTimeout(500);
    http.addHeader("Content-Type", "text/plain");
    http.POST("");
}
