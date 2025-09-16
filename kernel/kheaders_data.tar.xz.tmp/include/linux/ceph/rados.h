#ifndef CEPH_RADOS_H
#define CEPH_RADOS_H



#include <linux/ceph/msgr.h>


struct ceph_fsid {
	unsigned char fsid[16];
};

static inline int ceph_fsid_compare(const struct ceph_fsid *a,
				    const struct ceph_fsid *b)
{
	return memcmp(a, b, sizeof(*a));
}


typedef __le64 ceph_snapid_t;
#define CEPH_SNAPDIR ((__u64)(-1))  
#define CEPH_NOSNAP  ((__u64)(-2))  
#define CEPH_MAXSNAP ((__u64)(-3))  

struct ceph_timespec {
	__le32 tv_sec;
	__le32 tv_nsec;
} __attribute__ ((packed));



#define CEPH_OBJECT_LAYOUT_HASH     1
#define CEPH_OBJECT_LAYOUT_LINEAR   2
#define CEPH_OBJECT_LAYOUT_HASHINO  3


#define CEPH_PG_LAYOUT_CRUSH  0
#define CEPH_PG_LAYOUT_HASH   1
#define CEPH_PG_LAYOUT_LINEAR 2
#define CEPH_PG_LAYOUT_HYBRID 3

#define CEPH_PG_MAX_SIZE      16  


struct ceph_pg_v1 {
	__le16 preferred; 
	__le16 ps;        
	__le32 pool;      
} __attribute__ ((packed));


#define CEPH_NOPOOL  ((__u64) (-1))  

#define CEPH_POOL_TYPE_REP     1
#define CEPH_POOL_TYPE_RAID4   2 
#define CEPH_POOL_TYPE_EC      3


static inline int ceph_stable_mod(int x, int b, int bmask)
{
	if ((x & bmask) < b)
		return x & bmask;
	else
		return x & (bmask >> 1);
}


struct ceph_object_layout {
	struct ceph_pg_v1 ol_pgid;   
	__le32 ol_stripe_unit;    
} __attribute__ ((packed));


struct ceph_eversion {
	__le64 version;
	__le32 epoch;
} __attribute__ ((packed));




#define CEPH_OSD_EXISTS  (1<<0)
#define CEPH_OSD_UP      (1<<1)
#define CEPH_OSD_AUTOOUT (1<<2)  
#define CEPH_OSD_NEW     (1<<3)  

extern const char *ceph_osd_state_name(int s);


#define CEPH_OSD_IN  0x10000
#define CEPH_OSD_OUT 0


#define CEPH_OSD_MAX_PRIMARY_AFFINITY 0x10000
#define CEPH_OSD_DEFAULT_PRIMARY_AFFINITY 0x10000



#define CEPH_OSDMAP_NEARFULL (1<<0)  
#define CEPH_OSDMAP_FULL     (1<<1)  
#define CEPH_OSDMAP_PAUSERD  (1<<2)  
#define CEPH_OSDMAP_PAUSEWR  (1<<3)  
#define CEPH_OSDMAP_PAUSEREC (1<<4)  
#define CEPH_OSDMAP_NOUP     (1<<5)  
#define CEPH_OSDMAP_NODOWN   (1<<6)  
#define CEPH_OSDMAP_NOOUT    (1<<7)  
#define CEPH_OSDMAP_NOIN     (1<<8)  
#define CEPH_OSDMAP_NOBACKFILL (1<<9) 
#define CEPH_OSDMAP_NORECOVER (1<<10) 
#define CEPH_OSDMAP_NOSCRUB  (1<<11) 
#define CEPH_OSDMAP_NODEEP_SCRUB (1<<12) 
#define CEPH_OSDMAP_NOTIERAGENT (1<<13) 
#define CEPH_OSDMAP_NOREBALANCE (1<<14) 
#define CEPH_OSDMAP_SORTBITWISE (1<<15) 


#define OSD_WRITETOOBIG EMSGSIZE


#define CEPH_OSD_OP_MODE       0xf000
#define CEPH_OSD_OP_MODE_RD    0x1000
#define CEPH_OSD_OP_MODE_WR    0x2000
#define CEPH_OSD_OP_MODE_RMW   0x3000
#define CEPH_OSD_OP_MODE_SUB   0x4000
#define CEPH_OSD_OP_MODE_CACHE 0x8000

#define CEPH_OSD_OP_TYPE       0x0f00
#define CEPH_OSD_OP_TYPE_LOCK  0x0100
#define CEPH_OSD_OP_TYPE_DATA  0x0200
#define CEPH_OSD_OP_TYPE_ATTR  0x0300
#define CEPH_OSD_OP_TYPE_EXEC  0x0400
#define CEPH_OSD_OP_TYPE_PG    0x0500
#define CEPH_OSD_OP_TYPE_MULTI 0x0600 

#define __CEPH_OSD_OP1(mode, nr) \
	(CEPH_OSD_OP_MODE_##mode | (nr))

#define __CEPH_OSD_OP(mode, type, nr) \
	(CEPH_OSD_OP_MODE_##mode | CEPH_OSD_OP_TYPE_##type | (nr))

#define __CEPH_FORALL_OSD_OPS(f)					    \
								    \
								    \
	f(READ,		__CEPH_OSD_OP(RD, DATA, 1),	"read")		    \
	f(STAT,		__CEPH_OSD_OP(RD, DATA, 2),	"stat")		    \
	f(MAPEXT,	__CEPH_OSD_OP(RD, DATA, 3),	"mapext")	    \
									    \
							    \
	f(MASKTRUNC,	__CEPH_OSD_OP(RD, DATA, 4),	"masktrunc")	    \
	f(SPARSE_READ,	__CEPH_OSD_OP(RD, DATA, 5),	"sparse-read")	    \
									    \
	f(NOTIFY,	__CEPH_OSD_OP(RD, DATA, 6),	"notify")	    \
	f(NOTIFY_ACK,	__CEPH_OSD_OP(RD, DATA, 7),	"notify-ack")	    \
									    \
							    \
	f(ASSERT_VER,	__CEPH_OSD_OP(RD, DATA, 8),	"assert-version")   \
									    \
	f(LIST_WATCHERS, __CEPH_OSD_OP(RD, DATA, 9),	"list-watchers")    \
									    \
	f(LIST_SNAPS,	__CEPH_OSD_OP(RD, DATA, 10),	"list-snaps")	    \
									    \
								    \
	f(SYNC_READ,	__CEPH_OSD_OP(RD, DATA, 11),	"sync_read")	    \
									    \
								    \
	f(WRITE,	__CEPH_OSD_OP(WR, DATA, 1),	"write")	    \
	f(WRITEFULL,	__CEPH_OSD_OP(WR, DATA, 2),	"writefull")	    \
	f(TRUNCATE,	__CEPH_OSD_OP(WR, DATA, 3),	"truncate")	    \
	f(ZERO,		__CEPH_OSD_OP(WR, DATA, 4),	"zero")		    \
	f(DELETE,	__CEPH_OSD_OP(WR, DATA, 5),	"delete")	    \
									    \
							    \
	f(APPEND,	__CEPH_OSD_OP(WR, DATA, 6),	"append")	    \
	f(STARTSYNC,	__CEPH_OSD_OP(WR, DATA, 7),	"startsync")	    \
	f(SETTRUNC,	__CEPH_OSD_OP(WR, DATA, 8),	"settrunc")	    \
	f(TRIMTRUNC,	__CEPH_OSD_OP(WR, DATA, 9),	"trimtrunc")	    \
									    \
	f(TMAPUP,	__CEPH_OSD_OP(RMW, DATA, 10),	"tmapup")	    \
	f(TMAPPUT,	__CEPH_OSD_OP(WR, DATA, 11),	"tmapput")	    \
	f(TMAPGET,	__CEPH_OSD_OP(RD, DATA, 12),	"tmapget")	    \
									    \
	f(CREATE,	__CEPH_OSD_OP(WR, DATA, 13),	"create")	    \
	f(ROLLBACK,	__CEPH_OSD_OP(WR, DATA, 14),	"rollback")	    \
									    \
	f(WATCH,	__CEPH_OSD_OP(WR, DATA, 15),	"watch")	    \
									    \
								    \
	f(OMAPGETKEYS,	__CEPH_OSD_OP(RD, DATA, 17),	"omap-get-keys")    \
	f(OMAPGETVALS,	__CEPH_OSD_OP(RD, DATA, 18),	"omap-get-vals")    \
	f(OMAPGETHEADER, __CEPH_OSD_OP(RD, DATA, 19),	"omap-get-header")  \
	f(OMAPGETVALSBYKEYS, __CEPH_OSD_OP(RD, DATA, 20), "omap-get-vals-by-keys") \
	f(OMAPSETVALS,	__CEPH_OSD_OP(WR, DATA, 21),	"omap-set-vals")    \
	f(OMAPSETHEADER, __CEPH_OSD_OP(WR, DATA, 22),	"omap-set-header")  \
	f(OMAPCLEAR,	__CEPH_OSD_OP(WR, DATA, 23),	"omap-clear")	    \
	f(OMAPRMKEYS,	__CEPH_OSD_OP(WR, DATA, 24),	"omap-rm-keys")	    \
	f(OMAP_CMP,	__CEPH_OSD_OP(RD, DATA, 25),	"omap-cmp")	    \
									    \
								    \
	f(COPY_FROM,	__CEPH_OSD_OP(WR, DATA, 26),	"copy-from")	    \
	f(COPY_GET_CLASSIC, __CEPH_OSD_OP(RD, DATA, 27), "copy-get-classic") \
	f(UNDIRTY,	__CEPH_OSD_OP(WR, DATA, 28),	"undirty")	    \
	f(ISDIRTY,	__CEPH_OSD_OP(RD, DATA, 29),	"isdirty")	    \
	f(COPY_GET,	__CEPH_OSD_OP(RD, DATA, 30),	"copy-get")	    \
	f(CACHE_FLUSH,	__CEPH_OSD_OP(CACHE, DATA, 31),	"cache-flush")	    \
	f(CACHE_EVICT,	__CEPH_OSD_OP(CACHE, DATA, 32),	"cache-evict")	    \
	f(CACHE_TRY_FLUSH, __CEPH_OSD_OP(CACHE, DATA, 33), "cache-try-flush") \
									    \
						    \
	f(TMAP2OMAP,	__CEPH_OSD_OP(RMW, DATA, 34),	"tmap2omap")	    \
									    \
								    \
	f(SETALLOCHINT,	__CEPH_OSD_OP(WR, DATA, 35),	"set-alloc-hint")   \
									    \
								    \
	f(CLONERANGE,	__CEPH_OSD_OP(WR, MULTI, 1),	"clonerange")	    \
	f(ASSERT_SRC_VERSION, __CEPH_OSD_OP(RD, MULTI, 2), "assert-src-version") \
	f(SRC_CMPXATTR,	__CEPH_OSD_OP(RD, MULTI, 3),	"src-cmpxattr")	    \
									    \
								    \
								    \
	f(GETXATTR,	__CEPH_OSD_OP(RD, ATTR, 1),	"getxattr")	    \
	f(GETXATTRS,	__CEPH_OSD_OP(RD, ATTR, 2),	"getxattrs")	    \
	f(CMPXATTR,	__CEPH_OSD_OP(RD, ATTR, 3),	"cmpxattr")	    \
									    \
								    \
	f(SETXATTR,	__CEPH_OSD_OP(WR, ATTR, 1),	"setxattr")	    \
	f(SETXATTRS,	__CEPH_OSD_OP(WR, ATTR, 2),	"setxattrs")	    \
	f(RESETXATTRS,	__CEPH_OSD_OP(WR, ATTR, 3),	"resetxattrs")	    \
	f(RMXATTR,	__CEPH_OSD_OP(WR, ATTR, 4),	"rmxattr")	    \
									    \
								    \
	f(PULL,		__CEPH_OSD_OP1(SUB, 1),		"pull")		    \
	f(PUSH,		__CEPH_OSD_OP1(SUB, 2),		"push")		    \
	f(BALANCEREADS,	__CEPH_OSD_OP1(SUB, 3),		"balance-reads")    \
	f(UNBALANCEREADS, __CEPH_OSD_OP1(SUB, 4),	"unbalance-reads")  \
	f(SCRUB,	__CEPH_OSD_OP1(SUB, 5),		"scrub")	    \
	f(SCRUB_RESERVE, __CEPH_OSD_OP1(SUB, 6),	"scrub-reserve")    \
	f(SCRUB_UNRESERVE, __CEPH_OSD_OP1(SUB, 7),	"scrub-unreserve")  \
	f(SCRUB_STOP,	__CEPH_OSD_OP1(SUB, 8),		"scrub-stop")	    \
	f(SCRUB_MAP,	__CEPH_OSD_OP1(SUB, 9),		"scrub-map")	    \
									    \
								    \
	f(WRLOCK,	__CEPH_OSD_OP(WR, LOCK, 1),	"wrlock")	    \
	f(WRUNLOCK,	__CEPH_OSD_OP(WR, LOCK, 2),	"wrunlock")	    \
	f(RDLOCK,	__CEPH_OSD_OP(WR, LOCK, 3),	"rdlock")	    \
	f(RDUNLOCK,	__CEPH_OSD_OP(WR, LOCK, 4),	"rdunlock")	    \
	f(UPLOCK,	__CEPH_OSD_OP(WR, LOCK, 5),	"uplock")	    \
	f(DNLOCK,	__CEPH_OSD_OP(WR, LOCK, 6),	"dnlock")	    \
									    \
								    \
	 \
	f(CALL,		__CEPH_OSD_OP(RD, EXEC, 1),	"call")		    \
									    \
								    \
	f(PGLS,		__CEPH_OSD_OP(RD, PG, 1),	"pgls")		    \
	f(PGLS_FILTER,	__CEPH_OSD_OP(RD, PG, 2),	"pgls-filter")	    \
	f(PG_HITSET_LS,	__CEPH_OSD_OP(RD, PG, 3),	"pg-hitset-ls")	    \
	f(PG_HITSET_GET, __CEPH_OSD_OP(RD, PG, 4),	"pg-hitset-get")

enum {
#define GENERATE_ENUM_ENTRY(op, opcode, str)	CEPH_OSD_OP_##op = (opcode),
__CEPH_FORALL_OSD_OPS(GENERATE_ENUM_ENTRY)
#undef GENERATE_ENUM_ENTRY
};

static inline int ceph_osd_op_type_lock(int op)
{
	return (op & CEPH_OSD_OP_TYPE) == CEPH_OSD_OP_TYPE_LOCK;
}
static inline int ceph_osd_op_type_data(int op)
{
	return (op & CEPH_OSD_OP_TYPE) == CEPH_OSD_OP_TYPE_DATA;
}
static inline int ceph_osd_op_type_attr(int op)
{
	return (op & CEPH_OSD_OP_TYPE) == CEPH_OSD_OP_TYPE_ATTR;
}
static inline int ceph_osd_op_type_exec(int op)
{
	return (op & CEPH_OSD_OP_TYPE) == CEPH_OSD_OP_TYPE_EXEC;
}
static inline int ceph_osd_op_type_pg(int op)
{
	return (op & CEPH_OSD_OP_TYPE) == CEPH_OSD_OP_TYPE_PG;
}
static inline int ceph_osd_op_type_multi(int op)
{
	return (op & CEPH_OSD_OP_TYPE) == CEPH_OSD_OP_TYPE_MULTI;
}

static inline int ceph_osd_op_mode_subop(int op)
{
	return (op & CEPH_OSD_OP_MODE) == CEPH_OSD_OP_MODE_SUB;
}
static inline int ceph_osd_op_mode_read(int op)
{
	return (op & CEPH_OSD_OP_MODE_RD) &&
		op != CEPH_OSD_OP_CALL;
}
static inline int ceph_osd_op_mode_modify(int op)
{
	return op & CEPH_OSD_OP_MODE_WR;
}


#define CEPH_OSD_TMAP_HDR 'h'
#define CEPH_OSD_TMAP_SET 's'
#define CEPH_OSD_TMAP_CREATE 'c' 
#define CEPH_OSD_TMAP_RM  'r'
#define CEPH_OSD_TMAP_RMSLOPPY 'R'

extern const char *ceph_osd_op_name(int op);


enum {
	CEPH_OSD_FLAG_ACK =            0x0001,  
	CEPH_OSD_FLAG_ONNVRAM =        0x0002,  
	CEPH_OSD_FLAG_ONDISK =         0x0004,  
	CEPH_OSD_FLAG_RETRY =          0x0008,  
	CEPH_OSD_FLAG_READ =           0x0010,  
	CEPH_OSD_FLAG_WRITE =          0x0020,  
	CEPH_OSD_FLAG_ORDERSNAP =      0x0040,  
	CEPH_OSD_FLAG_PEERSTAT_OLD =   0x0080,  
	CEPH_OSD_FLAG_BALANCE_READS =  0x0100,
	CEPH_OSD_FLAG_PARALLELEXEC =   0x0200,  
	CEPH_OSD_FLAG_PGOP =           0x0400,  
	CEPH_OSD_FLAG_EXEC =           0x0800,  
	CEPH_OSD_FLAG_EXEC_PUBLIC =    0x1000,  
	CEPH_OSD_FLAG_LOCALIZE_READS = 0x2000,  
	CEPH_OSD_FLAG_RWORDERED =      0x4000,  
	CEPH_OSD_FLAG_IGNORE_CACHE =   0x8000,  
	CEPH_OSD_FLAG_SKIPRWLOCKS =   0x10000,  
	CEPH_OSD_FLAG_IGNORE_OVERLAY = 0x20000, 
	CEPH_OSD_FLAG_FLUSH =         0x40000,  
	CEPH_OSD_FLAG_MAP_SNAP_CLONE = 0x80000,  
	CEPH_OSD_FLAG_ENFORCE_SNAPC   = 0x100000,  
	CEPH_OSD_FLAG_REDIRECTED   = 0x200000,  
	CEPH_OSD_FLAG_KNOWN_REDIR = 0x400000,  
	CEPH_OSD_FLAG_FULL_TRY =    0x800000,  
	CEPH_OSD_FLAG_FULL_FORCE = 0x1000000,  
};

enum {
	CEPH_OSD_OP_FLAG_EXCL = 1,      
	CEPH_OSD_OP_FLAG_FAILOK = 2,    
};

#define EOLDSNAPC    ERESTART  
#define EBLACKLISTED ESHUTDOWN 


enum {
	CEPH_OSD_CMPXATTR_OP_NOP = 0,
	CEPH_OSD_CMPXATTR_OP_EQ  = 1,
	CEPH_OSD_CMPXATTR_OP_NE  = 2,
	CEPH_OSD_CMPXATTR_OP_GT  = 3,
	CEPH_OSD_CMPXATTR_OP_GTE = 4,
	CEPH_OSD_CMPXATTR_OP_LT  = 5,
	CEPH_OSD_CMPXATTR_OP_LTE = 6
};

enum {
	CEPH_OSD_CMPXATTR_MODE_STRING = 1,
	CEPH_OSD_CMPXATTR_MODE_U64    = 2
};

enum {
	CEPH_OSD_WATCH_OP_UNWATCH = 0,
	CEPH_OSD_WATCH_OP_LEGACY_WATCH = 1,
	
	CEPH_OSD_WATCH_OP_WATCH = 3,
	CEPH_OSD_WATCH_OP_RECONNECT = 5,
	CEPH_OSD_WATCH_OP_PING = 7,
};

const char *ceph_osd_watch_op_name(int o);


struct ceph_osd_op {
	__le16 op;           
	__le32 flags;        
	union {
		struct {
			__le64 offset, length;
			__le64 truncate_size;
			__le32 truncate_seq;
		} __attribute__ ((packed)) extent;
		struct {
			__le32 name_len;
			__le32 value_len;
			__u8 cmp_op;       
			__u8 cmp_mode;     
		} __attribute__ ((packed)) xattr;
		struct {
			__u8 class_len;
			__u8 method_len;
			__u8 argc;
			__le32 indata_len;
		} __attribute__ ((packed)) cls;
		struct {
			__le64 cookie, count;
		} __attribute__ ((packed)) pgls;
	        struct {
		        __le64 snapid;
	        } __attribute__ ((packed)) snap;
		struct {
			__le64 cookie;
			__le64 ver;     
			__u8 op;	
			__le32 gen;     
		} __attribute__ ((packed)) watch;
		struct {
			__le64 cookie;
		} __attribute__ ((packed)) notify;
		struct {
			__le64 offset, length;
			__le64 src_offset;
		} __attribute__ ((packed)) clonerange;
		struct {
			__le64 expected_object_size;
			__le64 expected_write_size;
		} __attribute__ ((packed)) alloc_hint;
	};
	__le32 payload_len;
} __attribute__ ((packed));


#endif
