# Contribuindo

Obrigado por querer melhorar o **moni-humid-leitos-rios**! Este guia mantém o
projeto consistente e fácil de revisar.

## Antes de começar

- Abra uma issue descrevendo o problema ou a ideia (use os templates disponíveis).
- Verifique se já existe uma issue ou PR relacionado.
- Para mudanças de hardware, registre o rationale em `docs/adr/`.

## Fluxo de trabalho

1. Faça um fork e crie um branch a partir de `main`:
   ```bash
   git checkout -b feat/minha-melhoria
   ```
2. Faça commits pequenos e com mensagens no imperativo:
   ```
   feat: adiciona leitura do sensor DS18B20
   fix: corrige fallback quando o SD falha
   docs: atualiza tabela de pinos
   test: cobre CRC de linha sem GPS
   ```
3. Rode **todas** as verificações antes do PR:
   ```bash
   arduino-cli compile --profile uno --warnings all firmware/moni_humid_leitos_rios

   g++ -std=c++17 -Wall -Wextra -Werror tests/test_pure_logic.cpp -o /tmp/moni-tests
   /tmp/moni-tests

   python3 scripts/check_links.py
   ```
4. Abra o PR usando o template e explique **o quê**, **por quê** e **como testar**.

## Padrões de código

- Namespaces em `UpperCamelCase`; funções e variáveis em `lowerCamelCase`.
- Comentários e documentação em português, acompanhando o resto do projeto.
- Lógica reutilizável vai em `pure_logic.h` (sem `Arduino.h`) e ganha teste em
  `tests/test_pure_logic.cpp`.
- Uma responsabilidade por módulo; `setup()` e `loop()` permanecem enxutos.
- Evite `String` dinâmica e `printf` de ponto flutuante no AVR.
- Novos periféricos entram atrás de feature flag em `config.h` até serem
  validados em campo (veja `docs/field-checklist.md`).

## Se mudou o formato CSV

Atualize **todos** os pontos:

1. `config.h` → `LOG_HEADER` e `CSV_ROW_CAPACITY`
2. `pure_logic.h` → `buildCsvRow()`
3. `tests/test_pure_logic.cpp` → linhas esperadas e contagem de vírgulas
4. `docs/data-format.md` → tabela de colunas e exemplos
5. `examples/analyze_log.ipynb` → se a validação mudar

## Reportando bugs e campanhas

Use os templates de issue: `[Bug]`, `[Feature]` ou `[Campo]`. Para bugs, inclua
hardware, toolchain, passos para reproduzir e a saída do monitor serial.

## Código de conduta

Ao participar, você concorda com o [Código de Conduta](CODE_OF_CONDUCT.md).
