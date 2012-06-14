#ifndef ZMAGIC_H
#define ZMAGIC_H

#define TRUE	1
#define FALSE	0

#include <stdio.h>
#include <stdlib.h>

typedef int Bool;

typedef struct context {
	int argc;
	char **argv;
} context_t;

extern int cmd_sleep(context_t *context);
extern int cmd_click(context_t *context);
extern int cmd_getmouselocation(context_t *context);
extern int cmd_key(context_t *context);
extern int cmd_mousedown(context_t *context);
extern int cmd_mousemove(context_t *context);
extern int cmd_mousemove_relative(context_t *context);
extern int cmd_mouseup(context_t *context);
extern int cmd_type(context_t *context);
extern int cmd_version(context_t *context);
extern int cmd_record(context_t *context);
extern int cmd_alert(context_t *context);

static int cmd_help();

#endif
