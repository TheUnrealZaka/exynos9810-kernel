

#ifndef _IIO_INKERN_H_
#define _IIO_INKERN_H_

struct iio_map;


int iio_map_array_register(struct iio_dev *indio_dev,
			   struct iio_map *map);


int iio_map_array_unregister(struct iio_dev *indio_dev);

#endif
