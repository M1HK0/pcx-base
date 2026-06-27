# Perception.cx GUI — C++ Port

> **Branch:** `cpp` — Standalone Win32 + DirectX 11 port with ImGui.  
> See also: [`main`](https://github.com/M1HK0/pcx-base) (overview) · [`angelscript`](https://github.com/M1HK0/pcx-base/tree/angelscript) (Perception.cx script base)

Standalone demo application porting the Perception.cx AngelScript GUI base to C++ with ImGui as the rendering backend.

## How this branch differs from `angelscript`

| | **C++** (this branch) | **AngelScript** (`angelscript` branch) |
|---|----------------------|----------------------------------------|
| Runtime | Standalone `.exe` | Perception.cx script host |
| Rendering | ImGui + DirectX 11 window | Host overlay `renderer::` API |
| Process memory | Not included | Full attach + read/write/scan |
| Build | CMake + MSVC | Load `.as` scripts in host |
| Entry point | `cpp/src/main.cpp` | `source/entry.as` |
| Use case | Local UI dev / native app | Production external tools |

Use **this branch** to run and tweak the menu without Perception.cx. Use **`angelscript`** for real overlay tooling with memory access.

---

## Requirements

- Windows 10/11
- Visual Studio 2019 or newer (MSVC toolset)
- CMake 3.20+
- Git (for FetchContent dependencies)
- Tahoma fonts at `C:\Windows\Fonts\tahoma.ttf` and `tahomabd.ttf`

## Dependencies (fetched automatically)

- [Dear ImGui](https://github.com/ocornut/imgui) — `docking` branch
- [nlohmann/json](https://github.com/nlohmann/json) — v3.11.3

## Build

```powershell
cd cpp
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Or with Visual Studio 2019:

```powershell
cmake -B build -G "Visual Studio 16 2019" -A x64
cmake --build build --config Release
```

The executable is written to `cpp/build/Release/perception_gui.exe` (or `Debug/perception_gui.exe` for Debug builds).

## Run

```powershell
.\build\Release\perception_gui.exe
```

Run from the `cpp` directory (or any directory) — config and theme files are created relative to the working directory:

- `configs/base/` — `.cfg` config files (base64-encoded JSON)
- `themes/base/` — `.theme` theme files

## Controls

| Key | Action |
|-----|--------|
| **Insert** | Toggle menu open/close (default menu key) |
| **Mouse** | Interact with GUI elements, drag window/widgets |
| **Shift + drag** | Snap widgets to 5px grid |
| **Scroll wheel** | Scroll containers, listboxes, cogwheel popups |

## Features

- Custom renderer wrapping `ImDrawList` matching the AngelScript `renderer::` API
- All GUI elements: checkbox, slider_int, slider_float, keybind, dropdown, multiselect, text_input, button, colorpicker, cogwheel (procedural gear icon), listbox, tooltip, label
- Containers: `CWindow`, `CTab`, `CContainer`
- Config and theme save/load with base64 JSON persistence
- Notifications with slide/fade animations
- Draggable watermark widget
- Identity role display (developer/beta)
- Menu open/close animation with glow and clip rect

## Project Structure

```
cpp/
├── CMakeLists.txt
├── README.md
└── src/
    ├── main.cpp                 # Win32 + DX11 entry point
    └── pcx/
        ├── renderer.cpp         # ImDrawList renderer wrapper
        ├── input.cpp            # Cursor and keyboard input
        ├── gui/
        │   ├── elements_all.hpp # Element class declarations
        │   ├── elements_impl.cpp
        │   ├── containers.cpp   # Window, tab, container
        │   ├── registration.cpp # Tab initialization
        │   └── ...
```

## Notes

- This is a **standalone demo window**, not an overlay.
- Window size is 500×480 with title **Perception.cx**, matching the AngelScript `entry.as` initialization.
- Colors match `gui/config.as` exactly and can be customized via the Themes tab.
