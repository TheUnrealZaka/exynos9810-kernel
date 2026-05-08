

#ifndef __DRM_ENCODER_SLAVE_H__
#define __DRM_ENCODER_SLAVE_H__

#include <drm/drmP.h>
#include <drm/drm_crtc.h>


struct drm_encoder_slave_funcs {
	void (*set_config)(struct drm_encoder *encoder,
			   void *params);

	void (*destroy)(struct drm_encoder *encoder);
	void (*dpms)(struct drm_encoder *encoder, int mode);
	void (*save)(struct drm_encoder *encoder);
	void (*restore)(struct drm_encoder *encoder);
	bool (*mode_fixup)(struct drm_encoder *encoder,
			   const struct drm_display_mode *mode,
			   struct drm_display_mode *adjusted_mode);
	int (*mode_valid)(struct drm_encoder *encoder,
			  struct drm_display_mode *mode);
	void (*mode_set)(struct drm_encoder *encoder,
			 struct drm_display_mode *mode,
			 struct drm_display_mode *adjusted_mode);

	enum drm_connector_status (*detect)(struct drm_encoder *encoder,
					    struct drm_connector *connector);
	int (*get_modes)(struct drm_encoder *encoder,
			 struct drm_connector *connector);
	int (*create_resources)(struct drm_encoder *encoder,
				 struct drm_connector *connector);
	int (*set_property)(struct drm_encoder *encoder,
			    struct drm_connector *connector,
			    struct drm_property *property,
			    uint64_t val);

};


struct drm_encoder_slave {
	struct drm_encoder base;

	const struct drm_encoder_slave_funcs *slave_funcs;
	void *slave_priv;
	void *bus_priv;
};
#define to_encoder_slave(x) container_of((x), struct drm_encoder_slave, base)

int drm_i2c_encoder_init(struct drm_device *dev,
			 struct drm_encoder_slave *encoder,
			 struct i2c_adapter *adap,
			 const struct i2c_board_info *info);



struct drm_i2c_encoder_driver {
	struct i2c_driver i2c_driver;

	int (*encoder_init)(struct i2c_client *client,
			    struct drm_device *dev,
			    struct drm_encoder_slave *encoder);

};
#define to_drm_i2c_encoder_driver(x) container_of((x),			\
						  struct drm_i2c_encoder_driver, \
						  i2c_driver)


static inline struct i2c_client *drm_i2c_encoder_get_client(struct drm_encoder *encoder)
{
	return (struct i2c_client *)to_encoder_slave(encoder)->bus_priv;
}


static inline int drm_i2c_encoder_register(struct module *owner,
					   struct drm_i2c_encoder_driver *driver)
{
	return i2c_register_driver(owner, &driver->i2c_driver);
}


static inline void drm_i2c_encoder_unregister(struct drm_i2c_encoder_driver *driver)
{
	i2c_del_driver(&driver->i2c_driver);
}

void drm_i2c_encoder_destroy(struct drm_encoder *encoder);




void drm_i2c_encoder_dpms(struct drm_encoder *encoder, int mode);
bool drm_i2c_encoder_mode_fixup(struct drm_encoder *encoder,
		const struct drm_display_mode *mode,
		struct drm_display_mode *adjusted_mode);
void drm_i2c_encoder_prepare(struct drm_encoder *encoder);
void drm_i2c_encoder_commit(struct drm_encoder *encoder);
void drm_i2c_encoder_mode_set(struct drm_encoder *encoder,
		struct drm_display_mode *mode,
		struct drm_display_mode *adjusted_mode);
enum drm_connector_status drm_i2c_encoder_detect(struct drm_encoder *encoder,
	    struct drm_connector *connector);
void drm_i2c_encoder_save(struct drm_encoder *encoder);
void drm_i2c_encoder_restore(struct drm_encoder *encoder);


#endif
