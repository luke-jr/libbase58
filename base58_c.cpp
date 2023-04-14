/*
 * Copyright 2012-2014 Luke Dashjr
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the standard MIT license.  See COPYING for more details.
 */



#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include "libbase58.h"
#include <algorithm>

bool (*b58_sha256_impl)(void *, const void *, size_t) = NULL;

static const int8_t b58digits_map[128] = {
	-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
	-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
	-1, 0, 1, 2, 3, 4, 5, 6,  7, 8,-1,-1,-1,-1,-1,-1,
	-1, 9,10,11,12,13,14,15, 16,-1,17,18,19,20,21,-1,
	22,23,24,25,26,27,28,29, 30,31,32,-1,-1,-1,-1,-1,
	-1,33,34,35,36,37,38,39, 40,41,42,43,-1,44,45,46,
	47,48,49,50,51,52,53,54, 55,56,57,-1,-1,-1,-1,-1,
};


bool b58tobin(void *bin /* out */ , size_t *binszp /* in - out */ , const char *b58 /* in */ , size_t b58sz /* in */ )
{
	size_t binsz = *binszp;
	const unsigned char *b58u = (const unsigned char *)b58;
	unsigned char *binu = (unsigned char *)bin;
	size_t outsz = (binsz + 3) / 4;
	uint32_t *out = new uint32_t[outsz]();
	uint64_t t;
	uint32_t c;
	size_t i, j;
	uint8_t bytesleft = binsz % 4;
	uint32_t zeromask = bytesleft == 0 ? 0 : (0xffffffff << (bytesleft * 8));
	unsigned int zerocount = 0;
	
	if (!b58sz)
		b58sz = strlen(b58);
	
	// Leading zeros, just count
	for (i = 0; i < b58sz && b58u[i] == '1'; i++)
		zerocount++;
	
	for ( ; i < b58sz; i++)
	{
		if (b58u[i] & 0x80)
		{
			delete[] out;

			// High-bit set on invalid digit
			return false;
		}
		if (b58digits_map[b58u[i]] == -1)
		{
			delete[] out;

			// Invalid base58 digit
			return false;
		}
		c = (unsigned short)b58digits_map[b58u[i]];
		for (j = outsz - 1; j + 1 > 0; j--)     
		{
			t = out[j] * 58ULL + c;
			c = (t  >> 32) & 0x3fULL;
			out[j] = t & 0xffffffffULL;
		}
		if (c)
		{
			delete[] out;

			// Output number too big (carry to the next int32)
			return false;
		}
		if (out[0] & zeromask)
		{
			// Output number too big (last int32 filled too far)
			delete[] out;
			return false;
		}
	}
	
	j = 0;
	switch (bytesleft) 
	{
		case 3:
			*(binu++) = *((uint8_t*)out + 2);
			/* Fall Through */ 
		case 2:
			*(binu++) = *((uint8_t*)out + 1);
			/* Fall Through */ 
		case 1:
			*(binu++) = *(uint8_t*)out;
			j++;
			break;
		default:
			break;
	}
	
	for (; j < outsz; j++)
	{
		std::reverse((char *)(out + j), (char *)(out + j) + 4);
		*(uint32_t *)binu =  out[j];
		binu = binu + 4;
	}
	delete[] out;
	//size of the result binary,modified that way that the number of leading zeroes in it replaced by the count of leading '1' symbols in given string.
	binu = (unsigned char *)bin;
	for (i = 0; i < binsz; i++)
	{
		if (binu[i])
			break;
		(*binszp)--;
	}
	*binszp = *binszp + zerocount;
	
	return true;
}

static bool my_dblsha256(void *hash, const void *data, size_t datasz)
{
	uint8_t buf[32];
	return b58_sha256_impl(buf, data, datasz) && b58_sha256_impl(hash, buf, sizeof(buf));
}

int b58check(const void *bin, size_t binsz, const char *base58str, size_t b58sz/* for fun */)
{
	unsigned char buf[32];
	const uint8_t *binc = (const uint8_t *)bin;
	unsigned i;
	if (binsz < 4)
		return -4;
	if (!my_dblsha256(buf, bin, binsz - 4))
		return -2;
	if (memcmp(&binc[binsz - 4], buf, 4))
		return -1;
	
	// Check number of zeros is correct AFTER verifying checksum (to avoid possibility of accessing base58str beyond the end)
	for (i = 0; binc[i] == '\0' && base58str[i] == '1'; i++) ; // Just finding the end of zeros, nothing to do in loop
	if (binc[i] == '\0' || base58str[i] == '1')
		return -3;
	
	return binc[0];
}

static const char b58digits_ordered[59] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

bool b58enc(char* b58 /* out */ ,  size_t *b58sz /* in - out */ , const void *data /* in */ , size_t binsz /* in */ )
{
	const uint8_t *bin = (const uint8_t *)data;
	int carry;
	long i, j, high, zcount = 0;	
	while (zcount < binsz && !bin[zcount])
		zcount++;
	const unsigned int size = (binsz - zcount) * 138 / 100 + 1; //latter is a smth like a logarithm of 256 to base 58 , but not exactly.
	unsigned char *buf = new unsigned char[size]();

	high = size - 1;
	for (i = zcount; i < binsz; i++)
	{
		carry = bin[i];	j = size - 1;
		while( carry || j > high )
		{
			carry = carry + 256 * buf[j];
			buf[j--] = carry % 58;  //as you all know 'int fifty_cent() { int j = 0; return j-- ; }' has a zero value      
			carry /= 58;
		}
		high = j;
	}
	
	for (j = 0; j < size && !buf[j]; j++);
	
	if (*b58sz < zcount + size - j + 1)
	{
		*b58sz = zcount + size - j + 1;
		delete[] buf;
		return false;
	}

	if (zcount)
		memset(b58, '1', zcount);

	for (i = zcount; j < size; i++, j++)
		b58[i] = b58digits_ordered[buf[j]];

	delete[] buf;
	b58[i] = '\0';
	*b58sz = i + 1;
	return true;
}
bool b58check_enc(char *b58c /* out */ , size_t *b58c_sz /* in - out */ , uint8_t ver, const void *data /* in */ , size_t datasz /* in */ )
{
	uint8_t *buf = new uint8_t[1 + datasz + 32];
	uint8_t *hash = &buf[1 + datasz];
	
	buf[0] = ver;
	memcpy(&buf[1], data, datasz);
	if (!my_dblsha256(hash, buf, datasz + 1))
	{
		delete[] buf;
		*b58c_sz = 0;
		return false;
	}
	bool c = b58enc(b58c, b58c_sz, buf, 1 + datasz + 4);
	delete[] buf;
	return c;
}
