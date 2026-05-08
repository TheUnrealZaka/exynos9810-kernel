#ifndef _LINUX_MPSD_H
#define _LINUX_MPSD_H
#include <linux/sched.h>

#define MPSD_KMD_VERSION (1)

#define MAX_PAGES (10)
#define BUFFER_LENGTH_MAX (MAX_PAGES * PAGE_SIZE)

#define DEFAULT_PID (0)
#define DEFAULT_EVENTS (0)
#define DEFAULT_CONFIG (0)
#define DEFAULT_VERSION (0)
#define DEFAULT_APP_FIELD_MASK (0)
#define DEFAULT_DEV_FIELD_MASK (0)
#define DEFAULT_TIMER_MIN_LIMIT (100)
#define DEFAULT_PARAM_VAL_INT (-99LL)
#define DEFAULT_PARAM_VAL_CHAR ('\0')
#define DEFAULT_PARAM_VAL_STR ("-99")

#define CPULOAD_HISTORY_INTERVAL (5000)
#define CPULOAD_CHECK_DELAY (20)

#define MAX_CHAR_BUF_SIZE (30)
#define MAX_FREQ_LIST_SIZE (40)
#define MAX_NUM_CPUS (NR_CPUS)
#define MAX_LOAD_HISTORY ((CPULOAD_HISTORY_INTERVAL / CPULOAD_CHECK_DELAY))
#define MAX_TAG_LENGTH (13)

#define APP_PARAM_MIN (0)

enum app_param {
	APP_PARAM_NAME = APP_PARAM_MIN,
	APP_PARAM_TGID,
	APP_PARAM_FLAGS,
	APP_PARAM_PRIORITY,
	APP_PARAM_NICE,
	APP_PARAM_CPUS_ALLOWED,
	APP_PARAM_VOLUNTARY_CTXT_SWITCHES,
	APP_PARAM_NONVOLUNTARY_CTXT_SWITCHES,
	APP_PARAM_NUM_THREADS,
	APP_PARAM_OOM_SCORE_ADJ,
	APP_PARAM_OOM_SCORE_ADJ_MIN,
	APP_PARAM_UTIME,
	APP_PARAM_STIME,
	APP_PARAM_MEM_MIN_PGFLT,
	APP_PARAM_MEM_MAJ_PGFLT,
	APP_PARAM_STARTTIME,
	APP_PARAM_MEM_VSIZE,
	APP_PARAM_MEM_DATA,
	APP_PARAM_MEM_STACK,
	APP_PARAM_MEM_TEXT,
	APP_PARAM_MEM_SWAP,
	APP_PARAM_MEM_SHARED,
	APP_PARAM_MEM_ANON,
	APP_PARAM_MEM_RSS,
	APP_PARAM_MEM_RSS_LIMIT,
	APP_PARAM_IO_RCHAR,
	APP_PARAM_IO_WCHAR,
	APP_PARAM_IO_SYSCR,
	APP_PARAM_IO_SYSCW,
	APP_PARAM_IO_SYSCFS,
	APP_PARAM_IO_BYTES_READ,
	APP_PARAM_IO_BYTES_WRITE,
	APP_PARAM_IO_BYTES_WRITE_CANCEL,
	APP_PARAM_MAX
};

#define TOTAL_APP_PARAMS (APP_PARAM_MAX - APP_PARAM_MIN)


struct app_params_struct {
	char name[TASK_COMM_LEN];
	signed long long tgid;
	signed long long flags;
	signed long long priority;
	signed long long nice;
	signed long long cpus_allowed;
	signed long long voluntary_ctxt_switches;
	signed long long nonvoluntary_ctxt_switches;
	signed long long num_threads;
	signed long long oom_score_adj;
	signed long long oom_score_adj_min;
	signed long long utime;
	signed long long stime;
	signed long long mem_min_pgflt;
	signed long long mem_maj_pgflt;
	signed long long start_time;
	signed long long mem_vsize;
	signed long long mem_data;
	signed long long mem_stack;
	signed long long mem_text;
	signed long long mem_swap;
	signed long long mem_shared;
	signed long long mem_anon;
	signed long long mem_rss;
	signed long long mem_rss_limit;
	signed long long io_rchar;
	signed long long io_wchar;
	signed long long io_syscr;
	signed long long io_syscw;
	signed long long io_syscfs;
	signed long long io_bytes_read;
	signed long long io_bytes_write;
	signed long long io_bytes_write_cancel;
};

#define PARAM_DELTA (0)
#define DEV_PARAM_MIN (APP_PARAM_MAX + PARAM_DELTA)

enum dev_param {
	DEV_PARAM_CPU_PRESENT = DEV_PARAM_MIN,
	DEV_PARAM_CPU_ONLINE,
	DEV_PARAM_CPU_UTIL,
	DEV_PARAM_CPU_FREQ_AVAIL,
	DEV_PARAM_CPU_MAX_LIMIT,
	DEV_PARAM_CPU_MIN_LIMIT,
	DEV_PARAM_CPU_CUR_FREQ,
	DEV_PARAM_CPU_FREQ_TIME_STATS,
	DEV_PARAM_CPU_FREQ_GOV,
	DEV_PARAM_CPU_LOADAVG_1MIN,
	DEV_PARAM_CPU_LOADAVG_5MIN,
	DEV_PARAM_CPU_LOADAVG_15MIN,
	DEV_PARAM_SYSTEM_UPTIME,
	DEV_PARAM_CPU_TIME_USER,
	DEV_PARAM_CPU_TIME_NICE,
	DEV_PARAM_CPU_TIME_SYSTEM,
	DEV_PARAM_CPU_TIME_IDLE,
	DEV_PARAM_CPU_TIME_IO_WAIT,
	DEV_PARAM_CPU_TIME_IRQ,
	DEV_PARAM_CPU_TIME_SOFT_IRQ,
	DEV_PARAM_MEM_TOTAL,
	DEV_PARAM_MEM_FREE,
	DEV_PARAM_MEM_AVAIL,
	DEV_PARAM_MEM_BUFFER,
	DEV_PARAM_MEM_CACHED,
	DEV_PARAM_MEM_SWAP_CACHED,
	DEV_PARAM_MEM_SWAP_TOTAL,
	DEV_PARAM_MEM_SWAP_FREE,
	DEV_PARAM_MEM_DIRTY,
	DEV_PARAM_MEM_ANON_PAGES,
	DEV_PARAM_MEM_MAPPED,
	DEV_PARAM_MEM_SHMEM,
	DEV_PARAM_MEM_SYSHEAP,
	DEV_PARAM_MEM_SYSHEAP_POOL,
	DEV_PARAM_MEM_VMALLOC_API,
	DEV_PARAM_MEM_KGSL_SHARED,
	DEV_PARAM_MEM_ZSWAP,
	DEV_PARAM_MAX
};

#define TOTAL_DEV_PARAMS (DEV_PARAM_MAX - DEV_PARAM_MIN)

#define CPUFREQ_GOV_NONE "none"
#define CPUFREQ_GOV_PERFORMANCE "performance"
#define CPUFREQ_GOV_POWERSAVE "powersave"
#define CPUFREQ_GOV_USERSPACE "userspace"
#define CPUFREQ_GOV_ONDEMAND "ondemand"
#define CPUFREQ_GOV_CONSERVATIVE "conservative"
#define CPUFREQ_GOV_INTERACTIVE "interactive"
#define CPUFREQ_GOV_SCHEDUTIL "schedutil"


struct dev_cpufreq_gov_performance {
	signed long long cur_freq;
};


struct dev_cpufreq_gov_powersave {
	signed long long cur_freq;
};


struct dev_cpufreq_gov_userspace {
	signed long long set_speed;
};


struct dev_cpufreq_gov_ondemand {
	signed long long min_sampling_rate;
	signed long long ignore_nice_load;
	signed long long sampling_rate;
	signed long long sampling_down_factor;
	signed long long up_threshold;
	signed long long io_is_busy;
	signed long long powersave_bias;
};


struct dev_cpufreq_gov_conservative {
	signed long long min_sampling_rate;
	signed long long ignore_nice_load;
	signed long long sampling_rate;
	signed long long sampling_down_factor;
	signed long long up_threshold;
	signed long long down_threshold;
	signed long long freq_step;
};


struct dev_cpufreq_gov_interactive {
	signed long long hispeed_freq;
	signed long long go_hispeed_load;
	signed long long target_loads;
	signed long long min_sample_time;
	signed long long timer_rate;
	signed long long above_hispeed_delay;
	signed long long boost;
	signed long long boostpulse_duration;
	signed long long timer_slack;
	signed long long io_is_busy;
	signed long long use_sched_load;
	signed long long use_migration_notif;
	signed long long align_windows;
	signed long long max_freq_hysteresis;
	signed long long ignore_hispeed_on_notif;
	signed long long fast_ramp_down;
	signed long long enable_prediction;
};


struct dev_cpufreq_gov_schedutil {
	signed long long up_rate_limit_us;
	signed long long down_rate_limit_us;
};


union cpufreq_gov_data {
	struct dev_cpufreq_gov_performance gov_performance;
	struct dev_cpufreq_gov_powersave gov_powersave;
	struct dev_cpufreq_gov_userspace gov_userspace;
	struct dev_cpufreq_gov_ondemand gov_ondemand;
	struct dev_cpufreq_gov_conservative gov_conservative;
	struct dev_cpufreq_gov_interactive gov_interactive;
	struct dev_cpufreq_gov_schedutil gov_schedutil;
};


struct dev_cpu_freq_gov {
	char gov[MAX_CHAR_BUF_SIZE];
	union cpufreq_gov_data cpu_freq_gov_data;
};


struct dev_cpu_freq_time_stats {
	signed long long freq;
	signed long long time;
};


struct dev_params_struct {
	signed long long cpu_present;
	signed long long cpu_online;
	signed long long cpu_util[MAX_NUM_CPUS][MAX_LOAD_HISTORY];
	signed long long cpu_freq_avail[MAX_NUM_CPUS][MAX_FREQ_LIST_SIZE];
	signed long long cpu_max_limit[MAX_NUM_CPUS];
	signed long long cpu_min_limit[MAX_NUM_CPUS];
	signed long long cpu_cur_freq[MAX_NUM_CPUS];
	struct dev_cpu_freq_time_stats cpu_freq_time_stats[MAX_NUM_CPUS][MAX_FREQ_LIST_SIZE];
	struct dev_cpu_freq_gov cpu_freq_gov[MAX_NUM_CPUS];
	signed long long cpu_loadavg_1min;
	signed long long cpu_loadavg_5min;
	signed long long cpu_loadavg_15min;
	signed long long system_uptime;
	signed long long cpu_time_user;
	signed long long cpu_time_nice;
	signed long long cpu_time_system;
	signed long long cpu_time_idle;
	signed long long cpu_time_io_Wait;
	signed long long cpu_time_irq;
	signed long long cpu_time_soft_irq;
	signed long long mem_total;
	signed long long mem_free;
	signed long long mem_avail;
	signed long long mem_buffer;
	signed long long mem_cached;
	signed long long mem_swap_cached;
	signed long long mem_swap_total;
	signed long long mem_swap_free;
	signed long long mem_dirty;
	signed long long mem_anon_pages;
	signed long long mem_mapped;
	signed long long mem_shmem;
	signed long long mem_sysheap;
	signed long long mem_sysheap_pool;
	signed long long mem_vmalloc_api;
	signed long long mem_kgsl_shared;
	signed long long mem_zswap;
};

#define EVENT_TYPE_MIN (0)


enum notifier_event_type {
	EVENT_VALUE_CHANGED = EVENT_TYPE_MIN,
	EVENT_REACHED_MAX_THRESHOLD,
	EVENT_REACHED_MIN_THRESHOLD,
	EVENT_TYPE_MAX,
};


struct param_info_notifier {
	int param;
	int events;
	signed long long delta;
	signed long long max_threshold;
	signed long long min_threshold;
};


struct param_data_notifier {
	unsigned int param;
	unsigned int events;
	signed long long prev;
	signed long long cur;
};


enum req_id {
	SYNC_READ = 0,
	TIMED_UPDATE,
	NOTIFICATION_UPDATE,
};


union req_data {
	unsigned int val;
	struct param_info_notifier info;
};


struct mpsd_req {
	unsigned int param_id;
	enum req_id id;
	union req_data data;
};


struct memory_area {
	unsigned int updating;
	unsigned long long app_field_mask;
	struct app_params_struct app_params;
	unsigned long long dev_field_mask;
	struct dev_params_struct dev_params;
	enum req_id req;
	unsigned int event;
};

bool get_mpsd_flag(void);
void mpsd_event_notify(int param, unsigned long prev, unsigned long cur);

#define MPSD_IOC_MAGIC 'M'


#define MPSD_IOC_REGISTER _IOWR(MPSD_IOC_MAGIC, 0, struct mpsd_req)


#define MPSD_IOC_UNREGISTER _IOWR(MPSD_IOC_MAGIC, 1, struct mpsd_req)


#define MPSD_IOC_READ _IOW(MPSD_IOC_MAGIC, 2, unsigned int)


#define MPSD_IOC_VERSION _IOR(MPSD_IOC_MAGIC, 3, unsigned int)


#define MPSD_IOC_CONFIG _IOW(MPSD_IOC_MAGIC, 4, unsigned int)


#define MPSD_IOC_SET_PID _IOW(MPSD_IOC_MAGIC, 5, unsigned int)

#endif 

