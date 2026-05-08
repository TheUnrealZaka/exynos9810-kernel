

#ifndef _SAA6588_H
#define _SAA6588_H

struct saa6588_command {
	unsigned int  block_count;
	bool          nonblocking;
	int           result;
	unsigned char __user *buffer;
	struct file   *instance;
	poll_table    *event_list;
};


#define SAA6588_CMD_CLOSE	_IOW('R', 2, int)
#define SAA6588_CMD_READ	_IOR('R', 3, int)
#define SAA6588_CMD_POLL	_IOR('R', 4, int)

#endif
