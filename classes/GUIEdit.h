#pragma once

#include <stdint.h>

typedef struct GUIEdit GUIEdit;
struct GUIEdit {
        GUIElement element;
        void *vtable;
        int32_t unknown1;
        char *text;
        int32_t unknown2;
        int32_t cursor_pos;
        int32_t unknown[0];
};
