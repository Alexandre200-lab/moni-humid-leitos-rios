/*
 * Relogio de tempo real DS3231 (feature flag MONI_ENABLE_RTC).
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <Arduino.h>

namespace Rtc {

// Retorna false se o RTC nao estiver presente/flag desligada.
bool begin();

// Preenche date/time quando ha tempo valido no RTC.
bool now(char* date, size_t dateSize, char* time, size_t timeSize);

}  // namespace Rtc
