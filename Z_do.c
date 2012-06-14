#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <malloc.h>
#include <sys/stat.h>

#include "Z_do.h"
#include "Z_do_util.h"


static int zdo_press_key_list(Display* disp, KeyCode *keys, int nKeys, const char *type) {
	int i;
	if (strcasecmp(type, "keyup") != 0) {
		for (i=0; i<nKeys; i++) {
			XTestFakeKeyEvent(disp, keys[i], True, CurrentTime);
		}
	}
	if (strcasecmp(type, "keydown") != 0)	{
		for (i=nKeys-1; i>=0; i--) {
			XTestFakeKeyEvent(disp, keys[i], False, CurrentTime);
		}
	}
	return ZDO_SUCCESS;
}

int zdo_key(char *keylist, const char *type) {
	int i;
	char *tok;
	KeySym sym;
    KeyCode key;
    KeyCode *keys = calloc(ZDO_MAX_KEY_LIST, sizeof(KeyCode));
    int nKeys = 0;
    Display* disp = XOpenDisplay( NULL );
    
    if (strcspn(keylist, " \t\n.-[]{}\\|") != strlen(keylist)) {
		zdo_alert('r', "Error: Invalid key sequence '%s'\n", keylist);
		return ZDO_ERROR;
	}
    
	if ((tok = strtok(keylist, "+")) != NULL) {
		do {				
			/* keysym_charmap comes from Z_do_util.h */
			for (i=0; keysym_charmap[i].keysym!=NULL; i++) {
				if (!strcasecmp(keysym_charmap[i].keysym, tok)) {
					if (keysym_charmap[i].printable == 0)
						tok = keysym_charmap[i].meaning;
					else
						tok = keysym_charmap[i].keysym;
					break;
				}
			}
					
			sym = XStringToKeysym(tok);
			if (sym == NoSymbol) {
				if (isdigit(tok[0])) {
					key = (unsigned int) atoi(tok);
				}
				else {
					zdo_alert('r', 
						"Error: No such key name '%s'. Ignoring it.\n", 
						tok);
					continue;
				}
			}
			else {
				key = XKeysymToKeycode(disp, sym);
				//Need a 'shift' here
				if ((XKeycodeToKeysym(disp, key, 0) != sym)) {
					keys[nKeys++] = XKeysymToKeycode(disp, XStringToKeysym("Shift_L"));
				}
			}
			keys[nKeys++] = key;
		} while ((tok = strtok(NULL, "+")) != NULL);
	}
	
	zdo_press_key_list(disp, keys, nKeys, type);
	XCloseDisplay(disp);
	return ZDO_SUCCESS;
}

int zdo_type(const char *words) {
	int i, k;
	char *tok;
	KeySym sym;
    KeyCode key;
    Display* disp = XOpenDisplay( NULL );
	int length = strlen(words);
	KeyCode *keys;
	int nKeys = 0;
	
	for (i=0; i<length; i++) {
		tok = calloc(1, sizeof(char));
		keys = calloc(ZDO_MAX_KEY_LIST, sizeof(KeyCode));
		nKeys = 0;
		
		int is_blank = 0;
		if (words[i] == '\\' && i+1<length) {
			if (words[i+1] == 'n') {
				tok = "\n";
				is_blank = 1;
				i++;
			}
			else if (words[i+1] == 't') {
				tok = "\t";
				is_blank = 1;
				i++;
			}
		}
		if (is_blank == 0) {
			*tok = words[i];
		}
		
		// keysym_charmap comes from Z_do_util.h 
		for (k=0; keysym_charmap[k].keysym!=NULL; k++) {
			if (keysym_charmap[k].printable == 1) {
				if (strcasecmp(keysym_charmap[k].meaning, tok) == 0) {
					tok = keysym_charmap[k].keysym;
					break;
				}
			}
		}

		sym = XStringToKeysym(tok);
		if (sym == NoSymbol) {
			if (isdigit(tok[0])) {
				key = (unsigned int) atoi(tok);
			}
			else {
				zdo_alert('r', 
					"Error: No such key name '%s'. Ignoring it.\n", 
					tok);
				continue;
			}
		}
		else {
			key = XKeysymToKeycode(disp, sym);
			//Need a 'shift' here
			if ((XKeycodeToKeysym(disp, key, 0) != sym)) {
				keys[nKeys++] = XKeysymToKeycode(disp, XStringToKeysym("Shift_L"));
			}
		}
		keys[nKeys++] = key;
		zdo_press_key_list(disp, keys, nKeys, "press");
		free(keys);
	}
	XCloseDisplay(disp);
}

int zdo_mousemove(int x, int y, int is_rel) {
	if (is_rel) {
		//move mouse relatively
		return zdo_mousemove_relative(x, y);	
	}
	else {
		Display* disp = XOpenDisplay( NULL );
		Window screen_root = DefaultRootWindow(disp);
	
		if (!XWarpPointer(disp, None, screen_root, 0, 0, 0, 0, x, y)) {
			return ZDO_ERROR;
		}
	
		XFlush(disp);
		return ZDO_SUCCESS;
	}
}

int zdo_mousemove_relative(int dx, int dy) {
	Display* disp = XOpenDisplay( NULL );
	
	if (!XWarpPointer(disp, None, None, 0, 0, 0, 0, dx, dy)) {
		return ZDO_ERROR;
	}
	
	XFlush(disp);
	return ZDO_SUCCESS;
}

int zdo_mousedown(int button) {
	Display* disp = XOpenDisplay( NULL );
	
	if (!XTestFakeButtonEvent(disp, button, 1, CurrentTime)) {
		return ZDO_ERROR;
	}
	
	XFlush(disp);
	return ZDO_SUCCESS;
}

int zdo_mouseup(int button) {
	Display* disp = XOpenDisplay( NULL );
	
	if (!XTestFakeButtonEvent(disp, button, 0, CurrentTime)) {
		return ZDO_ERROR;
	}
	
	XFlush(disp);
	return ZDO_SUCCESS;
}

int zdo_click(int button, int repeat, unsigned long delay) {
	int i;
	int ret;
	for (i=0; i<repeat; i++) {
		if (zdo_mousedown(button) == ZDO_ERROR)
			return ZDO_ERROR;
		if (zdo_mouseup(button) == ZDO_ERROR)
			return ZDO_ERROR;
		if (i != repeat-1)
			usleep(delay);
	}
	return ZDO_SUCCESS;
}


void addListener(Display* display, Window r) {
}

int waitForSpace() {
	return 1;
}

int zdo_record(char *file, int is_continue, int mouse_mask, int key_mask) {
	return 1;
  
}

void zdo_sleep(double seconds) {
	usleep(seconds * 1000000);
}

int zdo_getmouselocation(int *x, int *y) {
	Display* disp = XOpenDisplay( NULL );          
    Screen *screen=XDefaultScreenOfDisplay(disp);
    Window window=XRootWindowOfScreen(screen);
    Window root_return = 0;
    Window w_return = 0;
    int win_x_return, win_y_return;
    unsigned int mask_return;
    
    return XQueryPointer(disp, window,
                        &root_return, &w_return, x, y, &win_x_return, 
                        &win_y_return, &mask_return);
}

void zdo_alert(char color, char *format, ...) {
	char s[100];
	va_list args;
	va_start(args, format);

	vsprintf(s, format, args);
	if (color == 'g') {
		fprintf(stderr, "\x1b[32m%s\x1b[0m", s);
	}
	else if (color == 'b') {
		fprintf(stderr, "\x1b[34m%s\x1b[0m", s);
	}
	else {
		fprintf(stderr, "\x1b[31m%s\x1b[0m", s);
	}
}


