#ifndef _IP_SET_COMMENT_H
#define _IP_SET_COMMENT_H



#ifdef __KERNEL__

static inline char*
ip_set_comment_uget(struct nlattr *tb)
{
	return nla_data(tb);
}


static inline void
ip_set_init_comment(struct ip_set_comment *comment,
		    const struct ip_set_ext *ext)
{
	struct ip_set_comment_rcu *c = rcu_dereference_protected(comment->c, 1);
	size_t len = ext->comment ? strlen(ext->comment) : 0;

	if (unlikely(c)) {
		kfree_rcu(c, rcu);
		rcu_assign_pointer(comment->c, NULL);
	}
	if (!len)
		return;
	if (unlikely(len > IPSET_MAX_COMMENT_SIZE))
		len = IPSET_MAX_COMMENT_SIZE;
	c = kzalloc(sizeof(*c) + len + 1, GFP_ATOMIC);
	if (unlikely(!c))
		return;
	strlcpy(c->str, ext->comment, len + 1);
	rcu_assign_pointer(comment->c, c);
}


static inline int
ip_set_put_comment(struct sk_buff *skb, struct ip_set_comment *comment)
{
	struct ip_set_comment_rcu *c = rcu_dereference(comment->c);

	if (!c)
		return 0;
	return nla_put_string(skb, IPSET_ATTR_COMMENT, c->str);
}


static inline void
ip_set_comment_free(struct ip_set_comment *comment)
{
	struct ip_set_comment_rcu *c;

	c = rcu_dereference_protected(comment->c, 1);
	if (unlikely(!c))
		return;
	kfree_rcu(c, rcu);
	rcu_assign_pointer(comment->c, NULL);
}

#endif
#endif
