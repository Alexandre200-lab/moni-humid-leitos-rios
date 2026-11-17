# ADR 0003 — CSV com nomes 8.3, sequência e CRC-8

- **Status:** aceito
- **Data:** 2026-11-16

## Contexto

A biblioteca `SD` do Arduino usa nomes 8.3 e o sistema roda em campo, sem
supervisão. Linhas podem ser corrompidas por perda de energia durante a escrita,
e campanhas anteriores não podem ser sobrescritas.

## Decisão

- Arquivos `gpslog00.csv` … `gpslog99.csv` (até 100 campanhas).
- Uma linha por amostra, com `seq` monotônico e `crc8` calculado sobre os
  campos anteriores.
- A linha é montada em um único buffer e gravada com uma chamada `println()`.

## Consequências

- Campanhas antigas são preservadas; o firmware nunca sobrescreve um arquivo.
- O CRC permite detectar linhas parciais/corrompidas na análise
  (`examples/analyze_log.ipynb`).
- O formato tem 13 colunas fixas; mudanças exigem atualizar
  [data-format.md](../data-format.md), o cabeçalho em `config.h` e os testes.
- O `seq` é volátil (reinicia a cada boot); use `date`/`time` para ordenação
  entre campanhas.
