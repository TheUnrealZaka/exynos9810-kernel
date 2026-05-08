

#ifndef __XEN_PUBLIC_VCPU_H__
#define __XEN_PUBLIC_VCPU_H__




#define VCPUOP_initialise			 0


#define VCPUOP_up					 1


#define VCPUOP_down					 2


#define VCPUOP_is_up				 3


#define VCPUOP_get_runstate_info	 4
struct vcpu_runstate_info {
	
	int		 state;
	
	uint64_t state_entry_time;
	
#define XEN_RUNSTATE_UPDATE	(1ULL << 63)
	
	uint64_t time[4];
};
DEFINE_GUEST_HANDLE_STRUCT(vcpu_runstate_info);


#define RUNSTATE_running  0


#define RUNSTATE_runnable 1


#define RUNSTATE_blocked  2


#define RUNSTATE_offline  3


#define VCPUOP_register_runstate_memory_area 5
struct vcpu_register_runstate_memory_area {
		union {
				GUEST_HANDLE(vcpu_runstate_info) h;
				struct vcpu_runstate_info *v;
				uint64_t p;
		} addr;
};


#define VCPUOP_set_periodic_timer	 6 
#define VCPUOP_stop_periodic_timer	 7 
struct vcpu_set_periodic_timer {
		uint64_t period_ns;
};
DEFINE_GUEST_HANDLE_STRUCT(vcpu_set_periodic_timer);


#define VCPUOP_set_singleshot_timer	 8 
#define VCPUOP_stop_singleshot_timer 9 
struct vcpu_set_singleshot_timer {
		uint64_t timeout_abs_ns;
		uint32_t flags;			   
};
DEFINE_GUEST_HANDLE_STRUCT(vcpu_set_singleshot_timer);


 
#define _VCPU_SSHOTTMR_future (0)
#define VCPU_SSHOTTMR_future  (1U << _VCPU_SSHOTTMR_future)


#define VCPUOP_register_vcpu_info   10  
struct vcpu_register_vcpu_info {
    uint64_t mfn;    
    uint32_t offset; 
    uint32_t rsvd;   
};
DEFINE_GUEST_HANDLE_STRUCT(vcpu_register_vcpu_info);


#define VCPUOP_send_nmi             11
#endif 
