# Calibração do sensor de umidade

O sensor capacitivo fornece um valor bruto entre 0 e 1023. Sensores capacitivos
são **inversos**: solo seco gera valor alto e solo saturado gera valor baixo.
O firmware converte com `pure::moisturePercent()` (equivalente ao `map()`):

```
moisture_pct = map(raw, CAL_DRY, CAL_WET, 0, 100)
```

Os padrões (`CAL_DRY = 820`, `CAL_WET = 380`) são apenas orientativos e devem
ser ajustados para cada sensor e cada tipo de sedimento.

## Procedimento em campo

1. Retire o sensor do leito e limpe a sonda.
2. Meça o valor bruto em **solo seco** (ou ar, como referência de secura):
   carregue temporariamente um sketch que faça `Serial.println(analogRead(A0))`.
3. Meça o valor bruto em **solo saturado** (amostra do próprio leito encharcada).
4. No monitor serial a 9600 baud do firmware principal, envie:

```
CAL_DRY=815
CAL_WET=390
SAVE
SHOW
```

5. `SAVE` grava na EEPROM; `SHOW` confirma os valores persistidos.
6. Recoloque o sensor e confira que a leitura faz sentido.

## Comandos disponíveis

| Comando | Efeito |
|---|---|
| `SHOW` | Mostra `CAL_DRY`, `CAL_WET` e `RATE` |
| `CAL_DRY=<0..1023>` | Define a leitura em solo seco |
| `CAL_WET=<0..1023>` | Define a leitura em solo saturado |
| `RATE=<1000..60000>` | Intervalo entre amostras, em ms |
| `SAVE` | Persiste na EEPROM |
| `RESET` | Restaura os padrões e salva |

Sem `SAVE`, alterações valem apenas até o próximo reboot. Valores inválidos
(sensor igual, fora da faixa) são rejeitados com mensagem na serial.

## Dicas

- Repita a calibração com o sensor instalado: água e temperatura do sedimento
  deslocam a leitura.
- Leitura instável indica alimentação insuficiente; aumente `SENSOR_WARMUP_MS`.
- Registre os valores usados no relato de campanha (issue template `[Campo]`).
