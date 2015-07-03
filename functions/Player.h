#pragma once

// player_get_hp returns the player's current HP
// To find:
//   1. Set a breakpoint on draw_text.
//   2. Wait for draw_text to be called with current hitpoints string as argument.
//   3. Follow second return address on stack to the method drawing the status window.
//   4. Look above to find four calls to get_hp, get_hp_max, get_mp, get_mp_max.
int32_t (*player_get_hp)(void) = (void *)0x080ea6c0;

// player_get_hp_max returns the player's max HP
// To find: see get_hp
int32_t (*player_get_hp_max)(void) = (void *)0x080ea6d0;

// player_get_mp returns the player's current MP
// To find: see get_hp
int32_t (*player_get_mp)(void) = (void *)0x080ea830;

// player_get_mp_max returns the player's max MP
// To find: see get_hp
int32_t (*player_get_mp_max)(void) = (void *)0x080ea840;
