



#ifndef __ACTBL2_H__
#define __ACTBL2_H__




#define ACPI_SIG_ASF            "ASF!"	
#define ACPI_SIG_BOOT           "BOOT"	
#define ACPI_SIG_CSRT           "CSRT"	
#define ACPI_SIG_DBG2           "DBG2"	
#define ACPI_SIG_DBGP           "DBGP"	
#define ACPI_SIG_DMAR           "DMAR"	
#define ACPI_SIG_HPET           "HPET"	
#define ACPI_SIG_IBFT           "IBFT"	
#define ACPI_SIG_IORT           "IORT"	
#define ACPI_SIG_IVRS           "IVRS"	
#define ACPI_SIG_LPIT           "LPIT"	
#define ACPI_SIG_MCFG           "MCFG"	
#define ACPI_SIG_MCHI           "MCHI"	
#define ACPI_SIG_MSDM           "MSDM"	
#define ACPI_SIG_MTMR           "MTMR"	
#define ACPI_SIG_SLIC           "SLIC"	
#define ACPI_SIG_SPCR           "SPCR"	
#define ACPI_SIG_SPMI           "SPMI"	
#define ACPI_SIG_TCPA           "TCPA"	
#define ACPI_SIG_TPM2           "TPM2"	
#define ACPI_SIG_UEFI           "UEFI"	
#define ACPI_SIG_VRTC           "VRTC"	
#define ACPI_SIG_WAET           "WAET"	
#define ACPI_SIG_WDAT           "WDAT"	
#define ACPI_SIG_WDDT           "WDDT"	
#define ACPI_SIG_WDRT           "WDRT"	

#ifdef ACPI_UNDEFINED_TABLES

#define ACPI_SIG_ATKG           "ATKG"
#define ACPI_SIG_GSCI           "GSCI"	
#define ACPI_SIG_IEIT           "IEIT"
#endif


#pragma pack(1)





struct acpi_table_asf {
	struct acpi_table_header header;	
};



struct acpi_asf_header {
	u8 type;
	u8 reserved;
	u16 length;
};



enum acpi_asf_type {
	ACPI_ASF_TYPE_INFO = 0,
	ACPI_ASF_TYPE_ALERT = 1,
	ACPI_ASF_TYPE_CONTROL = 2,
	ACPI_ASF_TYPE_BOOT = 3,
	ACPI_ASF_TYPE_ADDRESS = 4,
	ACPI_ASF_TYPE_RESERVED = 5
};





struct acpi_asf_info {
	struct acpi_asf_header header;
	u8 min_reset_value;
	u8 min_poll_interval;
	u16 system_id;
	u32 mfg_id;
	u8 flags;
	u8 reserved2[3];
};



#define ACPI_ASF_SMBUS_PROTOCOLS    (1)



struct acpi_asf_alert {
	struct acpi_asf_header header;
	u8 assert_mask;
	u8 deassert_mask;
	u8 alerts;
	u8 data_length;
};

struct acpi_asf_alert_data {
	u8 address;
	u8 command;
	u8 mask;
	u8 value;
	u8 sensor_type;
	u8 type;
	u8 offset;
	u8 source_type;
	u8 severity;
	u8 sensor_number;
	u8 entity;
	u8 instance;
};



struct acpi_asf_remote {
	struct acpi_asf_header header;
	u8 controls;
	u8 data_length;
	u16 reserved2;
};

struct acpi_asf_control_data {
	u8 function;
	u8 address;
	u8 command;
	u8 value;
};



struct acpi_asf_rmcp {
	struct acpi_asf_header header;
	u8 capabilities[7];
	u8 completion_code;
	u32 enterprise_id;
	u8 command;
	u16 parameter;
	u16 boot_options;
	u16 oem_parameters;
};



struct acpi_asf_address {
	struct acpi_asf_header header;
	u8 eprom_address;
	u8 devices;
};



struct acpi_table_boot {
	struct acpi_table_header header;	
	u8 cmos_index;		
	u8 reserved[3];
};



struct acpi_table_csrt {
	struct acpi_table_header header;	
};



struct acpi_csrt_group {
	u32 length;
	u32 vendor_id;
	u32 subvendor_id;
	u16 device_id;
	u16 subdevice_id;
	u16 revision;
	u16 reserved;
	u32 shared_info_length;

	
};



struct acpi_csrt_shared_info {
	u16 major_version;
	u16 minor_version;
	u32 mmio_base_low;
	u32 mmio_base_high;
	u32 gsi_interrupt;
	u8 interrupt_polarity;
	u8 interrupt_mode;
	u8 num_channels;
	u8 dma_address_width;
	u16 base_request_line;
	u16 num_handshake_signals;
	u32 max_block_size;

	
};



struct acpi_csrt_descriptor {
	u32 length;
	u16 type;
	u16 subtype;
	u32 uid;

	
};



#define ACPI_CSRT_TYPE_INTERRUPT    0x0001
#define ACPI_CSRT_TYPE_TIMER        0x0002
#define ACPI_CSRT_TYPE_DMA          0x0003



#define ACPI_CSRT_XRUPT_LINE        0x0000
#define ACPI_CSRT_XRUPT_CONTROLLER  0x0001
#define ACPI_CSRT_TIMER             0x0000
#define ACPI_CSRT_DMA_CHANNEL       0x0000
#define ACPI_CSRT_DMA_CONTROLLER    0x0001



struct acpi_table_dbg2 {
	struct acpi_table_header header;	
	u32 info_offset;
	u32 info_count;
};

struct acpi_dbg2_header {
	u32 info_offset;
	u32 info_count;
};



struct acpi_dbg2_device {
	u8 revision;
	u16 length;
	u8 register_count;	
	u16 namepath_length;
	u16 namepath_offset;
	u16 oem_data_length;
	u16 oem_data_offset;
	u16 port_type;
	u16 port_subtype;
	u16 reserved;
	u16 base_address_offset;
	u16 address_size_offset;
	
};



#define ACPI_DBG2_SERIAL_PORT       0x8000
#define ACPI_DBG2_1394_PORT         0x8001
#define ACPI_DBG2_USB_PORT          0x8002
#define ACPI_DBG2_NET_PORT          0x8003



#define ACPI_DBG2_16550_COMPATIBLE  0x0000
#define ACPI_DBG2_16550_SUBSET      0x0001
#define ACPI_DBG2_ARM_PL011         0x0003
#define ACPI_DBG2_ARM_SBSA_32BIT    0x000D
#define ACPI_DBG2_ARM_SBSA_GENERIC  0x000E
#define ACPI_DBG2_ARM_DCC           0x000F
#define ACPI_DBG2_BCM2835           0x0010

#define ACPI_DBG2_1394_STANDARD     0x0000

#define ACPI_DBG2_USB_XHCI          0x0000
#define ACPI_DBG2_USB_EHCI          0x0001



struct acpi_table_dbgp {
	struct acpi_table_header header;	
	u8 type;		
	u8 reserved[3];
	struct acpi_generic_address debug_port;
};



struct acpi_table_dmar {
	struct acpi_table_header header;	
	u8 width;		
	u8 flags;
	u8 reserved[10];
};



#define ACPI_DMAR_INTR_REMAP        (1)
#define ACPI_DMAR_X2APIC_OPT_OUT    (1<<1)
#define ACPI_DMAR_X2APIC_MODE       (1<<2)



struct acpi_dmar_header {
	u16 type;
	u16 length;
};



enum acpi_dmar_type {
	ACPI_DMAR_TYPE_HARDWARE_UNIT = 0,
	ACPI_DMAR_TYPE_RESERVED_MEMORY = 1,
	ACPI_DMAR_TYPE_ROOT_ATS = 2,
	ACPI_DMAR_TYPE_HARDWARE_AFFINITY = 3,
	ACPI_DMAR_TYPE_NAMESPACE = 4,
	ACPI_DMAR_TYPE_RESERVED = 5	
};



struct acpi_dmar_device_scope {
	u8 entry_type;
	u8 length;
	u16 reserved;
	u8 enumeration_id;
	u8 bus;
};



enum acpi_dmar_scope_type {
	ACPI_DMAR_SCOPE_TYPE_NOT_USED = 0,
	ACPI_DMAR_SCOPE_TYPE_ENDPOINT = 1,
	ACPI_DMAR_SCOPE_TYPE_BRIDGE = 2,
	ACPI_DMAR_SCOPE_TYPE_IOAPIC = 3,
	ACPI_DMAR_SCOPE_TYPE_HPET = 4,
	ACPI_DMAR_SCOPE_TYPE_NAMESPACE = 5,
	ACPI_DMAR_SCOPE_TYPE_RESERVED = 6	
};

struct acpi_dmar_pci_path {
	u8 device;
	u8 function;
};





struct acpi_dmar_hardware_unit {
	struct acpi_dmar_header header;
	u8 flags;
	u8 reserved;
	u16 segment;
	u64 address;		
};



#define ACPI_DMAR_INCLUDE_ALL       (1)



struct acpi_dmar_reserved_memory {
	struct acpi_dmar_header header;
	u16 reserved;
	u16 segment;
	u64 base_address;	
	u64 end_address;	
};



#define ACPI_DMAR_ALLOW_ALL         (1)



struct acpi_dmar_atsr {
	struct acpi_dmar_header header;
	u8 flags;
	u8 reserved;
	u16 segment;
};



#define ACPI_DMAR_ALL_PORTS         (1)



struct acpi_dmar_rhsa {
	struct acpi_dmar_header header;
	u32 reserved;
	u64 base_address;
	u32 proximity_domain;
};



struct acpi_dmar_andd {
	struct acpi_dmar_header header;
	u8 reserved[3];
	u8 device_number;
	char device_name[1];
};



struct acpi_table_hpet {
	struct acpi_table_header header;	
	u32 id;			
	struct acpi_generic_address address;	
	u8 sequence;		
	u16 minimum_tick;	
	u8 flags;
};



#define ACPI_HPET_PAGE_PROTECT_MASK (3)



enum acpi_hpet_page_protect {
	ACPI_HPET_NO_PAGE_PROTECT = 0,
	ACPI_HPET_PAGE_PROTECT4 = 1,
	ACPI_HPET_PAGE_PROTECT64 = 2
};



struct acpi_table_ibft {
	struct acpi_table_header header;	
	u8 reserved[12];
};



struct acpi_ibft_header {
	u8 type;
	u8 version;
	u16 length;
	u8 index;
	u8 flags;
};



enum acpi_ibft_type {
	ACPI_IBFT_TYPE_NOT_USED = 0,
	ACPI_IBFT_TYPE_CONTROL = 1,
	ACPI_IBFT_TYPE_INITIATOR = 2,
	ACPI_IBFT_TYPE_NIC = 3,
	ACPI_IBFT_TYPE_TARGET = 4,
	ACPI_IBFT_TYPE_EXTENSIONS = 5,
	ACPI_IBFT_TYPE_RESERVED = 6	
};



struct acpi_ibft_control {
	struct acpi_ibft_header header;
	u16 extensions;
	u16 initiator_offset;
	u16 nic0_offset;
	u16 target0_offset;
	u16 nic1_offset;
	u16 target1_offset;
};

struct acpi_ibft_initiator {
	struct acpi_ibft_header header;
	u8 sns_server[16];
	u8 slp_server[16];
	u8 primary_server[16];
	u8 secondary_server[16];
	u16 name_length;
	u16 name_offset;
};

struct acpi_ibft_nic {
	struct acpi_ibft_header header;
	u8 ip_address[16];
	u8 subnet_mask_prefix;
	u8 origin;
	u8 gateway[16];
	u8 primary_dns[16];
	u8 secondary_dns[16];
	u8 dhcp[16];
	u16 vlan;
	u8 mac_address[6];
	u16 pci_address;
	u16 name_length;
	u16 name_offset;
};

struct acpi_ibft_target {
	struct acpi_ibft_header header;
	u8 target_ip_address[16];
	u16 target_ip_socket;
	u8 target_boot_lun[8];
	u8 chap_type;
	u8 nic_association;
	u16 target_name_length;
	u16 target_name_offset;
	u16 chap_name_length;
	u16 chap_name_offset;
	u16 chap_secret_length;
	u16 chap_secret_offset;
	u16 reverse_chap_name_length;
	u16 reverse_chap_name_offset;
	u16 reverse_chap_secret_length;
	u16 reverse_chap_secret_offset;
};



struct acpi_table_iort {
	struct acpi_table_header header;
	u32 node_count;
	u32 node_offset;
	u32 reserved;
};


struct acpi_iort_node {
	u8 type;
	u16 length;
	u8 revision;
	u32 reserved;
	u32 mapping_count;
	u32 mapping_offset;
	char node_data[1];
};



enum acpi_iort_node_type {
	ACPI_IORT_NODE_ITS_GROUP = 0x00,
	ACPI_IORT_NODE_NAMED_COMPONENT = 0x01,
	ACPI_IORT_NODE_PCI_ROOT_COMPLEX = 0x02,
	ACPI_IORT_NODE_SMMU = 0x03,
	ACPI_IORT_NODE_SMMU_V3 = 0x04
};

struct acpi_iort_id_mapping {
	u32 input_base;		
	u32 id_count;		
	u32 output_base;	
	u32 output_reference;	
	u32 flags;
};



#define ACPI_IORT_ID_SINGLE_MAPPING (1)

struct acpi_iort_memory_access {
	u32 cache_coherency;
	u8 hints;
	u16 reserved;
	u8 memory_flags;
};



#define ACPI_IORT_NODE_COHERENT         0x00000001	
#define ACPI_IORT_NODE_NOT_COHERENT     0x00000000	



#define ACPI_IORT_HT_TRANSIENT          (1)
#define ACPI_IORT_HT_WRITE              (1<<1)
#define ACPI_IORT_HT_READ               (1<<2)
#define ACPI_IORT_HT_OVERRIDE           (1<<3)



#define ACPI_IORT_MF_COHERENCY          (1)
#define ACPI_IORT_MF_ATTRIBUTES         (1<<1)


struct acpi_iort_its_group {
	u32 its_count;
	u32 identifiers[1];	
};

struct acpi_iort_named_component {
	u32 node_flags;
	u64 memory_properties;	
	u8 memory_address_limit;	
	char device_name[1];	
};

struct acpi_iort_root_complex {
	u64 memory_properties;	
	u32 ats_attribute;
	u32 pci_segment_number;
};



#define ACPI_IORT_ATS_SUPPORTED         0x00000001	
#define ACPI_IORT_ATS_UNSUPPORTED       0x00000000	

struct acpi_iort_smmu {
	u64 base_address;	
	u64 span;		
	u32 model;
	u32 flags;
	u32 global_interrupt_offset;
	u32 context_interrupt_count;
	u32 context_interrupt_offset;
	u32 pmu_interrupt_count;
	u32 pmu_interrupt_offset;
	u64 interrupts[1];	
};



#define ACPI_IORT_SMMU_V1               0x00000000	
#define ACPI_IORT_SMMU_V2               0x00000001	
#define ACPI_IORT_SMMU_CORELINK_MMU400  0x00000002	
#define ACPI_IORT_SMMU_CORELINK_MMU500  0x00000003	



#define ACPI_IORT_SMMU_DVM_SUPPORTED    (1)
#define ACPI_IORT_SMMU_COHERENT_WALK    (1<<1)



struct acpi_iort_smmu_gsi {
	u32 nsg_irpt;
	u32 nsg_irpt_flags;
	u32 nsg_cfg_irpt;
	u32 nsg_cfg_irpt_flags;
};

struct acpi_iort_smmu_v3 {
	u64 base_address;	
	u32 flags;
	u32 reserved;
	u64 vatos_address;
	u32 model;		
	u32 event_gsiv;
	u32 pri_gsiv;
	u32 gerr_gsiv;
	u32 sync_gsiv;
};



#define ACPI_IORT_SMMU_V3_COHACC_OVERRIDE   (1)
#define ACPI_IORT_SMMU_V3_HTTU_OVERRIDE     (1<<1)



struct acpi_table_ivrs {
	struct acpi_table_header header;	
	u32 info;		
	u64 reserved;
};



#define ACPI_IVRS_PHYSICAL_SIZE     0x00007F00	
#define ACPI_IVRS_VIRTUAL_SIZE      0x003F8000	
#define ACPI_IVRS_ATS_RESERVED      0x00400000	



struct acpi_ivrs_header {
	u8 type;		
	u8 flags;
	u16 length;		
	u16 device_id;		
};



enum acpi_ivrs_type {
	ACPI_IVRS_TYPE_HARDWARE = 0x10,
	ACPI_IVRS_TYPE_MEMORY1 = 0x20,
	ACPI_IVRS_TYPE_MEMORY2 = 0x21,
	ACPI_IVRS_TYPE_MEMORY3 = 0x22
};



#define ACPI_IVHD_TT_ENABLE         (1)
#define ACPI_IVHD_PASS_PW           (1<<1)
#define ACPI_IVHD_RES_PASS_PW       (1<<2)
#define ACPI_IVHD_ISOC              (1<<3)
#define ACPI_IVHD_IOTLB             (1<<4)



#define ACPI_IVMD_UNITY             (1)
#define ACPI_IVMD_READ              (1<<1)
#define ACPI_IVMD_WRITE             (1<<2)
#define ACPI_IVMD_EXCLUSION_RANGE   (1<<3)





struct acpi_ivrs_hardware {
	struct acpi_ivrs_header header;
	u16 capability_offset;	
	u64 base_address;	
	u16 pci_segment_group;
	u16 info;		
	u32 reserved;
};



#define ACPI_IVHD_MSI_NUMBER_MASK   0x001F	
#define ACPI_IVHD_UNIT_ID_MASK      0x1F00	


struct acpi_ivrs_de_header {
	u8 type;
	u16 id;
	u8 data_setting;
};



#define ACPI_IVHD_ENTRY_LENGTH      0xC0



enum acpi_ivrs_device_entry_type {
	

	ACPI_IVRS_TYPE_PAD4 = 0,
	ACPI_IVRS_TYPE_ALL = 1,
	ACPI_IVRS_TYPE_SELECT = 2,
	ACPI_IVRS_TYPE_START = 3,
	ACPI_IVRS_TYPE_END = 4,

	

	ACPI_IVRS_TYPE_PAD8 = 64,
	ACPI_IVRS_TYPE_NOT_USED = 65,
	ACPI_IVRS_TYPE_ALIAS_SELECT = 66,	
	ACPI_IVRS_TYPE_ALIAS_START = 67,	
	ACPI_IVRS_TYPE_EXT_SELECT = 70,	
	ACPI_IVRS_TYPE_EXT_START = 71,	
	ACPI_IVRS_TYPE_SPECIAL = 72	
};



#define ACPI_IVHD_INIT_PASS         (1)
#define ACPI_IVHD_EINT_PASS         (1<<1)
#define ACPI_IVHD_NMI_PASS          (1<<2)
#define ACPI_IVHD_SYSTEM_MGMT       (3<<4)
#define ACPI_IVHD_LINT0_PASS        (1<<6)
#define ACPI_IVHD_LINT1_PASS        (1<<7)



struct acpi_ivrs_device4 {
	struct acpi_ivrs_de_header header;
};



struct acpi_ivrs_device8a {
	struct acpi_ivrs_de_header header;
	u8 reserved1;
	u16 used_id;
	u8 reserved2;
};



struct acpi_ivrs_device8b {
	struct acpi_ivrs_de_header header;
	u32 extended_data;
};



#define ACPI_IVHD_ATS_DISABLED      (1<<31)



struct acpi_ivrs_device8c {
	struct acpi_ivrs_de_header header;
	u8 handle;
	u16 used_id;
	u8 variety;
};



#define ACPI_IVHD_IOAPIC            1
#define ACPI_IVHD_HPET              2



struct acpi_ivrs_memory {
	struct acpi_ivrs_header header;
	u16 aux_data;
	u64 reserved;
	u64 start_address;
	u64 memory_length;
};



struct acpi_table_lpit {
	struct acpi_table_header header;	
};



struct acpi_lpit_header {
	u32 type;		
	u32 length;		
	u16 unique_id;
	u16 reserved;
	u32 flags;
};



enum acpi_lpit_type {
	ACPI_LPIT_TYPE_NATIVE_CSTATE = 0x00,
	ACPI_LPIT_TYPE_RESERVED = 0x01	
};



#define ACPI_LPIT_STATE_DISABLED    (1)
#define ACPI_LPIT_NO_COUNTER        (1<<1)





struct acpi_lpit_native {
	struct acpi_lpit_header header;
	struct acpi_generic_address entry_trigger;
	u32 residency;
	u32 latency;
	struct acpi_generic_address residency_counter;
	u64 counter_frequency;
};



struct acpi_table_mcfg {
	struct acpi_table_header header;	
	u8 reserved[8];
};



struct acpi_mcfg_allocation {
	u64 address;		
	u16 pci_segment;	
	u8 start_bus_number;	
	u8 end_bus_number;	
	u32 reserved;
};



struct acpi_table_mchi {
	struct acpi_table_header header;	
	u8 interface_type;
	u8 protocol;
	u64 protocol_data;
	u8 interrupt_type;
	u8 gpe;
	u8 pci_device_flag;
	u32 global_interrupt;
	struct acpi_generic_address control_register;
	u8 pci_segment;
	u8 pci_bus;
	u8 pci_device;
	u8 pci_function;
};





struct acpi_table_msdm {
	struct acpi_table_header header;	
};



struct acpi_table_mtmr {
	struct acpi_table_header header;	
};



struct acpi_mtmr_entry {
	struct acpi_generic_address physical_address;
	u32 frequency;
	u32 irq;
};





struct acpi_table_slic {
	struct acpi_table_header header;	
};



struct acpi_table_spcr {
	struct acpi_table_header header;	
	u8 interface_type;	
	u8 reserved[3];
	struct acpi_generic_address serial_port;
	u8 interrupt_type;
	u8 pc_interrupt;
	u32 interrupt;
	u8 baud_rate;
	u8 parity;
	u8 stop_bits;
	u8 flow_control;
	u8 terminal_type;
	u8 reserved1;
	u16 pci_device_id;
	u16 pci_vendor_id;
	u8 pci_bus;
	u8 pci_device;
	u8 pci_function;
	u32 pci_flags;
	u8 pci_segment;
	u32 reserved2;
};



#define ACPI_SPCR_DO_NOT_DISABLE    (1)





struct acpi_table_spmi {
	struct acpi_table_header header;	
	u8 interface_type;
	u8 reserved;		
	u16 spec_revision;	
	u8 interrupt_type;
	u8 gpe_number;		
	u8 reserved1;
	u8 pci_device_flag;
	u32 interrupt;
	struct acpi_generic_address ipmi_register;
	u8 pci_segment;
	u8 pci_bus;
	u8 pci_device;
	u8 pci_function;
	u8 reserved2;
};



enum acpi_spmi_interface_types {
	ACPI_SPMI_NOT_USED = 0,
	ACPI_SPMI_KEYBOARD = 1,
	ACPI_SPMI_SMI = 2,
	ACPI_SPMI_BLOCK_TRANSFER = 3,
	ACPI_SPMI_SMBUS = 4,
	ACPI_SPMI_RESERVED = 5	
};



struct acpi_table_tcpa_hdr {
	struct acpi_table_header header;	
	u16 platform_class;
};


#define ACPI_TCPA_CLIENT_TABLE          0
#define ACPI_TCPA_SERVER_TABLE          1

struct acpi_table_tcpa_client {
	u32 minimum_log_length;	
	u64 log_address;	
};

struct acpi_table_tcpa_server {
	u16 reserved;
	u64 minimum_log_length;	
	u64 log_address;	
	u16 spec_revision;
	u8 device_flags;
	u8 interrupt_flags;
	u8 gpe_number;
	u8 reserved2[3];
	u32 global_interrupt;
	struct acpi_generic_address address;
	u32 reserved3;
	struct acpi_generic_address config_address;
	u8 group;
	u8 bus;			
	u8 device;
	u8 function;
};



#define ACPI_TCPA_PCI_DEVICE            (1)
#define ACPI_TCPA_BUS_PNP               (1<<1)
#define ACPI_TCPA_ADDRESS_VALID         (1<<2)



#define ACPI_TCPA_INTERRUPT_MODE        (1)
#define ACPI_TCPA_INTERRUPT_POLARITY    (1<<1)
#define ACPI_TCPA_SCI_VIA_GPE           (1<<2)
#define ACPI_TCPA_GLOBAL_INTERRUPT      (1<<3)



struct acpi_table_tpm2 {
	struct acpi_table_header header;	
	u16 platform_class;
	u16 reserved;
	u64 control_address;
	u32 start_method;

	
};



#define ACPI_TPM2_NOT_ALLOWED                       0
#define ACPI_TPM2_START_METHOD                      2
#define ACPI_TPM2_MEMORY_MAPPED                     6
#define ACPI_TPM2_COMMAND_BUFFER                    7
#define ACPI_TPM2_COMMAND_BUFFER_WITH_START_METHOD  8



struct acpi_table_uefi {
	struct acpi_table_header header;	
	u8 identifier[16];	
	u16 data_offset;	
};



struct acpi_table_vrtc {
	struct acpi_table_header header;	
};



struct acpi_vrtc_entry {
	struct acpi_generic_address physical_address;
	u32 irq;
};



struct acpi_table_waet {
	struct acpi_table_header header;	
	u32 flags;
};



#define ACPI_WAET_RTC_NO_ACK        (1)	
#define ACPI_WAET_TIMER_ONE_READ    (1<<1)	



struct acpi_table_wdat {
	struct acpi_table_header header;	
	u32 header_length;	
	u16 pci_segment;	
	u8 pci_bus;		
	u8 pci_device;		
	u8 pci_function;	
	u8 reserved[3];
	u32 timer_period;	
	u32 max_count;		
	u32 min_count;		
	u8 flags;
	u8 reserved2[3];
	u32 entries;		
};



#define ACPI_WDAT_ENABLED           (1)
#define ACPI_WDAT_STOPPED           0x80



struct acpi_wdat_entry {
	u8 action;
	u8 instruction;
	u16 reserved;
	struct acpi_generic_address register_region;
	u32 value;		
	u32 mask;		
};



enum acpi_wdat_actions {
	ACPI_WDAT_RESET = 1,
	ACPI_WDAT_GET_CURRENT_COUNTDOWN = 4,
	ACPI_WDAT_GET_COUNTDOWN = 5,
	ACPI_WDAT_SET_COUNTDOWN = 6,
	ACPI_WDAT_GET_RUNNING_STATE = 8,
	ACPI_WDAT_SET_RUNNING_STATE = 9,
	ACPI_WDAT_GET_STOPPED_STATE = 10,
	ACPI_WDAT_SET_STOPPED_STATE = 11,
	ACPI_WDAT_GET_REBOOT = 16,
	ACPI_WDAT_SET_REBOOT = 17,
	ACPI_WDAT_GET_SHUTDOWN = 18,
	ACPI_WDAT_SET_SHUTDOWN = 19,
	ACPI_WDAT_GET_STATUS = 32,
	ACPI_WDAT_SET_STATUS = 33,
	ACPI_WDAT_ACTION_RESERVED = 34	
};



enum acpi_wdat_instructions {
	ACPI_WDAT_READ_VALUE = 0,
	ACPI_WDAT_READ_COUNTDOWN = 1,
	ACPI_WDAT_WRITE_VALUE = 2,
	ACPI_WDAT_WRITE_COUNTDOWN = 3,
	ACPI_WDAT_INSTRUCTION_RESERVED = 4,	
	ACPI_WDAT_PRESERVE_REGISTER = 0x80	
};



struct acpi_table_wddt {
	struct acpi_table_header header;	
	u16 spec_version;
	u16 table_version;
	u16 pci_vendor_id;
	struct acpi_generic_address address;
	u16 max_count;		
	u16 min_count;		
	u16 period;
	u16 status;
	u16 capability;
};



#define ACPI_WDDT_AVAILABLE     (1)
#define ACPI_WDDT_ACTIVE        (1<<1)
#define ACPI_WDDT_TCO_OS_OWNED  (1<<2)
#define ACPI_WDDT_USER_RESET    (1<<11)
#define ACPI_WDDT_WDT_RESET     (1<<12)
#define ACPI_WDDT_POWER_FAIL    (1<<13)
#define ACPI_WDDT_UNKNOWN_RESET (1<<14)



#define ACPI_WDDT_AUTO_RESET    (1)
#define ACPI_WDDT_ALERT_SUPPORT (1<<1)



struct acpi_table_wdrt {
	struct acpi_table_header header;	
	struct acpi_generic_address control_register;
	struct acpi_generic_address count_register;
	u16 pci_device_id;
	u16 pci_vendor_id;
	u8 pci_bus;		
	u8 pci_device;		
	u8 pci_function;	
	u8 pci_segment;		
	u16 max_count;		
	u8 units;
};



#pragma pack()

#endif				
