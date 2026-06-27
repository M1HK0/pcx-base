# Perception.cx Base (pcx-base)

A shared GUI foundation for building external tooling on Windows. Both branches ship the same menu system, config/theme persistence, notifications, widgets, and visual design — implemented in two different languages and runtimes.

## What this is

**pcx-base** is a starter template for Perception.cx-style external tools. It includes:

- A custom menu/GUI (windows, tabs, containers, checkboxes, sliders, keybinds, dropdowns, color pickers, cogwheel sub-menus, listboxes, tooltips, and more)
- Config and theme save/load (base64-encoded JSON on disk)
- On-screen draggable widgets (watermark/HUD panels)
- Toast notifications with animations
- A consistent dark theme and menu open/close animations

The **AngelScript** branch is the production base for Perception.cx scripts: it attaches to a target process, reads/writes memory, and renders as an overlay via the host runtime.

The **C++** branch is a standalone Win32 + DirectX 11 demo that ports the same GUI to native code with ImGui — useful for local development, UI prototyping, or building a native app without the script host.

## Branches

| Branch | Language | Use when |
|--------|----------|----------|
| [`angelscript`](../../tree/angelscript) | AngelScript (`.as`) | Building scripts inside the Perception.cx host with process attachment and overlay rendering |
| [`cpp`](../../tree/cpp) | C++17 + ImGui | Running a standalone desktop window, testing GUI changes locally, or extending the native port |

Switch branches with:

```bash
git checkout angelscript   # AngelScript base
git checkout cpp           # C++ port
```

## AngelScript vs C++ — key differences

| | **AngelScript** (`angelscript`) | **C++** (`cpp`) |
|---|--------------------------------|-----------------|
| **Runtime** | Perception.cx script host | Standalone `.exe` (Win32 + DX11) |
| **Rendering** | Host `renderer::` API | ImGui / custom `ImDrawList` wrapper |
| **Process attachment** | Yes — `CAttachment`, memory I/O, pattern scan | No — demo window only |
| **Overlay** | Renders over the attached game/app window | Own 500×480 window titled "Perception.cx" |
| **Threading** | Host callback threads (`thread::register`) | Standard Win32 message loop |
| **Build** | Loaded by Perception.cx (no separate compile step) | CMake + MSVC (`cmake -B build && cmake --build build`) |
| **Dependencies** | Provided by the host | ImGui, nlohmann/json (fetched via CMake) |
| **Source layout** | `source/` — entry, GUI, features, threads | `cpp/src/` — same GUI structure in C++ |
| **Config / themes** | `configs/base/`, `themes/base/` | Same paths, relative to working directory |

### Same on both branches

- GUI hierarchy: `CWindow` → `CTab` → `CContainer` → elements
- Element types and attachment model (keybind/colorpicker/cogwheel/tooltip on rows)
- Config and theme file format (`.cfg` / `.theme`, base64 JSON)
- Default colors, menu key (Insert), widget snapping, notification styles
- Example tabs: Settings, Themes, Widgets demo

### Choose AngelScript if you…

- Write tools for Perception.cx and need process memory access
- Want the overlay to draw on top of a target application
- Rely on the host for fonts, input, and render callbacks

### Choose C++ if you…

- Want to run and debug the menu without the Perception.cx host
- Prefer native code, CMake, and ImGui
- Are porting or validating GUI behavior before scripting

## Quick start

### AngelScript

See the [`angelscript`](../../tree/angelscript) branch. Entry point: `source/entry.as`. Load the project in Perception.cx and set your target process in `source/globals.as`.

### C++

See the [`cpp`](../../tree/cpp) branch.

```powershell
cd cpp
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\build\Release\perception_gui.exe
```

Requires Windows 10/11, MSVC, CMake 3.20+, and Tahoma fonts in `C:\Windows\Fonts\`.

## Project structure (main)

```
pcx-base/
├── source/          # AngelScript — scripts, GUI, features (angelscript branch)
├── cpp/             # C++ port — CMake project, ImGui backend (cpp branch)
└── README.md        # This file (overview on main; branch-specific docs on each branch)
```

## License

Private repository — Perception.cx internal base template.
