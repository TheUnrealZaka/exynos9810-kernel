
#ifndef QEDE_ROCE_H
#define QEDE_ROCE_H

struct qedr_dev;
struct qed_dev;
struct qede_dev;

enum qede_roce_event {
	QEDE_UP,
	QEDE_DOWN,
	QEDE_CHANGE_ADDR,
	QEDE_CLOSE
};

struct qede_roce_event_work {
	struct list_head list;
	struct work_struct work;
	void *ptr;
	enum qede_roce_event event;
};

struct qedr_driver {
	unsigned char name[32];

	struct qedr_dev* (*add)(struct qed_dev *, struct pci_dev *,
				struct net_device *);

	void (*remove)(struct qedr_dev *);
	void (*notify)(struct qedr_dev *, enum qede_roce_event);
};


int qede_roce_register_driver(struct qedr_driver *drv);
void qede_roce_unregister_driver(struct qedr_driver *drv);

bool qede_roce_supported(struct qede_dev *dev);

#if IS_ENABLED(CONFIG_QED_RDMA)
int qede_roce_dev_add(struct qede_dev *dev);
void qede_roce_dev_event_open(struct qede_dev *dev);
void qede_roce_dev_event_close(struct qede_dev *dev);
void qede_roce_dev_remove(struct qede_dev *dev);
void qede_roce_event_changeaddr(struct qede_dev *qedr);
#else
static inline int qede_roce_dev_add(struct qede_dev *dev)
{
	return 0;
}

static inline void qede_roce_dev_event_open(struct qede_dev *dev) {}
static inline void qede_roce_dev_event_close(struct qede_dev *dev) {}
static inline void qede_roce_dev_remove(struct qede_dev *dev) {}
static inline void qede_roce_event_changeaddr(struct qede_dev *qedr) {}
#endif
#endif
