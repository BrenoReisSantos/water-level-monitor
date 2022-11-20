#include <DS1307RTC.h>
#include <HCSR04.h>
#include <WiFi.h>
#include <HTTPClient.h>
// #include <SPI.h>
#include "time.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#include "src/Factory/WaterMonitorFactory.hpp"
#include "src/Factory/ApiFactory.hpp"
#include "src/Config/MonitorConfig.hpp"

// #define CISTERNA 1
// #define CAIXA 2

const String CAIXA_IP = "192.168.68.106";
const String CISTERNA_IP = "192.168.68.109";

// const byte TIPO_DE_RECIPIENTE = CAIXA;

// Configuração do WIFI
const char *WIFI_SSID = "Saquarema";
const char *WIFI_PASSWORD = "25602874";

// const IPAddress SUBNET = IPAddress(255, 255, 255, 0);
// const IPAddress GATEWAY = IPAddress(192, 168, 0, 1);
// const IPAddress DNS(181, 213, 132, 2);
// const IPAddress DNS1(8, 8, 8, 8);
// const IPAddress DNS2(8, 8, 4, 4);

// AsyncWebServer webServer = AsyncWebServer(80);

// const char *CAIXA_HOST_NAME = "CAIXA_ESP32";
// const char *CISTERNA_HOST_NAME = "CISTERNA_ESP32";

// const IPAddress CAIXA_STATIC_IP = IPAddress(192, 168, 1, 10);
// const IPAddress CISTERNA_STATIC_IP = IPAddress(192, 168, 1, 11);

// Servidor para pegar horário
const char *TIME_SERVER = "pool.ntp.org";

// Configuração do fuso-horário
const int BRAZIL_TIME_ZONE = -3 * 3600;
struct tm timeinfo;

// Configuração do Sensor Ultrassônico
const byte TRIGGER_PIN = 13;
const byte ECHO_PIN = 12;
UltraSonicDistanceSensor distanceSensor(TRIGGER_PIN, ECHO_PIN);

// Configuração da dos alertas de Vazio e Cheio
// const float SENSOR_HEIGHT = 30.f;
// const float SENSOR_HEIGHT = 133.f; // CISTERNA
// const float SENSOR_HEIGHT = 92.f; // CAIXA

// // const float ALTURA_QUANDO_CHEIO = 25.f;
// // const float ALTURA_QUANDO_CHEIO = 124.f; // CISTERNA
// const float ALTURA_QUANDO_CHEIO = 65.f; // CAIXA
// // const float ALTURA_QUANDO_VAZIO = 5.f
// // const float ALTURA_QUANDO_VAZIO = 20.f; // CISTERNA
// const float ALTURA_QUANDO_VAZIO = 30.f; // CAIXA

// const int PORCENTAGEM_QUANDO_CHEIO = (int)((ALTURA_QUANDO_CHEIO / SENSOR_HEIGHT) * 100);
// const int PORCENTAGEM_QUANDO_VAZIO = (int)((ALTURA_QUANDO_VAZIO / SENSOR_HEIGHT) * 100);

// Variáveis de Controle / Estados
// #define VAZIO 0
// #define MEDIANO 1
// #define CHEIO 2

// byte estadoDoNivelDeAgua = VAZIO; // 0 VAZIO, 1 MEDIANO, 2 CHEIO
// bool trava = false;
// bool ligarBomba = false;
// bool enchendo = false;

// Nível da Água
// int porcentagemDoNivelDeAgua = 0;

// Configuração dos pinos de Vazio e Cheio (Vai ser retirado ao final)
// const int PINO_CHEIO = 2;
// const int PINO_VAZIO = 4;

const int PINO_BOMBEAR = 5; // Pino que vai controlar o relé e ligar a bomba

// Buffer para calcular o tempo estimado de esvaziar/encher
const int TAMANHO_HISTORICO_DE_NIVEL = 15;
int historicoNivelDeAgua[TAMANHO_HISTORICO_DE_NIVEL];

WaterMonitor *monitor;
BaseApi *api;

void setup()
{
    configuraFusoUTC();
    conectaAoWifi();

    MonitorConfig *config = new MonitorConfig(100.f, 100.f, 0.f, MonitorConfig::Caixa, "1.1.1.1");

    WaterMonitorFactory monitorFactory = WaterMonitorFactory();
    ApiFactory apiFactory = ApiFactory();

    monitor = monitorFactory.criaInstancia(config);

    api = apiFactory.criaInstancia(MonitorConfig::Caixa, monitor);
    api->configuraRotas();
}

// ------------------------- LOOP PRINCIPAL -------------------------
void loop()
{
    float distanceCm = distanceSensor.measureDistanceCm();
    monitor->atualizaNivel(distanceCm);

    monitor->gerenciaReservatorio();
    delay(1000);
}
// -------------------------------------------------------------------

void registraNivel(int nivelDeAguaEmPorcentagem)
{

    // if (filaCheia(historicoNivelDeAgua, TAMANHO_HISTORICO_DE_NIVEL))
    //     desenfileira(historicoNivelDeAgua, TAMANHO_HISTORICO_DE_NIVEL);
    // enfileira(historicoNivelDeAgua, TAMANHO_HISTORICO_DE_NIVEL, nivelDeAguaEmPorcentagem);

    Serial.print(year());
    Serial.print("-");
    Serial.print(month());
    Serial.print("-");
    Serial.print(day());
    Serial.print(" ");
    Serial.print(hour());
    Serial.print(":");
    Serial.print(minute());
    Serial.print(":");
    Serial.print(second());
    Serial.print(" ");
    Serial.print(nivelDeAguaEmPorcentagem);
    Serial.print("%");
    String stateText;
    // -----------------------------
    Serial.println();
}

// Conecta o ESP32 no WiFi
void conectaAoWifi()
{
    Serial.print("Conectando a:");
    Serial.print(WIFI_SSID);
    Serial.println();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("SSID: ");
    Serial.print(WiFi.SSID());
    Serial.print(" IP: ");
    Serial.print(WiFi.localIP());
    Serial.print(" SUBNET: ");
    Serial.print(WiFi.subnetMask());
    Serial.print(" GATEWAY: ");
    Serial.print(WiFi.gatewayIP());
    Serial.print(" DNS: ");
    Serial.print(WiFi.dnsIP());
    Serial.println("\nWiFi conectado.");
}

void configuraFusoUTC()
{
    conectaAoWifi();
    // Não funciona com ip estático...
    configTime(BRAZIL_TIME_ZONE, 0, TIME_SERVER);
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
    Serial.println("Time got with success");
    Serial.println("Disconnecting WiFi");
    WiFi.disconnect();
}

// Não está funcionando
// void connectToWifiWithStaticIP()
// {
//     // Serial.println("Connecting to WiFi with static IP");
//     switch (TIPO_DE_RECIPIENTE)
//     {
//     case CISTERNA:
//         WiFi.setHostname(CISTERNA_HOST_NAME);
//         // WiFi.config(CISTERNA_STATIC_IP, GATEWAY, SUBNET, DNS1, DNS2);
//         break;
//     case CAIXA:
//         WiFi.setHostname(CAIXA_HOST_NAME);
//         // WiFi.config(CAIXA_STATIC_IP, GATEWAY, SUBNET, DNS1, DNS2);
//         break;
//     }
//     Serial.println(WiFi.macAddress());
//     conectaAoWifi();
// }

// // --------------- Funções de Fila -------------------
// void inicializaArrayDeHistoricoDeNiveis()
// {
//     for (int index = 0; index < TAMANHO_HISTORICO_DE_NIVEL; index++)
//     {
//         historicoNivelDeAgua[index] = -1.f;
//     }
// }

// bool filaCheia(int *fila, int tamanhoDaFila)
// {
//     for (int index = 0; index < TAMANHO_HISTORICO_DE_NIVEL; index++)
//     {
//         if (historicoNivelDeAgua[index] == -1.f)
//             return false;
//     }
//     return true;
// }

// void enfileira(int *fila, int tamanhoDaFila, float elemento)
// {
//     if (filaCheia(fila, tamanhoDaFila))
//         return;
//     for (int index = 0; index < tamanhoDaFila; index++)
//     {
//         if (fila[index] <= -1.f)
//             fila[index] = elemento;
//     }
// }

// float desenfileira(int *fila, int tamanhoDaFila)
// {
//     if (fila[0] == -1.f)
//         return -1.f;
//     float poppedElement = fila[0];
//     moveElementosParaEsquerda(fila, tamanhoDaFila);
//     return poppedElement;
// }

// void moveElementosParaEsquerda(int *vetor, int tamanhoDoVetor)
// {
//     for (int index = 0; index < tamanhoDoVetor; index++)
//     {
//         if (index == tamanhoDoVetor)
//             vetor[index] = -1;
//         vetor[index] = vetor[index + 1];
//     }
// }

// void mostraArray(int *vetor, int tamanhoDoVetor)
// {
//     for (int index = 0; index < tamanhoDoVetor; index++)
//     {
//         Serial.printf("%d ", vetor[index]);
//     }
//     Serial.println();
// }
// // ----------------------------------------------------
