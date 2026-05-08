
#ifndef _LINUX_PMU_H
#define _LINUX_PMU_H

#include <uapi/linux/pmu.h>


extern int find_via_pmu(void);

extern int pmu_request(struct adb_request *req,
		void (*done)(struct adb_request *), int nbytes, ...);
extern int pmu_queue_request(struct adb_request *req);
extern void pmu_poll(void);
extern void pmu_poll_adb(void); 
extern void pmu_wait_complete(struct adb_request *req);


#if defined(CONFIG_ADB_PMU)
extern void pmu_suspend(void);
extern void pmu_resume(void);
#else
static inline void pmu_suspend(void)
{}
static inline void pmu_resume(void)
{}
#endif

extern void pmu_enable_irled(int on);

extern void pmu_restart(void);
extern void pmu_shutdown(void);
extern void pmu_unlock(void);

extern int pmu_present(void);
extern int pmu_get_model(void);

extern void pmu_backlight_set_sleep(int sleep);

#define PMU_MAX_BATTERIES	2


#define PMU_PWR_AC_PRESENT	0x00000001


#define PMU_BATT_PRESENT	0x00000001
#define PMU_BATT_CHARGING	0x00000002
#define PMU_BATT_TYPE_MASK	0x000000f0
#define PMU_BATT_TYPE_SMART	0x00000010 
#define PMU_BATT_TYPE_HOOPER	0x00000020 
#define PMU_BATT_TYPE_COMET	0x00000030 

struct pmu_battery_info
{
	unsigned int	flags;
	unsigned int	charge;		
	unsigned int	max_charge;	
	signed int	amperage;	
	unsigned int	voltage;	
	unsigned int	time_remaining;	
};

extern int pmu_battery_count;
extern struct pmu_battery_info pmu_batteries[PMU_MAX_BATTERIES];
extern unsigned int pmu_power_flags;


extern void pmu_backlight_init(void);


#if defined(CONFIG_SUSPEND) && defined(CONFIG_PPC32)
extern int pmu_sys_suspended;
#else

#define pmu_sys_suspended	0
#endif

#endif 
