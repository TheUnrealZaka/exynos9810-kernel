#ifndef _LINUX_TRACE_SEQ_H
#define _LINUX_TRACE_SEQ_H

#include <linux/seq_buf.h>

#include <asm/page.h>



struct trace_seq {
	char			buffer[PAGE_SIZE];
	struct seq_buf		seq;
	int			full;
};

static inline void
trace_seq_init(struct trace_seq *s)
{
	seq_buf_init(&s->seq, s->buffer, PAGE_SIZE);
	s->full = 0;
}


static inline int trace_seq_used(struct trace_seq *s)
{
	return seq_buf_used(&s->seq);
}


static inline char *
trace_seq_buffer_ptr(struct trace_seq *s)
{
	return s->buffer + seq_buf_used(&s->seq);
}


static inline bool trace_seq_has_overflowed(struct trace_seq *s)
{
	return s->full || seq_buf_has_overflowed(&s->seq);
}


#ifdef CONFIG_TRACING
extern __printf(2, 3)
void trace_seq_printf(struct trace_seq *s, const char *fmt, ...);
extern __printf(2, 0)
void trace_seq_vprintf(struct trace_seq *s, const char *fmt, va_list args);
extern void
trace_seq_bprintf(struct trace_seq *s, const char *fmt, const u32 *binary);
extern int trace_print_seq(struct seq_file *m, struct trace_seq *s);
extern int trace_seq_to_user(struct trace_seq *s, char __user *ubuf,
			     int cnt);
extern void trace_seq_puts(struct trace_seq *s, const char *str);
extern void trace_seq_putc(struct trace_seq *s, unsigned char c);
extern void trace_seq_putmem(struct trace_seq *s, const void *mem, unsigned int len);
extern void trace_seq_putmem_hex(struct trace_seq *s, const void *mem,
				unsigned int len);
extern int trace_seq_path(struct trace_seq *s, const struct path *path);

extern void trace_seq_bitmask(struct trace_seq *s, const unsigned long *maskp,
			     int nmaskbits);

#else 
static inline void trace_seq_printf(struct trace_seq *s, const char *fmt, ...)
{
}
static inline void
trace_seq_bprintf(struct trace_seq *s, const char *fmt, const u32 *binary)
{
}

static inline void
trace_seq_bitmask(struct trace_seq *s, const unsigned long *maskp,
		  int nmaskbits)
{
}

static inline int trace_print_seq(struct seq_file *m, struct trace_seq *s)
{
	return 0;
}
static inline int trace_seq_to_user(struct trace_seq *s, char __user *ubuf,
				    int cnt)
{
	return 0;
}
static inline void trace_seq_puts(struct trace_seq *s, const char *str)
{
}
static inline void trace_seq_putc(struct trace_seq *s, unsigned char c)
{
}
static inline void
trace_seq_putmem(struct trace_seq *s, const void *mem, unsigned int len)
{
}
static inline void trace_seq_putmem_hex(struct trace_seq *s, const void *mem,
				       unsigned int len)
{
}
static inline int trace_seq_path(struct trace_seq *s, const struct path *path)
{
	return 0;
}
#endif 

#endif 
