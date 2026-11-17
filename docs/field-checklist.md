# Checklist de validação em campo

Os recursos marcados como *feature flag* só podem ser confirmados com o
hardware montado. Este Checklist acompanha a primeira implantação.

## Antes de energizar

- [ ] Chave do GPS Logger Shield em **SW-UART**
- [ ] microSD formatado em FAT32 e com menos de 100 arquivos `gpslogNN.csv`
- [ ] Sensor capacitivo: `AO → A0`, `VCC → D7`, `GND → GND`
- [ ] Polaridade da fonte 5 V conferida
- [ ] Caixa estanque (IP65 ou superior) com passa-fios vedado

## Primeiros minutos (monitor serial a 9600 baud)

- [ ] Banner `moni-humid-leitos-rios v1.1.0`
- [ ] `Cartao SD inicializado.` e nome do arquivo exibidos
- [ ] Comando `SHOW` responde com `CAL_DRY`, `CAL_WET` e `RATE`
- [ ] Aguardar fix GPS e conferir `gps_valid = 1` no CSV
- [ ] Conferir que `date`/`time` fazem sentido (UTC)

## Calibração

- [ ] Medir valor bruto em solo seco → `CAL_DRY=<n>`
- [ ] Medir valor bruto em solo saturado → `CAL_WET=<n>`
- [ ] `SAVE` para gravar na EEPROM
- [ ] Repetir a leitura e conferir ~0% e ~100%
- [ ] Anotar os valores no relato de campanha (issue template `[Campo]`)

## Recursos opcionais (ativar flags em `config.h` um de cada vez)

- [ ] `MONI_ENABLE_STATUS_LED = 1` — LED em D6 pisca conforme o estado
- [ ] `MONI_ENABLE_RTC = 1` — DS3231 detectado; linhas sem GPS mantêm data/hora
- [ ] `MONI_ENABLE_LOW_POWER = 1` — consumo cai em sleep; **atenção:** o WDT do
      AVR dorme no máximo ~8 s por ciclo, então o intervalo efetivo de amostragem
      muda. Meça o consumo antes de adotar em campanha longa.

## Integridade dos dados

- [ ] Abrir `examples/analyze_log.ipynb` e confirmar 100% das linhas com CRC válido
- [ ] Verificar que o número de vírgulas é 12 em todas as linhas
- [ ] Arquivar o CSV com o relato da campanha
