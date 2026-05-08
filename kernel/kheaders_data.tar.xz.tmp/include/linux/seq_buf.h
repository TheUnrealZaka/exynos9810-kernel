#ifndef _LINUX_SEQ_BUF_H
#define _LINUX_SEQ_BUF_H

#include <linux/fs.h>




struct seq_buf {
	char			*buffer;
	size_t			size;
	size_t			len;
	loff_t			readpos;
};

static inline void seq_buf_clear(struct seq_buf *s)
{
	s->len = 0;
	s->readpos = 0;
}

static inline void
seq_buf_init(struct seq_buf *s, char *buf, unsigned int size)
{
	s->buffer = buf;
	s->size = size;
	seq_buf_clear(s);
}


static inline bool
seq_buf_has_overflowed(struct seq_buf *s)
{
	return s->len > s->size;
}

static inline void
seq_buf_set_overflow(struct seq_buf *s)
{
	s->len = s->size + 1;
}


static inline unsigned int
seq_buf_buffer_left(struct seq_buf *s)
{
	if (seq_buf_has_overflowed(s))
		return 0;

	return s->size - s->len;
}


static inline unsigned int seq_buf_used(struct seq_buf *s)
{
	return min(s->len, s->size);
}


static inline size_t seq_buf_get_buf(struct seq_buf *s, char **bufp)
{
	WARN_ON(s->len > s->size + 1);

	if (s->len < s->size) {
		*bufp = s->buffer + s->len;
		return s->size - s->len;
	}

	*bufp = NULL;
	return 0;
}


static inline void seq_buf_commit(struct seq_buf *s, int num)
{
	if (num < 0) {
		seq_buf_set_overflow(s);
	} else {
		
		BUG_ON(s->len + num > s->size);
		s->len += num;
	}
}

extern __printf(2, 3)
int seq_buf_printf(struct seq_buf *s, const char *fmt, ...);
extern __printf(2, 0)
int seq_buf_vprintf(struct seq_buf *s, const char *fmt, va_list args);
extern int seq_buf_print_seq(struct seq_file *m, struct seq_buf *s);
extern int seq_buf_to_user(struct seq_buf *s, char __user *ubuf,
			   int cnt);
extern int seq_buf_puts(struct seq_buf *s, const char *str);
extern int seq_buf_putc(struct seq_buf *s, unsigned char c);
extern int seq_buf_putmem(struct seq_buf *s, const void *mem, unsigned int len);
extern int seq_buf_putmem_hex(struct seq_buf *s, const void *mem,
			      unsigned int len);
extern int seq_buf_path(struct seq_buf *s, const struct path *path, const char *esc);

#ifdef CONFIG_BINARY_PRINTF
extern int
seq_buf_bprintf(struct seq_buf *s, const char *fmt, const u32 *binary);
#endif

#endif 
