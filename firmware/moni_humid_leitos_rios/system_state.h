/*
 * Estados do sistema — usados pelo LED de status e pelo loop principal.
 * Copyright (C) 2026 Alexandre Gouveia
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

namespace SystemState {

enum Value {
  INIT = 0,
  WAITING_GPS,
  LOGGING,
  SD_ERROR
};

inline const char* name(Value state) {
  switch (state) {
    case INIT:        return "INIT";
    case WAITING_GPS: return "WAITING_GPS";
    case LOGGING:     return "LOGGING";
    case SD_ERROR:    return "SD_ERROR";
    default:          return "UNKNOWN";
  }
}

}  // namespace SystemState
