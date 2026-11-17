/*
 * Sleep Profundo via watchdog (feature flag MONI_ENABLE_LOW_POWER).
 * Experimental — veja docs/field-checklist.md antes de ativar.
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <Arduino.h>

namespace LowPower {

void begin();

// Dorme `wdtCycles` periodos de ~8 s. Retorna quando acorda.
void sleepCycles(uint8_t wdtCycles);

}  // namespace LowPower
