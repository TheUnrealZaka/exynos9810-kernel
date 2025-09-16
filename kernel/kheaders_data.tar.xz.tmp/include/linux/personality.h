#ifndef _LINUX_PERSONALITY_H
#define _LINUX_PERSONALITY_H

#include <uapi/linux/personality.h>


#define personality(pers)	(pers & PER_MASK)


#define set_personality(pers)	(current->personality = (pers))

#endif 
