#ifndef ABF_BLOOM_H_
#define ABF_BLOOM_H_

#include <stdlib.h>
#include <stdint.h>

typedef uint64_t (*bl_hash_function_t)(uint64_t k0, uint64_t k1, const unsigned char *m, size_t len);

typedef struct bl_bloom_filter bloom_t;

bloom_t *bl_alloc(size_t n_bits, const unsigned int k_hashes,
                  bl_hash_function_t hashfun);
void bl_free(bloom_t *bl);

void bl_add(bloom_t *bl, const unsigned char *value, const size_t len);
int bl_test(bloom_t *bl, const unsigned char *value, const size_t len);

/* returns 0 on success */
int bl_serialize(bloom_t *bl, char **out, size_t *out_len);
bloom_t *bl_deserialize(const char *blob, size_t blob_len, bl_hash_function_t hash_function);


/* If in doubt about what hash function to use,
 * reach for this included siphash implementation */
uint64_t bl_siphash(uint64_t k0, uint64_t k1, const unsigned char *m, size_t len);

#endif
