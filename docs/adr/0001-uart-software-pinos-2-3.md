# ADR 0001 — UART por software nos pinos 2 e 3

- **Status:** aceito
- **Data:** 2026-11-16

## Contexto

O GPS Logger Shield v2.0 usa uma chave para selecionar UART por software (SW-UART)
ou hardware (HW-UART). No Uno, a UART de hardware (pinos 0/1) é usada pelo
monitor serial e pela gravação do firmware, então não está disponível.

## Decisão

Manter `SoftwareSerial(2, 3)` — exatamente a ordem do sketch original — e a
chave do shield em SW-UART.

## Consequências

- O comportamento do hardware existente não é alterado.
- Se outro lote de shield usar a pinagem invertida, basta trocar `PIN_GPS_TX` e
  `PIN_GPS_RX` em `config.h`; a abstração `Gps` isola essa mudança.
- `HardwareSerial` (Serial1) poderia ser usado em placas como Leonardo, mas
  exigiria um perfil separado no `sketch.yaml`.
