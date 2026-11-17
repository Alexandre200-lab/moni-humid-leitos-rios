/*
 * Persistencia das amostras em cartao microSD (CSV).
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "storage.h"

#include <SD.h>

#include "config.h"
#include "diag.h"
#include "pure_logic.h"

namespace {

// "gpslog99.csv" = 12 caracteres + terminador = 13.
char logFileName[13] = {0};

// Buffer estatico: evita pressao na pilha do ATmega328P (2 KB de SRAM).
char rowBuffer[config::CSV_ROW_CAPACITY] = {0};

uint32_t sampleSeq = 0;
bool sdReady = false;
unsigned long lastSdAttempt = 0;
uint8_t sdFailures = 0;

bool openNewFile() {
  for (int i = 0; i < config::MAX_LOG_FILES; ++i) {
    snprintf(logFileName, sizeof(logFileName), "%s%d.%s",
             config::LOG_FILE_PREFIX, i, config::LOG_FILE_SUFFIX);

    if (SD.exists(logFileName)) continue;

    File file = SD.open(logFileName, FILE_WRITE);
    if (!file) {
      Diag::error(F("Nao foi possivel criar o arquivo de log"));
      logFileName[0] = '\0';
      return false;
    }

    file.println(reinterpret_cast<const __FlashStringHelper*>(config::LOG_HEADER));
    file.close();

    Diag::print(F("Arquivo de log: "));
    Diag::println(logFileName);
    return true;
  }

  logFileName[0] = '\0';
  Diag::error(F("Limite de arquivos de log atingido"));
  return false;
}

bool tryInitialize() {
  if (!SD.begin(config::PIN_SD_CS)) return false;
  if (!openNewFile()) return false;
  return true;
}

}  // namespace

bool Storage::begin() {
  lastSdAttempt = millis();
  sdReady = tryInitialize();
  if (!sdReady) {
    Diag::error(F("Falha ao inicializar o cartao SD"));
    return false;
  }
  Diag::println(F("Cartao SD inicializado."));
  return true;
}

void Storage::poll() {
  if (sdReady) return;
  if (millis() - lastSdAttempt < config::SD_RETRY_MS) return;

  lastSdAttempt = millis();
  if (tryInitialize()) {
    sdReady = true;
    sdFailures = 0;
    Diag::println(F("Cartao SD recuperado."));
  } else {
    ++sdFailures;
  }
}

bool Storage::isReady() { return sdReady; }
uint8_t Storage::failureCount() { return sdFailures; }

bool Storage::prepareNewFile() { return openNewFile(); }

bool Storage::appendRow(const SensorReading& reading) {
  if (!sdReady || logFileName[0] == '\0') return false;

  pure::GpsSample sample;
  sample.valid         = reading.gps.valid;
  sample.timeValid     = reading.gps.timeValid;
  sample.date          = reading.gps.date;
  sample.time          = reading.gps.time;
  sample.latitude      = reading.gps.latitude;
  sample.longitude     = reading.gps.longitude;
  sample.altitudeMeters = reading.gps.altitudeMeters;
  sample.speedKmh      = reading.gps.speedKmh;
  sample.courseDeg     = reading.gps.courseDeg;
  sample.satellites    = reading.gps.satellites;

  pure::buildCsvRow(sample, reading.moistureRaw, reading.moisturePct,
                    ++sampleSeq, rowBuffer, sizeof(rowBuffer));

  File file = SD.open(logFileName, FILE_WRITE);
  if (!file) {
    sdReady = false;
    return false;
  }

  file.println(rowBuffer);
  file.close();
  return true;
}

const char* Storage::currentFileName() { return logFileName; }
