# DSFex360 v0.2b — Etapa 21

Projeto experimental de emulação de Nintendo DS para Xbox 360 RGH. Esta branch inclui testes portáveis e uma tentativa automatizada de build com a toolchain libxenon.

## Implementado
- Parser inicial do cabeçalho NDS e validação dos intervalos ARM9/ARM7.
- Carregador inicial de seções e espaço de endereçamento mapeado.
- Estado arquitetural inicial ARM9 e flags CPSR N/Z/C/V.
- Subconjunto experimental de instruções ARM-state com operandos imediatos e Operand2 com registrador.
- Condições ARM e testes de host.
- Workflow de CI para testes de host e workflow separado para build em ambiente libxenon.

## Toolchain na nuvem
O workflow `Xenon cloud build` tenta compilar o alvo experimental no ambiente `free60/toolchain` e publicar o ELF como artefato do GitHub Actions se a compilação concluir. O resultado só é confirmado quando a execução do workflow terminar com sucesso.

## Limitações
O executor ARM9 continua parcial. Ainda não há conjunto completo ARM/Thumb, core ARM7, hardware DS completo, vídeo, áudio, entrada, MMU/CP15 nem compatibilidade geral com jogos comerciais. Mesmo que o ELF seja gerado, isso não garante um `default.xex` compatível nem execução no Xbox 360 RGH; esses passos precisam ser validados separadamente.

## Testes
Com compilador C++ e Make disponíveis, execute `make test`. Para tentar compilar o alvo Xenon, consulte `README-CLOUD-TOOLCHAIN.md` e execute o workflow `Xenon cloud build` no GitHub Actions.