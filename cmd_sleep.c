#include "ZMagic.h"
#include "Z_do.h"
#include <stdio.h>
#include <getopt.h>

int cmd_sleep(context_t *context) {
	int ret;
	int option_index;
	char *cmd = *context->argv;
	char c;
	
	static const char *usage =
    "Usage: %s [--help] <seconds> \n"
    "Sleep for seconds.\n"
    "--help -h           - get help\n";
	
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
	
	if (context->argc == 0) {
		fprintf(stderr, usage, cmd);
		return 0;
	}
	else {
		zdo_sleep(atof(context->argv[0]));
	}
	return 1;
}

