# Hardware — netlist e diretrizes

Esta pasta reúne o insumo para uma futura placa dedicada. O projeto **atual**
usa shield e sensores de prateleira (veja [docs/hardware.md](../docs/hardware.md)).

## Netlist

[`netlist.csv`](netlist.csv) lista cada conexão (from, to, sinal). É o ponto de
partida para desenhar o esquemático no KiCad.

## Diretrizes de projeto

- Plano de terra contínuo sob a área analógica; separar o retorno do sensor do
  retorno do motor/relé, se houver.
- Capacitor de 100 nF o mais próximo possível do VCC de cada módulo.
- Sensor capacitivo com trilha curta entre `AO` e `A0`; evitar trilhas paralelas
  ao clock SPI (D13).
- Conector de cartão SD com retenção mecânica.
- Regulador 5 V com margem térmica; entrada protegida contra polaridade
  invertida e surtos.
- Caixa IP65 com passa-fios e dreno para condensação.

## Status

Esquemático `.sch` e layout de PCB ainda não foram criados — escrever arquivos
KiCad à mão é frágil. A netlist + estas diretrizes são a entrada para uma sessão
dedicada de CAD.
