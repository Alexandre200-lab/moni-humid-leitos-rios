# Telemetria (planejada)

A versão atual grava em microSD e não transmite dados. Esta página define o
contrato para uma futura versão com ESP32 + LoRa/Wi-Fi, sem alterar o CI do Uno.

## Contrato da interface

```cpp
class Telemetry {
 public:
  virtual bool begin() = 0;
  virtual bool send(const SensorReading& reading) = 0;
};
```

Requisitos:

- Implementação padrão **no-op** no Uno (compila e não consome pinos).
- Não bloquear o `loop()`: envio assíncrono ou com timeout curto.
- Reutilizar `SensorReading`; não criar um formato de dados paralelo.
- Falha de transmissão **nunca** impede a gravação no SD.

## Esboço para ESP32

| Item | Sugestão |
|---|---|
| Rede | LoRaWAN 915 MHz (região ANATEL) ou Wi-Fi MQTT |
| Payload | CBOR ou JSON compacto com `seq`, lat/lng, `moisture_pct` |
| Backup | microSD continua como fonte authoritative |
| Energia | deep sleep entre envios, bateria Li-ion + painel solar |

## Por que não entrou nesta entrega

A placa alvo é ATmega328P, sem rádio integrado. Adicionar telemetria agora
exigiria um segundo perfil de build (ESP32) e hardware ainda não disponível
para validação. O contrato acima deixa o caminho aberto sem comprometer o CI.
