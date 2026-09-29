/*
 * game.c — the Metal Slug (NGH-201) ROM set, shared by the player and the
 * generator so both load the program identically.
 */
#include "game.h"

/* MAME set "mslug". The 2 MB P ROM holds the banked half first; the fixed
 * $000000 half is its second MB. Both are stored byte-swapped. */
const ng_game_t k_mslug = {
    .name = "mslug",
    .title = "Metal Slug - Super Vehicle-001",
    .prom_size = 0x200000,
    .prom = {
        {"201-p1.p1", 0x100000, 0x000000, 0x100000, 1},
        {"201-p1.p1", 0x000000, 0x100000, 0x100000, 1},
    },
    .crom_size = 0x1000000,
    .crom_pairs = {
        {"201-c1.c1", "201-c2.c2"},
        {"201-c3.c3", "201-c4.c4"},
    },
    .srom = "201-s1.s1",
    .m1 = "201-m1.m1",
    .vrom = {"201-v1.v1", "201-v2.v2"},
};
