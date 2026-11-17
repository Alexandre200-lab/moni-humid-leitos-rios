/*
 * Aquisicao de dados GPS via TinyGPS++.
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "gps.h"

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#include "config.h"
#include "pure_logic.h"

namespace {

TinyGPSPlus tinyGps;
SoftwareSerial gpsSerial(config::PIN_GPS_TX, config::PIN_GPS_RX);

}  // namespace

void Gps::begin() {
  gpsSerial.begin(config::GPS_BAUD);
}

void Gps::feed() {
  while (gpsSerial.available() > 0) {
    tinyGps.encode(static_cast<char>(gpsSerial.read()));
  }
}

bool Gps::read(GpsReading& out) {
  if (!tinyGps.location.isUpdated()) {
    return false;
  }

  out.valid          = true;
  out.latitude       = tinyGps.location.lat();
  out.longitude      = tinyGps.location.lng();
  out.altitudeMeters = tinyGps.altitude.meters();
  out.speedKmh       = tinyGps.speed.kmph();
  out.courseDeg      = tinyGps.course.deg();
  out.satellites     = tinyGps.satellites.value();

  const bool hasDateTime = tinyGps.date.isValid() && tinyGps.time.isValid();
  if (hasDateTime) {
    pure::formatGpsDate(tinyGps.date.day(), tinyGps.date.month(),
                        tinyGps.date.year(), out.date, sizeof(out.date));
    pure::formatGpsTime(tinyGps.time.hour(), tinyGps.time.minute(),
                        tinyGps.time.second(), out.time, sizeof(out.time));
  }
  out.timeValid = hasDateTime;
  return true;
}
