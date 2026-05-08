

#ifndef G10_MPI_H
#define G10_MPI_H

#include <linux/types.h>
#include <linux/scatterlist.h>

#define BYTES_PER_MPI_LIMB	(BITS_PER_LONG / 8)
#define BITS_PER_MPI_LIMB	BITS_PER_LONG

typedef unsigned long int mpi_limb_t;
typedef signed long int mpi_limb_signed_t;

struct gcry_mpi {
	int alloced;		
	int nlimbs;		
	int nbits;		
	int sign;		
	unsigned flags;		
	
	
	mpi_limb_t *d;		
};

typedef struct gcry_mpi *MPI;

#define mpi_get_nlimbs(a)     ((a)->nlimbs)
#define mpi_is_neg(a)	      ((a)->sign)


MPI mpi_alloc(unsigned nlimbs);
MPI mpi_alloc_secure(unsigned nlimbs);
MPI mpi_alloc_like(MPI a);
void mpi_free(MPI a);
int mpi_resize(MPI a, unsigned nlimbs);
int mpi_copy(MPI *copy, const MPI a);
void mpi_clear(MPI a);
int mpi_set(MPI w, MPI u);
int mpi_set_ui(MPI w, ulong u);
MPI mpi_alloc_set_ui(unsigned long u);
void mpi_m_check(MPI a);
void mpi_swap(MPI a, MPI b);


MPI do_encode_md(const void *sha_buffer, unsigned nbits);
MPI mpi_read_raw_data(const void *xbuffer, size_t nbytes);
MPI mpi_read_from_buffer(const void *buffer, unsigned *ret_nread);
MPI mpi_read_raw_from_sgl(struct scatterlist *sgl, unsigned int len);
int mpi_fromstr(MPI val, const char *str);
u32 mpi_get_keyid(MPI a, u32 *keyid);
void *mpi_get_buffer(MPI a, unsigned *nbytes, int *sign);
int mpi_read_buffer(MPI a, uint8_t *buf, unsigned buf_len, unsigned *nbytes,
		    int *sign);
void *mpi_get_secure_buffer(MPI a, unsigned *nbytes, int *sign);
int mpi_write_to_sgl(MPI a, struct scatterlist *sg, unsigned nbytes,
		     int *sign);

#define log_mpidump g10_log_mpidump


int mpi_add_ui(MPI w, MPI u, ulong v);
int mpi_add(MPI w, MPI u, MPI v);
int mpi_addm(MPI w, MPI u, MPI v, MPI m);
int mpi_sub_ui(MPI w, MPI u, ulong v);
int mpi_sub(MPI w, MPI u, MPI v);
int mpi_subm(MPI w, MPI u, MPI v, MPI m);


int mpi_mul_ui(MPI w, MPI u, ulong v);
int mpi_mul_2exp(MPI w, MPI u, ulong cnt);
int mpi_mul(MPI w, MPI u, MPI v);
int mpi_mulm(MPI w, MPI u, MPI v, MPI m);


ulong mpi_fdiv_r_ui(MPI rem, MPI dividend, ulong divisor);
int mpi_fdiv_r(MPI rem, MPI dividend, MPI divisor);
int mpi_fdiv_q(MPI quot, MPI dividend, MPI divisor);
int mpi_fdiv_qr(MPI quot, MPI rem, MPI dividend, MPI divisor);
int mpi_tdiv_r(MPI rem, MPI num, MPI den);
int mpi_tdiv_qr(MPI quot, MPI rem, MPI num, MPI den);
int mpi_tdiv_q_2exp(MPI w, MPI u, unsigned count);
int mpi_divisible_ui(const MPI dividend, ulong divisor);


int mpi_gcd(MPI g, const MPI a, const MPI b);


int mpi_pow(MPI w, MPI u, MPI v);
int mpi_powm(MPI res, MPI base, MPI exp, MPI mod);


int mpi_mulpowm(MPI res, MPI *basearray, MPI *exparray, MPI mod);


int mpi_cmp_ui(MPI u, ulong v);
int mpi_cmp(MPI u, MPI v);


int mpi_getbyte(MPI a, unsigned idx);
void mpi_putbyte(MPI a, unsigned idx, int value);
unsigned mpi_trailing_zeros(MPI a);


void mpi_normalize(MPI a);
unsigned mpi_get_nbits(MPI a);
int mpi_test_bit(MPI a, unsigned n);
int mpi_set_bit(MPI a, unsigned n);
int mpi_set_highbit(MPI a, unsigned n);
void mpi_clear_highbit(MPI a, unsigned n);
void mpi_clear_bit(MPI a, unsigned n);
int mpi_rshift(MPI x, MPI a, unsigned n);


int mpi_invm(MPI x, MPI u, MPI v);




static inline unsigned int mpi_get_size(MPI a)
{
	return a->nlimbs * BYTES_PER_MPI_LIMB;
}
#endif 
