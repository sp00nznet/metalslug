# Metal Slug: notes

What this title needs from the toolkit, and what bringing it up showed. These notes cover behaviour only. Code addresses, disassembly and anything else lifted from the ROM are not published (REPO_RULES section 3).

## The set

MAME set `mslug` (NGH-201), plus the `neogeo` system set.

| File | Size | CRC32 | Role |
|---|---|---|---|
| `201-p1.p1` | 2 MB | `08d8daa5` | 68000 program. The second MB is the fixed `$000000` bank and the first MB is the `$200000` bank; stored byte-swapped |
| `201-s1.s1` | 128 KB | `2f55958d` | fix layer tiles |
| `201-c1.c1`..`201-c4.c4` | 4 × 4 MB | `72813676` `96f62574` `5121456a` `f4ad59a3` | sprite tiles, two odd/even pairs |
| `201-m1.m1` | 128 KB | `c28b3253` | Z80 sound driver |
| `201-v1.v1`, `201-v2.v2` | 2 × 4 MB | `23d22ed1` `472cf9db` | ADPCM samples |
| `sp-s2.sp1` | 128 KB | `9036d879` | MVS system ROM (Asia/Europe v2) |
| `sfix.sfix`, `sm1.sm1`, `000-lo.lo` | | `c2ea0cfd` `94416d67` `5a86cff2` | BIOS fix tiles, BIOS sound driver, sprite shrink table |

The board has no protection or encryption.

## What it exercises

- **Boot through the real MVS BIOS**: RAM test, Z80 handshake (the BIOS switches from its own sound driver to the cartridge's), eyecatcher, coin and credit handling, and the attract loop with its demo.
- **Sprites**: large objects such as the SV-001 and the title logo are many sprites wide, so they depend on sticky chains and the shrink tables being right.
- **VBlank** drives the game loop through the BIOS.

## What discovery had to learn

Most of Metal Slug's code is never the target of a direct call:

- **State machines dispatch through pointer and offset tables.** Following static calls alone from the vectors found about 3,000 instructions; the jump-table pass took that into the tens of thousands.
- **Objects run as coroutines.** A routine stores the address to resume at into the object's RAM with a PC-relative `lea`, and the object loop later jumps there. The code at those addresses is only ever referenced from data, which is why the recompiler scans the program for code pointers.
- **One routine is an unrolled loop entered at a computed offset.** No static analysis can know the landing points, so they come from the profile pass (`--dump-misses`). One attract session plus one play session covers them.

End state, with the profile pass: 453,545 instructions emitted across 15,195 routines and 33,613 dispatch entries. Both scripted sessions run with 0 interpreted instructions, and `--verify` finds 0 mismatches over 14.06M blocks.

## Not yet

- **Sound**: the Z80 driver runs and answers the BIOS, but the YM2610 is silent (neogeorecomp ROADMAP).
- **Later missions**: the soak script plays Mission 1. Other missions have not been driven by a script yet; any runtime-only code they reach runs in the interpreter until profiled.
