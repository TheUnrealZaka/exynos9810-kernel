#ifndef __NET_REGULATORY_H
#define __NET_REGULATORY_H


#include <linux/rcupdate.h>


enum environment_cap {
	ENVIRON_ANY,
	ENVIRON_INDOOR,
	ENVIRON_OUTDOOR,
};


struct regulatory_request {
	struct rcu_head rcu_head;
	int wiphy_idx;
	enum nl80211_reg_initiator initiator;
	enum nl80211_user_reg_hint_type user_reg_hint_type;
	char alpha2[3];
	enum nl80211_dfs_regions dfs_region;
	bool intersect;
	bool processed;
	enum environment_cap country_ie_env;
	struct list_head list;
};


enum ieee80211_regulatory_flags {
	REGULATORY_CUSTOM_REG			= BIT(0),
	REGULATORY_STRICT_REG			= BIT(1),
	REGULATORY_DISABLE_BEACON_HINTS		= BIT(2),
	REGULATORY_COUNTRY_IE_FOLLOW_POWER	= BIT(3),
	REGULATORY_COUNTRY_IE_IGNORE		= BIT(4),
	REGULATORY_ENABLE_RELAX_NO_IR           = BIT(5),
	REGULATORY_IGNORE_STALE_KICKOFF         = BIT(6),
	REGULATORY_WIPHY_SELF_MANAGED		= BIT(7),
};

struct ieee80211_freq_range {
	u32 start_freq_khz;
	u32 end_freq_khz;
	u32 max_bandwidth_khz;
};

struct ieee80211_power_rule {
	u32 max_antenna_gain;
	u32 max_eirp;
};

struct ieee80211_reg_rule {
	struct ieee80211_freq_range freq_range;
	struct ieee80211_power_rule power_rule;
	u32 flags;
	u32 dfs_cac_ms;
};

struct ieee80211_regdomain {
	struct rcu_head rcu_head;
	u32 n_reg_rules;
	char alpha2[3];
	enum nl80211_dfs_regions dfs_region;
	struct ieee80211_reg_rule reg_rules[];
};

#define MHZ_TO_KHZ(freq) ((freq) * 1000)
#define KHZ_TO_MHZ(freq) ((freq) / 1000)
#define DBI_TO_MBI(gain) ((gain) * 100)
#define MBI_TO_DBI(gain) ((gain) / 100)
#define DBM_TO_MBM(gain) ((gain) * 100)
#define MBM_TO_DBM(gain) ((gain) / 100)

#define REG_RULE_EXT(start, end, bw, gain, eirp, dfs_cac, reg_flags)	\
{									\
	.freq_range.start_freq_khz = MHZ_TO_KHZ(start),			\
	.freq_range.end_freq_khz = MHZ_TO_KHZ(end),			\
	.freq_range.max_bandwidth_khz = MHZ_TO_KHZ(bw),			\
	.power_rule.max_antenna_gain = DBI_TO_MBI(gain),		\
	.power_rule.max_eirp = DBM_TO_MBM(eirp),			\
	.flags = reg_flags,						\
	.dfs_cac_ms = dfs_cac,						\
}

#define REG_RULE(start, end, bw, gain, eirp, reg_flags) \
	REG_RULE_EXT(start, end, bw, gain, eirp, 0, reg_flags)

#endif
