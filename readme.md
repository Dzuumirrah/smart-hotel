# smart-hotel

smart-hotel/
│
├── src/
│   ├── main.cpp                 → Entry point utama, memilih project aktif
│   ├── project-control.h        → Pengendali utama (definisi target & konfigurasi pin)
│   ├── project-galih.cpp/.h     → Implementasi untuk ESP32
│   ├── project-galih2.cpp/.h    → Implementasi untuk Arduino Uno
│   └── modules/
│       ├── actuator_motor/
│       │   ├── module_motor.cpp/.h        → Kendali motor tirai
│       ├── sensor_BH1750/
│       │   ├── module_sensor_BH1750.cpp/.h → Pembacaan sensor cahaya BH1750
│       └── sensor_PIR/
│           ├── module_sensor_PIR.cpp/.h    → Pembacaan sensor gerak PIR
│
├── platformio.ini               → Konfigurasi environment build PlatformIO
├── .gitignore
└── README.md                    → Dokumentasi proyek
