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
	"\x1b[1;32mUsage:\x1b[0m \x1b[31m%s\x1b[0m \x1b[1;34m[--help] <button>\x1b[0m \n"
    "\x1b[32mRelease a mouse button.\x1b[0m\n"
    "\x1b[32m1     for     left(default).\x1b[0m\n"
    "\x1b[32m2     for     middle.\x1b[0m\n"
    "\x1b[32m3     for     right.\x1b[0m\n"
    "\x1b[32m4     for     wheel up.\x1b[0m\n"
    "\x1b[32m5     for     wheel down.\x1b[0m\n"
    "\x1b[1;34m--help     -h\x1b[0m           \x1b[32m- get help\x1b[0m\n";
	
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
	
	if (context->argc != 0)
		ret = zdo_mouseup(atoi(context->argv[0]));
	else
		ret = zdo_mouseup(1);
	if (ret == ZDO_ERROR) {
		zdo_alert('r', "zdo_mouseup reported an error.\n");
	}
	return 1;
}

