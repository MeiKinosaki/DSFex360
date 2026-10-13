# DSFex360 — toolchain na nuvem

## Build automático

O workflow `Xenon cloud build` usa o container público `free60/libxenon:latest` no GitHub Actions. Não é necessário instalar compilador no computador local.

- Workflow: https://github.com/MeiKinosaki/DSFex360/actions/workflows/xenon-cloud-build.yml
- Para iniciar manualmente: abra o workflow e escolha **Run workflow**.
- Ao terminar com sucesso, abra a execução e baixe o artefato **DSFex360-v0.2b-Xenon-ELF**.

## O que o artefato é

`DSFex360.elf` é um ELF homebrew para a plataforma Xenon, compilado contra libxenon. É o primeiro alvo executável/bootstrapping do projeto; o ponto de entrada atual mantém o processo ativo e ainda não apresenta interface nem executa ROMs. O restante do núcleo experimental é incluído no link para iniciar a integração futura.

## Importante sobre XEX

Este pipeline não gera `default.xex`. LibXenon é uma toolchain homebrew bare-metal que gera ELF; gerar um XEX compatível com o ambiente Xbox 360 normalmente depende de ferramentas e formatos do ecossistema XDK, que não fazem parte desta toolchain pública. Não vamos renomear um ELF para `.xex`, porque isso não o tornaria um XEX válido.

O artefato ainda precisa ser testado em console Xbox 360 RGH/JTAG. Um build bem-sucedido confirma compilação/linkedição, não compatibilidade em hardware.
