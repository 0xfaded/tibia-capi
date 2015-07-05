#pragma once

#include <stdint.h>
#include <classes/GUIComposite.h>

typedef struct MapWindow MapWindow;
typedef struct BattleList BattleList;
typedef struct BattleListEntry BattleListEntry;

struct BattleListEntry {
        int32_t tibia_id;
        int32_t x;
        int32_t y;
};

#pragma pack(1)
struct BattleList {
        int32_t monsters_length;;
        BattleListEntry monsters[0x200];
};

struct MapWindow {
        GUIComposite composite;
        int32_t unknown1;
        BattleList battle_list;
        // battle_list is assumed to continue indefinately
};
