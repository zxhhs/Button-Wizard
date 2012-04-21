#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#include "Z_do.h"
#include "Z_do_util.h"

static int zdo_press_key_list(Display* disp, KeyCode *keys, int nKeys) {
	int i;
	for (i=0; i<nKeys; i++) {
		XTestFakeKeyEvent(disp, keys[i], True, CurrentTime);
	}
	for (i=nKeys-1; i>=0; i--) {
		XTestFakeKeyEvent(disp, keys[i], False, CurrentTime);
	}
	return ZDO_SUCCESS;
}

int zdo_key(char *keylist) {
	int i;
	char *tok;
	KeySym sym;
    KeyCode key;
    KeyCode *keys = calloc(ZDO_MAX_KEY_LIST, sizeof(KeyCode));
    int nKeys = 0;
    Display* disp = XOpenDisplay( NULL );
    
    if (strcspn(keylist, " \t\n.-[]{}\\|") != strlen(keylist)) {
		fprintf(stderr, "Error: Invalid key sequence '%s'\n", keylist);
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
					fprintf(stderr, 
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
	
	zdo_press_key_list(disp, keys, nKeys);
	XCloseDisplay(disp);
	return ZDO_SUCCESS;
}



