#pragma once

#include <stdint.h>
#include <classes/GUIElement.h>

typedef struct GUITextRectangle GUITextRectangle;
struct GUITextRectangle {
        GUIElement element;
        int32_t unknown1;
        char *text;
        int unknown[0];
};
