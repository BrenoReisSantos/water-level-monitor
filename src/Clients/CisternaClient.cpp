#include "CisternaClient.hpp"
#include "../Monitors/WaterMonitor.hpp"

bool CisternaClient::checaVidaDaCaixa()
{
    std::string url = baseUrl + healthPath;
    http.begin(url.c_str());
    http.setConnectTimeout(500);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
        String responseText = http.getString();
        if (responseText == "pong" && httpResponseCode == 200)
            return true;
    }
    else
    {
        return false;
    }
    // Serial.println(response);
    return false;
};

CisternaClient::CisternaClient(std::string caixaIP)
{
    baseUrl = "http://" + caixaIP;
};

EstadoDoNivelDeAgua CisternaClient::checaEstadoDaCaixa()
{
    std::string url = baseUrl + statePath;

    http.begin(url.c_str());
    http.setConnectTimeout(500);
    int httpResponseCode = http.GET();
    String response = "";
    if (httpResponseCode > 0)
    {
        response = http.getString(); // Get the response to the request
    }
    else
    {
        Serial.print("Error on sending POST: ");
    }

    if (response == "CHEIO")
        return Cheio;
    if (response == "MEDIANO")
        return Mediano;
    if (response == "VAZIO")
        return Vazio;
    return Cheio;
}
