#include "ZMagic.h"
#include "Z_do.h"
#include "unistd.h"

#include <stdio.h>
#include <getopt.h>

int cmd_record(context_t *context) {
	int option_index;
	char *cmd = *context->argv;
	char c;
	int is_continue = 0;
	int key_mask = 0;
	int mouse_mask = 0;
	char *file;
	
	static const char *usage =
    "\x1b[1;32mUsage:\x1b[0m \x1b[31m%s\x1b[0m \x1b[1;34m[--mouse_only] [--key_only] [--continue] [--help] <file>\x1b[0m \n"
    "\x1b[32mRecord the motions of mouse and keyboard into the file(should be *.sh).\x1b[0m\n"
    "\x1b[1;34m--mouse_only -m\x1b[0m        \x1b[32m- record mouse movements only.\x1b[0m\n"
    "\x1b[1;34m--key_only   -k\x1b[0m        \x1b[32m- record keyboard movements only.\x1b[0m\n"
    "\x1b[1;34m--continue   -c\x1b[0m        \x1b[32m- write records from the end of the file(must exists).\x1b[0m\n"
    "\x1b[1;34m--help       -h\x1b[0m        \x1b[32m- get help\x1b[0m\n";
	
	static struct option longopts[] = {
		{ "mouse_only", no_argument, NULL, 'm'},
		{ "key_only", no_argument, NULL, 'k'},
		{ "continue", no_argument, NULL, 'c'},
		{ "help", no_argument, NULL, 'h'},
		{ 0, 0, 0, 0 },
	};
	
	while ((c = getopt_long_only(context->argc, context->argv, "mkhc",
                               longopts, &option_index)) != -1) {
		switch (c) {
			case 'm':
				key_mask = 1;
				mouse_mask = 0;
				break;
			case 'k':
				key_mask = 0;
				mouse_mask = 1;
				break;
			case 'c':
				is_continue = 1;
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
		file = context->argv[0];
		if (!is_continue && access(file, 0) != -1) {
			//file exists
			zdo_alert('r', "The %s exists. Do you want to overwrite it? (y/c/n)\n", file);
			zdo_alert('g', "--select 'y' to overwrite.\n");
			zdo_alert('g', "--select 'c' to continue.\n");
			zdo_alert('g', "--select 'n' to quit.\n");

			scanf("%c", &c);
			if (c == 'y') {
				is_continue = 0;
			}
			else if (c == 'c') {
				is_continue = 1;
			}
			else {
				return ZDO_SUCCESS;
			}
		}
		else if (is_continue && access(file, 0) == -1) {
			zdo_alert('r', "No such file(%s).\n", file);
			return ZDO_ERROR;
		}
		zdo_record(file, is_continue, mouse_mask, key_mask);
	}
	else {
		zdo_alert('r', "Need a file for record.\n");
		printf(usage, cmd);	
	}
	return ZDO_SUCCESS;
}

