#ifndef LIBBASE58_H
#define LIBBASE58_H

#include <stdbool.h>
#include <string.h>

extern bool _blkmk_b58tobin(void *bin, size_t binsz, const char *b58, size_t b58sz);
extern int _blkmk_b58check(void *bin, size_t binsz, const char *b58);

#endif
