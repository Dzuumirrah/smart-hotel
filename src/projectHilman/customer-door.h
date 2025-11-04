#ifndef CUSTOMER_DOOR_H
#define CUSTOMER_DOOR_H

#include "config.h"
// || ==================================================================== ||
// ||                         DEKLARASI FUNGSI                             ||
// || ==================================================================== ||

/**
 * @brief Inisialisasi untuk project Hilman.
 * @details Melakukan inisialisasi semua komponen:
 *          - Serial communication
 *          - WiFi module
 *          - RFID reader
 *          - Fingerprint sensor
 *          - RGB LED indicators
 *          - Solenoid lock
 * @return void
 */
void setup_hilman();

/**
 * @brief Loop utama untuk project Hilman.
 * @details Mengatur alur kerja utama sistem:
 *          1. Cek koneksi WiFi
 *          2. Handle mode perekaman jika aktif
 *          3. Baca input (RFID/Fingerprint)
 *          4. Verifikasi akses
 *          5. Kontrol akses pintu
 * @return void
 */
void loop_hilman();

/**
 * @brief Cek koneksi WiFi.
 * @details Mencoba menghubungkan ke WiFi jika belum terhubung.
 *          Menggunakan timeout 10 detik untuk setiap percobaan koneksi.
 * @return true Jika berhasil terhubung ke WiFi
 * @return false Jika gagal terhubung setelah timeout
 */
bool cekKoneksiWiFi();

/** 
 * @brief Membaca data fingerprint.
 * @details Mengambil gambar sidik jari, mengkonversi ke template,
 *          dan mencari kecocokan di database sensor.
 * @return String ID sidik jari dalam format hex jika ditemukan
 * @return String kosong jika tidak ada data atau terjadi error
 * @note Pesan error spesifik dikirim ke Serial monitor
 */ 
String BacaFingerprint();

/** 
 * @brief Membaca data RFID.
 * @details Membaca UID kartu RFID yang terdeteksi dan
 *          mengkonversinya ke format hex string.
 * @return String UID kartu dalam format hex jika kartu terdeteksi
 * @return String kosong jika tidak ada kartu atau gagal membaca
 */ 
String BacaRFID();

/** 
 * @brief Menyimpan data fingerprint dan/atau RFID ke dalam sistem.
 * @details Mengirim data ke server menggunakan HTTP POST request
 *          dengan format JSON yang berisi room number dan credential.
 * @param fingerprintData Data fingerprint yang akan disimpan (opsional)
 * @param rfidData Data RFID yang akan disimpan (opsional)
 * @note Minimal satu parameter harus berisi data
 * @note Response dari server dikirim ke Serial monitor
 */
void SimpanData(String fingerprintData = "", String rfidData = "");

/** 
 * @brief Verifikasi akses menggunakan fingerprint atau RFID.
 * @details Mengirim request GET ke server dengan credential yang diberikan
 *          untuk memverifikasi hak akses ke ruangan tertentu.
 * @param fingerprintData Data fingerprint untuk verifikasi (opsional)
 * @param rfidData Data RFID untuk verifikasi (opsional)
 * @return true Jika server memberikan response "GRANTED"
 * @return false Jika akses ditolak atau terjadi error komunikasi
 * @note Minimal satu parameter harus berisi data
 */ 
bool CekAkses(String fingerprintData = "", String rfidData = "");

/** 
 * @brief Mengecek apakah akses telah timeout.
 * @details Membandingkan waktu sekarang dengan waktu mulai
 *          untuk menentukan apakah sudah melewati batas timeout (5 detik).
 * @param startTime Waktu mulai dalam milidetik (dari millis())
 * @return true Jika sudah melewati batas timeout
 * @return false Jika masih dalam batas waktu
 * @note Jika startTime = 0, fungsi akan menggunakan waktu sekarang
 */
bool TimeoutAkses(unsigned long startTime = 0);

/** 
 * @brief Mengaktifkan solenoid untuk membuka pintu.
 * @details Mengatur pin solenoid ke HIGH untuk membuka kunci pintu.
 *          Harus digunakan bersama dengan TimeoutAkses untuk mencegah
 *          solenoid aktif terlalu lama.
 */
void BukaPintu();

/** 
 * @brief Mengatur status dan warna LED RGB.
 * @details Mengontrol 3 pin LED RGB untuk memberikan indikasi visual
 *          status sistem kepada pengguna.
 * @param status true untuk menyalakan LED, false untuk mematikan
 * @param color Warna yang diinginkan (default: WHITE)
 * @note Warna yang tersedia: RED, YELLOW, BLUE, GREEN, WHITE
 * @note Disabled due to hardware unavailability
 */
// void IndikatorLEDRGB(bool status, LEDColor color);

#endif // CUSTOMER_DOOR_H