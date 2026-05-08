

#ifndef __SCO_H
#define __SCO_H


#define SCO_DEFAULT_MTU		500


struct sockaddr_sco {
	sa_family_t	sco_family;
	bdaddr_t	sco_bdaddr;
};


#define SCO_OPTIONS	0x01
struct sco_options {
	__u16 mtu;
};

#define SCO_CONNINFO	0x02
struct sco_conninfo {
	__u16 hci_handle;
	__u8  dev_class[3];
};

#endif 
