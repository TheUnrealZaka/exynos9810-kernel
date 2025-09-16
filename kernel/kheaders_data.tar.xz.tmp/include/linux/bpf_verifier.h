
#ifndef _LINUX_BPF_VERIFIER_H
#define _LINUX_BPF_VERIFIER_H 1

#include <linux/bpf.h> 
#include <linux/filter.h> 
#include <linux/tnum.h>


#define BPF_MAX_VAR_OFF	(1 << 29)

#define BPF_MAX_VAR_SIZ	(1 << 29)


enum bpf_reg_liveness {
	REG_LIVE_NONE = 0, 
	REG_LIVE_READ, 
	REG_LIVE_WRITTEN, 
};

struct bpf_reg_state {
	enum bpf_reg_type type;
	union {
		
		u16 range;

		
		struct bpf_map *map_ptr;

		
		unsigned long raw;
	};
	
	s32 off;
	
	u32 id;
	
	
	struct tnum var_off;
	
	s64 smin_value; 
	s64 smax_value; 
	u64 umin_value; 
	u64 umax_value; 
	
	u32 frameno;
	
	enum bpf_reg_liveness live;
};

enum bpf_stack_slot_type {
	STACK_INVALID,    
	STACK_SPILL,      
	STACK_MISC,	  
	STACK_ZERO,	  
};

#define BPF_REG_SIZE 8	

struct bpf_stack_state {
	struct bpf_reg_state spilled_ptr;
	u8 slot_type[BPF_REG_SIZE];
};


struct bpf_func_state {
	struct bpf_reg_state regs[MAX_BPF_REG];
	struct bpf_verifier_state *parent;
	
	int callsite;
	
	u32 frameno;
	
	u32 subprogno;

	
	int allocated_stack;
	struct bpf_stack_state *stack;
};

#define MAX_CALL_FRAMES 8
struct bpf_verifier_state {
	
	struct bpf_func_state *frame[MAX_CALL_FRAMES];
	struct bpf_verifier_state *parent;
	u32 curframe;
	bool speculative;
};


struct bpf_verifier_state_list {
	struct bpf_verifier_state state;
	struct bpf_verifier_state_list *next;
};


#define BPF_ALU_SANITIZE_SRC		1U
#define BPF_ALU_SANITIZE_DST		2U
#define BPF_ALU_NEG_VALUE		(1U << 2)
#define BPF_ALU_NON_POINTER		(1U << 3)
#define BPF_ALU_SANITIZE		(BPF_ALU_SANITIZE_SRC | \
					 BPF_ALU_SANITIZE_DST)

struct bpf_insn_aux_data {
	union {
		enum bpf_reg_type ptr_type;     
		unsigned long map_state;	
		s32 call_imm;			
		u32 alu_limit;			
	};
	int ctx_field_size; 
	int converted_op_size; 
	int sanitize_stack_off; 
	bool seen; 
	u8 alu_state; 
};

#define MAX_USED_MAPS 64 

#define BPF_VERIFIER_TMP_LOG_SIZE	1024

struct bpf_verifier_log {
	u32 level;
	char kbuf[BPF_VERIFIER_TMP_LOG_SIZE];
	char __user *ubuf;
	u32 len_used;
	u32 len_total;
};

static inline bool bpf_verifier_log_full(const struct bpf_verifier_log *log)
{
	return log->len_used >= log->len_total - 1;
}

static inline bool bpf_verifier_log_needed(const struct bpf_verifier_log *log)
{
	return log->level && log->ubuf && !bpf_verifier_log_full(log);
}

#define BPF_MAX_SUBPROGS 256

struct bpf_subprog_info {
	u32 start; 
	u16 stack_depth; 
};


struct bpf_verifier_env {
	u32 insn_idx;
	u32 prev_insn_idx;
	struct bpf_prog *prog;		
	const struct bpf_verifier_ops *ops;
	struct bpf_verifier_stack_elem *head; 
	int stack_size;			
	bool strict_alignment;		
	struct bpf_verifier_state *cur_state; 
	struct bpf_verifier_state_list **explored_states; 
	struct bpf_map *used_maps[MAX_USED_MAPS]; 
	u32 used_map_cnt;		
	u32 id_gen;			
	bool allow_ptr_leaks;
	bool seen_direct_write;
	struct bpf_insn_aux_data *insn_aux_data; 
	struct bpf_verifier_log log;
	struct bpf_subprog_info subprog_info[BPF_MAX_SUBPROGS + 1];
	u32 subprog_cnt;
};

void bpf_verifier_vlog(struct bpf_verifier_log *log, const char *fmt,
		       va_list args);
__printf(2, 3) void bpf_verifier_log_write(struct bpf_verifier_env *env,
					   const char *fmt, ...);

int bpf_prog_offload_verifier_prep(struct bpf_verifier_env *env);
int bpf_prog_offload_verify_insn(struct bpf_verifier_env *env,
				 int insn_idx, int prev_insn_idx);

static inline struct bpf_reg_state *cur_regs(struct bpf_verifier_env *env)
{
	struct bpf_verifier_state *cur = env->cur_state;

	return cur->frame[cur->curframe]->regs;
}

#endif 
