/*
 * Saida de diagnostico pela serial (monitor serial).
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "diag.h"

#include "config.h"

void Diag::begin() {
  Serial.begin(config::SERIAL_BAUD);
}

void Diag::print(const __FlashStringHelper* msg) {
  Serial.print(msg);
}

void Diag::println(const __FlashStringHelper* msg) {
  Serial.println(msg);
}

void Diag::println(const char* msg) {
  Serial.println(msg);
}

void Diag::value(const __FlashStringHelper* label, float number, int digits) {
  Serial.print(label);
  Serial.println(number, digits);
}

void Diag::error(const __FlashStringHelper* msg) {
  Serial.print(F("[ERRO] "));
  Serial.println(msg);
}
