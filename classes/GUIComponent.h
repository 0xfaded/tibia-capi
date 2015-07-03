#pragma once

#include <stdint.h>

typedef struct GUIComponent GUIComponent;
struct GUIComponent {
        struct {
                void *(*deconstructor_fast)(GUIComponent *w);
                void *(*deconstructor_stack)(GUIComponent *w);
                void *unknown1[7];
                GUIComponent *(*get_children)(GUIComponent *w);
                void *unknown2[1];
                GUIComponent *(*get_sibling)(GUIComponent *w);
                void *unknown3[1];
                GUIComponent *(*get_parent)(GUIComponent *w);
                void *unknown4[6];
                int32_t (*render)(GUIComponent *w);
                void *unknown5[2];
                int32_t (*button_press)(GUIComponent *w, int32_t x, int32_t y, int32_t unknown);
                int32_t (*button_release)(GUIComponent *w, int32_t x, int32_t y, int32_t unknown);
                void *unknown6[4];
                int32_t (*key_press)(GUIComponent *w, int32_t code);
                int32_t (*key_release)(GUIComponent *w, int32_t code);
                void *unknown[0];
        } *vtable;
        int32_t unknown1;
        int32_t unknown2;
        GUIComponent *parent;
        GUIComponent *sibling;
        int32_t x;
        int32_t y;
        int32_t width;
        int32_t height;
};
