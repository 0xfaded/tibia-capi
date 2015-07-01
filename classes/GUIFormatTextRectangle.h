#pragma once

#include <stdint.h>
#include <classes/GUIComposite.h>

typedef struct GUIFormatTextRectangle GUIFormatTextRectangle;
struct GUIFormatTextRectangle {
        GUIComposite composite;
        int32_t unknown1;
        char *formatted_text;
        char *text;
        int32_t unknown[0];
};
