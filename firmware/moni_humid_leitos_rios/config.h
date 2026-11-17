/*
 * moni-humid-leitos-rios — configuracao central do firmware.
 *
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <Arduino.h>

namespace config {

// --- Pinagem ----------------------------------------------------------------
// GPS Logger Shield v2.0 com a chave UART posicionada em SW-UART.
// A ordem dos pinos do GPS preserva o sketch original: SoftwareSerial(2, 3).
static constexpr uint8_t PIN_GPS_TX       = 2;
static constexpr uint8_t PIN_GPS_RX       = 3;

// CS do cartao microSD no shield. SPI usa D11/D12/D13 (fixo no Uno).
static constexpr uint8_t PIN_SD_CS        = 8;

// Sensor capacitivo de umidade do solo (ex.: SEN0193 / v1.2).
static constexpr uint8_t PIN_MOISTURE_AO  = A0;  // saida analogica
static constexpr uint8_t PIN_MOISTURE_VCC = 7;   // alimentacao chaveada

// Opcionais (feature flags abaixo): GPS chaveado e LED de status.
static constexpr uint8_t PIN_GPS_VCC      = 4;
static constexpr uint8_t PIN_STATUS_LED   = 6;

// --- Comunicacao -------------------------------------------------------------
static constexpr uint32_t SERIAL_BAUD = 9600;
static constexpr uint32_t GPS_BAUD    = 9600;

// --- Aquisicao ---------------------------------------------------------------
static constexpr unsigned long LOG_RATE_MS      = 5000UL;
static constexpr unsigned long SENSOR_WARMUP_MS = 10UL;
static constexpr unsigned long SD_RETRY_MS      = 10000UL;

// --- Arquivo de log ----------------------------------------------------------
// O cartao usa nomes 8.3: "gpslog00.csv" .. "gpslog99.csv".
static constexpr char   LOG_FILE_PREFIX[] = "gpslog";
static constexpr char   LOG_FILE_SUFFIX[] = "csv";
static constexpr int    MAX_LOG_FILES     = 100;
static constexpr size_t CSV_ROW_CAPACITY  = 104;

// 13 colunas, na mesma ordem gerada por pure::buildCsvRow().
// Fica em PROGMEM (flash) para economizar SRAM, que e escassa no ATmega328P.
static constexpr char LOG_HEADER[] PROGMEM =
  "seq,gps_valid,date,time,latitude,longitude,altitude_m,speed_kmh,course_deg,"
  "satellites,moisture_raw,moisture_pct,crc8";

// --- Calibracao padrao -------------------------------------------------------
// Podem ser alteradas em campo pelo modulo Settings (EEPROM), sem recompilar.
static constexpr int MOISTURE_DRY_RAW = 820;
static constexpr int MOISTURE_WET_RAW = 380;

}  // namespace config

// --- Feature flags -----------------------------------------------------------
// Recursos que dependem de hardware ainda nao montado vem desligados.
// Ative apos conferir a montagem (veja docs/field-checklist.md).
#define MONI_ENABLE_SETTINGS     1  // EEPROM + comandos seriais
#define MONI_ENABLE_RTC          0  // DS3231 via RTClib
#define MONI_ENABLE_STATUS_LED   0  // LED em D6
#define MONI_ENABLE_LOW_POWER    0  // sleep + WDT (experimental)
