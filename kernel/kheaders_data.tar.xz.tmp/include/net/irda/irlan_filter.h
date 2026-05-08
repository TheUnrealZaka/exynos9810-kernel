

#ifndef IRLAN_FILTER_H
#define IRLAN_FILTER_H

void irlan_check_command_param(struct irlan_cb *self, char *param, 
			       char *value);
void irlan_filter_request(struct irlan_cb *self, struct sk_buff *skb);
#ifdef CONFIG_PROC_FS
void irlan_print_filter(struct seq_file *seq, int filter_type);
#endif

#endif 
