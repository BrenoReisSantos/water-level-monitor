#include "CisternaClient.hpp"
#include "../Monitors/WaterMonitor.hpp"

bool CisternaClient::checaVidaDaCaixa()
{
    int httpResponseCode = http.GET();
    bool response = false;
    if (httpResponseCode > 0)
    {
        String responseText = http.getString();
        if (responseText == "pong" && httpResponseCode == 200)
            response = true;
    }
    else
    {
        response = false;
    }
    // Serial.println(response);
    return response;
};

CisternaClient::CisternaClient(std::string caixaIP)
{
    baseUrl = "http://" + caixaIP;
};

EstadoDoNivelDeAgua CisternaClient::checaEstadoDaCaixa()
{
    std::string url = baseUrl + statePath;

    http.begin(url.c_str());
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
