#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <X11/Xlib.h>
#include "Z_do.h"
#include "Z_do_util.h"
#include "unistd.h"

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
    //get children list
    unsigned int listnum;
    Window parent;
    Window root;
    Window *wa;

    XQueryTree(display, r, &root, &parent, &wa, &listnum);
    
    
    //XSelectInput(display, root, ButtonPressMask);
    if (wa==NULL) {
        return;
    }
    int i=0;
    //printf("listnum:%d\n", listnum);
    for (i=0; i<listnum; i++) {
        Window w=wa[i];
        //printf("alive here?\n");
        XWindowAttributes tattributes;
        XGetWindowAttributes(display, w, &tattributes);
        
        //XSelectInput(display, w, PointerMotionMask);
        //printf("map_state:%d,width:%d,height:%d\n", tattributes.map_state,
        //        tattributes.width, tattributes.height);
        //if (tattributes.map_state==2) {
            //XSelectInput(display, w, KeyPressMask);
            //XSelectInput(display, w, PointerMotionMask);
            //XSelectInput(display, w, KeyReleaseMask);
            //XSelectInput(display, w, ButtonPressMask);
        //}
        if (tattributes.map_state==2) {
            //XSelectInput(display, w, ButtonPressMask);
            XGrabPointer(display, w, True, PointerMotionMask|ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
            //XGrabPointer(display,w,True,ButtonPressMask,GrabModeAsync,GrabModeAsync,None,None,CurrentTime);
            //XGrabPointer(display,w,True,ButtonPressMask,GrabModeAsync,GrabModeAsync,None,None,CurrentTime);
            //XGrabButton(display, AnyButton, AnyModifier, w, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
            //XSelectInput(display, w, PointerMotionMask);
            //XSelectInput(display, w, ButtonPressMask);
            XGrabKeyboard(display, w, True, GrabModeAsync, GrabModeAsync, CurrentTime );
        }
        
        addListener(display, w);
    }
}

void removeListener(Display* display) {

	XUngrabPointer(display, CurrentTime);
	//XUngrabButton(display, CurrentTime);
}

int zdo_getmouselocation(int *x, int *y) {
	Display* disp = XOpenDisplay( NULL );
	
	
          
    Screen *screen=XDefaultScreenOfDisplay(disp);
    Window window=XRootWindowOfScreen(screen);
    Window root_return = NULL;
    Window w_return = NULL;
    int win_x_return, win_y_return;
    unsigned int mask_return;
    
    return XQueryPointer(disp, window,
                        &root_return, &w_return, x, y, &win_x_return, 
                        &win_y_return, &mask_return);

    addListener(disp, window);
	XAllowEvents(disp, AsyncBoth, CurrentTime);
	
	int f=1;
    while (f) {
        if (XPending(disp)) {
            int eq=XEventsQueued(disp, QueuedAlready);
            printf("events num in queue:%d\n", eq);
            XEvent e;
            XNextEvent(disp, &e);
            printf("event type:%d\n", e.type);
            if (e.type == KeyPress) {
                XKeyEvent y=e.xkey;
                unsigned int keycode=y.keycode;
                unsigned int state=y.state;
                printf("keycode:%d,state:%d\n", keycode, state);
                break;
            }else if(e.type==ButtonPress){
            	//removeListener(disp, window);
                XButtonEvent xbutton=e.xbutton;
                printf("x:%d,y:%d,button:%d\n",xbutton.x,xbutton.y, xbutton.button);
                
                removeListener(disp);
                //XFlush(disp);
                //XPutBackEvent(disp, &e);
                //XSetInputFocus(disp, xbutton.window, RevertToParent, CurrentTime);
                zdo_click(3, 1, 0);
                //sleep(1);
                
                //XGrabPointer(disp,xbutton.window,True,ButtonPressMask,GrabModeAsync,GrabModeAsync,None,None,CurrentTime);
                //e.time = CurrentTime;
                //XAllowEvents(disp, SyncBoth, CurrentTime);
                //if (XSendEvent(disp,xbutton.window,0,ButtonPressMask,&e) == 0)
                //	printf("error\n");
                //XSync(disp, False);	
                
                //sleep(2);
                //XFlush(disp);
                //XSendEvent(disp,xbutton.window,0,KeyPressMask,&e);
                //removeListener(disp, window);
                //break;
                //printf("zx\n");
                addListener(disp, window);
            }
            else if (e.type==MotionNotify) {
                printf("Motion Notify - %d, %d\n", e.xmotion.x_root, e.xmotion.y_root);
            }
        }
    }
    removeListener(disp);
	return 1;
}

void zdo_sleep(double seconds) {
	usleep(seconds * 1000000);
}

