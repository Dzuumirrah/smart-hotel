#include "customer-door.h"

MFRC522 rfid(SS_PIN_RFID, RST_PIN_RFID);

#if defined(ARDUINO_ARCH_ESP32)
    HTTPClient http;
#endif

// Global SoftwareSerial instances
#if defined(ARDUINO_ARCH_AVR)
    SoftwareSerial esp8266(ESP8266_RX_PIN, ESP8266_TX_PIN);
#endif
SoftwareSerial fpSerial(FINGERPRINT_RX_PIN, FINGERPRINT_TX_PIN);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fpSerial);

// Flag untuk menandakan apakah sudah terhubung ke WiFi
bool reconnected = true;
// Asumsikan ruangan yang akan diakses adalah room 101
uint8_t roomNumber = 101;
// Flag untuk mode perekaman data
bool RECORDMODE = false;

// URL untuk cek akses. URL adalah script makro spreadsheet Google Apps Script yang sudah di-deploy sebagai web app.
const char* SERVER_URL = "https://script.google.com/macros/library/d/1BxkSqYyzzPTK_-Hk4rBHUh4t_EVHeCRiuYLa52Xx3Y0suTrfJzpXVgbB/1";

// flag untuk meminta password pada saat pertama kali mode perekaman aktif
bool NEEDPASS = true;
// flag status akses diterima
bool aksesDiterima = false;


void setup_hilman() {
    // Inisialisasi khusus untuk project Hilman
    Serial.begin(115200);
    Serial.println("Projek Pengaman Pintu dengan Fingerprint, RFID, dan Keypad");
    #ifdef USE_WIFI_ESP8266
        SoftwareSerial WifiSerial(ESP8266_RX_PIN, ESP8266_TX_PIN);
    #endif
    // Initialize SPI for RFID - Arduino UNO uses fixed hardware pins
    SPI.begin();  
    // Inisialisasi RFID
    rfid.PCD_Init();

    // Inisialisasi sensor fingerprint
    fpSerial.begin(57600);
    if (finger.verifyPassword()) {
        Serial.println("Found fingerprint sensor!");
    } else {
        Serial.println("Did not find fingerprint sensor :(");
    }

    // RGB LED initialization disabled due to hardware unavailability
    // pinMode(RGB_LED_R, OUTPUT);
    // pinMode(RGB_LED_G, OUTPUT);
    // pinMode(RGB_LED_B, OUTPUT);

    //  inisialisasi pin kunci selenoid
    pinMode(SELENOID_PIN, OUTPUT);
    // IndikatorLEDRGB(false, LEDColor::WHITE); // Matikan indikator LED RGB - Disabled
}

void loop_hilman() {
  digitalWrite(SELENOID_PIN, LOW); // Pastikan selenoid dalam keadaan mati

  // Cek koneksi WiFi
  if (!cekKoneksiWiFi()) {
    Serial.println("WiFi tidak terhubung. Mengulang koneksi...");
    delay(3000);
    // IndikatorLEDRGB(true, LEDColor::RED); // Indikator merah untuk koneksi gagal - Disabled  
    return;
  }
  // IndikatorLEDRGB(true, LEDColor::WHITE); // Disabled
  
  // Meminta password jika mode perekaman aktif dan pertama kali
  if (RECORDMODE && NEEDPASS) {
    Serial.println("Mode Perekaman Data Aktif. Silakan masukkan password untuk memulai perekaman.");
    String password = Serial.readStringUntil('\n');
    // Remove any whitespace
    password.trim();
    
    // Using constant-time comparison for security
    bool passwordCorrect = true;
    const char* correct = "admin123";
    if (password.length() != strlen(correct)) {
      passwordCorrect = false;
    } else {
      for (size_t i = 0; i < password.length(); i++) {
        if (password[i] != correct[i]) {
          passwordCorrect = false;
        }
      }
    }

    if (passwordCorrect) {
      Serial.println("Password benar. Silakan tempelkan RFID atau letakkan jari pada sensor fingerprint untuk perekaman.");
    } else {
      Serial.println("Password salah.");
      delay(3000); // Delay to prevent brute force
      return;
    }
  }
  //Ambil data RFID
  String UID = BacaRFID();
  String fingerprint = BacaFingerprint();
  

  if (UID != "") {  // Jika data RFID terbaca
    Serial.print("Data RFID Terbaca: ");
    Serial.println(UID);
    // IndikatorLEDRGB(true, LEDColor::BLUE); // Indikator biru untuk RFID terbaca - Disabled
    
    // Kondisi perekaman data
    if (RECORDMODE) {
      SimpanData("", UID);
      Serial.println("Data RFID telah disimpan.");
      Serial.println("Ingin merekam data lagi? (y/n)");
      // IndikatorLEDRGB(true, LEDColor::YELLOW); // Matikan indikator LED RGB - Disabled
      String response = Serial.readStringUntil('\n');
      if (response != "y") {
        RECORDMODE = false; // Matikan mode perekaman jika tidak ingin merekam lagi
        NEEDPASS = true; // Reset flag password
        return;
      }
      NEEDPASS = false;
    }

    // Kondisi verifikasi akses
    Serial.print("Mencoba verifikasi akses...");
    aksesDiterima = CekAkses("", UID);
  } else if (fingerprint != "") { // Jika data fingerprint terbaca
    Serial.print("Data Fingerprint Terbaca: ");
    Serial.println(fingerprint);
    // IndikatorLEDRGB(true, LEDColor::BLUE); // Indikator biru untuk fingerprint terbaca - Disabled

    // Kondisi perekaman data
    if (RECORDMODE) {
      SimpanData(fingerprint, "");
      Serial.println("Data Fingerprint telah disimpan.");
      Serial.println("Ingin merekam data lagi? (y/n)");
    //   IndikatorLEDRGB(true, LEDColor::YELLOW);
      String response = Serial.readStringUntil('\n');
      if (response != "y") {
        RECORDMODE = false; // Matikan mode perekaman jika tidak ingin merekam lagi
        NEEDPASS = true; // Reset flag password
        return;
      }
      NEEDPASS = false;
    }
    // kondisi cek akses
    // IndikatorLEDRGB(true, LEDColor::BLUE); // Matikan indikator LED RGB
    Serial.print("Mencoba verifikasi akses...");
    aksesDiterima =  CekAkses(fingerprint, "");
  } else {  // Jika tidak ada data yang terbaca, program kembali ke awal loop
    return;
  }

  // Penggerakan pintu saat akses diterima
  if (aksesDiterima) {
    Serial.println("Akses Diterima. Pintu Dibuka.");
    // IndikatorLEDRGB(true, LEDColor::GREEN); // Indikator hijau untuk akses diterima - Disabled
    // timeout pintu dibuka agar selenoida tidak terus menerus aktif
    unsigned long startTimeout = millis();
    while (!TimeoutAkses(startTimeout)) {
      BukaPintu();
    }

  } else {
    Serial.println("Akses Ditolak. Coba gunakan metode lain atau akses ke ruangan yang sesuai.");
    // IndikatorLEDRGB(true, LEDColor::RED); // Indikator merah untuk akses ditolak - Disabled
    delay(3000); // Tunda selama 3 detik untuk menampilkan status akses
  }

}


#if defined(ARDUINO_ARCH_ESP32)
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

#else // For Arduino UNO with ESP8266
static bool esp8266_initialized = false;

bool cekKoneksiWiFi() {
    if (!esp8266_initialized) {
        esp8266.begin(AT_BAUDRATE);
        esp8266_initialized = true;
        // Reset ESP8266
        esp8266.println("AT+RST");
        delay(1000);
        while (esp8266.available()) {
            esp8266.read(); // Clear buffer
        }
    }

    if (reconnected) {
        esp8266.println("AT");
        delay(500);
        if (!esp8266.find("OK")) return false;

        esp8266.println("AT+CWMODE=1");
        delay(500);
        if (!esp8266.find("OK")) return false;

        String cmd = "AT+CWJAP=\"";
        cmd += WIFI_SSID;
        cmd += "\",\"";
        cmd += WIFI_PASSWORD;
        cmd += "\"";
        esp8266.println(cmd);
        
        unsigned long startTime = millis();
        bool connectionSuccess = false;
        while (millis() - startTime < AT_TIMEOUT && !connectionSuccess) {
            if (esp8266.available()) {
                String response = esp8266.readStringUntil('\n');
                if (response.indexOf("OK") != -1) {
                    reconnected = false;
                    connectionSuccess = true;
                } else if (response.indexOf("ERROR") != -1 || response.indexOf("FAIL") != -1) {
                    break;
                }
            }
            delay(100); // Prevent tight loop
        }
        return connectionSuccess;
        
        Serial.println("WiFi connection failed!");
        reconnected = true;
        return false;
    }

    // Check if still connected
    esp8266.println("AT+CIPSTATUS");
    delay(500);
    return esp8266.find("STATUS:2") || esp8266.find("STATUS:3") || esp8266.find("STATUS:4");
}
#endif

String BacaFingerprint() {
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK) {
        switch (p) {
            case FINGERPRINT_NOFINGER:
                // Silent fail - no finger present
                break;
            case FINGERPRINT_PACKETRECIEVEERR:
                Serial.println("Komunikasi dengan sensor gagal");
                break;
            case FINGERPRINT_IMAGEFAIL:
                Serial.println("Error mengambil gambar");
                break;
            default:
                Serial.println("Error tidak dikenal");
                break;
        }
        return String("");
    }

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) {
        switch (p) {
            case FINGERPRINT_IMAGEMESS:
                Serial.println("Gambar terlalu berantakan");
                break;
            case FINGERPRINT_PACKETRECIEVEERR:
                Serial.println("Komunikasi dengan sensor gagal");
                break;
            case FINGERPRINT_FEATUREFAIL:
                Serial.println("Tidak dapat menemukan fitur sidik jari");
                break;
            case FINGERPRINT_INVALIDIMAGE:
                Serial.println("Gambar tidak valid");
                break;
            default:
                Serial.println("Error tidak dikenal");
                break;
        }
        return String("");
    }

    p = finger.fingerSearch();
    if (p != FINGERPRINT_OK) {
        if (p == FINGERPRINT_NOTFOUND) {
            Serial.println("Sidik jari tidak terdaftar");
        } else {
            Serial.println("Error dalam pencarian sidik jari");
        }
        return String("");
    }

    // Found a match!
    char fingerprintBuffer[8];  // Buffer untuk menyimpan ID dalam format hex
    sprintf(fingerprintBuffer, "%04X", finger.fingerID);  // Konversi ID ke hex string
    
    return String(fingerprintBuffer);
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

#if defined(ARDUINO_ARCH_ESP32)
void SimpanData(String fingerprintData, String rfidData) {
    if (fingerprintData.length() == 0 && rfidData.length() == 0) {
        Serial.println("Error: Tidak ada data untuk disimpan");
        return;
    }

    String url = String(SERVER_URL) + "?room=" + String(roomNumber);

    if (fingerprintData.length() > 0) {
        url += "&fingerprint=" + fingerprintData;
    }
    if (rfidData.length() > 0) {
        url += "&uid=" + rfidData;
    }

    http.begin(url);
    http.setTimeout(10000); // 10 detik timeout
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> jsonDoc;
    jsonDoc["fingerprint"] = fingerprintData;
    jsonDoc["uid"] = rfidData;
    jsonDoc["room"] = roomNumber;
    jsonDoc["timestamp"] = millis();

    String body;
    serializeJson(jsonDoc, body);

    int httpResponseCode = http.POST(body);

    if (httpResponseCode > 0) {
        String payload = http.getString();
        if (payload.length() > 0) {
            StaticJsonDocument<200> responseDoc;
            DeserializationError error = deserializeJson(responseDoc, payload);
            
            if (!error) {
                const char* status = responseDoc["status"];
                const char* message = responseDoc["message"];
                Serial.print("Status: ");
                Serial.println(status);
                Serial.print("Message: ");
                Serial.println(message);
            } else {
                Serial.print("Response parsing failed: ");
                Serial.println(error.c_str());
            }
        }
    } else {
        Serial.print("Request HTTP gagal: ");
        Serial.println(httpResponseCode);
        Serial.println("Mencoba sekali lagi...");
        
        delay(1000);
        httpResponseCode = http.POST(body);
        if (httpResponseCode <= 0) {
            Serial.println("Gagal setelah percobaan ulang");
        }
    }
    http.end();
}

#else
void SimpanData(String fingerprintData, String rfidData) {
    if (fingerprintData.length() == 0 && rfidData.length() == 0) {
        Serial.println("Error: Tidak ada data untuk disimpan");
        return;
    }

    StaticJsonDocument<200> jsonDoc;
    jsonDoc["fingerprint"] = fingerprintData;
    jsonDoc["uid"] = rfidData;
    jsonDoc["room"] = roomNumber;
    jsonDoc["timestamp"] = millis();

    String body;
    serializeJson(jsonDoc, body);

    // Extract host and path from SERVER_URL
    String url = SERVER_URL;
    url.replace("https://", "");
    int pathStart = url.indexOf('/');
    String host = url.substring(0, pathStart);
    String path = url.substring(pathStart);
    path += "?room=" + String(roomNumber);
    if (fingerprintData.length() > 0) {
        path += "&fingerprint=" + fingerprintData;
    }
    if (rfidData.length() > 0) {
        path += "&uid=" + rfidData;
    }

    // Connect to server
    String cmd = "AT+CIPSTART=\"SSL\",\"";
    cmd += host;
    cmd += "\",443";
    esp8266.println(cmd);
    if (!esp8266.find("OK")) {
        Serial.println("Connection failed");
        return;
    }

    // Prepare HTTP POST request
    String httpRequest = "POST ";
    httpRequest += path;
    httpRequest += " HTTP/1.1\r\n";
    httpRequest += "Host: ";
    httpRequest += host;
    httpRequest += "\r\n";
    httpRequest += "Content-Type: application/json\r\n";
    httpRequest += "Content-Length: ";
    httpRequest += body.length();
    httpRequest += "\r\n\r\n";
    httpRequest += body;

    // Send request length
    cmd = "AT+CIPSEND=";
    cmd += httpRequest.length();
    esp8266.println(cmd);
    if (!esp8266.find(">")) {
        Serial.println("Failed to send request");
        return;
    }

    // Send request
    esp8266.print(httpRequest);
    if (esp8266.find("SEND OK")) {
        String response = "";
        unsigned long timeout = millis();
        while (millis() - timeout < AT_TIMEOUT) {
            if (esp8266.available()) {
                response += (char)esp8266.read();
            }
        }

        // Parse response if it contains JSON
        if (response.indexOf("{") != -1) {
            int jsonStart = response.indexOf("{");
            int jsonEnd = response.lastIndexOf("}") + 1;
            String jsonStr = response.substring(jsonStart, jsonEnd);

            StaticJsonDocument<200> responseDoc;
            DeserializationError error = deserializeJson(responseDoc, jsonStr);
            
            if (!error) {
                const char* status = responseDoc["status"];
                const char* message = responseDoc["message"];
                Serial.print("Status: ");
                Serial.println(status);
                Serial.print("Message: ");
                Serial.println(message);
            }
        }
    }

    // Close connection
    esp8266.println("AT+CIPCLOSE");
}
#endif

#if defined(ARDUINO_ARCH_ESP32)
bool CekAkses(String fingerprintData, String rfidData) {
    if (fingerprintData.length() == 0 && rfidData.length() == 0) {
        Serial.println("Error: Tidak ada credential untuk dicek");
        return false;
    }

    // Buat URL dengan parameter yang sesuai
    String url = String(SERVER_URL) + "?room=" + String(roomNumber);
    
    // Tambahkan parameter sesuai data yang tersedia
    if (fingerprintData.length() > 0) {
        url += "&fingerprint=" + fingerprintData;
    }
    if (rfidData.length() > 0) {
        url += "&uid=" + rfidData;
    }

    http.begin(url);
    http.setTimeout(5000); // 5 detik timeout
    int maxRetries = 2;
    int attempt = 0;
    bool accessGranted = false;

    while (attempt < maxRetries && !accessGranted) {
        int httpResponseCode = http.GET();
        
        if (httpResponseCode > 0) {
            String payload = http.getString();
            
            if (payload.length() > 0) {
                StaticJsonDocument<200> responseDoc;
                DeserializationError error = deserializeJson(responseDoc, payload);
                
                if (!error) {
                    const char* status = responseDoc["status"];
                    const char* message = responseDoc["message"];
                    
                    if (strcmp(status, "GRANTED") == 0) {
                        Serial.print("Akses diterima: ");
                        Serial.println(message);
                        accessGranted = true;
                    } else {
                        Serial.print("Akses ditolak: ");
                        Serial.println(message);
                    }
                } else {
                    Serial.print("Response parsing failed: ");
                    Serial.println(error.c_str());
                }
            }
            break; // Exit if we got any valid response
        } else {
            Serial.print("Request HTTP gagal (attempt ");
            Serial.print(attempt + 1);
            Serial.print("): ");
            Serial.println(httpResponseCode);
            
            if (attempt < maxRetries - 1) {
                delay(1000); // Wait before retry
            }
        }
        attempt++;
    }

    http.end();
    return accessGranted;
}

#else
bool CekAkses(String fingerprintData, String rfidData) {
    if (fingerprintData.length() == 0 && rfidData.length() == 0) {
        Serial.println("Error: Tidak ada credential untuk dicek");
        return false;
    }

    // Extract host and path from SERVER_URL
    String url = SERVER_URL;
    url.replace("https://", "");
    int pathStart = url.indexOf('/');
    String host = url.substring(0, pathStart);
    String path = url.substring(pathStart);
    path += "?room=" + String(roomNumber);
    if (fingerprintData.length() > 0) {
        path += "&fingerprint=" + fingerprintData;
    }
    if (rfidData.length() > 0) {
        path += "&uid=" + rfidData;
    }

    bool accessGranted = false;
    int maxRetries = 2;
    int attempt = 0;

    while (attempt < maxRetries && !accessGranted) {
        // Connect to server
        String cmd = "AT+CIPSTART=\"SSL\",\"";
        cmd += host;
        cmd += "\",443";
        esp8266.println(cmd);
        if (!esp8266.find("OK")) {
            Serial.println("Connection failed");
            attempt++;
            continue;
        }

        // Prepare HTTP GET request
        String httpRequest = "GET ";
        httpRequest += path;
        httpRequest += " HTTP/1.1\r\n";
        httpRequest += "Host: ";
        httpRequest += host;
        httpRequest += "\r\nConnection: close\r\n\r\n";

        // Send request length
        cmd = "AT+CIPSEND=";
        cmd += httpRequest.length();
        esp8266.println(cmd);
        if (!esp8266.find(">")) {
            Serial.println("Failed to send request");
            attempt++;
            continue;
        }

        // Send request
        esp8266.print(httpRequest);
        if (esp8266.find("SEND OK")) {
            String response = "";
            unsigned long timeout = millis();
            bool endOfResponse = false;
            
            while (millis() - timeout < AT_TIMEOUT && !endOfResponse) {
                if (esp8266.available()) {
                    char c = esp8266.read();
                    response += c;
                    
                    // Check for end of response
                    if (response.length() > 4) {
                        if (response.endsWith("\r\n\r\n")) {
                            endOfResponse = true;
                        }
                    }
                }
                yield(); // Prevent watchdog reset
            }

            // Parse response if it contains JSON
            if (response.indexOf("{") != -1) {
                int jsonStart = response.indexOf("{");
                int jsonEnd = response.lastIndexOf("}") + 1;
                String jsonStr = response.substring(jsonStart, jsonEnd);

                #if defined(ARDUINO_ARCH_ESP32)
                    StaticJsonDocument<512> responseDoc;
                #else
                    StaticJsonDocument<128> responseDoc; // Smaller for Arduino UNO
                #endif
                DeserializationError error = deserializeJson(responseDoc, jsonStr);
                
                if (!error) {
                    const char* status = responseDoc["status"];
                    const char* message = responseDoc["message"];
                    
                    if (strcmp(status, "GRANTED") == 0) {
                        Serial.print("Akses diterima: ");
                        Serial.println(message);
                        accessGranted = true;
                    } else {
                        Serial.print("Akses ditolak: ");
                        Serial.println(message);
                    }
                }
            }
        }

        // Close connection
        esp8266.println("AT+CIPCLOSE");
        attempt++;
        
        if (!accessGranted && attempt < maxRetries) {
            delay(1000); // Wait before retry
        }
    }

    return accessGranted;
}
#endif

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

/** @brief Fungsi untuk mendapatkan nilai RGB dari warna - Disabled due to hardware unavailability */
/*
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
*/