# DSFex360 development roadmap — v0.3a work

This is an incremental development milestone, not a claim of a complete Nintendo DS emulator.

## Implemented in this branch
- ARM9 register/CPSR state scaffold.
- Basic ARM data-processing interpreter subset.
- Mapped byte-addressable memory foundation.
- NDS header parsing and ARM9/ARM7 section extraction.
- ARM instruction fetch from mapped memory.
- ARM B/BL branch execution with PC+8 target semantics.
- Host-side regression tests and experimental Xenon toolchain workflow.

## Limitations still open
- Full ARMv5TE instruction coverage, exception modes, banked registers, accurate pipeline/timing and coprocessor behavior.
- ARM7TDMI execution core and synchronization between both CPUs.
- Nintendo DS memory map, MMIO, DMA, timers, interrupts and cartridge bus.
- 2D/3D graphics, audio, touch input, firmware/BIOS integration and save states.
- The Xenon workflow has not yet demonstrated a complete DSFex360 target binary.
- No validated Xbox 360 default.xex is produced by this milestone.

## Validation
Run make test on a host C++11 compiler. The GitHub Actions host workflow runs the same test command. Target toolchain status must be checked separately in Actions.
