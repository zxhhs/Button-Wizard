#include "ZMagic.h"
#include "Z_do.h"
#include <stdio.h>

int cmd_type(context_t *context) {
	int i;
	int ret;
	for (i=1; i<context->argc; i++) {
		ret = zdo_type(context->argv[i]);
		if (ret == ZDO_ERROR) {
			fprintf(stderr, 
				"zdo_type reported an error for string '%s'\n", 
				context->argv[i]);
		}
	}
	return 1;
}

