#pragma once

#include <stdint.h>
#include <classes/GUIComponent.h>

typedef struct GUIComposite GUIComposite;
struct GUIComposite {
        GUIComponent component;
        GUIComponent *children;
};
