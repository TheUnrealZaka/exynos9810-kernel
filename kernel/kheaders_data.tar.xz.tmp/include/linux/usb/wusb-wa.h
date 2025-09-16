
#ifndef __LINUX_USB_WUSB_WA_H
#define __LINUX_USB_WUSB_WA_H


enum {
	WA_EXEC_RC_CMD = 40,	
};


enum {
	WUSB_REQ_ADD_MMC_IE     = 20,
	WUSB_REQ_REMOVE_MMC_IE  = 21,
	WUSB_REQ_SET_NUM_DNTS   = 22,
	WUSB_REQ_SET_CLUSTER_ID = 23,
	WUSB_REQ_SET_DEV_INFO   = 24,
	WUSB_REQ_GET_TIME       = 25,
	WUSB_REQ_SET_STREAM_IDX = 26,
	WUSB_REQ_SET_WUSB_MAS   = 27,
	WUSB_REQ_CHAN_STOP      = 28,
};



enum {
	WUSB_TIME_ADJ   = 0,
	WUSB_TIME_BPST  = 1,
	WUSB_TIME_WUSB  = 2,
};

enum {
	WA_ENABLE = 0x01,
	WA_RESET = 0x02,
	RPIPE_PAUSE = 0x1,
	RPIPE_STALL = 0x2,
};


enum {
	WA_STATUS_ENABLED = 0x01,
	WA_STATUS_RESETTING = 0x02
};

enum rpipe_crs {
	RPIPE_CRS_CTL = 0x01,
	RPIPE_CRS_ISO = 0x02,
	RPIPE_CRS_BULK = 0x04,
	RPIPE_CRS_INTR = 0x08
};


struct usb_rpipe_descriptor {
	u8	bLength;
	u8	bDescriptorType;
	__le16  wRPipeIndex;
	__le16	wRequests;
	__le16	wBlocks;		
	__le16	wMaxPacketSize;		
	union {
		u8	dwa_bHSHubAddress;		
		u8	hwa_bMaxBurst;			
	};
	union {
		u8	dwa_bHSHubPort;		
		u8	hwa_bDeviceInfoIndex;	
	};
	u8	bSpeed;			
	union {
		u8 dwa_bDeviceAddress;	
		u8 hwa_reserved;		
	};
	u8	bEndpointAddress;	
	u8	bDataSequence;		
	__le32	dwCurrentWindow;	
	u8	bMaxDataSequence;	
	u8	bInterval;		
	u8	bOverTheAirInterval;	
	u8	bmAttribute;		
	u8	bmCharacteristics;	
	u8	bmRetryOptions;		
	__le16	wNumTransactionErrors;	
} __attribute__ ((packed));


enum wa_notif_type {
	DWA_NOTIF_RWAKE = 0x91,
	DWA_NOTIF_PORTSTATUS = 0x92,
	WA_NOTIF_TRANSFER = 0x93,
	HWA_NOTIF_BPST_ADJ = 0x94,
	HWA_NOTIF_DN = 0x95,
};


struct wa_notif_hdr {
	u8 bLength;
	u8 bNotifyType;			
} __packed;


struct hwa_notif_dn {
	struct wa_notif_hdr hdr;
	u8 bSourceDeviceAddr;		
	u8 bmAttributes;
	struct wusb_dn_hdr dndata[];
} __packed;


enum wa_xfer_type {
	WA_XFER_TYPE_CTL = 0x80,
	WA_XFER_TYPE_BI = 0x81,		
	WA_XFER_TYPE_ISO = 0x82,
	WA_XFER_RESULT = 0x83,
	WA_XFER_ABORT = 0x84,
	WA_XFER_ISO_PACKET_INFO = 0xA0,
	WA_XFER_ISO_PACKET_STATUS = 0xA1,
};


struct wa_xfer_hdr {
	u8 bLength;			
	u8 bRequestType;		
	__le16 wRPipe;			
	__le32 dwTransferID;		
	__le32 dwTransferLength;	
	u8 bTransferSegment;
} __packed;

struct wa_xfer_ctl {
	struct wa_xfer_hdr hdr;
	u8 bmAttribute;
	__le16 wReserved;
	struct usb_ctrlrequest baSetupData;
} __packed;

struct wa_xfer_bi {
	struct wa_xfer_hdr hdr;
	u8 bReserved;
	__le16 wReserved;
} __packed;


struct wa_xfer_hwaiso {
	struct wa_xfer_hdr hdr;
	u8 bReserved;
	__le16 wPresentationTime;
	__le32 dwNumOfPackets;
} __packed;

struct wa_xfer_packet_info_hwaiso {
	__le16 wLength;
	u8 bPacketType;
	u8 bReserved;
	__le16 PacketLength[0];
} __packed;

struct wa_xfer_packet_status_len_hwaiso {
	__le16 PacketLength;
	__le16 PacketStatus;
} __packed;

struct wa_xfer_packet_status_hwaiso {
	__le16 wLength;
	u8 bPacketType;
	u8 bReserved;
	struct wa_xfer_packet_status_len_hwaiso PacketStatus[0];
} __packed;


struct wa_xfer_abort {
	u8 bLength;
	u8 bRequestType;
	__le16 wRPipe;			
	__le32 dwTransferID;		
} __packed;


struct wa_notif_xfer {
	struct wa_notif_hdr hdr;
	u8 bEndpoint;
	u8 Reserved;
} __packed;


enum {
	WA_XFER_STATUS_SUCCESS,
	WA_XFER_STATUS_HALTED,
	WA_XFER_STATUS_DATA_BUFFER_ERROR,
	WA_XFER_STATUS_BABBLE,
	WA_XFER_RESERVED,
	WA_XFER_STATUS_NOT_FOUND,
	WA_XFER_STATUS_INSUFFICIENT_RESOURCE,
	WA_XFER_STATUS_TRANSACTION_ERROR,
	WA_XFER_STATUS_ABORTED,
	WA_XFER_STATUS_RPIPE_NOT_READY,
	WA_XFER_INVALID_FORMAT,
	WA_XFER_UNEXPECTED_SEGMENT_NUMBER,
	WA_XFER_STATUS_RPIPE_TYPE_MISMATCH,
};


struct wa_xfer_result {
	struct wa_notif_hdr hdr;
	__le32 dwTransferID;
	__le32 dwTransferLength;
	u8     bTransferSegment;
	u8     bTransferStatus;
	__le32 dwNumOfPackets;
} __packed;


struct usb_wa_descriptor {
	u8	bLength;
	u8	bDescriptorType;
	__le16	bcdWAVersion;
	u8	bNumPorts;		
	u8	bmAttributes;		
	__le16	wNumRPipes;
	__le16	wRPipeMaxBlock;
	u8	bRPipeBlockSize;
	u8	bPwrOn2PwrGood;
	u8	bNumMMCIEs;
	u8	DeviceRemovable;	
} __packed;


struct hwa_dev_info {
	u8	bmDeviceAvailability[32];       
	u8	bDeviceAddress;
	__le16	wPHYRates;
	u8	bmDeviceAttribute;
} __packed;

#endif 
