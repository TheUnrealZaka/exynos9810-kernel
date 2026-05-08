

struct ntb_transport_qp;

struct ntb_transport_client {
	struct device_driver driver;
	int (*probe)(struct device *client_dev);
	void (*remove)(struct device *client_dev);
};

int ntb_transport_register_client(struct ntb_transport_client *drvr);
void ntb_transport_unregister_client(struct ntb_transport_client *drvr);
int ntb_transport_register_client_dev(char *device_name);
void ntb_transport_unregister_client_dev(char *device_name);

struct ntb_queue_handlers {
	void (*rx_handler)(struct ntb_transport_qp *qp, void *qp_data,
			   void *data, int len);
	void (*tx_handler)(struct ntb_transport_qp *qp, void *qp_data,
			   void *data, int len);
	void (*event_handler)(void *data, int status);
};

unsigned char ntb_transport_qp_num(struct ntb_transport_qp *qp);
unsigned int ntb_transport_max_size(struct ntb_transport_qp *qp);
struct ntb_transport_qp *
ntb_transport_create_queue(void *data, struct device *client_dev,
			   const struct ntb_queue_handlers *handlers);
void ntb_transport_free_queue(struct ntb_transport_qp *qp);
int ntb_transport_rx_enqueue(struct ntb_transport_qp *qp, void *cb, void *data,
			     unsigned int len);
int ntb_transport_tx_enqueue(struct ntb_transport_qp *qp, void *cb, void *data,
			     unsigned int len);
void *ntb_transport_rx_remove(struct ntb_transport_qp *qp, unsigned int *len);
void ntb_transport_link_up(struct ntb_transport_qp *qp);
void ntb_transport_link_down(struct ntb_transport_qp *qp);
bool ntb_transport_link_query(struct ntb_transport_qp *qp);
unsigned int ntb_transport_tx_free_entry(struct ntb_transport_qp *qp);
