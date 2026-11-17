/*
 * Configuracao persistida em EEPROM + comandos seriais.
 *
 * Comandos (terminados por Enter, 9600 baud):
 *   SHOW            mostra os valores atuais
 *   CAL_DRY=820     define a leitura bruta em solo seco
 *   CAL_WET=380     define a leitura bruta em solo saturado
 *   RATE=5000       intervalo entre amostras, em ms (1000..60000)
 *   SAVE            grava na EEPROM
 *   RESET           volta aos padroes e grava
 *
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "settings.h"

#include <EEPROM.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

namespace {

const uint8_t kMagic = 0xA5;
const int kEepromAddress = 0;

struct StoredSettings {
  uint8_t       magic;
  int           dryRaw;
  int           wetRaw;
  unsigned long rateMs;
};

StoredSettings current;

char command[24];
uint8_t commandLength = 0;

void applyDefaults() {
  current.magic  = kMagic;
  current.dryRaw = config::MOISTURE_DRY_RAW;
  current.wetRaw = config::MOISTURE_WET_RAW;
  current.rateMs = config::LOG_RATE_MS;
}

void save() {
  current.magic = kMagic;
  EEPROM.put(kEepromAddress, current);
}

void executeCommand() {
  if (commandLength == 0) return;
  command[commandLength] = '\0';

  if (strncmp(command, "SHOW", 4) == 0) {
    Settings::print();
  } else if (strncmp(command, "RESET", 5) == 0) {
    applyDefaults();
    save();
    Serial.println(F("Padroes restaurados."));
    Settings::print();
  } else if (strncmp(command, "SAVE", 4) == 0) {
    save();
    Serial.println(F("Configuracao salva na EEPROM."));
  } else if (strncmp(command, "CAL_DRY=", 8) == 0) {
    const int value = atoi(command + 8);
    if (value >= 0 && value <= 1023 && value != current.wetRaw) {
      current.dryRaw = value;
      Serial.println(F("CAL_DRY atualizado ( use SAVE )."));
    } else {
      Serial.println(F("CAL_DRY invalido."));
    }
  } else if (strncmp(command, "CAL_WET=", 8) == 0) {
    const int value = atoi(command + 8);
    if (value >= 0 && value <= 1023 && value != current.dryRaw) {
      current.wetRaw = value;
      Serial.println(F("CAL_WET atualizado ( use SAVE )."));
    } else {
      Serial.println(F("CAL_WET invalido."));
    }
  } else if (strncmp(command, "RATE=", 5) == 0) {
    const unsigned long value = strtoul(command + 5, 0, 10);
    if (value >= 1000UL && value <= 60000UL) {
      current.rateMs = value;
      Serial.println(F("RATE atualizado ( use SAVE )."));
    } else {
      Serial.println(F("RATE invalido (1000..60000)."));
    }
  } else {
    Serial.println(F("Comando desconhecido."));
  }

  commandLength = 0;
  command[0] = '\0';
}

}  // namespace

void Settings::begin() {
  StoredSettings stored;
  EEPROM.get(kEepromAddress, stored);
  if (stored.magic != kMagic || stored.dryRaw == stored.wetRaw ||
      stored.rateMs < 1000UL) {
    applyDefaults();
    save();
  } else {
    current = stored;
  }

  command[0] = '\0';
  commandLength = 0;

#if MONI_ENABLE_SETTINGS
  Serial.println(F("Comandos: SHOW, CAL_DRY=, CAL_WET=, RATE=, SAVE, RESET"));
#endif
}

void Settings::poll() {
#if MONI_ENABLE_SETTINGS
  while (Serial.available() > 0) {
    const char c = static_cast<char>(Serial.read());
    if (c == '\n' || c == '\r') {
      executeCommand();
    } else if (static_cast<size_t>(commandLength) + 1 < sizeof(command)) {
      command[commandLength++] = c;
      command[commandLength] = '\0';
    } else {
      commandLength = 0;
      command[0] = '\0';
      Serial.println(F("Comando muito longo."));
    }
  }
#endif
}

int Settings::dryRaw() { return current.dryRaw; }
int Settings::wetRaw() { return current.wetRaw; }
unsigned long Settings::rateMs() { return current.rateMs; }

void Settings::print() {
  Serial.print(F("CAL_DRY="));
  Serial.println(current.dryRaw);
  Serial.print(F("CAL_WET="));
  Serial.println(current.wetRaw);
  Serial.print(F("RATE="));
  Serial.println(current.rateMs);
}
