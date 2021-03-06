#include <string.h>
#include "ZMagic.h"

struct dispatch {
  const char *name;
  int (*func)(context_t *context);
} dispatch[] = {
  { "click", cmd_click, },
  { "mouseup", cmd_mouseup, },
  { "mousedown", cmd_mousedown, },
  { "mousemove", cmd_mousemove, },
  { "mousemove_relative", cmd_mousemove_relative, },

  { "key", cmd_key, },
  { "keydown", cmd_key, },
  { "keyup", cmd_key, },
  { "type", cmd_type, },

  { "sleep", cmd_sleep, },
  
  { "getmouselocation", cmd_getmouselocation, },
  { "record", cmd_record, },
  
  { "help", cmd_help, },
  { "alert", cmd_alert, },
  { "version", cmd_version, },

  { NULL, NULL, },
};

int main(int argc, char *argv[]) {
	return ZMagic_main(argc, argv);
}

Bool is_command(char *cmd) {
	int i;
	for (i=0; dispatch[i].name != NULL; i++) {
		if (!strcasecmp(dispatch[i].name, cmd)) {
        	return TRUE;
      	}
	}
	return FALSE;
}

int ZMagic_main(int argc, char *argv[]) {
	if (argc >= 2) {
		if (is_command(argv[1])) {
			//printf("%s\n", argv[1]);
			return cmd_exec(argc, argv);
		}
		else {
			zdo_alert('r', "Command '%s' is not found.\n", argv[1]);
			cmd_help();
			return 1;
		}
	}
	else {
		cmd_help();
	}
	return 0;
}

int cmd_exec(int argc, char *argv[]) {
	context_t context;
	context.argc = --argc;
	context.argv = ++argv;
	
	int i;
	for (i=0; dispatch[i].name != NULL; i++) {
		if (!strcasecmp(dispatch[i].name, argv[0])) {
        	return dispatch[i].func(&context);
      	}
	}
	return FALSE;
}

void consume_args(context_t *context, int count) {
	if (count > context->argc) {
		zdo_alert('r', "Can't consume %d args; are only %d available. This is a bug.\n", 
					count, context->argc);
		context->argv += context->argc;
		context->argc = 0;
		return;
	}

	context->argv += count;
	context->argc -= count;
}

int cmd_help() {
	int i;
	zdo_alert('g', "Available commands:\n");
	for (i = 0; dispatch[i].name != NULL; i++)
		zdo_alert('b', "--%s\n", dispatch[i].name);

	return 0;
}

