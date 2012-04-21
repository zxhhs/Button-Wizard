/**
 * Simulate the keyboard and mouse movements and actions.
 *
 * Created By Zhang Xiang In 2012/4/6
*/

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


/**
 * Function: Simulate the keyboard action. Such as push down, release, press and so on.
 * Inputs:
 * int 		fd		;KeyBoard Device
 * int 		type	;0 for release, 1 for push down and 2 for autorepeat(pressing)
 * int 		kval	;Which key to be pressed
 *
*/
void simulate_key(int fd, int type, int kval) {
    struct input_event event;
    event.type = EV_KEY;
    event.value = type;
    event.code = kval;
    gettimeofday(&event.time, 0);
    write(fd, &event, sizeof(event));

	sync_dev(fd);
}

/**
 * Function: Simulate the keyboard action. Such as push down, release, press and so on.
 * Inputs:
 * int 		fd		;Mouse Device
 * int 		type	;Now REL_X and REL_Y, How to EV_ABS is a question.....
 * int 		value	;The reletive mouse position
 *
*/
void simulate_mouse(int fd, int type, int value) {
    struct input_event event;
    event.type = EV_REL;
    event.value = value;
    event.code = type;
    gettimeofday(&event.time, 0);
    write(fd, &event, sizeof(event));

	sync_dev(fd);
}

/**
 * Function: Must synchronize the device after each action unless all the changes will not be effective.
 * Inputs:
 * int 		fd		;Device
 *
*/
void sync_dev(int fd) {
	struct input_event event;
    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    gettimeofday(&event.time,0);
    write(fd, &event, sizeof(event));
}


