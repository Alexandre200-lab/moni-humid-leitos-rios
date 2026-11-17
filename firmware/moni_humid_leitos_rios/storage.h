/*
 * Persistencia das amostras em cartao microSD (CSV).
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <Arduino.h>

#include "types.h"

namespace Storage {

// Primeira tentativa de inicializacao.
bool begin();

// Tenta recuperar o SD periodicamente se ele falhou no boot.
void poll();

bool isReady();
uint8_t failureCount();

// Cria o proximo arquivo gpslogNN.csv e grava o cabecalho.
bool prepareNewFile();

// Acrescenta uma linha ao arquivo atual (com seq e crc8).
bool appendRow(const SensorReading& reading);

// Nome do arquivo atual (vazio quando nenhum foi criado).
const char* currentFileName();

}  // namespace Storage
