/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */


#ifndef _PRIORITY_CONTROL_MANAGER_H_
#define _PRIORITY_CONTROL_MANAGER_H_

#include <linux/mm.h>
#include <linux/of.h>
#include <linux/version.h>

struct priority_control_manager_device;


struct priority_control_manager_ops {
	
	int (*pcm_scheduler_priority_check)(
		struct priority_control_manager_device *pcm_dev,
		struct task_struct *task, int requested_priority);
};


struct priority_control_manager_device {
	struct priority_control_manager_ops ops;
	void *data;
	struct module *owner;
};

#endif 
