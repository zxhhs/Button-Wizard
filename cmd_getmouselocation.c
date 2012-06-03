#include "ZMagic.h"
#include <stdio.h>

int cmd_getmouselocation(context_t *context) {
	zdo_getmouselocation();
	return 1;
}

