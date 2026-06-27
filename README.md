# Perception.cx AngelScript Base

> **Branch:** `angelscript` — AngelScript implementation for the Perception.cx script host.  
> See also: [`main`](https://github.com/M1HK0/pcx-base) (overview) · [`cpp`](https://github.com/M1HK0/pcx-base/tree/cpp) (native C++ port)

An AngelScript base for building external tooling. Provides a full custom menu/GUI system, process attachment + memory I/O, and a threading/callback system.

## How this branch differs from `cpp`

| | **AngelScript** (this branch) | **C++** (`cpp` branch) |
|---|-------------------------------|------------------------|
| Runtime | Perception.cx host | Standalone Win32 + DX11 `.exe` |
| Rendering | Host `renderer::` overlay API | ImGui / DirectX 11 window |
| Process memory | Full attachment + read/write/scan | Not included (demo UI only) |
| Build | Load scripts in Perception.cx | CMake + MSVC |
| Entry point | `source/entry.as` | `cpp/src/main.cpp` |

Use **this branch** when building real external tools inside Perception.cx. Use **`cpp`** to prototype the menu locally without the host.

---

## Table of Contents

- [Getting Started](#getting-started)
- [Menu / GUI System](#menu--gui-system)
- [Attachment System](#attachment-system)
- [Threading](#threading)
- [Globals](#globals)

---

## Getting Started

Entry point lives in [source/entry.as](source/entry.as). Minimal skeleton:

```angelscript
int main() {
    globals::username = get_username();
    gui::initialize("Perception.cx", vector2(500, 480));

    if (!globals::process.attach(globals::target_process))
        return 0;

    globals::hwnd = window::find_by_pid(globals::process.pid);
    if (globals::hwnd == 0)
        return 0;

    int x, y, w, h;
    if (!window::get_rect(globals::hwnd, x, y, w, h))
        return 0;

    renderer::fonts::initialize();
    thread::register("on_render", on_render, 0); // 0ms = vsync
    return 1;
}

void on_unload() {
    globals::process.detach();
}
```

The render callback in [source/threads/on_render.as](source/threads/on_render.as) drives the frame:

```angelscript
void on_render(int id, int data_index) {
    set_thread_to_highest_priority();
    int64 t = thread::begin_timing();

    cursor::update();
    gui::tick();
    gui::update();
    widgets::update();
    notifications::update();

    widgets::render();
    gui::render();
    notifications::render();

    thread::end_timing("on_render", t);
}
```

Set your target process in [source/globals.as](source/globals.as) (`globals::target_process`).

---

## Menu / GUI System

Everything lives under [source/dependencies/gui/](source/dependencies/gui/). The hierarchy is:

```
CWindow  ->  CTab  ->  CContainer  ->  CElement (checkbox, slider, ...)
```

### Containers

- **CWindow** — [containers/window.as](source/dependencies/gui/containers/window.as)
- **CTab** — [containers/tab.as](source/dependencies/gui/containers/tab.as)
- **CContainer** — [containers/container.as](source/dependencies/gui/containers/container.as) — factory for all elements

### Elements

| Element | Read value |
|---|---|
| `CCheckbox` | `bool get()` |
| `CSliderInt` / `CSliderFloat` | `int get()` / `float get()` |
| `CKeybind` | `bool is_active()` |
| `CDropdown` | `int get()` / `string get_string()` |
| `CMultiSelect` | `bool get(i)` / `array<int> get_selected()` |
| `CTextInput` | `string get()` |
| `CColorPicker` | `color_t get()` |
| `CListBox` | `array<int> get_selected()` |

Elements support attachments: keybind, colorpicker, cogwheel sub-menus, and tooltips on the same row.

### Config, themes, notifications, widgets

- **Config** — [gui/config_system.as](source/dependencies/gui/config_system.as) — `.cfg` files in `configs/base/`
- **Themes** — [gui/theme_system.as](source/dependencies/gui/theme_system.as) — `.theme` files in `themes/base/`
- **Notifications** — `notifications::create()`, `create_success()`, `create_error()`
- **Widgets** — draggable HUD panels via [gui/widgets.as](source/dependencies/gui/widgets.as)

---

## Attachment System

[source/dependencies/attachment.as](source/dependencies/attachment.as) — attach to a process by name, then use [process.as](source/dependencies/process.as) for typed memory reads/writes, pattern scans, and engine helpers (FString, FVector, etc.).

[window.as](source/dependencies/window.as) and [cursor.as](source/dependencies/cursor.as) handle target window lookup and per-frame input.

---

## Threading

[source/dependencies/thread.as](source/dependencies/thread.as) — register callbacks with `thread::register(name, callback, interval_ms)`. Use `interval_ms = 0` for every-frame (vsync) rendering.

---

## Globals

[source/globals.as](source/globals.as):

```angelscript
string      globals::target_process;   // e.g. "notepad.exe"
CAttachment globals::process;
uint64      globals::hwnd;
string      globals::username;
string      globals::config_path;      // default "configs/base"
uint8       globals::menu_alpha;
float       globals::menu_scale;
array<uint64> globals::fonts;
```
