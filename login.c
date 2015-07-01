#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <X11/Xlib.h>

#include <classes/classes.h>

static char *gui_component_name(GUIComponent *component);
static void gui_click(GUIComponent *component, int x, int y, int button);
static void gui_type(GUIComponent *component, const char *codes);
static GUIComponent *find_gui_component(GUIComponent *root, int (*test)(GUIComponent *, void *), void *arg);

static int is_password(GUIComponent *component, void *ignore);
static int is_text_rectangle(GUIComponent *component, void *text);
static int is_button(GUIComponent *component, void *label);

static void printComponent(GUIComponent *window, int depth);
static void printTree(GUIComponent *root, int depth);
static void tprintf(int tabs, char *format, ...);


void login(void) {
        static int state = 0;

        // Global variables holding the root window and the current dialog if
        // one is displayed. These do not change with stack/heap randomisation.
        GUIComponent **root = (GUIComponent **)0x83d4e84;
        GUIComponent **current = (GUIComponent **)0x83d4e80;

        if (state == 0) {
                // Find enter game button
                GUIButton *enter = (GUIButton *)find_gui_component(*root, is_button, "Enter Game");
                gui_click((GUIComponent *)enter, 1, 1, 0);

                state = 1;

        }
        
        if (*current != NULL) {
                if (state <= 1) {
                        // Find account / password entry components.
                        GUIPasswordEdit *account  = (GUIPasswordEdit *)find_gui_component(*current, is_password, NULL);
                        GUIPasswordEdit *password = (GUIPasswordEdit *)find_gui_component(*current, is_password, account);
                        GUIButton *ok = (GUIButton *)find_gui_component(*current, is_button, "Ok");

                        // We don't know which password edit is which, but we know account is above password.
                        // Sort on y offset.
                        if (account->edit.element.component.y > password->edit.element.component.y) {
                                GUIPasswordEdit *tmp = account;
                                account = password;
                                password = tmp;
                        }

                        gui_type((GUIComponent *)account, "account");
                        gui_type((GUIComponent *)password, "password");

                        gui_click((GUIComponent *)ok, 1, 1, 0);

                        state = 2;

                // Wait for the "Message of the Day" dialog to appear. Note that this
                // state may be skipped if the "Select Character" dialog appears first.
                } else if (state <= 2 && find_gui_component(*current, is_text_rectangle, "Message of the Day") != NULL) {
                        GUIButton *ok = (GUIButton *)find_gui_component(*current, is_button, "Ok");
                        gui_click((GUIComponent *)ok, 1, 1, 0);

                        state = 3;

                // Wait for the "Select Character" dialog to appear.
                } else if (state <= 3 && find_gui_component(*current, is_text_rectangle, "Select Character") != NULL) {
                        GUIButton *ok = (GUIButton *)find_gui_component(*current, is_button, "Ok");
                        gui_click((GUIComponent *)ok, 1, 1, 0);

                        state = 4;
                }
        }

        (void)printTree; // hide unused function warnings
}

char *gui_component_name(GUIComponent *component) {
        void **rtti = *((void ***)component->vtable - 1);
        char *name = *(rtti + 1);
        return name;
}

void gui_click(GUIComponent *component, int x, int y, int button) {
        component->vtable->button_press(component, x, y, button);
        component->vtable->button_release(component, x, y, button);
}

void gui_type(GUIComponent *component, const char *codes) {
        for (; *codes; ++codes) {
                component->vtable->key_press(component, *codes);
                component->vtable->key_release(component, *codes);
        }
}

// Search for a GUIComponent matching the test function.
GUIComponent *find_gui_component(GUIComponent *root, int (*test)(GUIComponent *, void *), void *arg) {
        GUIComponent *children, *sibling, *search;
        GUIComposite *composite;

        // Return root if we reach a NULL leaf or root satisfies test
        if (root == NULL || test(root, arg)) {
                return root;
        }

        // Only GUIComponents extending GUIComposite have children. In C++ a dynamic_cast<GUIComposite *> would
        // be made to check this. If we know where the type information is we can call __dynamic_cast directly.
        if ((composite = (GUIComposite*)__dynamic_cast(root, GUIComponentType, GUICompositeType, -1)) != NULL) {

                // Call the get_children method, which in C++ would be a virtual method on GUIComponent.
                // We can call the method directly through the vtable.
                if ((children = root->vtable->get_children(root)) != NULL) {
                        if ((search = find_gui_component(children, test, arg)) != NULL) {
                                return search;
                        }
                }
        }

        // Similarly for siblings, except all GUIComponents have a sibling.
        if ((sibling = root->vtable->get_sibling(root)) != NULL) {
                return find_gui_component(sibling, test, arg);
        }

        return NULL;
}

// Test functions
int is_password(GUIComponent *component, void *ignore) {
        GUIPasswordEdit *password = __dynamic_cast(component, GUIComponentType, GUIPasswordEditType, -1);
        return password != NULL && password != ignore;
}

int is_text_rectangle(GUIComponent *component, void *text) {
        GUITextRectangle *rect = __dynamic_cast(component, GUIComponentType, GUITextRectangleType, -1);
        return rect != NULL && strcmp(text, rect->text) == 0;
}

int is_button(GUIComponent *component, void *label) {
        GUITextButton *button = __dynamic_cast(component, GUIComponentType, GUITextButtonType, -1);
        return button != NULL && strcmp(label, button->label) == 0;
}

// Debug printing
void tprintf(int tabs, char *format, ...) {
        va_list(args);
        va_start(args, format);
        int i;
        for (i = 0; i < tabs; i += 1) printf("\t");
        vprintf(format, args);
}

void printComponent(GUIComponent *window, int depth) {
        tprintf(depth, "vtable   = %p\n", window->vtable);
        tprintf(depth, "unknown1 = 0x%x\n", window->unknown1);
        tprintf(depth, "unknown2 = 0x%x\n", window->unknown2);
        tprintf(depth, "parent   = %p\n", window->parent);
        tprintf(depth, "sibling  = %p\n", window->sibling);
        tprintf(depth, "x        = 0x%x\n", window->x);
        tprintf(depth, "y        = 0x%x\n", window->y);
        tprintf(depth, "width    = 0x%x\n", window->width);
        tprintf(depth, "height   = 0x%x\n", window->height);

        GUITextButton *button = (GUITextButton *)__dynamic_cast(window, GUIComponentType, GUITextButtonType, -1);
        if (button != NULL) {
                tprintf(depth, "label    = %s\n", button->label);
        }

        GUITextRectangle *rect = __dynamic_cast(window, GUIComponentType, GUITextRectangleType, -1);
        if (rect) {
                tprintf(depth, "text     = %s\n", rect->text);
        }

        GUIFormatTextRectangle *frect = __dynamic_cast(window, GUIComponentType, GUIFormatTextRectangleType, -1);
        if (frect) {
                tprintf(depth, "formatted= %s\n", frect->formatted_text);
        }
}

void printTree(GUIComponent *root, int depth) {
        GUIComponent *composite, *children, *sibling;

        tprintf(depth, "%p (%s) {\n", root, gui_component_name(root));
        printComponent(root, depth+1);

        composite = (GUIComponent *)__dynamic_cast(root, GUIComponentType, GUICompositeType, -1);
        if (composite) {
                if ((children = root->vtable->get_children(root)) != NULL) {
                        printTree(children, depth + 1);
                }
        }

        tprintf(depth, "}\n");

        if ((sibling = root->vtable->get_sibling(root)) != NULL) {
                printTree(sibling, depth);
        }
}
