# Roadmap

## Next

- **Sound**, once neogeorecomp has YM2610 synthesis. Nothing in this repo should need to change.
- **Script the later missions** (2-6) into the soak and the profile, so their runtime-only code runs natively and `--verify` covers them.
- **Gamepad input** and remappable keys (toolkit frontend).

## Later

- Load `mslug.zip` / `neogeo.zip` directly instead of unzipping (toolkit).
- A release process that ships the tool, never the game: the generator plus the runtime, which recompile the user's dump on first run.

## Out of scope

- Distributing ROMs, the recompiled C, or anything else derived from the game.
- Game-specific patches. If Metal Slug needs something, it is hardware behaviour and goes in the toolkit.
