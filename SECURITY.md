# Política de segurança

## Versões suportadas

| Versão | Suporte |
|---|---|
| 1.1.x | ✅ correções e novas funcionalidades |
| 1.0.x | ⚠️ apenas correções críticas |

## Reportando vulnerabilidades

**Não abra uma issue pública** com detalhes de uma vulnerabilidade.

1. Use **Security → Report vulnerability** (ou, se preferir, envie um e-mail ao
   mantenedor associado à conta `Alexandre200-lab` no GitHub).
2. Inclua: versão afetada, impacto, passos para reproduzir e, se possível, uma
   correção sugerida.
3. Você receberá um retorno em até 7 dias úteis com a análise e o plano.

Este projeto roda em hardware dedicado e em ambiente de campo; riscos típicos
são gravação de dados corrompidos, ataques de "juice jacking" na porta USB e
manipulação do cartão SD. Se o seu uso envolver transmissão de dados, siga as
orientações de [telemetria](docs/telemetry.md) e não exponha o canal serial.
