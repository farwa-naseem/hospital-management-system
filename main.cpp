#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#include "GUI.h"
// ── SFML 2.6 link instructions (Visual Studio) ──────────────────────────────
// Project Properties -> Linker -> Input -> Additional Dependencies:
//   Release: sfml-graphics.lib; sfml-window.lib; sfml-system.lib
//   Debug:   sfml-graphics-d.lib; sfml-window-d.lib; sfml-system-d.lib
// Copy sfml-graphics-2.dll, sfml-window-2.dll, sfml-system-2.dll
// from SFML/bin/ next to your .exe
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    GUI app;
    app.run();
    return 0;
}
