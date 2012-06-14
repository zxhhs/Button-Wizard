#include "ZMagic.h"
#include "Z_do.h"
#include "unistd.h"
#include <stdio.h>
#include <getopt.h>

int cmd_getmouselocation(context_t *context) {
	int ret;
	int option_index;
	char *cmd = *context->argv;
	char c;
	int x, y;
	
	static const char *usage =
	"\x1b[1;32mUsage:\x1b[0m \x1b[31m%s\x1b[0m \x1b[1;34m[--help]\x1b[0m \n"
    "\x1b[32mGet locations of the mouse.\x1b[0m\n"
    "\x1b[1;34m--help     -h\x1b[0m        \x1b[32m- get help\x1b[0m\n";
	
	static struct option longopts[] = {
		{ "help", no_argument, NULL, 'h'},
		{ 0, 0, 0, 0 },
	};
	
	while ((c = getopt_long_only(context->argc, context->argv, "h",
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
	ret = zdo_getmouselocation(&x, &y);
	
	if (ret) {
		zdo_alert('r', "X=%d\n", x);
		zdo_alert('r', "Y=%d\n", y);
	}
	
	return ret;
}

