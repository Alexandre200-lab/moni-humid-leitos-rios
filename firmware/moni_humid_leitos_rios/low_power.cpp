/*
 * Sleep Profundo via watchdog (feature flag MONI_ENABLE_LOW_POWER).
 * O WDT do AVR tem no maximo ~8 s, entao cada ciclo dorme 8 s.
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "low_power.h"

#include "config.h"

#if MONI_ENABLE_LOW_POWER

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

ISR(WDT_vect) {}

void LowPower::begin() {
  pinMode(config::PIN_GPS_VCC, OUTPUT);
  digitalWrite(config::PIN_GPS_VCC, HIGH);
}

void LowPower::sleepCycles(uint8_t wdtCycles) {
  // Desliga modulo GPS e sensor durante o sono.
  digitalWrite(config::PIN_GPS_VCC, LOW);
  digitalWrite(config::PIN_MOISTURE_VCC, LOW);

  wdt_disable();
  MCUSR &= ~(1 << WDRF);
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  WDTCSR = (1 << WDIE) | (1 << WDP3) | (1 << WDP0);  // 8 s, modo interrupcao

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  for (uint8_t i = 0; i < wdtCycles; ++i) {
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
  }

  wdt_disable();
  wdt_enable(WDTO_8S);

  digitalWrite(config::PIN_GPS_VCC, HIGH);
}

#else

void LowPower::begin() {}
void LowPower::sleepCycles(uint8_t) {}

#endif
