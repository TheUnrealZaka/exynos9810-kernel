
#ifndef _LINUX_SELINUX_H
#define _LINUX_SELINUX_H

struct selinux_audit_rule;
struct audit_context;
struct kern_ipc_perm;

#ifdef CONFIG_SECURITY_SELINUX


bool selinux_is_enabled(void);
#else

static inline bool selinux_is_enabled(void)
{
	return false;
}
#endif	

#endif 
