#include "DHT.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// #define BLYNK_PRINT Serial
// ---------------- DHT Sensor ----------------
#define DHTPIN D5
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int dhtDelay = 2000;
unsigned long dhtTime = 0;
String humidity = "";
String temperature = "";

void dht_sensor() {
  if (millis() >= dhtTime + dhtDelay) {
    humidity = (String)dht.readHumidity();
    temperature = (String)dht.readTemperature();

    // Serial.println("Humidity : " + humidity);
    // Serial.println("Temperature : " + temperature);

    dhtTime += dhtDelay;
  }
}
// --------------------------------------------

// ---------------- Soil Moisture Sensor ----------------
int soilMoistureDelay = 1000;
unsigned long soilMoistureTime = 0;
String soilMoisture_digital = "";
String soilMoisture_analog = "";
int indexSoilMoistureSensor = 0;

void soilMoisture_sensor() {
  if (millis() >= soilMoistureTime + soilMoistureDelay) {
    soilMoisture_digital = (String)!digitalRead(D6);
    soilMoisture_analog = (String)map(analogRead(A0), 0, 1024, 100, 0);
    // Serial.println("Soil Moisture_digital" + soilMoisture_digital);
    // Serial.println("Soil Moisture_analog" + soilMoisture_analog);

    soilMoistureTime += soilMoistureDelay;
  }
}

// ------------------------------------------------------


// ---------------- Print Data ----------------
int printDataDelay = 2000;
unsigned long printDataTime = 0;

void printData() {
  if (millis() >= printDataTime + printDataDelay) {
    Serial.println("Temperature : " + temperature);
    Serial.println("Humididy : " + humidity);
    Serial.println("Soil moisture status : " + soilMoisture_digital);
    Serial.println("Soil moisture level : " + soilMoisture_analog);
    Serial.println();

    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V2, humidity);
    Blynk.virtualWrite(V3, soilMoisture_analog);

    printDataTime += printDataDelay;
  }
}

// -------------------------------------------------

void setup() {
  Serial.begin(115200);
  pinMode(D6, INPUT);
  dht.begin();
  Blynk.begin("3qA0yxukgG5BgUuw8uI0rFD5qggG1Ehn", "OPPO Reno4", "0981966915");
}

void loop() {
  Blynk.run();
  dht_sensor();
  soilMoisture_sensor();
  printData();
}
