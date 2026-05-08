
#ifndef __ASM_GENERIC_PCI_IOMAP_H
#define __ASM_GENERIC_PCI_IOMAP_H

struct pci_dev;
#ifdef CONFIG_PCI

extern void __iomem *pci_iomap(struct pci_dev *dev, int bar, unsigned long max);
extern void __iomem *pci_iomap_wc(struct pci_dev *dev, int bar, unsigned long max);
extern void __iomem *pci_iomap_range(struct pci_dev *dev, int bar,
				     unsigned long offset,
				     unsigned long maxlen);
extern void __iomem *pci_iomap_wc_range(struct pci_dev *dev, int bar,
					unsigned long offset,
					unsigned long maxlen);

#ifdef CONFIG_NO_GENERIC_PCI_IOPORT_MAP
extern void __iomem *__pci_ioport_map(struct pci_dev *dev, unsigned long port,
				      unsigned int nr);
#else
#define __pci_ioport_map(dev, port, nr) ioport_map((port), (nr))
#endif

#elif defined(CONFIG_GENERIC_PCI_IOMAP)
static inline void __iomem *pci_iomap(struct pci_dev *dev, int bar, unsigned long max)
{
	return NULL;
}

static inline void __iomem *pci_iomap_wc(struct pci_dev *dev, int bar, unsigned long max)
{
	return NULL;
}
static inline void __iomem *pci_iomap_range(struct pci_dev *dev, int bar,
					    unsigned long offset,
					    unsigned long maxlen)
{
	return NULL;
}
static inline void __iomem *pci_iomap_wc_range(struct pci_dev *dev, int bar,
					       unsigned long offset,
					       unsigned long maxlen)
{
	return NULL;
}
#endif

#endif 
