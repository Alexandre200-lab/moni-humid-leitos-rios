# ADR 0002 — Sensor capacitivo de umidade do solo

- **Status:** aceito
- **Data:** 2026-11-16

## Contexto

Sensores resistivos de umidade (como FC-28 com eletrodos expostos) sofrem
eletrólise rápida quando permanecem energizados em sedimento úmido, típico de
leitos de rios. A vida útil em campo é curta e a calibração deriva com corrosão.

## Decisão

Usar sensor capacitivo (SEN0193 / v1.2), com o VCC chaveado pelo firmware no
pino D7: o sensor só é alimentado durante a leitura (`Moisture::readRaw`).

## Consequências

- Maior durabilidade e menor consumo.
- A conversão bruta → porcentagem é invertida e calibrável em campo via
  comandos seriais salvos na EEPROM.
- O sensor precisa ser calibrado para cada tipo de sedimento
  ([calibration.md](../calibration.md)).
