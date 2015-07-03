#pragma once

#include <stdint.h>
#include <classes/GUIContentBody.h>
#include <classes/GUIComposite.h>

typedef struct ContainerWindow ContainerWindow;
struct ContainerWindow {
        GUIContentBody content_body;
        int32_t unknown[0];
};

typedef struct ContainerWindowContent ContainerWindowContent;
struct ContainerWindowContent {
        GUIContentBody composite;
        int32_t unknown[0];
};

typedef struct ContainerWindowFooter ContainerWindowFooter;
struct ContainerWindowFooter {
        GUIContentBody composite;
        int32_t unknown[0];
};

