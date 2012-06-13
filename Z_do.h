#ifndef ZDO_H
#define ZDO_H

#define ZDO_SUCCESS			0
#define ZDO_ERROR			-1

#define ZDO_MAX_KEY_LIST	10	//max num of keys to press simultaneously

typedef struct keysym_charmap {
  char *keysym;
  char *meaning;
  int printable;
} keysym_charmap_t;

extern int zdo_key(char *keylist, const char *type);
extern int zdo_type(const char *words);
extern int zdo_mousemove(int x, int y, int is_rel);
extern int zdo_mousemove_relative(int dx, int dy);
extern int zdo_mousedown(int button);
extern int zdo_mouseup(int button);
extern int zdo_click(int button, int repeat, unsigned long delay);
extern int zdo_getmouselocation(int *x, int *y);
extern void zdo_sleep(double seconds);

#endif
