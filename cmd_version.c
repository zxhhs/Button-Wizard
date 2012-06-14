#include "ZMagic.h"

int cmd_version(context_t *context) {
	zdo_alert('g', "About Us     :\n");
	zdo_alert('b', "Program Name : ZMagic\n");
	zdo_alert('b', "Version      : 1.0.0\n");
	zdo_alert('r', "Created By   : Zhang Xiang & Zhong Guang Pei\n");
	zdo_alert('b', "Copyright    : 2012-2020\n");
	
	return 1;
}

