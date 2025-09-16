

#ifndef _LINUX_WAKEUP_REASON_H
#define _LINUX_WAKEUP_REASON_H

#define MAX_SUSPEND_ABORT_LEN 256

void log_wakeup_reason(int irq);
int check_wakeup_reason(int irq);

#ifdef CONFIG_SUSPEND
void log_suspend_abort_reason(const char *fmt, ...);
#else
static inline void log_suspend_abort_reason(const char *fmt, ...) { }
#endif

#endif 
