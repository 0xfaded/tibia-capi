#include <stdio.h>
#include <dlfcn.h>
#include <X11/Xlib.h>

void login(void);

int XPending(Display *display) {
        static int (*xpending)(Display * display) = NULL;
        static void *handle;

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
        } else {
                login();
        }

        return xpending(display);
}
