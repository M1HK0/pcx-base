# Perception.cx Base (pcx-base)

Shared GUI foundation for Perception.cx-style tools on Windows — same menu, config/themes, widgets, and notifications in two implementations.

## What's included

- Custom menu/GUI (windows, tabs, containers, checkboxes, sliders, keybinds, dropdowns, color pickers, and more)
- Config and theme save/load
- Draggable on-screen widgets
- Toast notifications

## Branches

| Branch | Language | Purpose |
|--------|----------|---------|
| [`angelscript`](https://github.com/M1HK0/pcx-base/tree/angelscript) | AngelScript | Scripts in the Perception.cx host (process attach + overlay) |
| [`cpp`](https://github.com/M1HK0/pcx-base/tree/cpp) | C++17 + ImGui | Standalone Win32 + DX11 demo for local UI work |

```bash
git checkout angelscript
git checkout cpp
```

## Quick start

**AngelScript** — see the [`angelscript`](https://github.com/M1HK0/pcx-base/tree/angelscript) branch. Entry: `source/entry.as`. Set the target process in `source/globals.as`.

**C++** — see the [`cpp`](https://github.com/M1HK0/pcx-base/tree/cpp) branch:

```powershell
cd cpp
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\build\Release\perception_gui.exe
```

Requires Windows 10/11, MSVC, CMake 3.20+, and Tahoma fonts in `C:\Windows\Fonts\`.

## License

Open-source GUI base template for Perception.cx tooling.
