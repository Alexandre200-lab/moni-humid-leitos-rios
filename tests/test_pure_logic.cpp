/*
 * Testes unitarios da logica pura (sem hardware, executados no CI).
 * g++ -std=c++17 -Wall -Wextra -Werror tests/test_pure_logic.cpp -o /tmp/tests
 */
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>

#include "../firmware/moni_humid_leitos_rios/pure_logic.h"

int main() {
  using namespace pure;

  // --- Umidade ---------------------------------------------------------------
  assert(moisturePercent(820, 820, 380) == 0.0f);    // solo seco
  assert(moisturePercent(380, 820, 380) == 100.0f);  // saturado
  assert(moisturePercent(600, 820, 380) == 50.0f);   // meio da faixa
  assert(moisturePercent(0, 820, 380) == 100.0f);    // abaixo do minimo
  assert(moisturePercent(1023, 820, 380) == 0.0f);   // acima do maximo
  assert(moisturePercent(500, 500, 500) == 0.0f);    // calibracao degenerada

  // --- Data e hora -----------------------------------------------------------
  char date[11] = {0};
  char time[9] = {0};
  assert(formatGpsDate(1, 2, 2026, date, sizeof(date)));
  assert(std::strcmp(date, "01/02/2026") == 0);
  assert(formatGpsTime(9, 5, 7, time, sizeof(time)));
  assert(std::strcmp(time, "09:05:07") == 0);
  assert(!formatGpsDate(1, 1, 2026, date, 5));  // buffer pequeno

  // --- CRC-8: vetor conhecido "123456789" = 0xF4 ----------------------------
  assert(crc8("123456789", 9) == 0xF4);

  // --- Linha CSV com GPS valido ---------------------------------------------
  GpsSample sample;
  sample.valid = true;
  sample.timeValid = true;
  sample.date = date;
  sample.time = time;
  sample.latitude = -3.119167;
  sample.longitude = -60.021111;
  sample.altitudeMeters = 92.4;
  sample.speedKmh = 0.0;
  sample.courseDeg = 0.0;
  sample.satellites = 8;

  char row[128] = {0};
  buildCsvRow(sample, 612, 46.6f, 7, row, sizeof(row));
  std::printf("linha valida: %s\n", row);

  const std::string expected =
      "7,1,01/02/2026,09:05:07,-3.119167,-60.021111,92.4,0.0,0.0,8,612,46.6,";
  assert(std::strncmp(row, expected.c_str(), expected.size()) == 0);

  // 13 colunas => 12 virgulas antes do fim da linha.
  const char* checksumSep = std::strrchr(row, ',');
  assert(checksumSep != nullptr);
  size_t commaCount = 0;
  for (const char* p = row; *p != '\0'; ++p) {
    if (*p == ',') ++commaCount;
  }
  assert(commaCount == 12);

  char expectedHex[4] = {0};
  std::snprintf(expectedHex, sizeof(expectedHex), "%02X",
                crc8(row, static_cast<size_t>(checksumSep - row)));
  assert(std::strcmp(checksumSep + 1, expectedHex) == 0);

  // --- Linha CSV sem GPS (umidade nao pode ser perdida) ---------------------
  GpsSample noGps;
  noGps.valid = false;
  noGps.timeValid = false;
  noGps.date = "";
  noGps.time = "";
  noGps.latitude = 0;
  noGps.longitude = 0;
  noGps.altitudeMeters = 0;
  noGps.speedKmh = 0;
  noGps.courseDeg = 0;
  noGps.satellites = 0;

  buildCsvRow(noGps, 618, 44.2f, 8, row, sizeof(row));
  std::printf("linha sem GPS: %s\n", row);
  const std::string expectedNoGps = "8,0,,,,,,,,,618,44.2,";
  assert(std::strncmp(row, expectedNoGps.c_str(), expectedNoGps.size()) == 0);

  // --- RTC fornece hora sem GPS ---------------------------------------------
  noGps.timeValid = true;
  noGps.date = date;
  noGps.time = time;
  buildCsvRow(noGps, 620, 43.0f, 9, row, sizeof(row));
  assert(std::strncmp(row, "9,0,01/02/2026,09:05:07,", 24) == 0);

  std::puts("Todos os testes passaram.");
  return 0;
}
