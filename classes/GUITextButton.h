#pragma once

#include <stdint.h>
#include <classes/GUIButton.h>

typedef struct GUITextButton GUITextButton;
struct GUITextButton {
        GUIButton button;
        int32_t unknown1[8];
        char label[32]; // Exact length not known
        int32_t unknown[0];
};
