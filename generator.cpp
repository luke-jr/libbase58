#include <iostream>
#include "libbase58.h"
#include <cstring>
#include <openssl/sha.h>
#include <openssl/ripemd.h>

int main()
{	
	char *t = new char[1000];
	char *tbitaddr = new char[1000];
	size_t c = 1000;
	size_t cbit = 1000;
	unsigned char bitaddr[25] = {};
	unsigned char pubhash_md[20] = {};
	unsigned char pubhash_mdprefx[21] = {};

	unsigned char pubhash[32] = {};
	unsigned char hashtag[32] = {};
	unsigned char hashtag_f[32] = {};

	const unsigned char b[66] = "BurnItAll0000000000000000000000000000000000000000000000000000000b";
	SHA256(b, 65, pubhash);
	RIPEMD160(pubhash,32,pubhash_md);
	pubhash_mdprefx[0] = 0x0;
	memcpy(pubhash_mdprefx + 1, pubhash_md , 20);
	SHA256(pubhash_mdprefx, 21, hashtag);
	SHA256(hashtag, 32, hashtag_f);
	bitaddr[0]  = 0x0;
	memcpy(bitaddr + 1, pubhash_md, 20);
	memcpy(bitaddr + 21, hashtag_f, 4);
	b58enc(tbitaddr,&cbit,(void *)bitaddr,(size_t)(sizeof(bitaddr)));
	b58enc(t,&c,(void *)b,(size_t)(sizeof(b)-1));
	std::cout << "pubkey :" << std::endl << t << std::endl << "address:" << std::endl << tbitaddr << std::endl;

	return 0;
}
