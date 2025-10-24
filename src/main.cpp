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
