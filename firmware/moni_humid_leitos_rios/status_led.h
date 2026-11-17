/*
 * LED de status (feature flag MONI_ENABLE_STATUS_LED).
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "system_state.h"

namespace StatusLed {

void begin();
void update(SystemState::Value state);

}  // namespace StatusLed
