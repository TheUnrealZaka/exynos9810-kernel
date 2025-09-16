

#ifndef __EXYNOS_PCIE_CTRL_H
#define __EXYNOS_PCIE_CTRL_H


#define PCIE_L1SS_CTRL_ARGOS            (0x1 << 0)
#define PCIE_L1SS_CTRL_BOOT             (0x1 << 1)
#define PCIE_L1SS_CTRL_CAMERA           (0x1 << 2)
#define PCIE_L1SS_CTRL_MODEM_IF         (0x1 << 3)
#define PCIE_L1SS_CTRL_WIFI             (0x1 << 4)
#define PCIE_L1SS_CTRL_MST              (0x1 << 5)

#if defined(CONFIG_PCI_EXYNOS)
extern int exynos_pcie_l1ss_ctrl(int enable, int id);
#endif

#endif
