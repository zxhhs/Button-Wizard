#include "ZMagic.h"
#include "Z_do.h"
#include <stdio.h>
#include <getopt.h>

int cmd_mousemove(context_t *context) {
	int x, y;
	int ret;
	char c;
	int is_rel = 0;
	int option_index;
	char *cmd = *context->argv;
	
	static const char *usage =
	"\x1b[1;32mUsage:\x1b[0m \x1b[31m%s\x1b[0m \x1b[1;34m[--relative] [--help] <x> <y>\x1b[0m \n"
    "\x1b[32mMove the mouse to the specific X and Y coordinates on the screen.\x1b[0m\n"
    "\x1b[1;34m--relative   -r\x1b[0m           \x1b[32m- move the pointer relative to the current position.\x1b[0m\n"
    "\x1b[1;34m--help       -h\x1b[0m           \x1b[32m- get help\x1b[0m\n";
	
	static struct option longopts[] = {
		{ "relative", no_argument, NULL, 'r' },
		{ "help", no_argument, NULL, 'h'},
		{ 0, 0, 0, 0 },
	};
	
	while ((c = getopt_long_only(context->argc, context->argv, "rh",
                               longopts, &option_index)) != -1) {
		switch (c) {
			case 'r':
				is_rel = 1;
				break;
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
		x = atoi(context->argv[0]);
    	y = atoi(context->argv[1]);
    	ret = zdo_mousemove(x, y, is_rel);
    	if (ret == ZDO_ERROR) {
    		zdo_alert('r', "zdo_mousemove reported an error.\n");
		}
	}
	
	return 1;
}

