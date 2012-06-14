#include "ZMagic.h"
#include "Z_do.h"
#include "unistd.h"
#include <stdio.h>
#include <getopt.h>

int cmd_click(context_t *context) {
	int ret;
	int option_index;
	char *cmd = *context->argv;
	char c;
	int repeat = 1;
	unsigned long delay = 100; // 100ms
	
	static const char *usage =
	"\x1b[1;32mUsage:\x1b[0m \x1b[31m%s\x1b[0m \x1b[1;34m[--repeat N] [--delay MS] [--help] <button>\x1b[0m \n"
    "\x1b[32mClick a mouse button.\x1b[0m\n"
    "\x1b[32m1     for     left(default).\x1b[0m\n"
    "\x1b[32m2     for     middle.\x1b[0m\n"
    "\x1b[32m3     for     right.\x1b[0m\n"
    "\x1b[32m4     for     wheel up.\x1b[0m\n"
    "\x1b[32m5     for     wheel down.\x1b[0m\n"
    "\x1b[1;34m--repeat   -r N\x1b[0m         \x1b[32m- repeat N times to click. Default is 1.\x1b[0m\n"
    "\x1b[1;34m--delay    -d MS\x1b[0m        \x1b[32m- delay in MS between clicks. Default is 100ms.\x1b[0m\n"
    "\x1b[1;34m--help     -h\x1b[0m           \x1b[32m- get help\x1b[0m\n";
	
	static struct option longopts[] = {
		{ "repeat", required_argument, NULL, 'r'},
		{ "delay", required_argument, NULL, 'd' },
		{ "help", no_argument, NULL, 'h'},
		{ 0, 0, 0, 0 },
	};
	
	while ((c = getopt_long_only(context->argc, context->argv, "r:d:h",
                               longopts, &option_index)) != -1) {
		switch (c) {
		  	case 'r':
				repeat = atoi(optarg);
				if (repeat <= 0) { 
					zdo_alert('r', "Invalid repeat value '%s' (must be >= 1).\n", optarg);
					//fprintf(stderr, "Invalid repeat value '%s' (must be >= 1)\n", optarg);
					fprintf(stderr, usage, cmd);
					return ZDO_ERROR;
				}
				break;
		    case 'd':
				delay = strtoul(optarg, NULL, 10);
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
	
	if (context->argc != 0)
		ret = zdo_click(atoi(context->argv[0]), repeat, delay * 1000);
	else
		ret = zdo_click(1, repeat, delay * 1000);
	if (ret == ZDO_ERROR) {
		zdo_alert('r', "zdo_click reported an error.\n");
	}
	return 1;
}

