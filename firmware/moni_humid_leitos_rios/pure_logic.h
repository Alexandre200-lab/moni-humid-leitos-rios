/*
 * Logica pura do firmware — sem dependencia de Arduino.h.
 *
 * Este header e compilado tanto pelo avr-g++ (firmware) quanto pelo g++ nativo
 * (testes unitarios no CI). Por isso nao usaSerial, String, File ou printf de
 * ponto flutuante — todos os numeros sao formatados manualmente.
 *
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace pure {

// Amostra de GPS em formato independente de hardware.
struct GpsSample {
  bool        valid;
  bool        timeValid;  // pode vir do GPS ou do RTC
  const char* date;
  const char* time;
  double      latitude;
  double      longitude;
  double      altitudeMeters;
  double      speedKmh;
  double      courseDeg;
  uint16_t    satellites;
};

// --- Conversao de umidade ----------------------------------------------------
// Equivalente ao map() do Arduino, inclusive intervalos invertidos.
inline long mapLong(long x, long inMin, long inMax, long outMin, long outMax) {
  const long divisor = inMax - inMin;
  if (divisor == 0) return outMin;
  const long dividend = outMax - outMin;
  const long delta = x - inMin;
  return (delta * dividend + (divisor / 2)) / divisor + outMin;
}

inline float moisturePercent(int raw, int dryRaw, int wetRaw) {
  if (dryRaw == wetRaw) return 0.0f;
  long percent = mapLong(raw, dryRaw, wetRaw, 0L, 100L);
  if (percent < 0L) percent = 0L;
  if (percent > 100L) percent = 100L;
  return static_cast<float>(percent);
}

// --- Formatacao de data e hora ----------------------------------------------
inline void appendChar(char* out, size_t& pos, size_t size, char c) {
  if (pos + 1 < size) {
    out[pos++] = c;
    out[pos] = '\0';
  }
}

inline void appendPadded(char* out, size_t& pos, size_t size,
                         unsigned long value, int width) {
  char tmp[12];
  uint8_t n = 0;
  if (value == 0) tmp[n++] = '0';
  while (value > 0) {
    tmp[n++] = static_cast<char>('0' + (value % 10));
    value /= 10;
  }
  for (int i = n; i < width; ++i) appendChar(out, pos, size, '0');
  while (n > 0) appendChar(out, pos, size, tmp[--n]);
}

inline bool formatGpsDate(unsigned day, unsigned month, unsigned year,
                          char* out, size_t size) {
  if (out == 0 || size < 11) return false;
  out[0] = '\0';
  size_t pos = 0;
  appendPadded(out, pos, size, day, 2);
  appendChar(out, pos, size, '/');
  appendPadded(out, pos, size, month, 2);
  appendChar(out, pos, size, '/');
  appendPadded(out, pos, size, year, 4);
  return true;
}

inline bool formatGpsTime(unsigned hour, unsigned minute, unsigned second,
                          char* out, size_t size) {
  if (out == 0 || size < 9) return false;
  out[0] = '\0';
  size_t pos = 0;
  appendPadded(out, pos, size, hour, 2);
  appendChar(out, pos, size, ':');
  appendPadded(out, pos, size, minute, 2);
  appendChar(out, pos, size, ':');
  appendPadded(out, pos, size, second, 2);
  return true;
}

// --- CRC-8 (polinomio 0x07, init 0) -----------------------------------------
inline uint8_t crc8(const char* data, size_t length) {
  uint8_t crc = 0;
  for (size_t i = 0; i < length; ++i) {
    crc ^= static_cast<uint8_t>(data[i]);
    for (int bit = 0; bit < 8; ++bit) {
      if (crc & 0x80) {
        crc = static_cast<uint8_t>((crc << 1) ^ 0x07);
      } else {
        crc = static_cast<uint8_t>(crc << 1);
      }
    }
  }
  return crc;
}

// --- Formatacao manual de numeros (sem %f do printf do AVR) -----------------
inline void appendUnsigned(char* out, size_t& pos, size_t size,
                           unsigned long value) {
  char tmp[12];
  uint8_t n = 0;
  if (value == 0) tmp[n++] = '0';
  while (value > 0) {
    tmp[n++] = static_cast<char>('0' + (value % 10));
    value /= 10;
  }
  while (n > 0) appendChar(out, pos, size, tmp[--n]);
}

inline void appendFixed(char* out, size_t& pos, size_t size,
                        double value, int digits) {
  if (value < 0) {
    appendChar(out, pos, size, '-');
    value = -value;
  }
  unsigned long scale = 1;
  for (int i = 0; i < digits; ++i) scale *= 10UL;

  const unsigned long scaled =
      static_cast<unsigned long>(value * static_cast<double>(scale) + 0.5);

  appendUnsigned(out, pos, size, scaled / scale);
  if (digits > 0) {
    appendChar(out, pos, size, '.');
    unsigned long div = scale / 10UL;
    for (int i = 0; i < digits; ++i) {
      appendChar(out, pos, size,
                 static_cast<char>('0' + (scaled / div) % 10UL));
      div /= 10UL;
    }
  }
}

inline void appendHex2(char* out, size_t& pos, size_t size, uint8_t value) {
  static const char kDigits[] = "0123456789ABCDEF";
  appendChar(out, pos, size, kDigits[(value >> 4) & 0x0F]);
  appendChar(out, pos, size, kDigits[value & 0x0F]);
}

inline void appendField(char* out, size_t& pos, size_t size, const char* value) {
  if (value != 0) {
    while (*value != '\0') {
      appendChar(out, pos, size, *value);
      ++value;
    }
  }
}

// Monta uma linha CSV completa, terminada em '\0'.
// Colunas: seq,gps_valid,date,time,latitude,longitude,altitude_m,speed_kmh,
//          course_deg,satellites,moisture_raw,moisture_pct,crc8
inline void buildCsvRow(const GpsSample& gps, int moistureRaw,
                        float moisturePct, uint32_t seq,
                        char* out, size_t size) {
  if (out == 0 || size == 0) return;
  out[0] = '\0';
  size_t pos = 0;

  appendUnsigned(out, pos, size, seq);
  appendChar(out, pos, size, ',');
  appendUnsigned(out, pos, size, gps.valid ? 1UL : 0UL);
  appendChar(out, pos, size, ',');

  if (gps.timeValid) appendField(out, pos, size, gps.date);
  appendChar(out, pos, size, ',');
  if (gps.timeValid) appendField(out, pos, size, gps.time);
  appendChar(out, pos, size, ',');

  if (gps.valid) appendFixed(out, pos, size, gps.latitude, 6);
  appendChar(out, pos, size, ',');
  if (gps.valid) appendFixed(out, pos, size, gps.longitude, 6);
  appendChar(out, pos, size, ',');
  if (gps.valid) appendFixed(out, pos, size, gps.altitudeMeters, 1);
  appendChar(out, pos, size, ',');
  if (gps.valid) appendFixed(out, pos, size, gps.speedKmh, 1);
  appendChar(out, pos, size, ',');
  if (gps.valid) appendFixed(out, pos, size, gps.courseDeg, 1);
  appendChar(out, pos, size, ',');
  if (gps.valid) appendUnsigned(out, pos, size, gps.satellites);
  appendChar(out, pos, size, ',');

  appendUnsigned(out, pos, size, static_cast<unsigned long>(moistureRaw));
  appendChar(out, pos, size, ',');
  appendFixed(out, pos, size, moisturePct, 1);

  const uint8_t check = crc8(out, pos);
  appendChar(out, pos, size, ',');
  appendHex2(out, pos, size, check);
}

}  // namespace pure
