#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <dlfcn.h>
#include <X11/Xlib.h>

typedef struct TWindow TWindow;
typedef struct TComposite TComposite;
typedef struct TModalDialog TModalDialog;
typedef struct TTextRectangle TTextRectangle;
typedef struct TFormatTextRectangle TFormatTextRectangle;
typedef struct TButton TButton;
typedef struct TEdit TEdit;
typedef struct TPasswordEdit TPasswordEdit;

void *__dynamic_cast(void *ptr, void *srctype, void *dsttype, int hint);

void *GUIComponentType = (void *)0x82eea18;
void *GUICompositeType = (void *)0x82eea30;
void *GUIModalDialogType = (void *)0x830756c;
void *GUITextButtonType = (void *)0x82ec75c;
void *GUIEditType = (void *)0x82ec888;
void *GUIPasswordEditType = (void *)0x82ec8a8;
void *GUITextRectangleType = (void *)0x82ec5e8;
void *GUIFormatTextRectangleType = (void *)0x82ec610;

struct TWindow {
        struct {
                void *unknown1[8];
                int (*has_children)(TWindow *w);
                TWindow *(*get_children)(TWindow *w);
                void *unknown2[1];
                TWindow *(*get_sibling)(TWindow *w);
                void *unknown3[1];
                TWindow *(*get_parent)(TWindow *w);
                void *unknown4[9];
                int (*button_press)(TWindow *w, int x, int y, int unknown);
                int (*button_release)(TWindow *w, int x, int y, int unknown);
                void *unknown5[4];
                int (*key_press)(TWindow *w, int code);
                int (*key_release)(TWindow *w, int code);
        } *vtable;
        int unknown1;
        int unknown2;
        TWindow *parent;
        TWindow *sibling;
        int x;
        int y;
        int width;
        int height;
};

struct TComposite {
        TWindow window;
        TWindow *children;
};

struct TButton {
        TWindow window;
        int colour;
        int depressed_color;
        int unknown[8];
        char label[16];
};

struct TTextRectangle {
        TWindow window;
        int unknown1[11];
        char text[24];
        int unknown[];
};

struct TFormatTextRectangle {
        TComposite composite;
        int unknown1;
        char *formatted_text;
        char *text;
        int unknown[];
};

struct TEdit {
        TWindow window;
        int unknown1;
        int unknown2;
        char *text;
        int unknown3;
        int cursor_pos;
        int unknown[];
};

struct TModalDialog {
        TComposite composite;
        int unknown[15];
        //char title[128]; // size unknown, probably less
        //int unknown[];
};

struct TPasswordEdit {
        TEdit edit;
        int unknown[];
};



//  0 -
//  4 -
//  8 -
// 12 - parent
// 16 - sibling
// 20 - offset x
// 24 - offset y
// 28 - width
// 32 - height
// 36 - children
// 40 - id?


// 32 - has children
// 36 - get children
// 40 - something with children?
// 44 - get sibling
// 48 - iterate over children?
// 52 - get parent
//
// 60 - set(x, y)
// 64 - button return false
// 68 - button return false
// 72 - button return false
// 76 - void nothing
// 80
// 84 - button void nothing
// 88
// 92 - on_button_press
// 120 - on_key_press
// 124 - on_key_release
void tprintf(int tabs, char *format, ...) {
        va_list(args);
        va_start(args, format);
        int i;
        for (i = 0; i < tabs; i += 1) printf("\t");
        vprintf(format, args);
}

char *gui_component_name(TWindow *component) {
        void **rtti = *((void ***)component->vtable - 1);
        char *name = *(rtti + 1);
        return name;
}

void gui_click(TWindow *component, int x, int y, int button) {
        component->vtable->button_press(component, x, y, button);
        component->vtable->button_release(component, x, y, button);
}

void gui_type(TWindow *component, const char *codes) {
        for (; *codes; ++codes) {
                component->vtable->key_press(component, *codes);
                component->vtable->key_release(component, *codes);
        }
}

void printWindow(TWindow *window, int depth) {
        tprintf(depth, "vtable   = %p\n", window->vtable);
        tprintf(depth, "unknown1 = 0x%x\n", window->unknown1);
        tprintf(depth, "unknown2 = 0x%x\n", window->unknown2);
        tprintf(depth, "parent   = %p\n", window->parent);
        tprintf(depth, "sibling  = %p\n", window->sibling);
        tprintf(depth, "x        = 0x%x\n", window->x);
        tprintf(depth, "y        = 0x%x\n", window->y);
        tprintf(depth, "width    = 0x%x\n", window->width);
        tprintf(depth, "height   = 0x%x\n", window->height);

        TButton *button = (TButton *)__dynamic_cast(window, GUIComponentType, GUITextButtonType, -1);
        if (button != NULL) {
                tprintf(depth, "label    = %s\n", button->label);
        }

        TTextRectangle *rect = __dynamic_cast(window, GUIComponentType, GUITextRectangleType, -1);
        if (rect) {
                tprintf(depth, "text     = %s\n", rect->text);
        }

        TFormatTextRectangle *frect = __dynamic_cast(window, GUIComponentType, GUIFormatTextRectangleType, -1);
        if (frect) {
                tprintf(depth, "formatted= %s\n", frect->formatted_text);
        }

}

void printTree(TWindow *root, int depth) {
        TWindow *composite, *children, *sibling;

        tprintf(depth, "%p (%s) {\n", root, gui_component_name(root));
        printWindow(root, depth+1);

        composite = (TWindow *)__dynamic_cast(root, GUIComponentType, GUICompositeType, -1);
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

TWindow *find_gui_component(TWindow *root, int (*test)(TWindow *, void *), void *arg) {
        TWindow *composite, *children, *sibling, *search;

        if (root == NULL || test(root, arg)) {
                return root;
        }

        if ((composite = (TWindow *)__dynamic_cast(root, GUIComponentType, GUICompositeType, -1)) != NULL) {
                if ((children = root->vtable->get_children(root)) != NULL) {
                        if ((search = find_gui_component(children, test, arg)) != NULL) {
                                return search;
                        }
                }
        }

        if ((sibling = root->vtable->get_sibling(root)) != NULL) {
                return find_gui_component(sibling, test, arg);
        }

        return NULL;
}

int is_password(TWindow *component, void *ignore) {
        TPasswordEdit *password = __dynamic_cast(component, GUIComponentType, GUIPasswordEditType, -1);
        return password != NULL && password != ignore;
}

int is_text_rectangle(TWindow *component, void *text) {
        TTextRectangle *rect = __dynamic_cast(component, GUIComponentType, GUITextRectangleType, -1);
        return rect != NULL && strcmp(text, rect->text) == 0;
}

int is_button(TWindow *component, void *label) {
        TButton *button = __dynamic_cast(component, GUIComponentType, GUITextButtonType, -1);
        return button != NULL && strcmp(label, button->label) == 0;
}

extern char *__progname;
int XPending(Display *display) {
        static int (*xpending)(Display * display) = NULL;
        static void *handle;
        static int state = 0;

        if (xpending == NULL) {
                handle = dlopen("libX11.so", RTLD_NOW | RTLD_LOCAL);
                if (handle == NULL) {
                        printf("%s\n", dlerror());
                } else {
                        xpending = dlsym(handle, "XPending");
                        if (xpending == NULL) {
                                printf("%s\n", dlerror());
                        }
                }
        }

        TWindow **root = (TWindow **)0x83d4e84;
        TWindow **current = (TWindow **)0x83d4e80;

        if (state == 0) {
                TButton *enter = (TButton *)find_gui_component(*root, is_button, "Enter Game");
                gui_click((TWindow *)enter, 1, 1, 0);

                state = 1;

        }
        
        if (*current != NULL) {
                if (state <= 1) {
                        TPasswordEdit *account  = (TPasswordEdit *)find_gui_component(*current, is_password, NULL);
                        TPasswordEdit *password = (TPasswordEdit *)find_gui_component(*current, is_password, account);
                        TButton *ok = (TButton *)find_gui_component(*current, is_button, "Ok");

                        if (account->edit.window.y > password->edit.window.y) {
                                TPasswordEdit *tmp = account;
                                account = password;
                                password = tmp;
                        }

                        gui_type((TWindow *)account, "account");
                        gui_type((TWindow *)password, "password");

                        gui_click((TWindow *)ok, 1, 1, 0);

                        state = 2;

                } else if (state <= 2 && find_gui_component(*current, is_text_rectangle, "Message of the Day") != NULL) {
                        TButton *ok = (TButton *)find_gui_component(*current, is_button, "Ok");
                        gui_click((TWindow *)ok, 1, 1, 0);

                        state = 3;

                } else if (state <= 3 && find_gui_component(*current, is_text_rectangle, "Select Character") != NULL) {
                        TButton *ok = (TButton *)find_gui_component(*current, is_button, "Ok");
                        gui_click((TWindow *)ok, 1, 1, 0);

                        state = 4;
                }
        }

        return xpending(display);
}
