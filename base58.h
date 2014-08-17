#ifndef LIBBASE58_H
#define LIBBASE58_H

#include <stdbool.h>
#include <stddef.h>

extern bool (*b58_sha256_impl)(void *, const void *, size_t);

extern bool b58tobin(void *bin, size_t binsz, const char *b58, size_t b58sz);
extern int b58check(const void *bin, size_t binsz, const char *b58, size_t b58sz);

extern bool b58enc(char *b58, size_t *b58sz, const void *bin, size_t binsz);

#endif
