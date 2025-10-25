#ifndef CUSTOMER_DOOR_H
#define CUSTOMER_DOOR_H

#include "config.h"
// || ==================================================================== ||
// ||                         DEKLARASI FUNGSI                             ||
// || ==================================================================== ||

/**
 * @brief Cek koneksi WiFi.
 * @return Mengembalikan true jika terhubung ke WiFi, false jika tidak.
 */
bool cekKoneksiWiFi();

/** 
 * @brief Membaca data fingerprint.
 * @return Mengembalikan data fingerprint yang dibaca. None jika tidak ada data yang sesuai.
 */ 
String BacaFingerprint();


/** 
 * @brief Membaca data RFID.
 * @return Mengembalikan data RFID yang dibaca. None jika tidak ada data yang sesuai.
 */ 
String BacaRFID();

/** 
 * @brief Menyimpan data fingerprint dan/atau RFID ke dalam sistem.
 * @param fingerprintData Data fingerprint yang akan disimpan (opsional).
 * @param rfidData Data RFID yang akan disimpan (opsional).
 */
void SimpanData(String fingerprintData = "", String rfidData = "");

/** 
 * @brief Cek akses data dari fingerprint dan RFID.
 * @return Mengembalikan true jika akses diizinkan, false jika tidak.
 * @param fingerprintData Data fingerprint yang akan diperiksa.
 * @param rfidData Data RFID yang akan diperiksa.
 */ 
bool CekAkses(String fingerprintData = "", String rfidData = "");


/** 
 * @brief Mengecek apakah akses telah timeout. Timeout diatur selama 5 detik.
 * @return Mengembalikan true jika akses telah timeout, false jika belum.
 * @param startTime Waktu mulai akses diberikan (dalam milidetik).
 */
bool TimeoutAkses(unsigned long startTime  = 0);


/** 
 * @brief Membuka pintu (terbuka/tutup). Proses perubahan disesuaikan dengan mekanisme pintu yang digunakan.
 */
void BukaPintu();

/** 
 * @brief Mengatur indikator LED RGB.
 * @param status Status indikator (true untuk menyala, false untuk mati).
 * @param color Warna LED yang diinginkan.
 */
void IndikatorLEDRGB(bool status, LEDColor color);

#endif // CUSTOMER_DOOR_H