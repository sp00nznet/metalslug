/*
 * Metal Slug: Super Vehicle-001 — Static Recompilation Entry Point
 *
 * This file initializes the Neo Geo runtime, loads the Metal Slug ROM
 * set, registers all recompiled functions, and starts execution.
 *
 * ROM files required (place in --rom-path directory):
 *   201-p1.p1    68000 program code (2 MB)
 *   201-s1.s1    Fix layer tiles (128 KB)
 *   201-c1.c1    Sprite tiles pair 1 (4 MB each)
 *   201-c2.c2
 *   201-c3.c3    Sprite tiles pair 2 (4 MB each)
 *   201-c4.c4
 *   201-m1.m1    Z80 audio program (128 KB)
 *   201-v1.v1    ADPCM samples (4 MB each)
 *   201-v2.v2
 *   sp-s2.sp1    Neo Geo BIOS
 *   sfix.sfix    BIOS fix tiles
 *   000-lo.lo    BIOS lookup table
 */

#include <neogeorecomp/neogeorecomp.h>
#include <stdio.h>
#include <string.h>

/* ----- Forward declarations for recompiled functions ----- */
/*
 * As functions are recompiled from the 68000 P ROM, they will be
 * declared here and registered in the function table below.
 *
 * Example (uncomment as functions are recompiled):
 *
 *   extern void func_000200(void);  // Reset vector entry
 *   extern void func_000400(void);  // VBlank handler
 *   extern void func_001000(void);  // Main game loop
 *   ...
 */

/* ----- ROM Path Helpers ----- */

static char s_rom_path[512] = ".";

static void make_path(char *buf, size_t size, const char *filename) {
    snprintf(buf, size, "%s/%s", s_rom_path, filename);
}

/* ----- ROM Loading ----- */

static int load_roms(void) {
    char path[512];
    int rc;

    /* P ROM — 68k program code */
    make_path(path, sizeof(path), "201-p1.p1");
    rc = bus_load_prom(path, NULL);
    if (rc != 0) return rc;

    /* BIOS */
    make_path(path, sizeof(path), "sp-s2.sp1");
    rc = bus_load_bios(path);
    if (rc != 0) {
        fprintf(stderr, "[metalslug] BIOS not found, continuing without BIOS\n");
        /* Non-fatal: we can run without BIOS for recomp purposes */
    }

    /* S ROM — fix layer tiles */
    make_path(path, sizeof(path), "201-s1.s1");
    rc = video_load_srom(path);
    if (rc != 0) return rc;

    /* SFIX — BIOS fix tiles */
    make_path(path, sizeof(path), "sfix.sfix");
    video_load_sfix(path);  /* Non-fatal if missing */

    /* C ROMs — sprite tiles (2 pairs: c1/c2 and c3/c4) */
    char c1[512], c2[512], c3[512], c4[512];
    make_path(c1, sizeof(c1), "201-c1.c1");
    make_path(c2, sizeof(c2), "201-c2.c2");
    make_path(c3, sizeof(c3), "201-c3.c3");
    make_path(c4, sizeof(c4), "201-c4.c4");
    const char *crom_paths[] = { c1, c2, c3, c4 };
    rc = video_load_crom(crom_paths, 4);
    if (rc != 0) return rc;

    /* M ROM — Z80 audio program */
    make_path(path, sizeof(path), "201-m1.m1");
    rc = z80_load_mrom(path);
    if (rc != 0) return rc;

    /* V ROMs — ADPCM audio samples */
    char v1[512], v2[512];
    make_path(v1, sizeof(v1), "201-v1.v1");
    make_path(v2, sizeof(v2), "201-v2.v2");
    const char *vrom_paths[] = { v1, v2 };
    rc = ym2610_load_vrom(vrom_paths, 2);
    if (rc != 0) return rc;

    return 0;
}

/* ----- Function Registration ----- */

static void register_functions(void) {
    /*
     * Register recompiled functions at their original 68k addresses.
     *
     * As functions are lifted from the P ROM disassembly, add them here:
     *
     *   func_table_register(0x000200, func_000200);
     *   func_table_register(0x000400, func_000400);
     *   ...
     *
     * The addresses come from the disassembly. Use Ghidra or similar
     * to identify function boundaries in the 201-p1.p1 ROM.
     */

    printf("[metalslug] Function registration: %u functions\n",
           func_table_count());
}

/* ----- Entry Point ----- */

int main(int argc, char *argv[]) {
    printf("===========================================\n");
    printf("  Metal Slug: Super Vehicle-001\n");
    printf("  Static Recompilation by sp00nznet\n");
    printf("  Runtime: neogeorecomp v%s\n", neogeo_version_string());
    printf("===========================================\n\n");

    /* Parse command line */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--rom-path") == 0 && i + 1 < argc) {
            strncpy(s_rom_path, argv[++i], sizeof(s_rom_path) - 1);
        }
    }

    /* Initialize the Neo Geo runtime */
    int rc = neogeo_init(&(neogeo_config_t){
        .rom_path = s_rom_path,
        .window_scale = 3,
        .fullscreen = false,
        .vsync = true,
        .mvs_mode = true,   /* Metal Slug is primarily an MVS game */
        .region = 1,         /* USA */
    });
    if (rc != 0) {
        fprintf(stderr, "[metalslug] Runtime initialization failed\n");
        return 1;
    }

    /* Load ROM files */
    rc = load_roms();
    if (rc != 0) {
        fprintf(stderr, "[metalslug] ROM loading failed\n");
        return 1;
    }

    /* Register recompiled functions */
    register_functions();

    /* Start execution */
    platform_set_title("Metal Slug: Super Vehicle-001 [neogeorecomp]");
    neogeo_run();

    return 0;
}
