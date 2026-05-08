

#ifndef LIB80211_H
#define LIB80211_H

#include <linux/types.h>
#include <linux/list.h>
#include <linux/atomic.h>
#include <linux/if.h>
#include <linux/skbuff.h>
#include <linux/ieee80211.h>
#include <linux/timer.h>
#include <linux/seq_file.h>

#define NUM_WEP_KEYS	4

enum {
	IEEE80211_CRYPTO_TKIP_COUNTERMEASURES = (1 << 0),
};

struct module;

struct lib80211_crypto_ops {
	const char *name;
	struct list_head list;

	
	void *(*init) (int keyidx);

	
	void (*deinit) (void *priv);

	
	int (*encrypt_mpdu) (struct sk_buff * skb, int hdr_len, void *priv);
	int (*decrypt_mpdu) (struct sk_buff * skb, int hdr_len, void *priv);

	
	int (*encrypt_msdu) (struct sk_buff * skb, int hdr_len, void *priv);
	int (*decrypt_msdu) (struct sk_buff * skb, int keyidx, int hdr_len,
			     void *priv);

	int (*set_key) (void *key, int len, u8 * seq, void *priv);
	int (*get_key) (void *key, int len, u8 * seq, void *priv);

	
	void (*print_stats) (struct seq_file *m, void *priv);

	
	unsigned long (*get_flags) (void *priv);
	unsigned long (*set_flags) (unsigned long flags, void *priv);

	
	int extra_mpdu_prefix_len, extra_mpdu_postfix_len;
	int extra_msdu_prefix_len, extra_msdu_postfix_len;

	struct module *owner;
};

struct lib80211_crypt_data {
	struct list_head list;	
	struct lib80211_crypto_ops *ops;
	void *priv;
	atomic_t refcnt;
};

struct lib80211_crypt_info {
	char *name;
	
	spinlock_t *lock;

	struct lib80211_crypt_data *crypt[NUM_WEP_KEYS];
	int tx_keyidx;		
	struct list_head crypt_deinit_list;
	struct timer_list crypt_deinit_timer;
	int crypt_quiesced;
};

int lib80211_crypt_info_init(struct lib80211_crypt_info *info, char *name,
                                spinlock_t *lock);
void lib80211_crypt_info_free(struct lib80211_crypt_info *info);
int lib80211_register_crypto_ops(struct lib80211_crypto_ops *ops);
int lib80211_unregister_crypto_ops(struct lib80211_crypto_ops *ops);
struct lib80211_crypto_ops *lib80211_get_crypto_ops(const char *name);
void lib80211_crypt_delayed_deinit(struct lib80211_crypt_info *info,
				    struct lib80211_crypt_data **crypt);

#endif 
