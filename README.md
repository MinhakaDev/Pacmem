# Pacmem

A lightweight memory scanner and editor for Linux, written in modern C++23 with a docked [Dear ImGui](https://github.com/ocornut/imgui) interface.

Pacmem lets you attach to a running process, search its writable memory for a value, narrow the results down with follow-up scans, and write new values back. It works the same way Cheat Engine does, but it's built for Linux on top of `ptrace` and `/proc`.

---

## Features

- **Process picker**: lists every running process by name and connects with a double-click.
- **Exact value scan**: finds every address holding a given value.
- **Unknown initial value scan**: takes a full snapshot of memory when you don't know the starting value.
- **Follow-up scans** to narrow results:
  - **ReScan** (still equals a value)
  - **Higher** / **Lower** (value increased / decreased)
  - **Same** / **Changed** (value unchanged / changed)
- **Live memory table**: shows matching addresses and their current values.
- **Memory editing**: double-click a result to write a new value into the target process.
- **Supported types**: `int32`, `int64`, `float`
- **Tiling, dockable UI**: panels can be rearranged and docked, much like a tiling window manager.

## How it works

| Step | Mechanism |
|------|-----------|
| Find the process | Reads `/proc/<pid>/comm` for every PID |
| Find scannable memory | Parses `/proc/<pid>/maps` and keeps only **writable** regions |
| Pause the target | `ptrace(PTRACE_ATTACH)` + `waitpid` |
| Read / write memory | `pread` / `pwrite` on `/proc/<pid>/mem` |
| Resume the target | `ptrace(PTRACE_DETACH)` |

The binary needs the `cap_sys_ptrace` capability so it can attach to processes it didn't spawn. The build sets this for you (see below).

---

## Requirements

- **Linux** (Pacmem relies on `/proc` and `ptrace`)
- **CMake ≥ 3.20**
- **A C++23 compiler**: GCC 14+ (or Clang with libstdc++ 14+), needed for `<print>` and `<stacktrace>`
- **OpenGL 3.3** capable GPU and drivers
- **GLFW build dependencies** (X11 and Wayland development headers)
- `setcap` (from `libcap`)

Dear ImGui (`v1.92.0-docking`) and GLFW (`3.4`) are downloaded automatically by CMake's `FetchContent`, so you don't install them yourself.

### Installing dependencies

**Arch Linux**
```bash
sudo pacman -S --needed base-devel cmake git mesa libx11 libxrandr libxinerama \
    libxcursor libxi wayland wayland-protocols libxkbcommon libcap
```

**Debian / Ubuntu**
```bash
sudo apt install build-essential g++-14 cmake git libgl-dev libx11-dev libxrandr-dev \
    libxinerama-dev libxcursor-dev libxi-dev libwayland-dev wayland-protocols \
    libxkbcommon-dev libcap2-bin
```

---

## Building

```bash
git clone https://github.com/MinhakaDev/Pacmem.git
cd Pacmem
cmake -S . -B build
cmake --build build -j
```

After it links, the build runs:

```bash
sudo setcap cap_sys_ptrace=eip build/pacmem
```

so expect a `sudo` password prompt at the end of the build. The capability has to be set again each time the binary is rebuilt, and the build does that automatically.

## Running

Pacmem loads its font from `fonts/Roboto-Medium.ttf` **relative to the current working directory**. Dear ImGui ships this font, so you can copy it from the downloaded sources:

```bash
mkdir -p fonts
cp build/_deps/imgui-src/misc/fonts/Roboto-Medium.ttf fonts/
./build/pacmem
```

---

## Usage

1. Open **Process** in the menu bar and **double-click** the process you want, or select it and click **Connect**. Click **Update** to refresh the list.
2. Choose a **Type** (`int32`, `int64` or `float`) in the **Pacmem** panel.
3. Start a search:
   - **Known value:** type it into **Value** and click **Scan**.
   - **Unknown value:** click **Unknow** to take a baseline snapshot.
4. Change the value in the target program, then narrow the results:
   - **ReScan** with the new exact value, or
   - **Higher** / **Lower** / **Same** / **Changed**
5. Repeat until only a few addresses are left in the **Memory Table**.
6. **Double-click** an address, enter a **New Value**, and click **Write**.

> **Tip:** right now every scan button needs some text in the **Value** field, even the ones that don't use it (Unknown, Higher, Lower, Same, Changed).

---

## Project structure

```
Pacmem/
├── CMakeLists.txt          # Build config + post-build setcap
├── cmake/
│   └── ImGui.cmake         # Fetches Dear ImGui and GLFW
└── src/
    ├── main.cpp            # Entry point / main loop
    ├── Menu.{h,cpp}        # Window, OpenGL and ImGui setup, panel layout
    ├── Scanner.{h,cpp}     # Snapshotting and scan / rescan logic (templated per type)
    ├── process.{h,cpp}     # /proc parsing, ptrace attach/detach, memory read/write
    ├── ErrorReporter.{h,cpp}
    ├── UIContext.h         # Shared UI state (selected type, current screen…)
    └── Panels/
        ├── Panel.h         # Base panel interface
        ├── MenuPanel       # Top menu bar
        ├── ProcessPanel    # Process selection window
        ├── MainPanel       # Type selector + scan controls
        ├── MemoryScanned   # Results table + value editor
        ├── TypeRegistry.h  # Per-type function table (int32 / int64 / float)
        └── TypeSelectPanel
```

## Roadmap

- [ ] Show more than the first 50 results / add pagination
- [ ] Full type support for the Same / Changed scans
- [ ] `File → Open` (save/load address lists)
- [ ] Disassembly view using [Capstone](https://www.capstone-engine.org/)

---

## Disclaimer

Pacmem is a learning project for exploring how processes and memory work on Linux. Only use it on software you own or have permission to inspect. Don't use it in online games or with anything protected by anti-cheat, where it may break the terms of service or get your account banned.

## Acknowledgements

- [Dear ImGui](https://github.com/ocornut/imgui) by Omar Cornut
- [GLFW](https://www.glfw.org/)
