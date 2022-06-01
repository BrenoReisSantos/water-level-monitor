#include <DS1307RTC.h>
#include <HCSR04.h>
#include <WiFi.h>
// #include <SPI.h>
#include "time.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

#define CISTERNA 1
#define CAIXA 2

const byte SYSTEM_APPLICATION_TYPE = CISTERNA;

// Configuração do WIFI
const char *WIFI_SSID = "BRENAN";
const char *WIFI_PASSWORD = "25602874";

const IPAddress SUBNET = IPAddress(255, 255, 255, 0);
const IPAddress GATEWAY = IPAddress(192, 168, 0, 1);
const IPAddress DNS(181, 213, 132, 2);
const IPAddress DNS1(8, 8, 8, 8);
const IPAddress DNS2(8, 8, 4, 4);

AsyncWebServer webServer(80);

const char *CAIXA_HOST_NAME = "CAIXA_ESP32";
const char *CISTERNA_HOST_NAME = "CISTERNA_ESP32";

const IPAddress CAIXA_STATIC_IP = IPAddress(192, 168, 1, 10);
const IPAddress CISTERNA_STATIC_IP = IPAddress(192, 168, 1, 11);

// Servidor para pegar horário
const char *TIME_SERVER = "pool.ntp.org";

// Configração do fuso-horário
const int BRAZIL_TIME_ZONE = -3 * 3600;
struct tm timeinfo;

// Configuração do Sensor Ultrassônico
const byte TRIGGER_PIN = 13;
const byte ECHO_PIN = 12;
UltraSonicDistanceSensor distanceSensor(TRIGGER_PIN, ECHO_PIN);

// Configuração da dos alertas de Vazio e Cheio
const float SENSOR_HEIGHT = 30.f;
const float FULL_LEVEL_PERCENTAGE = 85.f;
const float EMPTY_LEVEL_PERCENTAGE = 50.f;

// Mudar para esse tipo de entrada
const float FULL_HEIGHT = 25.f;
const float EMPTY_HEIGHT = 5.f;

float fullPercentage;
float emptyPercentage;

// Variáveis de Controle / Estados
byte waterLevelState = 0; // 0 EMPTY, 1 FULL, 2 NEITHER
bool lock = false;
bool waterOut = false;
bool waterIn = false;

// Nível da Água
float waterLevelPercentage = 0.f;

// Configuração dos pinos de Vazio e Cheio (Vai ser retirado ao final)
const int FULL_PIN = 2;
const int EMPTY_PIN = 4;

// const int WATER_OUT_PIN; // Pino que vai controlar o relé

// Buffer para calcular o tempo estimado de esvaziar/encher
const int HISTORY_QUEUE_SIZE = 15;
float waterLevelHistoryQueue[HISTORY_QUEUE_SIZE];

void setup()
{
    Serial.begin(115200);
    pinMode(FULL_PIN, OUTPUT);
    pinMode(EMPTY_PIN, OUTPUT);

    configUTCTime();
    connectToWifiWithStaticIP();
    configureWebServer();

    initializeWaterLevelHistoryQueue();

    fullPercentage = (FULL_HEIGHT / SENSOR_HEIGHT) * 100;
    emptyPercentage = (EMPTY_HEIGHT / SENSOR_HEIGHT) * 100;
    Serial.printf("Porcentagem máxima: %f \n", fullPercentage);
    Serial.printf("Porcentagem mínima: %f \n", emptyPercentage);
}

/// ------------------------- LOOP PRINCIPAL -------------------------
void loop()
{

    // Serial.print("Printando a distancia: ");
    float distanceCm = distanceSensor.measureDistanceCm();
    float levelPercentage = (1 - (distanceCm / SENSOR_HEIGHT)) * 100;
    levelPercentage = limitLevelPercentage(levelPercentage);
    waterLevelPercentage = levelPercentage;

    // Serial.print(distance_cm);
    // printLevelInPercentage(levelPercentage);

    updateLevelPins(levelPercentage);
    registerLevel(levelPercentage);

    calculateEstimateTime();

    // Serial.println();
    delay(1000);
}
/// -------------------------------------------------------------------

float limitLevelPercentage(float levelPercentage)
{
    if (levelPercentage > 100.f)
        levelPercentage = 100.f;
    if (levelPercentage < 0.f)
        levelPercentage = 0.f;
    return levelPercentage;
}

// Por enquanto só printando
void registerLevel(float levelPercentage)
{
    if (isQueueFull(waterLevelHistoryQueue, HISTORY_QUEUE_SIZE))
        popQueue(waterLevelHistoryQueue, HISTORY_QUEUE_SIZE);
    pushQueue(waterLevelHistoryQueue, HISTORY_QUEUE_SIZE, levelPercentage);

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
    Serial.print(levelPercentage);
    Serial.print("%");
    String stateText;
    // Remover isso ao escrever no SD
    switch (waterLevelState)
    {
    case 0:
        stateText = "EMPTY";
        break;
    case 1:
        stateText = "FULL";
        break;
    case 2:
        stateText = "AVERAGE";
        break;
    }
    Serial.printf(" MAX: %f MIN: %f %s", fullPercentage, emptyPercentage, stateText);
    // -----------------------------
    Serial.println();
}

void printLevelInPercentage(float levelPercentage)
{
    Serial.print("Distance %: ");
    Serial.print(levelPercentage);
    Serial.println();
}

void updateLevelPins(float levelPercentage)
{
    if (levelPercentage > 100.f)
        levelPercentage = 100.f;
    if (levelPercentage <= emptyPercentage)
    {
        digitalWrite(FULL_PIN, LOW);
        digitalWrite(EMPTY_PIN, HIGH);
        waterLevelState = 0;
    }
    else if (levelPercentage >= fullPercentage)
    {
        digitalWrite(FULL_PIN, HIGH);
        digitalWrite(EMPTY_PIN, LOW);
        waterLevelState = 1;
    }
    else
    {
        digitalWrite(FULL_PIN, LOW);
        digitalWrite(EMPTY_PIN, LOW);
        waterLevelState = 2;
    }
}

void calculateEstimateTime()
{
    printArray(waterLevelHistoryQueue, HISTORY_QUEUE_SIZE);
}

void connectToWifi()
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

void configUTCTime()
{
    connectToWifi();
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

void connectToWifiWithStaticIP()
{
    // Serial.println("Connecting to WiFi with static IP");
    switch (SYSTEM_APPLICATION_TYPE)
    {
    case CISTERNA:
        WiFi.setHostname(CISTERNA_HOST_NAME);
        // WiFi.config(CISTERNA_STATIC_IP, GATEWAY, SUBNET, DNS1, DNS2);
        break;
    case CAIXA:
        WiFi.setHostname(CAIXA_HOST_NAME);
        // WiFi.config(CAIXA_STATIC_IP, GATEWAY, SUBNET, DNS1, DNS2);
        break;
    }
    Serial.println(WiFi.macAddress());
    connectToWifi();
}

void configureWebServer()
{
    // Também não funciona com o IP estático.
    Serial.println("Configurando o WebServer...");
    Serial.println("Configurando rotas");
    webServer.on("/stats/fullness", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
        Serial.println("Acesso a rota: /stats/fullness");
        String responseText;
        switch (waterLevelState) {
            case 0:
                responseText = "EMPTY";
                break;
            case 1:
                responseText = "FULL";
                break;
            case 2:
                responseText = "AVERAGE";
                break;
        }
        request->send(200, "text/plain", responseText); });
    webServer.on("/stats/waterlevel", HTTP_GET, [](AsyncWebServerRequest *request)
                 {
        Serial.println("Acesso a rota: /stats/waterlevel");
        String responseText = String(waterLevelPercentage, 2);
        request->send(200, "text/plain", responseText); });
    Serial.println("Iniciando Servidor");
    webServer.begin();
}

// --------------- Funçoes de Fila -------------------
void initializeWaterLevelHistoryQueue()
{
    for (int index = 0; index < HISTORY_QUEUE_SIZE; index++)
    {
        waterLevelHistoryQueue[index] = -1.f;
    }
}

bool isQueueFull(float *queue, int queueSize)
{
    for (int index = 0; index < HISTORY_QUEUE_SIZE; index++)
    {
        if (waterLevelHistoryQueue[index] == -1.f)
            return true;
    }
    return false;
}

void pushQueue(float *queue, int queueSize, float element)
{
    if (isQueueFull(queue, queueSize))
        return;
    for (int index = 0; index < queueSize; index++)
    {
        if (queue[index] == -1.f)
            queue[index] = element;
    }
}

float popQueue(float *queue, int queueSize)
{
    if (queue[0] == -1.f)
        return -1.f;
    float poppedElement = queue[0];
    shiftQueueLeft(queue, queueSize);
    return poppedElement;
}

void shiftQueueLeft(float *array, int arraySize)
{
    for (int index = 0; index < arraySize; index++)
    {
        if (index == arraySize)
            array[index] = -1;
        array[index] = array[index + 1];
    }
}

void printArray(float *array, int arraySize)
{
    for (int index = 0; index < arraySize; index++)
    {
        Serial.printf("%f ", array[index]);
    }
    Serial.println();
}
// ----------------------------------------------------
