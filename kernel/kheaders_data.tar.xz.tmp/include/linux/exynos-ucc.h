

struct ucc_req {
	struct plist_node node;
	int active;
	char name[20];
};

extern int filter_cstate(int cpu, int index);
extern void __init init_exynos_ucc(void);

#ifdef CONFIG_ARM64_EXYNOS_CPUIDLE
extern int filter_cstate(int cpu, int index);
extern void ucc_add_request(struct ucc_req *req, int value);
extern void ucc_update_request(struct ucc_req *req, int value);
extern void ucc_remove_request(struct ucc_req *req);
#else
static inline int filter_cstate(int cpu, int index) { return index; }
static inline void ucc_add_request(struct ucc_req *req, int value) { }
static inline void ucc_update_request(struct ucc_req *req, int value) { }
static inline void ucc_remove_request(struct ucc_req *req) { }
#endif