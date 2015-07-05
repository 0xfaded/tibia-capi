#pragma once

// creature_by_id returns a pointer to the creature information
// with tibia_id, or NULL if missing.
// To find:
//   1. Set a break point on is_blocking
//   2. Called when testing if a creature is blocking.

typedef struct CreatureInfo CreatureInfo;
struct CreatureInfo {
        int32_t tibia_id;
        char name[32];
        int32_t global_z;
        int32_t global_y;
        int32_t global_x;
        int32_t unknown1[11];
        int32_t is_blocking;
        int32_t unknown2[11];
        int32_t hp_percent;
        int32_t unknown[27];
};
CreatureInfo *(*creature_by_id)(int32_t index) = (void *)0x080e75c0;

// creature_is_player returns lower byte set if the tibia_id is a player id, unset otherwise.
// To find:
//   1. Look for call in is_blocking to battle_get_creature_by_id.
//   2. is_player is called on the result.
int32_t creature_is_player(int32_t tibia_id) {
        return ((int32_t (*)(int32_t tibia_id))0x080e9c10)(tibia_id) & 0xff;
}
