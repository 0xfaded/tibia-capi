#pragma once

#include <stdint.h>
#include <classes/GUIElement.h>

typedef struct GUIButton GUIButton;
struct GUIButton {
        GUIElement element;
        int32_t colour;
        int32_t depressed_color;
        int32_t unknown[0];
};
