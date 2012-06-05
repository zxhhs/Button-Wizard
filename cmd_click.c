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
    "Usage: %s [--repeat N] [--help] <button> \n"
    "Click a mouse button.\n"
    "1     for     left.(default)\n"
    "2     for     middle.\n"
    "3     for     right.\n"
    "4     for     wheel up.\n"
    "5     for     wheel down.\n"
    "--repeat -r N    - repeat N times to click. Default is 1\n"
    "--delay -d MS    - delay in MS between clicks. Default is 100ms\n"
    "--help -h        - get help\n";
	
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
					fprintf(stderr, "Invalid repeat value '%s' (must be >= 1)\n", optarg);
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
				printf("unknown opt: %d\n", c);
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
		fprintf(stderr, "zdo_click reported an error.\n");
	}
	return 1;
}

