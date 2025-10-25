#include <Arduino.h>

// Pilih project yang mau di-compile
#define PROJECT_HILMAN
// #define PROJECT_MERLIN
// #define PROJECT_GALIH
// #define PROJECT_FARREL
// #define PROJECT_UNKNOWN

#ifdef PROJECT_HILMAN
#include "project-pintu_otomatis/customer-door.h"
void setup() {
    // Inisialisasi khusus untuk project Hilman
    Serial.begin(115200);
    Serial.println("Projek Pengaman Pintu dengan Fingerprint, RFID, dan Keypad");
    
    // Inisialisasi RFID
    rfid.PCD_Init();

    //  inisialisasi pin kunci selenoid
    pinMode(SELENOID_PIN, OUTPUT);
    IndikatorLEDRGB(false, LEDColor::WHITE); // Matikan indikator LED RGB
}

// flag untuk meminta password pada saat pertama kali mode perekaman aktif
bool NEEDPASS = true;
// flag status akses diterima
bool aksesDiterima = false;

void loop() {
  digitalWrite(SELENOID_PIN, LOW); // Pastikan selenoid dalam keadaan mati

  // Cek koneksi WiFi
  if (!cekKoneksiWiFi()) {
    Serial.println("WiFi tidak terhubung. Mengulang koneksi...");
    delay(3000);
    IndikatorLEDRGB(true, LEDColor::RED); // Indikator merah untuk koneksi gagal  
    return;
  }
  IndikatorLEDRGB(true, LEDColor::WHITE);
  
  // Meminta password jika mode perekaman aktif dan pertama kali
  if (RECORDMODE && NEEDPASS) {
    Serial.println("Mode Perekaman Data Aktif. Silakan masukkan password untuk memulai perekaman.");
    String password = "";
    Serial.readStringUntil('\n');
    if (password == "admin123") {
      Serial.println("Password benar. Silakan tempelkan RFID atau letakkan jari pada sensor fingerprint untuk perekaman.");
    } else {
      Serial.println("Password salah.");
      return;
    }
  }
  //Ambil data RFID
  String UID = BacaRFID();
  String fingerprint = BacaFingerprint();
  

  if (UID != "") {  // Jika data RFID terbaca
    Serial.print("Data RFID Terbaca: ");
    Serial.println(UID);
    IndikatorLEDRGB(true, LEDColor::BLUE); // Indikator biru untuk RFID terbaca
    
    // Kondisi perekaman data
    if (RECORDMODE) {
      SimpanData("", UID);
      Serial.println("Data RFID telah disimpan.");
      Serial.println("Ingin merekam data lagi? (y/n)");
      IndikatorLEDRGB(true, LEDColor::YELLOW); // Matikan indikator LED RGB
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
    IndikatorLEDRGB(true, LEDColor::BLUE); // Indikator biru untuk fingerprint terbaca

    // Kondisi perekaman data
    if (RECORDMODE) {
      SimpanData(fingerprint, "");
      Serial.println("Data Fingerprint telah disimpan.");
      Serial.println("Ingin merekam data lagi? (y/n)");
      IndikatorLEDRGB(true, LEDColor::YELLOW);
      String response = Serial.readStringUntil('\n');
      if (response != "y") {
        RECORDMODE = false; // Matikan mode perekaman jika tidak ingin merekam lagi
        NEEDPASS = true; // Reset flag password
        return;
      }
      NEEDPASS = false;
    }
    // kondisi cek akses
    IndikatorLEDRGB(true, LEDColor::BLUE); // Matikan indikator LED RGB
    Serial.print("Mencoba verifikasi akses...");
    aksesDiterima =  CekAkses(fingerprint, "");
  } else {  // Jika tidak ada data yang terbaca, program kembali ke awal loop
    return;
  }

  // Penggerakan pintu saat akses diterima
  if (aksesDiterima) {
    Serial.println("Akses Diterima. Pintu Dibuka.");
    IndikatorLEDRGB(true, LEDColor::GREEN); // Indikator hijau untuk akses diterima
    // timeout pintu dibuka agar selenoida tidak terus menerus aktif
    unsigned long startTimeout = millis();
    while (!TimeoutAkses(startTimeout)) {
      BukaPintu();
    }

  } else {
    Serial.println("Akses Ditolak. Coba gunakan metode lain\\
      atau akses ke ruangan yang sesuai.");
    IndikatorLEDRGB(true, LEDColor::RED); // Indikator merah untuk akses ditolak
    delay(3000); // Tunda selama 3 detik untuk menampilkan status akses
  }

}
#elif defined(PROJECT_MERLIN)
void setup() {
  
    // Inisialisasi khusus untuk project Hilman
    Serial.begin(115200);
    Serial.println("Projek Pengaman Pintu dengan Fingerprint, RFID, dan Keypad");
    
    // Inisialisasi RFID
    rfid.PCD_Init();
  
}

void loop() {
  //Ambil data RFID
  String rfidData = BacaRFID();
  if (rfidData != "") {
    Serial.print("Data RFID Terbaca: ");
    Serial.println(rfidData);

    Serial.print("Mencoba verifikasi akses...");
  } else {
    return;
  }

}
#endif
