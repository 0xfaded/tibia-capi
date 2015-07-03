#pragma once


// map_local_z returns the local player z. This differs depending on whether or not the
// player is above or below the surface.
// To find:
//   1. Set a breakpoint on MapWindow::button_release.
//   2. Look for the void function call towards the beginning of the function.
//   3. The return value of map_local_z is subsequently passed to map_local_to_global.
int32_t (*map_local_z)(void) = (void *)0x08185a70;

// map_local_to_global converts (x, y, z) relative to (0, 0) of the game map into global coordinates.
// To find:
//   1. Set a breakpoint on MapWindow::button_release.
//   2. Click a tile on he map.
//   3. Look for call taking three consecutive stack pointers as arguments.
//   4. Before the call, these stack address will contain the map local (x, y, z) of the click.
//      After the call, they will contain the global (x, y, z).
int32_t (*map_local_to_global)(int32_t *x, int32_t *y, int32_t *z) = (void *)0x8185a90;

// map_set_target sets the target location for the client to walk to. Call with game_map=1 for
// game map, 0 for mini map behaviour.
// To find:
//   1. Set a breakpoint on MapWindow::button_release.
//   2. There is a 5 argument function call made using the (x, y, z) computed by map_local_to_global.
//      The 4th argument (game_map) will be 1, the 5th (unknown) will be 0.
int32_t (*map_set_target)(int32_t x, int32_t y, int32_t z, int32_t game_map, int32_t unknown) = (void *)0x080e8930;


// map_go_to_target starts the client's walk to target sequence.
// To find:
//   1. Look for the void function call call at the end of MapWindow::button_release.
int32_t (*map_go_to_target)(void) = (void *)0x080837e0;
