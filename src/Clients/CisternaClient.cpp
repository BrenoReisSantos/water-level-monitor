#include "CisternaClient.hpp"
#include "../Monitors/WaterMonitor.hpp"

bool CisternaClient::checaVidaDaCaixa()
{
    std::string url = baseUrl + healthPath;
    http.begin(url.c_str());
    http.setConnectTimeout(1000);
    int httpResponseCode = http.GET();
    Serial.printf("Caminho: %s\nResposta: %d\n", url.c_str(), httpResponseCode);
    if (httpResponseCode > 0)
    {
        String responseText = http.getString();
        Serial.printf("%s\n", responseText.c_str());
        if (responseText == "pong" && httpResponseCode == 200)
            return true;
    }
    else
    {
        return false;
    }
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
    http.setConnectTimeout(1000);
    int httpResponseCode = http.GET();
    Serial.printf("Caminho: %s\nResposta: %d\n", url.c_str(), httpResponseCode);
    String response = "";
    if (httpResponseCode > 0)
    {
        response = http.getString(); // Get the response to the request
    }
    else
    {
        Serial.print("Erro ao pegar o estado da Caixa.\n");
    }

    if (response == "CHEIO")
        return Cheio;
    if (response == "MEDIANO")
        return Mediano;
    if (response == "VAZIO")
        return Vazio;
    return Cheio;
}
