# Changelog

Format: [Keep a Changelog](https://keepachangelog.com/en/1.1.0/); versions follow SemVer.

## [Unreleased]

### Added
- Boots and plays on the rebuilt neogeorecomp: the real MVS BIOS, attract mode, coin/start, and Mission 1, running on recompiled 68000 code (needs neogeorecomp `feat/generic-runtime`).
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
