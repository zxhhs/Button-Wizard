#include "ZMagic.h"
#include "Z_do.h"
#include <stdio.h>
#include <getopt.h>

int cmd_type(context_t *context) {
	int i;
	int ret;
	int option_index;
	char *cmd = *context->argv;
	char c;
	
	static const char *usage =
	"\x1b[1;32mUsage:\x1b[0m \x1b[31m%s\x1b[0m \x1b[1;34m[--repeat N][--help] <button>\x1b[0m \n"
    "\x1b[32mSend keystrokes.\x1b[0m\n"
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
	
	if (context->argc != 0) {
		for (i=0; i<context->argc; i++) {
			ret = zdo_type(context->argv[i]);
			if (ret == ZDO_ERROR) {
				zdo_alert('r', "zdo_key reported an error for string '%s'.\n", 
					context->argv[i]);
			}
		}
	}
	else {
		zdo_alert('r', "No keys.\n");
		printf(usage, cmd);
	}
	return 1;
}

