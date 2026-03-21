# Metal Slug: Super Vehicle-001 — Static Recompilation

**A native PC port of Metal Slug (1996) via static recompilation of the original Neo Geo 68000 code.**

Metal Slug is the run-and-gun masterpiece that defined a genre. Built by Nazca Corporation — a team of former Irem veterans who had cut their teeth on In The Hunt and Gunforce II — it pushed the Neo Geo MVS hardware to its limits with fluid hand-drawn animation, destructible environments, and the iconic SV-001 tank. This project aims to bring it to modern hardware not through emulation, but by recompiling the original machine code into native x86-64.

## The Game

- **Title**: Metal Slug: Super Vehicle-001 (メタルスラッグ)
- **Developer**: Nazca Corporation
- **Publisher**: SNK
- **Platform**: Neo Geo MVS / AES (NGH-201)
- **Year**: 1996
- **Genre**: Side-scrolling run-and-gun
- **Players**: 1-2 simultaneous

### Missions

| # | Mission | Setting | Boss |
|---|---------|---------|------|
| 1 | Villeneuve Mt. System | Forest / Mountain | Tetsuyuki (gunship fortress) |
| 2 | Ronbertburg City | Urban | Hairbuster Riberts (bomber) |
| 3 | Kurthehirt Valley | Valley | Tani Oh (siege tank) |
| 4 | Ridge 256 | Ridge / Fortifications | Shoe & Karn (twin tanks) |
| 5 | Gerhardt City | City | Iron Nokana (heavy tank) |
| 6 | Straits of Traven | Coastal / Airport | Hi-Do (Morden's gunship) |

Six missions of escalating chaos, from jungle ambushes to the final assault on General Morden's forces. The game was originally conceived as a pure tank combat game before the on-foot gameplay was added — remnants of that early design survive in the ROM as unused sprite data.

## ROM Details

| ROM | Type | Size | Purpose |
|-----|------|------|---------|
| `201-p1.p1` | P ROM | 2 MB | 68000 program code |
| `201-s1.s1` | S ROM | 128 KB | Fix layer text/HUD tiles |
| `201-c1.c1` / `c2` | C ROM pair | 4 MB each | Sprite graphics (bitplanes 0-1, 2-3) |
| `201-c3.c3` / `c4` | C ROM pair | 4 MB each | Sprite graphics (continued) |
| `201-m1.m1` | M ROM | 128 KB | Z80 audio driver |
| `201-v1.v1` | V ROM | 4 MB | ADPCM audio samples |
| `201-v2.v2` | V ROM | 4 MB | ADPCM audio samples |

**Total: ~22.5 MB** — no encryption, no protection, no bankswitching needed. The 2 MB P ROM fits within the Neo Geo's two 1 MB address windows ($000000-$0FFFFF fixed, $200000-$2FFFFF secondary). This makes Metal Slug an ideal recompilation target: the entire program can be linearly disassembled.

### PCB Details
- **PROG board**: PROGBK1 (standard, no protection CPLD)
- **CHA board**: CHA256 (standard character board)
- **MAME driver**: Standard `neogeo.cpp` — no game-specific handlers needed

## How This Project Works

This repository contains the game-specific recompiled code for Metal Slug. It depends on [neogeorecomp](https://github.com/sp00nznet/neogeorecomp), which provides the Neo Geo hardware runtime (video, audio, input, memory map).

```
┌──────────────────────────┐
│   metalslug (this repo)  │
│  ┌────────────────────┐  │
│  │  recomp/*.c        │  │  ← recompiled 68k functions
│  │  src/main.c        │  │  ← entry point, function registration
│  └────────┬───────────┘  │
│           │ links        │
│  ┌────────▼───────────┐  │
│  │   neogeorecomp     │  │  ← Neo Geo hardware runtime
│  │   (git submodule)  │  │
│  └────────────────────┘  │
└──────────────────────────┘
```

### Project Structure

```
metalslug/
├── src/
│   └── main.c              — entry point, ROM loading, function table setup
├── recomp/
│   └── (recompiled 68k function files will live here)
├── docs/
│   └── game_notes.md       — technical analysis, function map, game-specific quirks
├── CMakeLists.txt
└── README.md
```

## Building

### Prerequisites

- **CMake** 3.20+
- **C17 compiler** (MSVC 2022, Clang 14+, or GCC 12+)
- **SDL2** development libraries
- **A legally obtained Metal Slug ROM dump** (you must own the game)

### Build Steps

```bash
git clone --recursive https://github.com/sp00nznet/metalslug.git
cd metalslug
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Running

```bash
./build/metalslug --rom-path /path/to/your/roms/
```

The ROM path should contain the Metal Slug ROM files (`201-p1.p1`, etc.) plus the Neo Geo BIOS files.

## Recompilation Progress

The recompilation process involves disassembling the 2 MB P ROM, identifying function boundaries, and lifting each function into C. Progress is tracked here:

| Phase | Description | Status |
|-------|-------------|--------|
| ROM analysis | Disassemble P ROM, map function boundaries | Not started |
| Vector table | Identify reset, VBlank, timer interrupt handlers | Not started |
| BIOS interface | Map BIOS call conventions and system vectors | Not started |
| Core game loop | Recompile main loop, VBlank handler, state machine | Not started |
| Input handling | Player controls, coin/start, DIP switches | Not started |
| Sprite management | Object tables, spawn/update/destroy logic | Not started |
| Scroll engine | Background scrolling, camera, level streaming | Not started |
| Player mechanics | Marco/Tarma movement, weapons, vehicle entry/exit | Not started |
| Enemy AI | Soldier behavior, vehicle patterns, spawning | Not started |
| Boss logic | Per-boss state machines and attack patterns | Not started |
| POW system | Prisoner rescue, scoring, item drops | Not started |
| Audio commands | Sound effect triggers, music cues via Z80 commands | Not started |
| Attract mode | Title screen, demo play, high score display | Not started |
| Full playthrough | All 6 missions completable | Not started |

## Technical Notes

### Why Metal Slug Is a Good Recomp Target

1. **No protection**: Unlike Metal Slug X (ALTERA CPLD) or MS3-5 (encrypted ROMs), the original has zero copy protection
2. **No bankswitching**: 2 MB P ROM fits in the standard two-window layout
3. **Standard hardware**: Uses no custom mapper chips or exotic cartridge features
4. **Well-studied game**: Extensive community knowledge of game mechanics, enemy behavior, and level design
5. **Linear execution**: As a side-scrolling action game, the code flow is relatively straightforward compared to RPGs or strategy games

### Known Technical Details

- The game uses the Neo Geo's auto-animation feature for environmental effects (waterfalls, fire, etc.)
- Sprite chaining is used extensively for large bosses (Tetsuyuki uses dozens of chained sprites)
- The SV-001 tank is implemented as a compound object with separate turret/body/tread sprites
- Debug DIP switches exist that display hex debug info on screen during gameplay
- Significant unused content exists in the ROM including early character designs and prototype weapon sprites ([The Cutting Room Floor](https://tcrf.net/Metal_Slug:_Super_Vehicle-001_(Neo_Geo)))

## Legal Notice

This project contains no copyrighted game code or data. You must provide your own legally obtained ROM dump from a Metal Slug MVS or AES cartridge that you own. The recompiled source code in this repository represents a transformative reimplementation of the game's logic.

## Related Projects

- [neogeorecomp](https://github.com/sp00nznet/neogeorecomp) — the Neo Geo hardware runtime this project depends on
- [Neo Drift Out recomp](https://github.com/sp00nznet/neodriftout) — our other Neo Geo recomp target
- [genrecomp](https://github.com/sp00nznet/genrecomp) — Sega Genesis 68000 recompiler (sister project, same CPU)
- [ngdevkit](https://github.com/dciabrin/ngdevkit) — open-source Neo Geo development toolkit
- [N64Recomp](https://github.com/N64Recomp/N64Recomp) — the pioneering static recompiler for N64 games

## Community

- [Neo Geo Forever](https://neogeoforever.com) — forums and Discord
- [Neo-Geo.com Forums](https://www.neo-geo.com/forums/)
- [Neo Geo Dev Wiki](https://wiki.neogeodev.org)
- [Arcade-Projects](https://www.arcade-projects.com)

## License

MIT — see [LICENSE](LICENSE) for details.
