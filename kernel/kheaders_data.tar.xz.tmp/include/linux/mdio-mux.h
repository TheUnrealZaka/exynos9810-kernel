
#ifndef __LINUX_MDIO_MUX_H
#define __LINUX_MDIO_MUX_H
#include <linux/device.h>
#include <linux/phy.h>

int mdio_mux_init(struct device *dev,
		  int (*switch_fn) (int cur, int desired, void *data),
		  void **mux_handle,
		  void *data,
		  struct mii_bus *mux_bus);

void mdio_mux_uninit(void *mux_handle);

#endif 
