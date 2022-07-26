
#ifndef HEADER_SM3_H
#define HEADER_SM3_H

#include <stddef.h>
#include <string.h>

#define SM3_A 0x7380166fUL
#define SM3_B 0x4914b2b9UL
#define SM3_C 0x172442d7UL
#define SM3_D 0xda8a0600UL
#define SM3_E 0xa96f30bcUL
#define SM3_F 0x163138aaUL
#define SM3_G 0xe38dee4dUL
#define SM3_H 0xb0fb0e4eUL

# define SM3_WORD       unsigned int
# define SM3_CBLOCK     64
# define SM3_LBLOCK     (SM3_CBLOCK/4)

#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))

# define HOST_c2l(c,l)  (l =(((unsigned long)(*((c)++)))<<24),          \
                         l|=(((unsigned long)(*((c)++)))<<16),          \
                         l|=(((unsigned long)(*((c)++)))<< 8),          \
                         l|=(((unsigned long)(*((c)++)))    )           )
# define HOST_l2c(l,c)  (*((c)++)=(unsigned char)(((l)>>24)&0xff),      \
                         *((c)++)=(unsigned char)(((l)>>16)&0xff),      \
                         *((c)++)=(unsigned char)(((l)>> 8)&0xff),      \
                         *((c)++)=(unsigned char)(((l)    )&0xff),      \
                         l)

#define HASH_MAKE_STRING(c, s)              \
      do {                                  \
        unsigned long ll;                   \
        ll=(c)->A; (void)HOST_l2c(ll, (s)); \
        ll=(c)->B; (void)HOST_l2c(ll, (s)); \
        ll=(c)->C; (void)HOST_l2c(ll, (s)); \
        ll=(c)->D; (void)HOST_l2c(ll, (s)); \
        ll=(c)->E; (void)HOST_l2c(ll, (s)); \
        ll=(c)->F; (void)HOST_l2c(ll, (s)); \
        ll=(c)->G; (void)HOST_l2c(ll, (s)); \
        ll=(c)->H; (void)HOST_l2c(ll, (s)); \
      } while (0)

#define P0(X) (X ^ ROTATE(X, 9) ^ ROTATE(X, 17))
#define P1(X) (X ^ ROTATE(X, 15) ^ ROTATE(X, 23))

#define FF0(X,Y,Z) (X ^ Y ^ Z)
#define GG0(X,Y,Z) (X ^ Y ^ Z)

#define FF1(X,Y,Z) ((X & Y) | ((X | Y) & Z))
#define GG1(X,Y,Z) ((Z ^ (X & (Y ^ Z))))

#define EXPAND(W0,W7,W13,W3,W10) \
   (P1(W0 ^ W7 ^ ROTATE(W13, 15)) ^ ROTATE(W3, 7) ^ W10)

#define RND(A, B, C, D, E, F, G, H, TJ, Wi, Wj, FF, GG)           \
     do {                                                         \
       const SM3_WORD A12 = ROTATE(A, 12);                        \
       const SM3_WORD A12_SM = A12 + E + TJ;                      \
       const SM3_WORD SS1 = ROTATE(A12_SM, 7);                    \
       const SM3_WORD TT1 = FF(A, B, C) + D + (SS1 ^ A12) + (Wj); \
       const SM3_WORD TT2 = GG(E, F, G) + H + SS1 + Wi;           \
       B = ROTATE(B, 9);                                          \
       D = TT1;                                                   \
       F = ROTATE(F, 19);                                         \
       H = P0(TT2);                                               \
     } while(0)

#define R1(A,B,C,D,E,F,G,H,TJ,Wi,Wj) \
   RND(A,B,C,D,E,F,G,H,TJ,Wi,Wj,FF0,GG0)

#define R2(A,B,C,D,E,F,G,H,TJ,Wi,Wj) \
   RND(A,B,C,D,E,F,G,H,TJ,Wi,Wj,FF1,GG1)

typedef struct SM3state_st {
   SM3_WORD A, B, C, D, E, F, G, H;
   SM3_WORD Nl, Nh;
   SM3_WORD data[SM3_LBLOCK];
   unsigned int num;
} SM3_CTX;

int sm3_init(SM3_CTX *c);
int sm3_update(SM3_CTX *c, const void *data, size_t len);
int sm3_final(unsigned char *md, SM3_CTX *c);

#endif
