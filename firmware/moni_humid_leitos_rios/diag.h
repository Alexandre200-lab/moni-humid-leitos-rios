/*
 * Saida de diagnostico pela serial (monitor serial).
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <Arduino.h>

namespace Diag {

void begin();
void print(const __FlashStringHelper* msg);
void println(const __FlashStringHelper* msg);
void println(const char* msg);
void value(const __FlashStringHelper* label, float number, int digits);
void error(const __FlashStringHelper* msg);

}  // namespace Diag
