
#ifndef _LINUX_EVENTPOLL_H
#define _LINUX_EVENTPOLL_H

#include <uapi/linux/eventpoll.h>



struct file;


#ifdef CONFIG_EPOLL


static inline void eventpoll_init_file(struct file *file)
{
	INIT_LIST_HEAD(&file->f_ep_links);
	INIT_LIST_HEAD(&file->f_tfile_llink);
}



void eventpoll_release_file(struct file *file);


static inline void eventpoll_release(struct file *file)
{

	
	if (likely(list_empty(&file->f_ep_links)))
		return;

	
	eventpoll_release_file(file);
}

#else

static inline void eventpoll_init_file(struct file *file) {}
static inline void eventpoll_release(struct file *file) {}

#endif

#endif 
