/*
 * moni-humid-leitos-rios
 *
 * Monitora a umidade do leito de um rio com sensor capacitivo, georreferencia
 * cada amostra via GPS e registra tudo em CSV em um cartao microSD.
 *
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Documentacao: docs/ e README.md
 */
#include <avr/wdt.h>

#include "config.h"
#include "types.h"
#include "system_state.h"

#include "diag.h"
#include "gps.h"
#include "low_power.h"
#include "moisture.h"
#include "rtc.h"
#include "settings.h"
#include "status_led.h"
#include "storage.h"

namespace {

unsigned long lastSampleMs = 0;
SystemState::Value state = SystemState::INIT;
SystemState::Value previousState = SystemState::INIT;
bool rtcAvailable = false;

}  // namespace

void setup() {
  Diag::begin();
  Diag::println(F("moni-humid-leitos-rios v1.1.0"));

  Settings::begin();
  LowPower::begin();
  Gps::begin();
  Moisture::begin();
  StatusLed::begin();

  rtcAvailable = Rtc::begin();
  if (!rtcAvailable && MONI_ENABLE_RTC) {
    Diag::error(F("RTC nao detectado; usando apenas hora do GPS"));
  }

  Storage::begin();

  wdt_enable(WDTO_8S);
}

void loop() {
  wdt_reset();

  Settings::poll();
  Gps::feed();
  Storage::poll();

  state = Storage::isReady() ? SystemState::WAITING_GPS : SystemState::SD_ERROR;

  if (state != previousState) {
    Diag::print(F("Estado: "));
    Diag::println(SystemState::name(state));
    previousState = state;
  }

  // Subtracao tolera a voltagem do millis() apos ~49 dias.
  if (millis() - lastSampleMs >= Settings::rateMs()) {
    lastSampleMs = millis();

    SensorReading reading;
    reading.gps.valid = Gps::read(reading.gps);

    // Sem fix GPS, o RTC pode fornecer o carimbo de data/hora.
    if (!reading.gps.timeValid && rtcAvailable) {
      reading.gps.timeValid = Rtc::now(reading.gps.date,
                                       sizeof(reading.gps.date),
                                       reading.gps.time,
                                       sizeof(reading.gps.time));
    }

    reading.moistureRaw = Moisture::readRaw();
    reading.moisturePct = Moisture::toPercent(reading.moistureRaw);

    if (Storage::isReady() && Storage::appendRow(reading)) {
      state = SystemState::LOGGING;
      Diag::value(F("Umidade: "), reading.moisturePct, 1);
      if (!reading.gps.valid) {
        Diag::println(F("Sem fix GPS; linha gravada sem coordenadas."));
      }
    } else {
      state = SystemState::SD_ERROR;
      Diag::error(F("Falha ao gravar a amostra no SD"));
    }
  }

#if MONI_ENABLE_LOW_POWER
  // O WDT do AVR dorme no maximo ~8 s por ciclo.
  {
    const unsigned long cycleMs = 8000UL;
    unsigned long cycles = (Settings::rateMs() + cycleMs - 1UL) / cycleMs;
    if (cycles < 1UL) cycles = 1UL;
    if (cycles > 255UL) cycles = 255UL;
    LowPower::sleepCycles(static_cast<uint8_t>(cycles));
    lastSampleMs = millis();
  }
#endif

  StatusLed::update(state);
}
