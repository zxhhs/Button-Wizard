#include "ZMagic.h"
#include "Z_do.h"
#include <stdio.h>
#include <string.h>

int cmd_key(context_t *context) {
	int i;
	int ret;
	for (i=1; i<context->argc; i++) {
		ret = zdo_key(context->argv[i], context->argv[0]);
		if (ret == ZDO_ERROR) {
			fprintf(stderr, 
				"zdo_key reported an error for string '%s'\n", 
				context->argv[i]);
		}
	}
	return 1;
}

