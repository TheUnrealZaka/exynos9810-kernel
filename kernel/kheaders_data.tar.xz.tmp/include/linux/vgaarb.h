

#ifndef LINUX_VGA_H
#define LINUX_VGA_H

#include <video/vga.h>


#define VGA_RSRC_NONE	       0x00
#define VGA_RSRC_LEGACY_IO     0x01
#define VGA_RSRC_LEGACY_MEM    0x02
#define VGA_RSRC_LEGACY_MASK   (VGA_RSRC_LEGACY_IO | VGA_RSRC_LEGACY_MEM)

#define VGA_RSRC_NORMAL_IO     0x04
#define VGA_RSRC_NORMAL_MEM    0x08


#define VGA_DEFAULT_DEVICE     (NULL)

struct pci_dev;




#if defined(CONFIG_VGA_ARB)
extern void vga_set_legacy_decoding(struct pci_dev *pdev,
				    unsigned int decodes);
#else
static inline void vga_set_legacy_decoding(struct pci_dev *pdev,
					   unsigned int decodes) { };
#endif

#if defined(CONFIG_VGA_ARB)
extern int vga_get(struct pci_dev *pdev, unsigned int rsrc, int interruptible);
#else
static inline int vga_get(struct pci_dev *pdev, unsigned int rsrc, int interruptible) { return 0; }
#endif


static inline int vga_get_interruptible(struct pci_dev *pdev,
					unsigned int rsrc)
{
       return vga_get(pdev, rsrc, 1);
}


static inline int vga_get_uninterruptible(struct pci_dev *pdev,
					  unsigned int rsrc)
{
       return vga_get(pdev, rsrc, 0);
}

#if defined(CONFIG_VGA_ARB)
extern int vga_tryget(struct pci_dev *pdev, unsigned int rsrc);
#else
static inline int vga_tryget(struct pci_dev *pdev, unsigned int rsrc) { return 0; }
#endif

#if defined(CONFIG_VGA_ARB)
extern void vga_put(struct pci_dev *pdev, unsigned int rsrc);
#else
#define vga_put(pdev, rsrc)
#endif


#ifdef CONFIG_VGA_ARB
extern struct pci_dev *vga_default_device(void);
extern void vga_set_default_device(struct pci_dev *pdev);
#else
static inline struct pci_dev *vga_default_device(void) { return NULL; };
static inline void vga_set_default_device(struct pci_dev *pdev) { };
#endif


#ifndef __ARCH_HAS_VGA_CONFLICT
static inline int vga_conflicts(struct pci_dev *p1, struct pci_dev *p2)
{
       return 1;
}
#endif

#if defined(CONFIG_VGA_ARB)
int vga_client_register(struct pci_dev *pdev, void *cookie,
			void (*irq_set_state)(void *cookie, bool state),
			unsigned int (*set_vga_decode)(void *cookie, bool state));
#else
static inline int vga_client_register(struct pci_dev *pdev, void *cookie,
				      void (*irq_set_state)(void *cookie, bool state),
				      unsigned int (*set_vga_decode)(void *cookie, bool state))
{
	return 0;
}
#endif

#endif 
