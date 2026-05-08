
#ifndef _I915_PCIIDS_H
#define _I915_PCIIDS_H


#define INTEL_VGA_DEVICE(id, info) {		\
	0x8086,	id,				\
	~0, ~0,					\
	0x030000, 0xff0000,			\
	(unsigned long) info }

#define INTEL_QUANTA_VGA_DEVICE(info) {		\
	0x8086,	0x16a,				\
	0x152d,	0x8990,				\
	0x030000, 0xff0000,			\
	(unsigned long) info }

#define INTEL_I830_IDS(info)				\
	INTEL_VGA_DEVICE(0x3577, info)

#define INTEL_I845G_IDS(info)				\
	INTEL_VGA_DEVICE(0x2562, info)

#define INTEL_I85X_IDS(info)				\
	INTEL_VGA_DEVICE(0x3582, info),  \
	INTEL_VGA_DEVICE(0x358e, info)

#define INTEL_I865G_IDS(info)				\
	INTEL_VGA_DEVICE(0x2572, info) 

#define INTEL_I915G_IDS(info)				\
	INTEL_VGA_DEVICE(0x2582, info),  \
	INTEL_VGA_DEVICE(0x258a, info)  

#define INTEL_I915GM_IDS(info)				\
	INTEL_VGA_DEVICE(0x2592, info) 

#define INTEL_I945G_IDS(info)				\
	INTEL_VGA_DEVICE(0x2772, info) 

#define INTEL_I945GM_IDS(info)				\
	INTEL_VGA_DEVICE(0x27a2, info),  \
	INTEL_VGA_DEVICE(0x27ae, info)  

#define INTEL_I965G_IDS(info)				\
	INTEL_VGA_DEVICE(0x2972, info), 	\
	INTEL_VGA_DEVICE(0x2982, info),		\
	INTEL_VGA_DEVICE(0x2992, info),		\
	INTEL_VGA_DEVICE(0x29a2, info)	

#define INTEL_G33_IDS(info)				\
	INTEL_VGA_DEVICE(0x29b2, info),  \
	INTEL_VGA_DEVICE(0x29c2, info),	 \
	INTEL_VGA_DEVICE(0x29d2, info)	

#define INTEL_I965GM_IDS(info)				\
	INTEL_VGA_DEVICE(0x2a02, info),	 \
	INTEL_VGA_DEVICE(0x2a12, info)  

#define INTEL_GM45_IDS(info)				\
	INTEL_VGA_DEVICE(0x2a42, info) 

#define INTEL_G45_IDS(info)				\
	INTEL_VGA_DEVICE(0x2e02, info),  \
	INTEL_VGA_DEVICE(0x2e12, info),  \
	INTEL_VGA_DEVICE(0x2e22, info),  \
	INTEL_VGA_DEVICE(0x2e32, info),  \
	INTEL_VGA_DEVICE(0x2e42, info),  \
	INTEL_VGA_DEVICE(0x2e92, info)	

#define INTEL_PINEVIEW_IDS(info)			\
	INTEL_VGA_DEVICE(0xa001, info),			\
	INTEL_VGA_DEVICE(0xa011, info)

#define INTEL_IRONLAKE_D_IDS(info) \
	INTEL_VGA_DEVICE(0x0042, info)

#define INTEL_IRONLAKE_M_IDS(info) \
	INTEL_VGA_DEVICE(0x0046, info)

#define INTEL_SNB_D_IDS(info) \
	INTEL_VGA_DEVICE(0x0102, info), \
	INTEL_VGA_DEVICE(0x0112, info), \
	INTEL_VGA_DEVICE(0x0122, info), \
	INTEL_VGA_DEVICE(0x010A, info)

#define INTEL_SNB_M_IDS(info) \
	INTEL_VGA_DEVICE(0x0106, info), \
	INTEL_VGA_DEVICE(0x0116, info), \
	INTEL_VGA_DEVICE(0x0126, info)

#define INTEL_IVB_M_IDS(info) \
	INTEL_VGA_DEVICE(0x0156, info),  \
	INTEL_VGA_DEVICE(0x0166, info)  

#define INTEL_IVB_D_IDS(info) \
	INTEL_VGA_DEVICE(0x0152, info),  \
	INTEL_VGA_DEVICE(0x0162, info),  \
	INTEL_VGA_DEVICE(0x015a, info),  \
	INTEL_VGA_DEVICE(0x016a, info)  

#define INTEL_IVB_Q_IDS(info) \
	INTEL_QUANTA_VGA_DEVICE(info) 

#define INTEL_HSW_IDS(info) \
	INTEL_VGA_DEVICE(0x0402, info),  \
	INTEL_VGA_DEVICE(0x0412, info),  \
	INTEL_VGA_DEVICE(0x0422, info),  \
	INTEL_VGA_DEVICE(0x040a, info),  \
	INTEL_VGA_DEVICE(0x041a, info),  \
	INTEL_VGA_DEVICE(0x042a, info),  \
	INTEL_VGA_DEVICE(0x040B, info),  \
	INTEL_VGA_DEVICE(0x041B, info),  \
	INTEL_VGA_DEVICE(0x042B, info),  \
	INTEL_VGA_DEVICE(0x040E, info),  \
	INTEL_VGA_DEVICE(0x041E, info),  \
	INTEL_VGA_DEVICE(0x042E, info),  \
	INTEL_VGA_DEVICE(0x0C02, info),  \
	INTEL_VGA_DEVICE(0x0C12, info),  \
	INTEL_VGA_DEVICE(0x0C22, info),  \
	INTEL_VGA_DEVICE(0x0C0A, info),  \
	INTEL_VGA_DEVICE(0x0C1A, info),  \
	INTEL_VGA_DEVICE(0x0C2A, info),  \
	INTEL_VGA_DEVICE(0x0C0B, info),  \
	INTEL_VGA_DEVICE(0x0C1B, info),  \
	INTEL_VGA_DEVICE(0x0C2B, info),  \
	INTEL_VGA_DEVICE(0x0C0E, info),  \
	INTEL_VGA_DEVICE(0x0C1E, info),  \
	INTEL_VGA_DEVICE(0x0C2E, info),  \
	INTEL_VGA_DEVICE(0x0A02, info),  \
	INTEL_VGA_DEVICE(0x0A12, info),  \
	INTEL_VGA_DEVICE(0x0A22, info),  \
	INTEL_VGA_DEVICE(0x0A0A, info),  \
	INTEL_VGA_DEVICE(0x0A1A, info),  \
	INTEL_VGA_DEVICE(0x0A2A, info),  \
	INTEL_VGA_DEVICE(0x0A0B, info),  \
	INTEL_VGA_DEVICE(0x0A1B, info),  \
	INTEL_VGA_DEVICE(0x0A2B, info),  \
	INTEL_VGA_DEVICE(0x0D02, info),  \
	INTEL_VGA_DEVICE(0x0D12, info),  \
	INTEL_VGA_DEVICE(0x0D22, info),  \
	INTEL_VGA_DEVICE(0x0D0A, info),  \
	INTEL_VGA_DEVICE(0x0D1A, info),  \
	INTEL_VGA_DEVICE(0x0D2A, info),  \
	INTEL_VGA_DEVICE(0x0D0B, info),  \
	INTEL_VGA_DEVICE(0x0D1B, info),  \
	INTEL_VGA_DEVICE(0x0D2B, info),  \
	INTEL_VGA_DEVICE(0x0D0E, info),  \
	INTEL_VGA_DEVICE(0x0D1E, info),  \
	INTEL_VGA_DEVICE(0x0D2E, info),   \
	INTEL_VGA_DEVICE(0x0406, info),  \
	INTEL_VGA_DEVICE(0x0416, info),  \
	INTEL_VGA_DEVICE(0x0426, info),  \
	INTEL_VGA_DEVICE(0x0C06, info),  \
	INTEL_VGA_DEVICE(0x0C16, info),  \
	INTEL_VGA_DEVICE(0x0C26, info),  \
	INTEL_VGA_DEVICE(0x0A06, info),  \
	INTEL_VGA_DEVICE(0x0A16, info),  \
	INTEL_VGA_DEVICE(0x0A26, info),  \
	INTEL_VGA_DEVICE(0x0A0E, info),  \
	INTEL_VGA_DEVICE(0x0A1E, info),  \
	INTEL_VGA_DEVICE(0x0A2E, info),  \
	INTEL_VGA_DEVICE(0x0D06, info),  \
	INTEL_VGA_DEVICE(0x0D16, info),  \
	INTEL_VGA_DEVICE(0x0D26, info)  

#define INTEL_VLV_IDS(info) \
	INTEL_VGA_DEVICE(0x0f30, info), \
	INTEL_VGA_DEVICE(0x0f31, info), \
	INTEL_VGA_DEVICE(0x0f32, info), \
	INTEL_VGA_DEVICE(0x0f33, info), \
	INTEL_VGA_DEVICE(0x0157, info), \
	INTEL_VGA_DEVICE(0x0155, info)

#define INTEL_BDW_GT12_IDS(info)  \
	INTEL_VGA_DEVICE(0x1602, info),  \
	INTEL_VGA_DEVICE(0x1606, info),  \
	INTEL_VGA_DEVICE(0x160B, info),  \
	INTEL_VGA_DEVICE(0x160E, info),  \
	INTEL_VGA_DEVICE(0x1612, info),  \
	INTEL_VGA_DEVICE(0x1616, info),  \
	INTEL_VGA_DEVICE(0x161B, info),  \
	INTEL_VGA_DEVICE(0x161E, info),   \
	INTEL_VGA_DEVICE(0x160A, info),  \
	INTEL_VGA_DEVICE(0x160D, info),  \
	INTEL_VGA_DEVICE(0x161A, info),  \
	INTEL_VGA_DEVICE(0x161D, info)  

#define INTEL_BDW_GT3_IDS(info) \
	INTEL_VGA_DEVICE(0x1622, info),  \
	INTEL_VGA_DEVICE(0x1626, info),  \
	INTEL_VGA_DEVICE(0x162B, info),  \
	INTEL_VGA_DEVICE(0x162E, info),  \
	INTEL_VGA_DEVICE(0x162A, info),  \
	INTEL_VGA_DEVICE(0x162D, info)  

#define INTEL_BDW_RSVD_IDS(info) \
	INTEL_VGA_DEVICE(0x1632, info),  \
	INTEL_VGA_DEVICE(0x1636, info),  \
	INTEL_VGA_DEVICE(0x163B, info),  \
	INTEL_VGA_DEVICE(0x163E, info),  \
	INTEL_VGA_DEVICE(0x163A, info),  \
	INTEL_VGA_DEVICE(0x163D, info)  

#define INTEL_BDW_IDS(info) \
	INTEL_BDW_GT12_IDS(info), \
	INTEL_BDW_GT3_IDS(info), \
	INTEL_BDW_RSVD_IDS(info)

#define INTEL_CHV_IDS(info) \
	INTEL_VGA_DEVICE(0x22b0, info), \
	INTEL_VGA_DEVICE(0x22b1, info), \
	INTEL_VGA_DEVICE(0x22b2, info), \
	INTEL_VGA_DEVICE(0x22b3, info)

#define INTEL_SKL_GT1_IDS(info)	\
	INTEL_VGA_DEVICE(0x1906, info),  \
	INTEL_VGA_DEVICE(0x190E, info),  \
	INTEL_VGA_DEVICE(0x1902, info),  \
	INTEL_VGA_DEVICE(0x190B, info),  \
	INTEL_VGA_DEVICE(0x190A, info) 

#define INTEL_SKL_GT2_IDS(info)	\
	INTEL_VGA_DEVICE(0x1916, info),  \
	INTEL_VGA_DEVICE(0x1921, info),  \
	INTEL_VGA_DEVICE(0x191E, info),  \
	INTEL_VGA_DEVICE(0x1912, info),  \
	INTEL_VGA_DEVICE(0x191B, info),  \
	INTEL_VGA_DEVICE(0x191A, info),  \
	INTEL_VGA_DEVICE(0x191D, info)  

#define INTEL_SKL_GT3_IDS(info) \
	INTEL_VGA_DEVICE(0x1923, info),  \
	INTEL_VGA_DEVICE(0x1926, info),  \
	INTEL_VGA_DEVICE(0x1927, info),  \
	INTEL_VGA_DEVICE(0x192B, info),  \
	INTEL_VGA_DEVICE(0x192A, info)  

#define INTEL_SKL_GT4_IDS(info) \
	INTEL_VGA_DEVICE(0x1932, info),  \
	INTEL_VGA_DEVICE(0x193B, info),  \
	INTEL_VGA_DEVICE(0x193D, info),  \
	INTEL_VGA_DEVICE(0x193A, info)  

#define INTEL_SKL_IDS(info)	 \
	INTEL_SKL_GT1_IDS(info), \
	INTEL_SKL_GT2_IDS(info), \
	INTEL_SKL_GT3_IDS(info), \
	INTEL_SKL_GT4_IDS(info)

#define INTEL_BXT_IDS(info) \
	INTEL_VGA_DEVICE(0x0A84, info), \
	INTEL_VGA_DEVICE(0x1A84, info), \
	INTEL_VGA_DEVICE(0x1A85, info), \
	INTEL_VGA_DEVICE(0x5A84, info),  \
	INTEL_VGA_DEVICE(0x5A85, info)  

#define INTEL_KBL_GT1_IDS(info)	\
	INTEL_VGA_DEVICE(0x5913, info),  \
	INTEL_VGA_DEVICE(0x5915, info),  \
	INTEL_VGA_DEVICE(0x5917, info),  \
	INTEL_VGA_DEVICE(0x5906, info),  \
	INTEL_VGA_DEVICE(0x590E, info),  \
	INTEL_VGA_DEVICE(0x5902, info),  \
	INTEL_VGA_DEVICE(0x5908, info),  \
	INTEL_VGA_DEVICE(0x590B, info),  \
	INTEL_VGA_DEVICE(0x590A, info) 

#define INTEL_KBL_GT2_IDS(info)	\
	INTEL_VGA_DEVICE(0x5916, info),  \
	INTEL_VGA_DEVICE(0x5921, info),  \
	INTEL_VGA_DEVICE(0x591E, info),  \
	INTEL_VGA_DEVICE(0x5912, info),  \
	INTEL_VGA_DEVICE(0x591B, info),  \
	INTEL_VGA_DEVICE(0x591A, info),  \
	INTEL_VGA_DEVICE(0x591D, info) 

#define INTEL_KBL_GT3_IDS(info) \
	INTEL_VGA_DEVICE(0x5923, info),  \
	INTEL_VGA_DEVICE(0x5926, info),  \
	INTEL_VGA_DEVICE(0x5927, info) 

#define INTEL_KBL_GT4_IDS(info) \
	INTEL_VGA_DEVICE(0x593B, info) 

#define INTEL_KBL_IDS(info) \
	INTEL_KBL_GT1_IDS(info), \
	INTEL_KBL_GT2_IDS(info), \
	INTEL_KBL_GT3_IDS(info), \
	INTEL_KBL_GT4_IDS(info)

#endif 
