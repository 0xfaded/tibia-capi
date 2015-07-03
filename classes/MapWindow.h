#pragma once

#include <stdint.h>
#include <classes/GUIComposite.h>

typedef struct MapWindow MapWindow;
typedef struct BattleList BattleList;
typedef struct BattleListEntry BattleListEntry;

struct BattleListEntry {
        int32_t x;
        int32_t y;
        int32_t monster_id;
};

struct BattleList {
        int32_t num_monsters;
        BattleListEntry monsters[92];
};

struct MapWindow {
        GUIComposite composite;
        int32_t unknown1;
        BattleList battle_list;
        int32_t unknown[0];
};
