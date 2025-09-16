

#ifndef EXYNOS_SNAPSHOT_SOC_H
#define EXYNOS_SNAPSHOT_SOC_H


#define ESS_REG_MCT_ADDR	(0)
#define ESS_REG_MCT_SIZE	(0)
#define ESS_REG_UART_ADDR	(0)
#define ESS_REG_UART_SIZE	(0)

struct exynos_ss_ops {
        int (*pd_status)(unsigned int id);
};

extern struct exynos_ss_ops ess_ops;
#endif
