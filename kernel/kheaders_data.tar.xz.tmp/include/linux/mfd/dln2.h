#ifndef __LINUX_USB_DLN2_H
#define __LINUX_USB_DLN2_H

#define DLN2_CMD(cmd, id)		((cmd) | ((id) << 8))

struct dln2_platform_data {
	u16 handle;		
	u8 port;		
};



typedef void (*dln2_event_cb_t)(struct platform_device *pdev, u16 echo,
				const void *data, int len);


int dln2_register_event_cb(struct platform_device *pdev, u16 event,
			   dln2_event_cb_t event_cb);


void dln2_unregister_event_cb(struct platform_device *pdev, u16 event);


int dln2_transfer(struct platform_device *pdev, u16 cmd,
		  const void *obuf, unsigned obuf_len,
		  void *ibuf, unsigned *ibuf_len);



static inline int dln2_transfer_rx(struct platform_device *pdev, u16 cmd,
				   void *ibuf, unsigned *ibuf_len)
{
	return dln2_transfer(pdev, cmd, NULL, 0, ibuf, ibuf_len);
}


static inline int dln2_transfer_tx(struct platform_device *pdev, u16 cmd,
				   const void *obuf, unsigned obuf_len)
{
	return dln2_transfer(pdev, cmd, obuf, obuf_len, NULL, NULL);
}

#endif
