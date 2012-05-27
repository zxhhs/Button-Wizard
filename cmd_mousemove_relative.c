#include "ZMagic.h"
#include "Z_do.h"
#include <stdio.h>

int cmd_mousemove_relative(context_t *context) {
	int dx, dy;
	int ret;
	if (context->argc < 1) {
		fprintf(stderr, "You specified the wrong number of args \
			(expected 2 coordinates).\n");
	}
	else {
		dx = atoi(context->argv[1]);
    	dy = atoi(context->argv[2]);
    	ret = zdo_mousemove_relative(dx, dy);
    	if (ret == ZDO_ERROR) {
			fprintf(stderr, "zdo_mousemove_relative reported an error.\n");
		}
	}
	
	return 1;
}

