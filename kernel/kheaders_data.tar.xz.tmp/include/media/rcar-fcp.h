
#ifndef __MEDIA_RCAR_FCP_H__
#define __MEDIA_RCAR_FCP_H__

struct device_node;
struct rcar_fcp_device;

#if IS_ENABLED(CONFIG_VIDEO_RENESAS_FCP)
struct rcar_fcp_device *rcar_fcp_get(const struct device_node *np);
void rcar_fcp_put(struct rcar_fcp_device *fcp);
int rcar_fcp_enable(struct rcar_fcp_device *fcp);
void rcar_fcp_disable(struct rcar_fcp_device *fcp);
#else
static inline struct rcar_fcp_device *rcar_fcp_get(const struct device_node *np)
{
	return ERR_PTR(-ENOENT);
}
static inline void rcar_fcp_put(struct rcar_fcp_device *fcp) { }
static inline int rcar_fcp_enable(struct rcar_fcp_device *fcp)
{
	return 0;
}
static inline void rcar_fcp_disable(struct rcar_fcp_device *fcp) { }
#endif

#endif 
