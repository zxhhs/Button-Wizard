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

extern int zdo_key(char *keylist);

#endif
