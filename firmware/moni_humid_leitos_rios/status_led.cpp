/*
 * LED de status (feature flag MONI_ENABLE_STATUS_LED).
 * Padroes: INIT = aceso fixo, WAITING_GPS = 2 piscadas, LOGGING = 1 piscada,
 * SD_ERROR = 4 piscadas rapidas. Ciclo de 2 s.
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "status_led.h"

#include <Arduino.h>

#include "config.h"

#if MONI_ENABLE_STATUS_LED

namespace {

SystemState::Value currentState = SystemState::INIT;
unsigned long cycleStart = 0;

int blinkCountFor(SystemState::Value state) {
  switch (state) {
    case SystemState::INIT:        return -1;  // aceso continuo
    case SystemState::WAITING_GPS: return 2;
    case SystemState::LOGGING:     return 1;
    case SystemState::SD_ERROR:    return 4;
    default:                       return 0;
  }
}

}  // namespace

void StatusLed::begin() {
  pinMode(config::PIN_STATUS_LED, OUTPUT);
  digitalWrite(config::PIN_STATUS_LED, LOW);
  cycleStart = millis();
}

void StatusLed::update(SystemState::Value state) {
  currentState = state;
  const int pulses = blinkCountFor(currentState);

  if (pulses < 0) {
    digitalWrite(config::PIN_STATUS_LED, HIGH);
    return;
  }

  const unsigned long phase = (millis() - cycleStart) % 2000UL;
  const unsigned long slot = 2000UL / (pulses > 0 ? static_cast<unsigned long>(pulses) : 1UL);
  const unsigned long half = slot / 2UL;

  if (pulses == 0) {
    digitalWrite(config::PIN_STATUS_LED, LOW);
    return;
  }

  const bool lit = (phase % slot) < half;
  digitalWrite(config::PIN_STATUS_LED, lit ? HIGH : LOW);
}

#else

void StatusLed::begin() {}
void StatusLed::update(SystemState::Value) {}

#endif
