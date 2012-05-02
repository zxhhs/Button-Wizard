#ifndef ZDO_UTIL_H
#define ZDO_UTIL_H

#include "Z_do.h"

static const keysym_charmap_t keysym_charmap[] = {
	{ "ampersand", "&", 1, },
	{ "apostrophe", "'", 1, },
	{ "asciicircum", "^", 1, },
	{ "asciitilde", "~", 1, },
	{ "asterisk", "*", 1, },
	{ "at", "@", 1, },
	{ "backslash", "\\", 1, },
	{ "bar", "|", 1, },
	{ "braceleft", "{", 1, },
	{ "braceright", "}", 1, },
	{ "bracketleft", "[", 1, },
	{ "bracketright", "]", 1, },
	{ "colon", ":", 1, },
	{ "comma", ",", 1, },
	{ "dollar", "$", 1, },
	{ "equal", "=", 1, },
	{ "exclam", "!", 1, },
	{ "grave", "`", 1, },
	{ "greater", ">", 1, },
	{ "less", "<", 1, },
	{ "minus", "-", 1, },
	{ "numbersign", "#", 1, },
	{ "parenleft", "(", 1, },
	{ "parenright", ")", 1, },
	{ "percent", "%", 1, },
	{ "period", ".", 1, },
	{ "plus", "+", 1, },
	{ "question", "?", 1, },
	{ "quotedbl", "\"", 1, },
	{ "semicolon", ";", 1, },
	{ "slash", "/", 1, },
	{ "space", " ", 1, },
	{ "Tab", "\t", 1, },
	{ "Return", "\n", 1, },
	{ "underscore", "_", 1, },
	
	{ "escape", "Escape", 0, },
	{ "backSpace", "BackSpace", 0, },
	{ "delete", "Delete", 0, },
	
	{ "up", "Up", 0, },
	{ "down", "Down", 0, },
	{ "left", "Left", 0, },
	{ "right", "Right", 0, },
	
	{ "home", "Home", 0, },
	{ "end", "End", 0, },
	{ "page_up", "Page_Up", 0, },
	{ "page_down", "Page_Down", 0, },
	
	{ "alt", "Alt_L", 0, },
	{ "ctrl", "Control_L", 0, },
	{ "control", "Control_L", 0, },
	{ "meta", "Meta_L", 0, },
	{ "super", "Super_L", 0, },
	{ "shift", "Shift_L", 0, },
	
	{ "altr", "Alt_R", 0, },
	{ "ctrlr", "Control_R", 0, },
	{ "controlr", "Control_R", 0, },
	{ "metar", "Meta_R", 0, },
	{ "superr", "Super_R", 0, },
	{ "shiftr", "Shift_R", 0, },
	
	{ "caps_lock", "Caps_Lock", 0, },
	{ "num_lock", "Num_Lock", 0, },
	{ "scroll_lock", "Scroll_Lock", 0, },
	{ NULL, NULL, 0, },
};

#endif /* ifndef _XDO_UTIL_H_ */
