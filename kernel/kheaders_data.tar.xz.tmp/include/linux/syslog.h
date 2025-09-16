

#ifndef _LINUX_SYSLOG_H
#define _LINUX_SYSLOG_H


#define SYSLOG_ACTION_CLOSE          0

#define SYSLOG_ACTION_OPEN           1

#define SYSLOG_ACTION_READ           2

#define SYSLOG_ACTION_READ_ALL       3

#define SYSLOG_ACTION_READ_CLEAR     4

#define SYSLOG_ACTION_CLEAR          5

#define SYSLOG_ACTION_CONSOLE_OFF    6

#define SYSLOG_ACTION_CONSOLE_ON     7

#define SYSLOG_ACTION_CONSOLE_LEVEL  8

#define SYSLOG_ACTION_SIZE_UNREAD    9

#define SYSLOG_ACTION_SIZE_BUFFER   10

#define SYSLOG_FROM_READER           0
#define SYSLOG_FROM_PROC             1

int do_syslog(int type, char __user *buf, int count, int source);

#ifdef CONFIG_PRINTK
int check_syslog_permissions(int type, int source);
#else
static inline int check_syslog_permissions(int type, int source)
{
	return 0;
}
#endif

#endif 
