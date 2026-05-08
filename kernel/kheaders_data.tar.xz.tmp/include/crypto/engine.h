
#ifndef _CRYPTO_ENGINE_H
#define _CRYPTO_ENGINE_H

#include <linux/crypto.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <crypto/algapi.h>
#include <crypto/hash.h>

#define ENGINE_NAME_LEN	30

struct crypto_engine {
	char			name[ENGINE_NAME_LEN];
	bool			idling;
	bool			busy;
	bool			running;
	bool			cur_req_prepared;

	struct list_head	list;
	spinlock_t		queue_lock;
	struct crypto_queue	queue;

	bool			rt;

	int (*prepare_crypt_hardware)(struct crypto_engine *engine);
	int (*unprepare_crypt_hardware)(struct crypto_engine *engine);

	int (*prepare_cipher_request)(struct crypto_engine *engine,
				      struct ablkcipher_request *req);
	int (*unprepare_cipher_request)(struct crypto_engine *engine,
					struct ablkcipher_request *req);
	int (*prepare_hash_request)(struct crypto_engine *engine,
				    struct ahash_request *req);
	int (*unprepare_hash_request)(struct crypto_engine *engine,
				      struct ahash_request *req);
	int (*cipher_one_request)(struct crypto_engine *engine,
				  struct ablkcipher_request *req);
	int (*hash_one_request)(struct crypto_engine *engine,
				struct ahash_request *req);

	struct kthread_worker           kworker;
	struct task_struct              *kworker_task;
	struct kthread_work             pump_requests;

	void				*priv_data;
	struct crypto_async_request	*cur_req;
};

int crypto_transfer_cipher_request(struct crypto_engine *engine,
				   struct ablkcipher_request *req,
				   bool need_pump);
int crypto_transfer_cipher_request_to_engine(struct crypto_engine *engine,
					     struct ablkcipher_request *req);
int crypto_transfer_hash_request(struct crypto_engine *engine,
				 struct ahash_request *req, bool need_pump);
int crypto_transfer_hash_request_to_engine(struct crypto_engine *engine,
					   struct ahash_request *req);
void crypto_finalize_cipher_request(struct crypto_engine *engine,
				    struct ablkcipher_request *req, int err);
void crypto_finalize_hash_request(struct crypto_engine *engine,
				  struct ahash_request *req, int err);
int crypto_engine_start(struct crypto_engine *engine);
int crypto_engine_stop(struct crypto_engine *engine);
struct crypto_engine *crypto_engine_alloc_init(struct device *dev, bool rt);
int crypto_engine_exit(struct crypto_engine *engine);

#endif 
