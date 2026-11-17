/*
 * Aquisicao de dados GPS via TinyGPS++.
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <Arduino.h>

#include "types.h"

namespace Gps {

// Inicializa a UART por software na velocidade do modulo.
void begin();

// Consome bytes NMEA disponiveis. Deve ser chamado com frequencia no loop().
void feed();

// Copia a ultima posicao valida para `out`. Retorna false sem fix.
bool read(GpsReading& out);

}  // namespace Gps
