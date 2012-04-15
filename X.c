#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <stdio.h>

int main(void) {
    Display *disp = XOpenDisplay(NULL);
    XTestFakeKeyEvent(disp, XKeysymToKeycode (disp, XK_1), True, CurrentTime);
    XSync(disp, False);
    XTestFakeKeyEvent(disp, XKeysymToKeycode (disp, XK_1), False, CurrentTime);
	XSync(disp, False);    
    XCloseDisplay(disp);
    
    return 0;
}

