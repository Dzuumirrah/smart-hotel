// || ==================================================================== ||
// ||   Berisi konfirgurasi umum seperti pinout, deklarasi, dan lain-lain  ||
// || ==================================================================== ||
#ifndef CONFIG_H
#define CONFIG_H

#include <MFRC522.h>

// || ==================================================================== ||
// ||                               PINOUT                                 ||
// || ==================================================================== ||

#define SS_PIN 5
#define RST_PIN 4
#define BTN_PIN 15
#define BUZ_PIN 2

// || ==================================================================== ||
// ||                          VARIABEL GLOBAL                             ||
// || ==================================================================== ||

/** 
 * @brief Deklarasi objek RFID.
 */
extern MFRC522 rfid;

/** 
 * @brief Menentukan apakah sistem dalam mode perekaman data atau tidak.
 */
extern bool RECORDMODE;

#endif // CONFIG_H