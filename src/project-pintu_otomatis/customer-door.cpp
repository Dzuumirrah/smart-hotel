
#include "customer-door.h"

MFRC522 rfid(SS_PIN, RST_PIN);

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

void SimpanData(bool RECORDMODE, String fingerprintData, String rfidData) {

}

bool CekAkses(String fingerprintData, String rfidData) {

}

bool TimeoutAkses(unsigned long startTime) {

}

bool KondisiPintu() {

}

