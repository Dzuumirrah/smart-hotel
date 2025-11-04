// || ==================================================================== ||
// ||   Berisi konfirgurasi umum seperti pinout, deklarasi, dan lain-lain  ||
// || ==================================================================== ||
#ifndef CONFIG_H
#define CONFIG_H

#include <MFRC522.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// || ==================================================================== ||
// ||                               PINOUT                                 ||
// || ==================================================================== ||
#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <HTTPClient.h>
#define MISO_PIN 19
#define MOSI_PIN 23
#define SCK_PIN 18
// RFID ---------------------------------------------------
#define SS_PIN_RFID 13    // SDA pin
#define RST_PIN_RFID 22   // RST pin
// Fingerprint ---------------------------------------------
#define FINGERPRINT_RX_PIN 16
#define FINGERPRINT_TX_PIN 17
// Selenoid -----------------------------------------------
#define SELENOID_PIN 15
// ESP8266 AT Commands Constants
#define AT_BAUDRATE 115200
#define AT_TIMEOUT 10000
// RGB LED pins - Disabled due to hardware unavailability
// #define RGB_LED_R 4  // Red pin
// #define RGB_LED_G 5  // Green pin
// #define RGB_LED_B 21 // Blue pin

#elif defined(ARDUINO_ARCH_AVR)
#define USE_WIFI_ESP8266   // Gunakan modul WiFi ESP8266
// RFID uses hardware SPI pins
#define MOSI_PIN MOSI     // Pin 11 on Arduino UNO
#define MISO_PIN MISO     // Pin 12 on Arduino UNO
#define SCK_PIN SCK       // Pin 13 on Arduino UNO
// RFID ---------------------------------------------------
#define SS_PIN_RFID 10    // SDA pin
#define RST_PIN_RFID 9   // RST pin
// Fingerprint ---------------------------------------------
#define FINGERPRINT_TX_PIN A1
#define FINGERPRINT_RX_PIN A2  
// ESP8266 Wifi module------------------------------------------------
#define ESP8266_RX_PIN 2
#define ESP8266_TX_PIN 3
// Selenoid -----------------------------------------------
#define SELENOID_PIN A0

// ESP8266 AT Commands Constants
#define AT_BAUDRATE 115200
#define AT_TIMEOUT 10000

#else
  #error "Board not supported! Please use ESP32 DevKit or Arduino UNO."
#endif

// || ==================================================================== ||
// ||                          VARIABEL GLOBAL                             ||
// || ==================================================================== ||

#ifdef USE_WIFI_ESP8266
    /**
     * @brief Deklarasi objek SoftwareSerial untuk komunikasi dengan modul WiFi ESP8266.
     */
    extern SoftwareSerial WifiSerial; // RX, TX
#endif

/**
 * @brief Deklarasi objek RFID.
 */
extern MFRC522 rfid;

#if defined(ARDUINO_ARCH_ESP32)
/**
 * @brief Deklarasi objek HTTPClient - ESP32 only.
 */
extern HTTPClient http;
#endif

/**
 * @brief Deklarasi objek sensor fingerprint.
 */
extern Adafruit_Fingerprint finger;

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

/** @brief Enumerasi untuk warna LED RGB - Disabled due to hardware unavailability */
// enum class LEDColor {
//     RED,
//     YELLOW,
//     BLUE,
//     GREEN,
//     WHITE
// };

/** 
 * @brief Menentukan apakah sistem dalam mode perekaman data atau tidak.
 */
extern bool RECORDMODE;

#endif // CONFIG_H