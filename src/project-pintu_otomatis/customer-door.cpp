
#include "customer-door.h"

MFRC522 rfid(SS_PIN, RST_PIN);
HTTPClient http;

// Flag untuk menandakan apakah sudah terhubung ke WiFi
bool reconnected = true;
// Asumsikan ruangan yang akan diakses adalah room 101
uint8_t roomNumber = 101;
// Flag untuk mode perekaman data
bool RECORDMODE = false;

// URL untuk cek akses. URL adalah script makro spreadsheet Google Apps Script yang sudah di-deploy sebagai web app.
const char* SERVER_URL = "https://script.google.com/macros/library/d/1BxkSqYyzzPTK_-Hk4rBHUh4t_EVHeCRiuYLa52Xx3Y0suTrfJzpXVgbB/1";

bool cekKoneksiWiFi() {
    if (reconnected) {
        WiFi.setHostname("Dzuu-ESP32");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);
        Serial.print("Connecting to WiFi");
        unsigned long startAttemptTime = millis();
        const unsigned long wifiTimeout = 10000; // 10 seconds timeout   

        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout) {
            Serial.print(".");
            delay(100);
        }
        reconnected = false;
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\nWiFi connection failed!");
        reconnected = true;
        return false;
    } else {
        return true;
    }   
}

String BacaFingerprint() {
    
}

String BacaRFID() {
    // cek apakah ada kartu yang terdeteksi. Jika tidak, keluar dari fungsi dan kembalikan string kosong.
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return String("");
    }

    char rfidBuffer[rfid.uid.size * 2 + 1]; // +1 untuk karakter null-terminator
    for (unsigned int i = 0; i < rfid.uid.size; i++) {
        byte nib1 = (rfid.uid.uidByte[i] >> 4) & 0x0F;  // ambil bit atas 4
        byte nib2 = (rfid.uid.uidByte[i] >> 0) & 0x0F;  // ambil bit bawah 4

        rfidBuffer[i * 2 + 0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
        rfidBuffer[i * 2 + 1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
        
    }
    rfidBuffer[rfid.uid.size * 2] = '\0';   //

    // Hentikan komunikasi dengan kartu
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    return String(rfidBuffer);
}

void SimpanData(String fingerprintData, String rfidData) {
    String url = String(SERVER_URL) + "?room=" + String(roomNumber);

    if (fingerprintData.length() > 0) {
        url += "&fingerprint=" + fingerprintData;
    }
    if (rfidData.length() > 0) {
        url += "&uid=" + rfidData;
    }

    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument <200> jsonDoc;
    jsonDoc["fingerprint"] = fingerprintData;
    jsonDoc["uid"] = rfidData;
    jsonDoc["room"] = roomNumber;

    String body;
    serializeJson(jsonDoc, body);

    int httpResponseCode = http.POST(body);

    if (httpResponseCode > 0) {
        String payload = http.getString();
        Serial.print("Respons dari server: ");
        Serial.println(payload);
    } else {
        Serial.print("Request HTTP gagal: ");
        Serial.println(httpResponseCode);
    }
    http.end();
}

bool CekAkses(String fingerprintData, String rfidData) {
    String url = String(SERVER_URL) + "?room=" + String(roomNumber) + "&uid=" + rfidData;

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        String payload = http.getString();
        Serial.print("Response from server: ");
        Serial.println(payload);

        http.end();

        if (payload.indexOf("GRANTED") > 0) {
            return true;
        } else {
            return false;
        }
    } else {
        Serial.print("Request HTTP gagal: ");
        Serial.println(httpResponseCode);
        http.end();
        return false;
    }
}

bool TimeoutAkses(unsigned long startTime) {
    const unsigned long aksesTimeout = 5000; // 5 detik timeout
    if (millis() - startTime >= aksesTimeout) {
        Serial.println("Timeout akses! Pintu ditutup kembali.");
        return true;
    }
    return false;
}

void BukaPintu() {
    digitalWrite(SELENOID_PIN, HIGH); // Aktifkan solenoid untuk membuka pintu
}

/** @brief Fungsi untuk mendapatkan nilai RGB dari warna */
const uint8_t* getRGBValues(LEDColor color) {
    static const uint8_t RED[3] = {255, 0, 0};
    static const uint8_t YELLOW[3] = {255, 255, 0};
    static const uint8_t BLUE[3] = {0, 0, 255};
    static const uint8_t GREEN[3] = {0, 255, 0};
    static const uint8_t WHITE[3] = {255, 255, 255};
    
    switch (color) {
        case LEDColor::RED: return RED;
        case LEDColor::YELLOW: return YELLOW;
        case LEDColor::BLUE: return BLUE;
        case LEDColor::GREEN: return GREEN;
        case LEDColor::WHITE: return WHITE;
        default: return WHITE;
    }
}

void IndikatorLEDRGB(bool status, LEDColor color = LEDColor::WHITE) {
    if (status) {
        const uint8_t* rgbValues = getRGBValues(color);
        digitalWrite(RGB_LED_R, rgbValues[0]);  // Komponen Merah
        digitalWrite(RGB_LED_G, rgbValues[1]);  // Komponen Hijau
        digitalWrite(RGB_LED_B, rgbValues[2]);  // Komponen Biru
    } else {
        digitalWrite(RGB_LED_R, LOW);
        digitalWrite(RGB_LED_G, LOW);
        digitalWrite(RGB_LED_B, LOW);
    }
}

