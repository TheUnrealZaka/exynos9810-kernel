#ifndef _MATRIX_KEYPAD_H
#define _MATRIX_KEYPAD_H

#include <linux/types.h>
#include <linux/input.h>
#include <linux/of.h>

#define MATRIX_MAX_ROWS		32
#define MATRIX_MAX_COLS		32

#define KEY(row, col, val)	((((row) & (MATRIX_MAX_ROWS - 1)) << 24) |\
				 (((col) & (MATRIX_MAX_COLS - 1)) << 16) |\
				 ((val) & 0xffff))

#define KEY_ROW(k)		(((k) >> 24) & 0xff)
#define KEY_COL(k)		(((k) >> 16) & 0xff)
#define KEY_VAL(k)		((k) & 0xffff)

#define MATRIX_SCAN_CODE(row, col, row_shift)	(((row) << (row_shift)) + (col))


struct matrix_keymap_data {
	const uint32_t *keymap;
	unsigned int	keymap_size;
};


struct matrix_keypad_platform_data {
	const struct matrix_keymap_data *keymap_data;

	const unsigned int *row_gpios;
	const unsigned int *col_gpios;

	unsigned int	num_row_gpios;
	unsigned int	num_col_gpios;

	unsigned int	col_scan_delay_us;

	
	unsigned int	debounce_ms;

	unsigned int	clustered_irq;
	unsigned int	clustered_irq_flags;

	bool		active_low;
	bool		wakeup;
	bool		no_autorepeat;
};

int matrix_keypad_build_keymap(const struct matrix_keymap_data *keymap_data,
			       const char *keymap_name,
			       unsigned int rows, unsigned int cols,
			       unsigned short *keymap,
			       struct input_dev *input_dev);

#ifdef CONFIG_OF

int matrix_keypad_parse_of_params(struct device *dev,
				  unsigned int *rows, unsigned int *cols);
#else
static inline int matrix_keypad_parse_of_params(struct device *dev,
				  unsigned int *rows, unsigned int *cols)
{
	return -ENOSYS;
}
#endif 

#endif 
