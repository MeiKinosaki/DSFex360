// DSFex360 bare-metal Xbox 360 bootstrap entry point.
// This only proves that the project can be linked for Xenon; it is not yet
// an interactive emulator frontend.
int main() {
    // Keep the homebrew process alive while the emulator runtime is developed.
    for (;;) {
#if defined(__GNUC__)
        __asm__ __volatile__("or 27,27,27");
#endif
    }
    return 0;
}
