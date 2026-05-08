

#ifndef NET_9P_TRANSPORT_H
#define NET_9P_TRANSPORT_H

#define P9_DEF_MIN_RESVPORT	(665U)
#define P9_DEF_MAX_RESVPORT	(1023U)



struct p9_trans_module {
	struct list_head list;
	char *name;		
	int maxsize;		
	int def;		
	struct module *owner;
	int (*create)(struct p9_client *, const char *, char *);
	void (*close) (struct p9_client *);
	int (*request) (struct p9_client *, struct p9_req_t *req);
	int (*cancel) (struct p9_client *, struct p9_req_t *req);
	int (*cancelled)(struct p9_client *, struct p9_req_t *req);
	int (*zc_request)(struct p9_client *, struct p9_req_t *,
			  struct iov_iter *, struct iov_iter *, int , int, int);
};

void v9fs_register_trans(struct p9_trans_module *m);
void v9fs_unregister_trans(struct p9_trans_module *m);
struct p9_trans_module *v9fs_get_trans_by_name(char *s);
struct p9_trans_module *v9fs_get_default_trans(void);
void v9fs_put_trans(struct p9_trans_module *m);
#endif 
