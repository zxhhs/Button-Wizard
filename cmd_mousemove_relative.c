#include "ZMagic.h"
#include "Z_do.h"
#include <stdio.h>
#include <getopt.h>

int cmd_mousemove_relative(context_t *context) {
	int dx, dy;
	int ret;
	char c;
	int option_index;
	char *cmd = *context->argv;
	
	static const char *usage =
	"\x1b[1;32mUsage:\x1b[0m \x1b[31m%s\x1b[0m \x1b[1;34m[--help] <x> <y>\x1b[0m \n"
    "\x1b[32mMove the pointer relative to the current position.\x1b[0m\n"
    "\x1b[1;34m--help       -h\x1b[0m           \x1b[32m- get help\x1b[0m\n";
	
	static struct option longopts[] = {
		{ "help", no_argument, NULL, 'h'},
		{ 0, 0, 0, 0 },
	};
	
	while ((c = getopt_long_only(context->argc, context->argv, "rh",
                               longopts, &option_index)) != -1) {
		switch (c) {
		  	case 'h':
				printf(usage, cmd);
				return ZDO_SUCCESS;
				break;
		  	default:
				zdo_alert('r', "unknown option.\n");
				fprintf(stderr, usage, cmd);
				return ZDO_ERROR;
		}
	}
	consume_args(context, optind);
	
	if (context->argc < 2) {
		zdo_alert('r', "You specified the wrong number of args (expected 2 coordinates).\n");
		fprintf(stderr, usage, cmd);
	}
	else {
		dx = atoi(context->argv[0]);
    	dy = atoi(context->argv[1]);
    	ret = zdo_mousemove_relative(dx, dy);
    	if (ret == ZDO_ERROR) {
    		zdo_alert('r', "zdo_mousemove_relative reported an error.\n");
		}
	}
	
	return ZDO_SUCCESS;
}

