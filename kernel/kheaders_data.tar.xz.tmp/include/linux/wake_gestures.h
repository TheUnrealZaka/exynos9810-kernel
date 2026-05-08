

#ifndef _LINUX_WAKE_GESTURES_H
#define _LINUX_WAKE_GESTURES_H

#include <linux/input.h>

extern int s2w_switch;
extern int s2w_switch_temp;
extern bool s2w_switch_changed;
extern int dt2w_switch;
extern int dt2w_switch_temp;
extern bool dt2w_switch_changed;
extern int dt2w_custom_tap;
extern int dt2w_custom_tap_temp;
extern bool dt2w_custom_tap_changed;
extern bool gestures_enabled;
bool scr_suspended(void);
void vib_trigger(int value);

#endif	

