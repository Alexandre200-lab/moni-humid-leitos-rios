/*
 * Tipos compartilhados pelos modulos do firmware.
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <Arduino.h>

struct GpsReading {
  bool     valid      = false;
  bool     timeValid  = false;  // data/hora do GPS ou do RTC
  char     date[11]   = {0};    // DD/MM/AAAA
  char     time[9]    = {0};    // HH:MM:SS
  double   latitude   = 0.0;
  double   longitude  = 0.0;
  float    altitudeMeters = 0.0f;
  float    speedKmh   = 0.0f;
  float    courseDeg  = 0.0f;
  uint16_t satellites = 0;
};

struct SensorReading {
  GpsReading gps;
  int        moistureRaw = 0;
  float      moisturePct = 0.0f;
};
