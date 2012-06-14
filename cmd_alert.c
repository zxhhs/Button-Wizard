#include "ZMagic.h"
#include "Z_do.h"

#include <stdio.h>
#include <getopt.h>

int cmd_alert(context_t *context) {
	int option_index;
	char *cmd = *context->argv;
	char c;
	char color = 'r';
	char *s;
	
	static const char *usage =
    "\x1b[1;32mUsage:\x1b[0m \x1b[31m%s\x1b[0m \x1b[1;34m[--red] [--green] [--blue] [--help] <text>\x1b[0m \n"
    "\x1b[32mPrint the text in the console.\x1b[0m\n"
    "\x1b[1;34m--red      -m\x1b[0m        \x1b[32m- print red text(default).\x1b[0m\n"
    "\x1b[1;34m--green    -k\x1b[0m        \x1b[32m- print green text.\x1b[0m\n"
    "\x1b[1;34m--blue     -c\x1b[0m        \x1b[32m- print blue text.\x1b[0m\n"
    "\x1b[1;34m--help     -h\x1b[0m        \x1b[32m- get help\x1b[0m\n";
	
	static struct option longopts[] = {
		{ "red", no_argument, NULL, 'r'},
		{ "green", no_argument, NULL, 'g'},
		{ "blue", no_argument, NULL, 'b'},
		{ "help", no_argument, NULL, 'h'},
		{ 0, 0, 0, 0 },
	};
	
	while ((c = getopt_long_only(context->argc, context->argv, "rgbh",
                               longopts, &option_index)) != -1) {
		switch (c) {
			case 'r':
			case 'g':
			case 'b':
				color = c;
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
	
	if (context->argc != 0) {
		char *format = context->argv[0];
va_list args;
		zdo_alert(color, format, args);
	}
	else {
		printf(usage, cmd);
	}
	return ZDO_SUCCESS;
}

