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
    "Usage: %s [--help] \n"
    "--help -h        - get help\n";
	
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
				printf("unknown opt: %d\n", c);
				fprintf(stderr, usage, cmd);
				return ZDO_ERROR;
		}
	}
	consume_args(context, optind);
	ret = zdo_getmouselocation(&x, &y);
	
	if (ret) {
		fprintf(stderr, "X=%d\n", x);
		fprintf(stderr, "Y=%d\n", y);
	}
	
	return ret;
}

