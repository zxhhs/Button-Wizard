#include "ZMagic.h"
#include "Z_do.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "Z_do_util.h"

int cmd_key(context_t *context) {
	int i;
	int ret;
	int option_index;
	char *cmd = *context->argv;
	char c;
	
	static const char *usage =
	"\x1b[1;32mUsage:\x1b[0m \x1b[31m%s\x1b[0m \x1b[1;34m[--help] <key>\x1b[0m \n"
    "\x1b[32mPress a key or a list of key(devided by '+').\x1b[0m\n"
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
				int i;
				zdo_alert('g', "\nAvailable keys:\n");
				zdo_alert('g', "Meaning\t\tkey_name\n");
				for (i = 0; keysym_charmap[i].meaning != NULL; i++) {
					zdo_alert('b', "%-15s%-10s\n", keysym_charmap[i].meaning, 
							keysym_charmap[i].keysym);
				}
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
			ret = zdo_key(context->argv[i], cmd);
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
	return ZDO_SUCCESS;
}

