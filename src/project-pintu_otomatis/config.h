// || ==================================================================== ||
// ||   Berisi konfirgurasi umum seperti pinout, deklarasi, dan lain-lain  ||
// || ==================================================================== ||
#ifndef CONFIG_H
#define CONFIG_H

#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// || ==================================================================== ||
// ||                               PINOUT                                 ||
// || ==================================================================== ||

#define SS_PIN 5
#define RST_PIN 4
#define SELENOID_PIN 15
#define RGB_LED_R 18
#define RGB_LED_G 19
#define RGB_LED_B 21

// || ==================================================================== ||
// ||                          VARIABEL GLOBAL                             ||
// || ==================================================================== ||

/** 
 * @brief Deklarasi objek RFID.
 */
extern MFRC522 rfid;

/**
 * @brief Deklarasi objek HTTPClient.
 */
extern HTTPClient http;

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

/** @brief Enumerasi untuk warna LED RGB */
enum class LEDColor {
    RED,
    YELLOW,
    BLUE,
    GREEN,
    WHITE
};

/** 
 * @brief Menentukan apakah sistem dalam mode perekaman data atau tidak.
 */
extern bool RECORDMODE;

#endif // CONFIG_H