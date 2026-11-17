# Solução de problemas

## O GPS nunca dá fix

- Confirme que a chave do shield está em **SW-UART**.
- Verifique se `PIN_GPS_TX` / `PIN_GPS_RX` correspondem à sua placa. O padrão
  preserva o sketch original `(2, 3)`; se necessário, inverta os dois valores.
- Aguarde até 5 minutos ao ar livre. Antenas com vista obstruída podem nunca fixar.
- O LED (quando ativado) pisca 2 vezes nesse estado.
- Sem GPS, o firmware continua gravando umidade. Se houver RTC ativo, data/hora
  também são preenchidas; a coluna `gps_valid` ficará `0`.

## "Falha ao inicializar o cartao SD"

- Confirme que o cartão está formatado em **FAT32** (não exFAT) e tem até 32 GB.
- Verifique o pino `PIN_SD_CS` (padrão D8).
- O firmware **tenta de novo a cada 10 s** (`SD_RETRY_MS`). Deixe o sistema ligado
  e acompanhe a serial; a mensagem `Cartao SD recuperado.` aparece quando volta.
- Se o cartão estiver cheio de campanhas antigas, o limite é `gpslog00` a
  `gpslog99`. Retire e arquive os arquivos antes de continuar.

## Umidade sempre 0% ou sempre 100%

- Rode `SHOW` no monitor serial e confira `CAL_DRY` / `CAL_WET`.
- Refaça a calibração conforme [calibration.md](calibration.md): os valores padrão
  (820/380) são apenas orientativos.
- Se `CAL_DRY` e `CAL_WET` estiverem iguais, o firmware rejeita e volta ao padrão.
- Lembre-se: sensores capacitivos são **inversos** — solo úmido dá valor bruto menor.

## Linhas CSV com campos vazios

Isso é esperado quando `gps_valid = 0`: as colunas de posição ficam vazias, mas
umidade, sequência e CRC continuam preenchidos. O número de vírgulas é sempre 12.
Use o notebook `examples/analyze_log.ipynb` para validar o CRC de cada linha.

## "Comando desconhecido" ao configurar

Os comandos são `SHOW`, `CAL_DRY=`, `CAL_WET=`, `RATE=`, `SAVE` e `RESET`,
terminados por Enter, a 9600 baud. Não use espaços ao redor do `=`.
Lembranças alteradas com `CAL_DRY=` / `CAL_WET=` / `RATE=` só persistem após `SAVE`.

## O sistema reinicia sozinho

O watchdog está ativo (`WDTO_8S`) e reinicia o microcontrolador se o `loop()`
travar por mais de 8 s. Reinícios repetidos indicam código bloqueante — evite
`delay()` longos e nunca use `while()` sem condição de saída. Se você adicionou
um sensor com biblioteca que bloqueia, verifique-a.

## Compilação falha no CI mas passa na IDE

A IDE inclui a biblioteca `SD` no core AVR; o Arduino CLI não. Ambas estão
declaradas em `sketch.yaml`, e o CI instala `TinyGPSPlus`, `SD` e `RTClib`.
Use sempre `arduino-cli compile --profile uno` para reproduzir o CI localmente.
