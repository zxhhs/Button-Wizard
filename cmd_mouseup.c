#include "ZMagic.h"
#include "Z_do.h"
#include <stdio.h>
#include <getopt.h>

int cmd_mouseup(context_t *context) {
	int ret;
	int option_index;
	char *cmd = *context->argv;
	char c;
	
	static const char *usage =
    "Usage: %s [--help] <button> \n"
    "Release a mouse button.\n"
    "1     for     left.(default)\n"
    "2     for     middle.\n"
    "3     for     right.\n"
    "4     for     wheel up.\n"
    "5     for     wheel down.\n"
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
	
	if (context->argc != 0)
		ret = zdo_mouseup(atoi(context->argv[0]));
	else
		ret = zdo_mouseup(1);
	if (ret == ZDO_ERROR) {
		fprintf(stderr, "zdo_mouseup reported an error.\n");
	}
	return 1;
}

