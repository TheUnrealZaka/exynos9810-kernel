

#ifndef _LINUX_BRCMFMAC_PLATFORM_H
#define _LINUX_BRCMFMAC_PLATFORM_H


#define BRCMFMAC_PDATA_NAME		"brcmfmac"

#define BRCMFMAC_COUNTRY_BUF_SZ		4








enum brcmf_bus_type {
	BRCMF_BUSTYPE_SDIO,
	BRCMF_BUSTYPE_USB,
	BRCMF_BUSTYPE_PCIE
};



struct brcmfmac_sdio_pd {
	int		txglomsz;
	unsigned int	drive_strength;
	bool		oob_irq_supported;
	unsigned int	oob_irq_nr;
	unsigned long	oob_irq_flags;
	bool		broken_sg_support;
	unsigned short	sd_head_align;
	unsigned short	sd_sgentry_align;
	void		(*reset)(void);
};


struct brcmfmac_pd_cc_entry {
	char	iso3166[BRCMFMAC_COUNTRY_BUF_SZ];
	char	cc[BRCMFMAC_COUNTRY_BUF_SZ];
	s32	rev;
};


struct brcmfmac_pd_cc {
	int				table_size;
	struct brcmfmac_pd_cc_entry	table[0];
};


struct brcmfmac_pd_device {
	unsigned int		id;
	unsigned int		rev;
	enum brcmf_bus_type	bus_type;
	unsigned int		feature_disable;
	struct brcmfmac_pd_cc	*country_codes;
	union {
		struct brcmfmac_sdio_pd sdio;
	} bus;
};


struct brcmfmac_platform_data {
	void	(*power_on)(void);
	void	(*power_off)(void);
	char	*fw_alternative_path;
	int	device_count;
	struct brcmfmac_pd_device devices[0];
};


#endif 
