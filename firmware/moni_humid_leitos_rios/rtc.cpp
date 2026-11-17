/*
 * Relogio de tempo real DS3231 (feature flag MONI_ENABLE_RTC).
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "rtc.h"

#include "config.h"
#include "pure_logic.h"

#if MONI_ENABLE_RTC

#include <RTClib.h>

namespace {
RTC_DS3231 rtc;
}

bool Rtc::begin() {
  if (!rtc.begin()) return false;
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  return true;
}

bool Rtc::now(char* date, size_t dateSize, char* time, size_t timeSize) {
  const DateTime t = rtc.now();
  pure::formatGpsDate(t.day(), t.month(), t.year(), date, dateSize);
  pure::formatGpsTime(t.hour(), t.minute(), t.second(), time, timeSize);
  return true;
}

#else

bool Rtc::begin() { return false; }
bool Rtc::now(char*, size_t, char*, size_t) { return false; }

#endif
