/*
 * Leitura do sensor capacitivo de umidade do solo.
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <Arduino.h>

namespace Moisture {

// Configura os pinos e deixa o sensor desligado (economia e menor corrosao).
void begin();

// Alimenta o sensor, estabiliza e devolve a leitura bruta (0..1023).
int readRaw();

// Converte a leitura bruta em porcentagem (0..100) usando a calibracao.
float toPercent(int raw);

}  // namespace Moisture
