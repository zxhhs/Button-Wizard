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
    "Usage: %s [--relative] [--help] <x> <y> \n"
    "Move the mouse to the specific X and Y coordinates on the screen.\n"
    "--relative -r       - move the pointer relative to the current position\n"
    "--help -h           - get help\n";
	
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
				printf("unknown opt: %d\n", c);
				fprintf(stderr, usage, cmd);
				return ZDO_ERROR;
		}
	}
	consume_args(context, optind);
	
	if (context->argc < 2) {
		fprintf(stderr, usage, cmd);
		fprintf(stderr, "You specified the wrong number of args (expected 2 coordinates).\n");
	}
	else {
		x = atoi(context->argv[0]);
    	y = atoi(context->argv[1]);
    	ret = zdo_mousemove(x, y, is_rel);
    	if (ret == ZDO_ERROR) {
			fprintf(stderr, "zdo_mousemove reported an error.\n");
		}
	}
	
	return 1;
}

