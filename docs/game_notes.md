# Metal Slug: Super Vehicle-001 — Technical Notes

Game-specific observations, function maps, and quirks discovered during recompilation.

## ROM Analysis

- **P ROM**: `201-p1.p1`, 2 MB (2,097,152 bytes), CRC32: `08d8daa5`
- **No bankswitching**: 2 MB fits in two 1 MB windows
- **No protection**: Standard PROGBK1 board, no CPLD or encryption
- **Board**: PROGBK1 (PROG) + CHA256 (CHA)

## Vector Table

| Address | Vector | Notes |
|---------|--------|-------|
| $000000 | Initial SSP | Stack pointer value |
| $000004 | Reset PC | Entry point after power-on |
| $000068 | Level 1 (VBlank) | Main game timing interrupt |
| $00006C | Level 2 (Timer) | Raster effects (if used) |

## Key RAM Locations

(To be filled in as disassembly progresses)

| Address | Size | Description |
|---------|------|-------------|
| | | |

## Function Map

(To be filled in as functions are identified and recompiled)

| Address | Name | Description | Status |
|---------|------|-------------|--------|
| | | | |

## Known Quirks

### Auto-Animation
Metal Slug uses the Neo Geo's hardware auto-animation feature for:
- Water/waterfall effects
- Fire and explosions
- Environmental animations (flags, machinery)

The LSPC auto-animation counter toggles tile bits automatically, cycling through 4 or 8 frames without CPU intervention.

### Sprite Chaining
Large objects are built from chained sprites:
- **Tetsuyuki** (Mission 1 boss): 20+ chained sprites
- **SV-001 tank**: Separate sprites for body, turret, treads
- **Large explosions**: Multiple overlapping sprites with different palettes

### Debug Features
Setting certain DIP switch bits displays hex debug info during gameplay. No full debug menu exists (unlike Metal Slug 2-5 which have stage select debug menus).

### Unused Content
Per [The Cutting Room Floor](https://tcrf.net/Metal_Slug:_Super_Vehicle-001_(Neo_Geo)):
- Early character sprite designs
- Alternate tank turret variants
- Prototype weapon graphics
- The game was originally "tank-only" (no on-foot gameplay)

## Sound Commands

(To be documented as the Z80 communication is analyzed)

| Command | Effect |
|---------|--------|
| | |

## References

- [MAME Neo Geo driver](https://github.com/mamedev/mame/blob/master/src/mame/neogeo/neogeo.cpp)
- [Neo Geo Dev Wiki](https://wiki.neogeodev.org)
- [TCRF: Metal Slug](https://tcrf.net/Metal_Slug:_Super_Vehicle-001_(Neo_Geo))
- [Arcade Quartermaster: Metal Slug](https://www.arcadequartermaster.com/mslug1.html)
