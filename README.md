# DSFex360 v0.2b — Etapa 21

Projeto experimental de emulação de Nintendo DS para Xbox 360 RGH. Este snapshot é uma base de desenvolvimento, não um emulador completo nem um XEX pronto.

## Implementado
- Parser inicial do cabeçalho NDS e validação dos intervalos ARM9/ARM7.
- Carregador inicial de seções e espaço de endereçamento mapeado.
- Estado arquitetural inicial ARM9 e flags CPSR N/Z/C/V.
- Subconjunto experimental de instruções ARM-state com operandos imediatos.
- Etapa 21: executor separado de processamento de dados ARM-state com Operand2 de registrador e shifts imediatos LSL/LSR/ASR/ROR/RRX.
- Condições ARM e testes de host para aritmética, lógica, carry e execução condicional.

## Limitações
O executor continua parcial: não implementa todas as instruções ARM, shifts por registrador, acesso à memória/branches completos, exceções, Thumb, core ARM7, hardware DS completo, vídeo, áudio, entrada ou MMU/CP15. Não há compatibilidade geral com jogos comerciais. Build Xenon real, ELF, default.xex e execução no Xbox 360 RGH não estão validados.

## Testes
Com compilador C++ e Make disponíveis, execute `make test`. Os testes no host não demonstram compatibilidade com Xbox 360.

## Próximos passos
Adicionar testes de limites e melhorar a semântica dos shifts/flags; depois ampliar o conjunto ARM com branches e operações de memória, mantendo cada etapa testável.
