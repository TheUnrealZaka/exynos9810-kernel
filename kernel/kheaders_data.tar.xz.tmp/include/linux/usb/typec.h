/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __LINUX_USB_TYPEC_H
#define __LINUX_USB_TYPEC_H

#include <linux/types.h>


#define ALTMODE_MAX_MODES	6


#define USB_TYPEC_REV_1_0	0x100 
#define USB_TYPEC_REV_1_1	0x110 
#define USB_TYPEC_REV_1_2	0x120 

struct typec_altmode;
struct typec_partner;
struct typec_cable;
struct typec_plug;
struct typec_port;

struct fwnode_handle;

enum typec_port_type {
	TYPEC_PORT_DFP,
	TYPEC_PORT_UFP,
	TYPEC_PORT_DRP,
};

enum typec_plug_type {
	USB_PLUG_NONE,
	USB_PLUG_TYPE_A,
	USB_PLUG_TYPE_B,
	USB_PLUG_TYPE_C,
	USB_PLUG_CAPTIVE,
};

enum typec_data_role {
	TYPEC_DEVICE,
	TYPEC_HOST,
};

enum typec_role {
	TYPEC_SINK,
	TYPEC_SOURCE,
};

enum typec_pwr_opmode {
	TYPEC_PWR_MODE_USB,
	TYPEC_PWR_MODE_1_5A,
	TYPEC_PWR_MODE_3_0A,
	TYPEC_PWR_MODE_PD,
};

enum typec_accessory {
	TYPEC_ACCESSORY_NONE,
	TYPEC_ACCESSORY_AUDIO,
	TYPEC_ACCESSORY_DEBUG,
};

#define TYPEC_MAX_ACCESSORY	3


struct usb_pd_identity {
	u32			id_header;
	u32			cert_stat;
	u32			product;
};

int typec_partner_set_identity(struct typec_partner *partner);
int typec_cable_set_identity(struct typec_cable *cable);


struct typec_mode_desc {
	int			index;
	u32			vdo;
	char			*desc;
	
	enum typec_port_type	roles;
};


struct typec_altmode_desc {
	u16			svid;
	int			n_modes;
	struct typec_mode_desc	modes[ALTMODE_MAX_MODES];
};

struct typec_altmode
*typec_partner_register_altmode(struct typec_partner *partner,
				const struct typec_altmode_desc *desc);
struct typec_altmode
*typec_plug_register_altmode(struct typec_plug *plug,
			     const struct typec_altmode_desc *desc);
struct typec_altmode
*typec_port_register_altmode(struct typec_port *port,
			     const struct typec_altmode_desc *desc);
void typec_unregister_altmode(struct typec_altmode *altmode);

struct typec_port *typec_altmode2port(struct typec_altmode *alt);

void typec_altmode_update_active(struct typec_altmode *alt, int mode,
				 bool active);

enum typec_plug_index {
	TYPEC_PLUG_SOP_P,
	TYPEC_PLUG_SOP_PP,
};


struct typec_plug_desc {
	enum typec_plug_index	index;
};


struct typec_cable_desc {
	enum typec_plug_type	type;
	unsigned int		active:1;
	struct usb_pd_identity	*identity;
};


struct typec_partner_desc {
	unsigned int		usb_pd:1;
	enum typec_accessory	accessory;
	struct usb_pd_identity	*identity;
};


struct typec_capability {
	enum typec_port_type	type;
	u16			revision; 
	u16			pd_revision; 
	int			prefer_role;
	enum typec_accessory	accessory[TYPEC_MAX_ACCESSORY];

	struct fwnode_handle	*fwnode;

	int		(*try_role)(const struct typec_capability *,
				    int role);

	int		(*dr_set)(const struct typec_capability *,
				  enum typec_data_role);
	int		(*pr_set)(const struct typec_capability *,
				  enum typec_role);
	int		(*vconn_set)(const struct typec_capability *,
				     enum typec_role);

	int		(*activate_mode)(const struct typec_capability *,
					 int mode, int activate);
	int		(*port_type_set)(const struct typec_capability *,
					enum typec_port_type);

};


#define TYPEC_NO_PREFERRED_ROLE	(-1)

struct typec_port *typec_register_port(struct device *parent,
				       const struct typec_capability *cap);
void typec_unregister_port(struct typec_port *port);

struct typec_partner *typec_register_partner(struct typec_port *port,
					     struct typec_partner_desc *desc);
void typec_unregister_partner(struct typec_partner *partner);

struct typec_cable *typec_register_cable(struct typec_port *port,
					 struct typec_cable_desc *desc);
void typec_unregister_cable(struct typec_cable *cable);

struct typec_plug *typec_register_plug(struct typec_cable *cable,
				       struct typec_plug_desc *desc);
void typec_unregister_plug(struct typec_plug *plug);

void typec_set_data_role(struct typec_port *port, enum typec_data_role role);
void typec_set_pwr_role(struct typec_port *port, enum typec_role role);
void typec_set_vconn_role(struct typec_port *port, enum typec_role role);
void typec_set_pwr_opmode(struct typec_port *port, enum typec_pwr_opmode mode);

#endif 
