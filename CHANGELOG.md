# Changelog

Todas as mudanças relevantes deste projeto são documentadas aqui. Formato
baseado em [Keep a Changelog](https://keepachangelog.com/pt-BR/1.1.0/) e
[versionamento semântico](https://semver.org/lang/pt-BR/).

## [1.1.0] - 2026-11-17

### Adicionado

- Sensor capacitivo de umidade do solo (A0) com alimentação chaveada (D7).
- Configuração persistida em EEPROM e comandos seriais (`SHOW`, `CAL_DRY=`,
  `CAL_WET=`, `RATE=`, `SAVE`, `RESET`).
- Recuperação automática do cartão SD (nova tentativa a cada 10 s).
- Watchdog `WDTO_8S` e máquina de estados do sistema, com transições na serial.
- Colunas `seq` e `crc8` no CSV (agora 13 colunas, 12 vírgulas).
- Módulos opcionais por *feature flag*: RTC DS3231, LED de status e deep sleep.
- Testes unitários da lógica pura (`tests/test_pure_logic.cpp`) e CI com
  `build`, `unit-tests`, `static-analysis`, `docs` e `doxygen`.
- Release automatizada com `.hex`, Dependabot, `CITATION.cff`, templates de
  issue/PR, Doxygen, troubleshooting, ADRs e checklist de campo.
- Notebook de análise (`examples/analyze_log.ipynb`) com validação de CRC.

### Alterado

- Firmware modularizado (`config`, `types`, `pure_logic`, `diag`, `gps`,
  `moisture`, `storage`, `settings`, `rtc`, `status_led`, `low_power`).
- Data/hora formatadas com zeros à esquerda; umidade preservada sem fix GPS.

### Alterado (cont.)

- Cabeçalho CSV movido para `PROGMEM` (flash): SRAM de 76% → 71%.

### Corrigido

- Bug de `memset` com `strlen` em buffer não inicializado no nome do arquivo.

## [1.0.0] - 2026-11-16

- Versão inicial: logger de GPS com TinyGPS++, SD e SoftwareSerial.
