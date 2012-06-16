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
    //get children list
    unsigned int listnum;
    Window parent;
    Window root;
    Window *wa;

    XQueryTree(display, r, &root, &parent, &wa, &listnum);
    
    XGrabKeyboard(display, root, True, GrabModeAsync, GrabModeAsync, CurrentTime );
    XGrabPointer(display, root, True, PointerMotionMask|ButtonPressMask, 
    		GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    return;
    
    /*if (wa == NULL) {
        return;
    }
    int i=0;

    for (i=0; i<listnum; i++) {
        Window w = wa[i];

        XWindowAttributes tattributes;
        XGetWindowAttributes(display, w, &tattributes);
 
        if (tattributes.map_state == 2) { //visible
        	XGrabKeyboard(display, w, True, GrabModeAsync, GrabModeAsync, CurrentTime );
            XGrabPointer(display, w, True, PointerMotionMask|ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
        }
        addListener(display, w);
    }*/
}

int waitForSpace() {
	fd_set rfds, rs;
	struct timeval tv;

	int i, r, j;
	struct termios saveterm, nt;
	int fd = 0;
	unsigned char buf[32];

	tcgetattr(fd, &saveterm);
	nt = saveterm;

	nt.c_lflag &= ~ECHO;      
	nt.c_lflag &= ~ISIG;      
	nt.c_lflag &= ~ICANON;    

	tcsetattr(fd, TCSANOW, &nt);

	FD_ZERO(&rs);
	FD_SET(fd, &rs);
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	i = 0;
	zdo_alert('g', "\npress <Space> to start record.\n");
	zdo_alert('g', "press <Esc> to end.\n");
	while (1) {
		read(fd, buf+i, 1);
		i++;		
		r = select(fd+1, &rfds, NULL, NULL, &tv);    
		if (r < 0)
		{
		   zdo_alert('r', "select error\n");
		   return ZDO_ERROR;
		}
		if(r == 1)
			return ZDO_ERROR;

		rfds = rs;
		for(j=0; j<i; j++)
		{
		   switch (buf[j])
		   {
			case 32 : //Space
				tcsetattr(fd, TCSANOW, &saveterm);
				return ZDO_SUCCESS;
			case 27 : //esc
				tcsetattr(fd, TCSANOW, &saveterm);
				return ZDO_ERROR;
			default :
				break;
			}
		}
	}
}

int zdo_record(char *file, int is_continue, int mouse_mask, int key_mask) {
	FILE *output = NULL;
	clock_t start, finish;
	double duration;
	
	if (!is_continue)
		output = fopen(file, "w");
	else 
		output = fopen(file, "a");
		
	if (output == NULL) {
		zdo_alert('r', "Open File Error.\n");
		return ZDO_ERROR;
	}
	
	
	while (waitForSpace() == ZDO_SUCCESS) {
		Display* disp = XOpenDisplay( NULL );          
		Screen *screen=XDefaultScreenOfDisplay(disp);
		Window window=XRootWindowOfScreen(screen);

		addListener(disp, window);
	
		start = clock();
		zdo_alert('g', "\nRecord Started.\n");
		zdo_alert('g', "Press <Esc> to pause record.\n");
		while (1) {
		    if (XPending(disp)) {
		        XEvent e;
		        XNextEvent(disp, &e);
		        
		        //keyborad event
		        if (e.type == KeyPress) {
		        	duration = (double)(clock() - start) / CLOCKS_PER_SEC;
	        		fprintf(output, "./ZMagic sleep %f\n", duration);
	            	start = clock();
		        
		            XKeyEvent key = e.xkey;
		            unsigned int keycode = key.keycode;
		            unsigned int state = key.state;
		            char *str = (char*)calloc(20, sizeof(char));
		            KeySym keysym;
		            
		            switch (state) {
		            case 0:
		            case 1:
		            	keysym = XKeycodeToKeysym(disp, keycode, state);
		            	str = XKeysymToString(keysym);
		            	break;
		            case 4:
		            	keysym = XKeycodeToKeysym(disp, keycode, 0);
		            	sprintf(str, "Ctrl+%s", XKeysymToString(keysym));
		            	break;
		            case 8:
		            	keysym = XKeycodeToKeysym(disp, keycode, 0);
		            	sprintf(str, "Alt+%s", XKeysymToString(keysym));
		            	break;
		            case 64:
		            	keysym = XKeycodeToKeysym(disp, keycode, 0);
		            	sprintf(str, "Super+%s", XKeysymToString(keysym));
		            	break;
		            default:
		            	keysym = XKeycodeToKeysym(disp, keycode, 0);
		            	str = XKeysymToString(keysym);
		            	break;
		            }
		            if (!strcmp(str, "Escape"))
		            	break;

					if (!mouse_mask)
		            	fprintf(output, "./ZMagic mousemove %d %d\n", key.x_root, key.y_root);
		            if (!key_mask) {
				        fprintf(output, "./ZMagic key %s\n", str);
				        zdo_alert('b', "key:%s - x:%d, y:%d\n", str, key.x_root, key.y_root);
		            }
		        } 
		        //mouse click event
		        else if (!mouse_mask && e.type == ButtonPress){
		        	duration = (double)(clock() - start) / CLOCKS_PER_SEC;
	        		fprintf(output, "./ZMagic sleep %f\n", duration);
	            	start = clock();
	            	
		            XButtonEvent xbutton=e.xbutton;
		            fprintf(output, "./ZMagic mousemove %d %d\n", xbutton.x, xbutton.y);
		            fprintf(output, "./ZMagic click %d\n", xbutton.button);
		            
		            zdo_alert('b', "mouse-click:%d - x:%d, y:%d\n", xbutton.button, xbutton.x, xbutton.y);
		        }
		        //mouse move event
		        else if (!mouse_mask && e.type == MotionNotify) {
		        	duration = (double)(clock() - start) / CLOCKS_PER_SEC;
		        	if (duration > 0.2) {
		        		fprintf(output, "./ZMagic sleep %f\n", duration);
		        		fprintf(output, "./ZMagic mousemove %d %d\n", e.xmotion.x_root, e.xmotion.y_root);
		            	zdo_alert('b', "mouse-move - x:%d, y:%d\n", e.xmotion.x_root, e.xmotion.y_root);
		            	start = clock();
		        	}
		        }
		    }
		}
		
		XUngrabPointer(disp, CurrentTime);
		XUngrabKeyboard(disp, CurrentTime);
		XCloseDisplay(disp);
    }
    
    if (chmod(file, 0777) == -1) {
    	zdo_alert('r', "chmod error.\n");
    }
    fclose(output);
  
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


