
#ifndef _HID_SENSORS_HUB_H
#define _HID_SENSORS_HUB_H

#include <linux/hid.h>
#include <linux/hid-sensor-ids.h>
#include <linux/iio/iio.h>
#include <linux/iio/trigger.h>


struct hid_sensor_hub_attribute_info {
	u32 usage_id;
	u32 attrib_id;
	s32 report_id;
	s32 index;
	s32 units;
	s32 unit_expo;
	s32 size;
	s32 logical_minimum;
	s32 logical_maximum;
};


struct sensor_hub_pending {
	bool status;
	struct completion ready;
	u32 usage_id;
	u32 attr_usage_id;
	int raw_size;
	u8  *raw_data;
};


struct hid_sensor_hub_device {
	struct hid_device *hdev;
	u32 vendor_id;
	u32 product_id;
	u32 usage;
	int start_collection_index;
	int end_collection_index;
	struct mutex *mutex_ptr;
	struct sensor_hub_pending pending;
};


struct hid_sensor_hub_callbacks {
	struct platform_device *pdev;
	int (*suspend)(struct hid_sensor_hub_device *hsdev, void *priv);
	int (*resume)(struct hid_sensor_hub_device *hsdev, void *priv);
	int (*capture_sample)(struct hid_sensor_hub_device *hsdev,
			u32 usage_id, size_t raw_len, char *raw_data,
			void *priv);
	int (*send_event)(struct hid_sensor_hub_device *hsdev, u32 usage_id,
			 void *priv);
};


int sensor_hub_device_open(struct hid_sensor_hub_device *hsdev);


void sensor_hub_device_close(struct hid_sensor_hub_device *hsdev);




int sensor_hub_register_callback(struct hid_sensor_hub_device *hsdev,
			u32 usage_id,
			struct hid_sensor_hub_callbacks *usage_callback);


int sensor_hub_remove_callback(struct hid_sensor_hub_device *hsdev,
			u32 usage_id);





int sensor_hub_input_get_attribute_info(struct hid_sensor_hub_device *hsdev,
			u8 type,
			u32 usage_id, u32 attr_usage_id,
			struct hid_sensor_hub_attribute_info *info);



enum sensor_hub_read_flags {
	SENSOR_HUB_SYNC,
	SENSOR_HUB_ASYNC,
};

int sensor_hub_input_attr_get_raw_value(struct hid_sensor_hub_device *hsdev,
 					u32 usage_id,
 					u32 attr_usage_id, u32 report_id,
 					enum sensor_hub_read_flags flag
);


int sensor_hub_set_feature(struct hid_sensor_hub_device *hsdev, u32 report_id,
			   u32 field_index, int buffer_size, void *buffer);


int sensor_hub_get_feature(struct hid_sensor_hub_device *hsdev, u32 report_id,
			   u32 field_index, int buffer_size, void *buffer);




struct hid_sensor_common {
	struct hid_sensor_hub_device *hsdev;
	struct platform_device *pdev;
	unsigned usage_id;
	atomic_t data_ready;
	atomic_t user_requested_state;
	int poll_interval;
	int raw_hystersis;
	struct iio_trigger *trigger;
	struct hid_sensor_hub_attribute_info poll;
	struct hid_sensor_hub_attribute_info report_state;
	struct hid_sensor_hub_attribute_info power_state;
	struct hid_sensor_hub_attribute_info sensitivity;
	struct work_struct work;
};


static inline int hid_sensor_convert_exponent(int unit_expo)
{
	if (unit_expo < 0x08)
		return unit_expo;
	else if (unit_expo <= 0x0f)
		return -(0x0f-unit_expo+1);
	else
		return 0;
}

int hid_sensor_parse_common_attributes(struct hid_sensor_hub_device *hsdev,
					u32 usage_id,
					struct hid_sensor_common *st);
int hid_sensor_write_raw_hyst_value(struct hid_sensor_common *st,
					int val1, int val2);
int hid_sensor_read_raw_hyst_value(struct hid_sensor_common *st,
					int *val1, int *val2);
int hid_sensor_write_samp_freq_value(struct hid_sensor_common *st,
					int val1, int val2);
int hid_sensor_read_samp_freq_value(struct hid_sensor_common *st,
					int *val1, int *val2);

int hid_sensor_get_usage_index(struct hid_sensor_hub_device *hsdev,
				u32 report_id, int field_index, u32 usage_id);

int hid_sensor_format_scale(u32 usage_id,
			    struct hid_sensor_hub_attribute_info *attr_info,
			    int *val0, int *val1);

s32 hid_sensor_read_poll_value(struct hid_sensor_common *st);

#endif
