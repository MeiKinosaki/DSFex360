# DSFex360

Nintendo DS emulator project targeting Xbox 360 RGH. Current development version: **v0.2b**.

> **Status:** early host-tested emulator scaffolding. This repository does not yet contain a complete DS emulator or a verified Xbox 360 executable.

## Current scope

- Nintendo DS ROM header parsing and basic section loader.
- A mapped-region address-space abstraction with bounds/overlap checks.
- Initial ARM9 architectural state (registers and CPSR flags).
- An early ARM-state data-processing instruction slice, including condition checks and immediate/register-shifted operands.
- Host-side unit tests and an experimental GitHub Actions Xenon-toolchain check.

## Build host tests

A C++11 compiler and GNU Make are required:

```sh
make test
```

These tests run on the host computer; they are not Xbox 360 tests.

## Xenon toolchain and console status

The cloud workflow is experimental and may need upstream-path or dependency adjustments. It must succeed in GitHub Actions before its toolchain can be considered available. A successful upstream toolchain smoke test would **not** by itself build DSFex360.

No validated `DSFex360.elf` or `default.xex` is currently included. Xbox 360 RGH/XeLL loading and hardware execution have not been verified.

## Next milestones

1. Publish and run the host-tested source/test suite.
2. Make the cloud Xenon toolchain workflow reproducible.
3. Add a Xenon-specific entry point and linker configuration and build this project’s own ELF.
4. Inspect the ELF and validate it on Xbox 360 RGH/XeLL.
5. Continue ARM9/ARM7 CPU execution, memory/MMIO, interrupts, graphics, audio and input incrementally.

Development advances one tested stage at a time; planned work is not marked complete until the tests/build actually pass.
