# DSFex360 v0.2b — Etapa 20

Projeto experimental de emulação de Nintendo DS para Xbox 360 RGH. Este snapshot é uma base de desenvolvimento, não um emulador completo nem um XEX pronto.

## Implementado
- Parser inicial do cabeçalho NDS e validação dos intervalos ARM9/ARM7.
- Carregador inicial de seções e espaço de endereçamento mapeado.
- Estado arquitetural inicial ARM9, registradores e flags CPSR N/Z/C/V.
- Executor experimental de um subconjunto de instruções ARM-state com operandos imediatos.
- Avaliação de condições ARM e decodificador separado de shifts LSL/LSR/ASR/ROR/RRX.
- Testes unitários portáveis.

## Limitações
O decodificador de shifts ainda não está integrado ao executor de instruções com Operand2 de registrador. Ainda não há loop completo de busca/execução, Thumb completo, core ARM7, mapa de hardware DS completo, vídeo, áudio, entrada, MMU/CP15 ou compatibilidade geral com jogos comerciais. Build Xenon real, ELF, default.xex e execução no Xbox 360 RGH não estão validados.

## Testes
Com compilador C++ e Make disponíveis, execute `make test`. Os testes no host não demonstram compatibilidade com Xbox 360. Consulte README-CLOUD-TOOLCHAIN.md para informações sobre a toolchain.

## Próximos passos
Integrar Operand2 ao executor ARM9, ampliar instruções e testes, implementar Thumb e ARM7, e só então validar ELF/XEX com uma toolchain Xenon real e hardware Xbox 360 RGH.