/*
 * Configuracao persistida em EEPROM + comandos seriais.
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <Arduino.h>

namespace Settings {

void begin();

// Le caracteres da serial sem bloquear. Chamar no loop().
void poll();

int dryRaw();
int wetRaw();
unsigned long rateMs();

void print();

}  // namespace Settings
