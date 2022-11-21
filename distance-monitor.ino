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

const String CAIXA_IP = "192.168.68.106";
const String CISTERNA_IP = "192.168.68.109";

// const byte TIPO_DE_RECIPIENTE = CAIXA;

// Configuração do WIFI
const char *WIFI_SSID = "BRENAN";
const char *WIFI_PASSWORD = "25602874";

// Servidor para pegar horário
const char *TIME_SERVER = "pool.ntp.org";

// Configuração do fuso-horário
const int BRAZIL_TIME_ZONE = -3 * 3600;
struct tm timeinfo;

// Configuração do Sensor Ultrassônico
const byte TRIGGER_PIN = 13;
const byte ECHO_PIN = 12;
// UltraSonicDistanceSensor distanceSensor(TRIGGER_PIN, ECHO_PIN);

const int PINO_BOMBEAR = 5; // Pino que vai controlar o relé e ligar a bomba

// Buffer para calcular o tempo estimado de esvaziar/encher
// const int TAMANHO_HISTORICO_DE_NIVEL = 15;
// int historicoNivelDeAgua[TAMANHO_HISTORICO_DE_NIVEL];

WaterMonitor *monitor;
BaseApi *api;

void setup()
{
    Serial.begin(115200);

    configuraFusoUTC();
    conectaAoWifi();

    MonitorConfig *config = new MonitorConfig(100.f, 80.f, 40.f, MonitorConfig::Caixa, "1.1.1.1");

    WaterMonitorFactory monitorFactory = WaterMonitorFactory();
    ApiFactory apiFactory = ApiFactory();

    monitor = monitorFactory.criaInstancia(config);

    api = apiFactory.criaInstancia(MonitorConfig::Caixa, monitor);
    api->configuraRotas();
}

// ------------------------- LOOP PRINCIPAL -------------------------
void loop()
{
    float distanciaMedidaEmCentimetros = medeDistanciaComSensorUltrassonico();
    Serial.printf("[Distância medida: %f]\n", distanciaMedidaEmCentimetros);

    monitor->atualizaNivel(distanciaMedidaEmCentimetros);

    monitor->gerenciaReservatorio();

    printaStatus();
    delay(1000);
}
// -------------------------------------------------------------------

float medeDistanciaComSensorUltrassonico()
{
    // Clears the trigPin condition
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(20);
    digitalWrite(TRIGGER_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(ECHO_PIN, HIGH);
    // Calculating the distance
    return duration * 0.034 / 2;
}

void printaStatus()
{

    // if (filaCheia(historicoNivelDeAgua, TAMANHO_HISTORICO_DE_NIVEL))
    //     desenfileira(historicoNivelDeAgua, TAMANHO_HISTORICO_DE_NIVEL);
    // enfileira(historicoNivelDeAgua, TAMANHO_HISTORICO_DE_NIVEL, nivelDeAguaEmPorcentagem);

    Serial.printf("%d/%d/%d %d:%d:%d ", year(), month(), day(), hour(), minute(), second());

    Serial.printf("%d\%", monitor->getNivel());

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
