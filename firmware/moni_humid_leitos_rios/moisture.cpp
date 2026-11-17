/*
 * Leitura do sensor capacitivo de umidade do solo.
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "moisture.h"

#include "config.h"
#include "pure_logic.h"
#include "settings.h"

void Moisture::begin() {
  pinMode(config::PIN_MOISTURE_VCC, OUTPUT);
  digitalWrite(config::PIN_MOISTURE_VCC, LOW);
  pinMode(config::PIN_MOISTURE_AO, INPUT);
}

int Moisture::readRaw() {
  digitalWrite(config::PIN_MOISTURE_VCC, HIGH);
  delay(config::SENSOR_WARMUP_MS);

  const int raw = analogRead(config::PIN_MOISTURE_AO);

  digitalWrite(config::PIN_MOISTURE_VCC, LOW);
  return raw;
}

float Moisture::toPercent(int raw) {
  return pure::moisturePercent(raw, Settings::dryRaw(), Settings::wetRaw());
}
