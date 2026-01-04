#pragma once

// --- I2C BUS ---
#define PIN_I2C_SDA 8
#define PIN_I2C_SCL 9

// --- SPI BUS ---
#define PIN_SPI_SCK  17
#define PIN_SPI_MISO 16
#define PIN_SPI_MOSI 15

// --- SCREEN (OLED SH1106) ---
// No more separate pins

// --- INPUTS ---
#define PIN_BTN_UP    10
#define PIN_BTN_DOWN  11
#define PIN_BTN_LEFT  12
#define PIN_BTN_RIGHT 13
#define PIN_BTN_OK    14
#define PIN_BTN_BACK  21

// --- OUTPUTS ---
#define PIN_BUZZER 42
#define PIN_LED    18
#define PIN_VIBRO  41

// --- STORAGE (Micro SD) ---
#define PIN_SD_CS  4

// --- INFRARED ---
#define PIN_IR_RX  47
#define PIN_IR_TX  48

// --- POWER ---
#define PIN_BATTERY_ADC 6

// --- RADIO (CC1101 Sub-GHz) ---
#define PIN_CC1101_CS   38
#define PIN_CC1101_GDO0 39
#define PIN_CC1101_GDO2 40

// --- NFC (PN532 I2C) ---
#define PIN_NFC_IRQ     37
#define PIN_NFC_RESET   36
