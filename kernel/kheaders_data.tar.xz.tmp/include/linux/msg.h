#ifndef _LINUX_MSG_H
#define _LINUX_MSG_H

#include <linux/list.h>
#include <uapi/linux/msg.h>


struct msg_msg {
	struct list_head m_list;
	long m_type;
	size_t m_ts;		
	struct msg_msgseg *next;
	void *security;
	
};


struct msg_queue {
	struct kern_ipc_perm q_perm;
	time_t q_stime;			
	time_t q_rtime;			
	time_t q_ctime;			
	unsigned long q_cbytes;		
	unsigned long q_qnum;		
	unsigned long q_qbytes;		
	pid_t q_lspid;			
	pid_t q_lrpid;			

	struct list_head q_messages;
	struct list_head q_receivers;
	struct list_head q_senders;
};


extern long do_msgsnd(int msqid, long mtype, void __user *mtext,
			size_t msgsz, int msgflg);
extern long do_msgrcv(int msqid, void __user *buf, size_t bufsz, long msgtyp,
		      int msgflg,
		      long (*msg_fill)(void __user *, struct msg_msg *,
				       size_t));

#endif 
