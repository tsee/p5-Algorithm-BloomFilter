#include "bloom.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct bl_bloom_filter {
  char *bitmap;
  bl_hash_function_t hash_function;
  unsigned int k;
  unsigned int significant_bits;
  uint64_t shift;
};

/* round up to nearest power of 2 - not efficient, but who cares? */
static inline uint64_t
S_which_power_of_two(uint64_t n)
{
  uint64_t power = 1;
  uint64_t num = 1;
  while(num < n) {
    ++power;
    num *=2;
  }
  return power;
}

bloom_t *
bl_alloc(const size_t n_bits, const unsigned int k_hashes,
         bl_hash_function_t hashfun)
{
  size_t nbytes;

  bloom_t *bl = malloc(sizeof(bloom_t));
  if (!bl)
    return NULL;

  bl->significant_bits = (unsigned int)S_which_power_of_two(n_bits);
  bl->shift = 64 - bl->significant_bits;

  nbytes = (1ll << bl->significant_bits) / 8ll + 1ll;

  bl->bitmap = calloc(sizeof(char), nbytes);
  if (!(bl->bitmap)) {
    free(bl);
    return NULL;
  }

  bl->hash_function = hashfun;
  bl->k = k_hashes;

  return bl;
}


void
bl_free(bloom_t *bl)
{
  free(bl);
}


void
bl_add(bloom_t *bl, const char *value, const size_t len)
{
  unsigned int i;
  const unsigned int k = bl->k;
  char *bitfield = bl->bitmap;

  for (i = 0; i < k; ++i)
  {
    uint64_t l = bl_siphash((uint64_t)i, 0, value, len);
    l >>= bl->shift;
    bitfield[l / 8] |= 1 << (l % 8);
  }
}


int
bl_test(bloom_t *bl, const char * value, const size_t len)
{
  unsigned int i;
  const unsigned int k = bl->k;
  char *bitfield = bl->bitmap;

  for (i = 0; i < k; ++i)
  {
    uint64_t l = bl_siphash((uint64_t)i, 0, value, len);
    l >>= bl->shift;
    if (! (bitfield[l / 8] & (1 << (l % 8))) )
      return 0;
  }

  return 1;
}


/* Floodyberry's public-domain siphash: https://github.com/floodyberry/siphash */
static inline uint64_t
U8TO64_LE(const unsigned char *p)
{
  return *(const uint64_t *) p;
}

#define ROTL64(a,b) (((a)<<(b))|((a)>>(64-b)))

uint64_t
bl_siphash(uint64_t k0, uint64_t k1, const unsigned char *m, size_t len)
{
  uint64_t v0, v1, v2, v3;
  uint64_t mi;
  uint64_t last7;
  size_t i, blocks;

  v0 = k0 ^ 0x736f6d6570736575ull;
  v1 = k1 ^ 0x646f72616e646f6dull;
  v2 = k0 ^ 0x6c7967656e657261ull;
  v3 = k1 ^ 0x7465646279746573ull;

  last7 = (uint64_t) (len & 0xff) << 56;

#define sipcompress() \
  do { \
    v0 += v1; v2 += v3; \
    v1 = ROTL64(v1,13);  v3 = ROTL64(v3,16); \
    v1 ^= v0; v3 ^= v2; \
    v0 = ROTL64(v0,32); \
    v2 += v1; v0 += v3; \
    v1 = ROTL64(v1,17); v3 = ROTL64(v3,21); \
    v1 ^= v2; v3 ^= v0; \
    v2 = ROTL64(v2,32); \
  } while (0)

  for (i = 0, blocks = (len & ~7); i < blocks; i += 8) {
    mi = U8TO64_LE(m + i);
    v3 ^= mi;
    sipcompress();
    sipcompress();
    v0 ^= mi;
  }

  switch (len - blocks) {
    case 7:
      last7 |= (uint64_t) m[i + 6] << 48;
    case 6:
      last7 |= (uint64_t) m[i + 5] << 40;
    case 5:
      last7 |= (uint64_t) m[i + 4] << 32;
    case 4:
      last7 |= (uint64_t) m[i + 3] << 24;
    case 3:
      last7 |= (uint64_t) m[i + 2] << 16;
    case 2:
      last7 |= (uint64_t) m[i + 1] << 8;
    case 1:
      last7 |= (uint64_t) m[i + 0];
    case 0:
    default:;
  };
  v3 ^= last7;
  sipcompress();
  sipcompress();
  v0 ^= last7;
  v2 ^= 0xff;
  sipcompress();
  sipcompress();
  sipcompress();
  sipcompress();
  return v0 ^ v1 ^ v2 ^ v3;
}

