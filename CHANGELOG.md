# Changelog

Format: [Keep a Changelog](https://keepachangelog.com/en/1.1.0/); versions follow SemVer.

## [Unreleased]

## [0.1.0] - 2026-10-20

### Added
- **Gamepads**: the toolkit is pinned at neogeorecomp v0.2.0 (af96b2b), which reads up to two SDL2 gamepads as players 1 and 2 (neogeorecomp #6).
- **Sound**: the toolkit is pinned at neogeorecomp #3 (YM2610 synthesis through ymfm). The window plays it, `--wav FILE` writes it, `--mute` silences it. The build now enables C++ for ymfm, and `setup.sh` checks for g++. Sound is by *[@danilopaulinodasilva](https://github.com/danilopaulinodasilva)* (neogeorecomp #3).
- CI: builds the interpreter-only player on Ubuntu, Windows and macOS on every push and PR.
- **Installer** (recomp-launcher): `launcher/launcher.json` and the `installer` workflow build Metal-Slug-Installer for Windows, Linux and macOS on every push; a `v*` tag attaches them to the release.
- `setup.sh` runs on macOS (Homebrew), and both setup scripts print `[n/N]` steps for the installer's progress bar. On Windows, installing Git or CMake no longer stops setup for a rerun.
- Boots and plays on the rebuilt neogeorecomp: the real MVS BIOS, attract mode, coin/start, and Mission 1, running on recompiled 68000 code (needs neogeorecomp #1).
- `mslug_recomp`, the generator that recompiles the user's own ROMs into `build/generated` at build time (`MSLUG_ROM_DIR`).
- The `profile` target: headless attract and play sessions record runtime-only entry points; the next build includes them.
- `tests/coin_start.txt` (reach Mission 1) and `tests/play_long.txt` (5-minute soak).
- `Setup.cmd` / `setup.sh` quick start: prerequisite checks that ask before installing, ROM discovery with CRC checks, build, profile, launcher.
- neogeorecomp as a git submodule; `toolkit.ref` pins it for plain downloads.
- README with Getting Started and screenshots; `docs/game-notes.md`.

### Removed
- The scaffold's placeholder notes and empty `recomp/` directory.

## [0.0.1]
- Initial scaffolding.
