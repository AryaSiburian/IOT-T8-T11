#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
#include <params.h>
#include <secret.h>
#include <UrusanWiFi.h>
#include <UrusanBuzzer.h>
#include <UrusanIoT.h>
#include <ArduinoJson.h>


// Tulis parameter lokal mu di sini
const uint8_t pinINA = 22;
const uint8_t pinINB = 23;
const uint8_t pinBuzzer = 13;
String clientId = String(CURRENT_FIRMWARE_TITLE) + String(NIM) + String("-") + String(random(1, 9999)); // Client ID untuk MQTT
unsigned long lastSent = 0;
bool flagUpdate = false;
bool lastState = 0;
uint8_t lastSpeed = 0;
bool lastDirection = 0;


// Tulis instansiasi objek global mu di sini
UrusanWiFi urusanWiFi(wssid, wpass);
UrusanBuzzer buzzer(pinBuzzer);
UrusanIoT urusanIoT(tbAddr, tbPort, clientId.c_str(), "latihan", "bukanbudakai");


// Tulis deklarasi fungsi mu di sini:
void cetakIdentitasDeveloper();
void i2cScanner();
void penangkapPesan(String topic, String message);
void subscribe();
void kontrolBlower(bool state, uint8_t speed, bool direction);


#endif
