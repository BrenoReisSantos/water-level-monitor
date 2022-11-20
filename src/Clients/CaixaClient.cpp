#include "CaixaClient.hpp"

CaixaClient::CaixaClient(std::string cisternaIP)
{
    baseUrl = "http://" + cisternaIP;
};

void CaixaClient::controlaBomba(bool deveLigar)
{
    std::string url = baseUrl + pumpPath + (deveLigar ? "on" : "off");
    http.begin(url.c_str());
    http.setTimeout(500);
    http.addHeader("Content-Type", "text/plain");
    http.POST("");
}
