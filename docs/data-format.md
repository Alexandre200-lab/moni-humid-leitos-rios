# Formato dos dados

Cada amostra gera uma linha CSV em `gpslogNN.csv`. O cabeçalho é gravado
automaticamente na criação do arquivo. São **13 colunas fixas** e 12 vírgulas.

## Colunas

| # | Coluna | Tipo | Exemplo | Descrição |
|---:|---|---|---|---|
| 1 | `seq` | inteiro | `7` | Número da amostra (reinicia a cada boot) |
| 2 | `gps_valid` | 0/1 | `1` | Indica se as coordenadas são válidas |
| 3 | `date` | DD/MM/AAAA | `16/11/2026` | Data do GPS ou do RTC |
| 4 | `time` | HH:MM:SS | `14:35:07` | Hora UTC do GPS ou do RTC |
| 5 | `latitude` | graus decimais | `-3.119167` | 6 casas decimais |
| 6 | `longitude` | graus decimais | `-60.021111` | 6 casas decimais |
| 7 | `altitude_m` | metros | `92.4` | 1 casa decimal |
| 8 | `speed_kmh` | km/h | `0.0` | Velocidade de solo |
| 9 | `course_deg` | graus | `0.0` | Rumo |
| 10 | `satellites` | inteiro | `8` | Satélites em uso |
| 11 | `moisture_raw` | 0..1023 | `612` | Leitura bruta do ADC |
| 12 | `moisture_pct` | 0.0..100.0 | `46.6` | Umidade calibrada |
| 13 | `crc8` | hexadecimal (2 dígitos) | `37` | CRC-8 (poli 0x07) dos campos 1..12 |

Quando `gps_valid = 0`, as colunas 5 a 10 ficam vazias, mas umidade, sequência
e CRC continuam preenchidos. Com RTC ativo (`MONI_ENABLE_RTC = 1`), data/hora
também são gravadas nesse caso. O número de vírgulas é sempre 12.

## Exemplo

```csv
seq,gps_valid,date,time,latitude,longitude,altitude_m,speed_kmh,course_deg,satellites,moisture_raw,moisture_pct,crc8
7,1,01/02/2026,09:05:07,-3.119167,-60.021111,92.4,0.0,0.0,8,612,46.6,37
8,0,,,,,,,,,618,44.2,D6
9,0,01/02/2026,09:05:17,,,,,,,,620,43.0,5A
```

> Os CRC do exemplo foram gerados pela própria lógica do firmware e são
> verificados em `tests/test_pure_logic.cpp`.

## Validação e importação

O notebook [`examples/analyze_log.ipynb`](../examples/analyze_log.ipynb), valida
o CRC de cada linha antes de plotar:

```python
import pandas as pd

df = pd.read_csv("gpslog00.csv")

def crc8(text: str) -> int:
    crc = 0
    for ch in text.encode():
        crc ^= ch
        for _ in range(8):
            crc = ((crc << 1) ^ 0x07) & 0xFF if crc & 0x80 else (crc << 1) & 0xFF
    return crc

lines = open("gpslog00.csv").read().splitlines()[1:]
ok = [int(line.rpartition(",")[2], 16) == crc8(line.rpartition(",")[0])
      for line in lines]
df = df[ok]  # descarta linhas corrompidas
df = df[df["gps_valid"] == 1]  # apenas amostras georreferenciadas
```
