#include <iostream>
#include "libbase58.h"
#include <cstring>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include "keccak.h"
#include <string>
#include <sstream>
#include <iomanip>


const	std::string bin2hex(const unsigned char *p, size_t length) {
	std::stringstream f;
	f<<std::hex << std::setfill('0');
	for (int i = 0; i < length; i++) f << std::setw(2) << (int)p[i];
	return f.str();
}

size_t hex2bin(unsigned char *p , const char *hexstr,const size_t length) {
	size_t wcount = 0;
	while ( wcount++ < length && *hexstr && *(hexstr + 1)) {    //last condition cause np if check fails on middle one.thats coz of short-circuit evaluation
		sscanf(hexstr, "%2hhx",p++);  //7x slower than tables but doesnt metter 
		hexstr = hexstr+2;
	}
	return  --wcount;     // error check here is a waste  
}	

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
	

	const unsigned char etpubkey[65] = "BurnItAll0000000000000000000000000000000000000000000000000000000";
	unsigned char ethashtag[32] = {};
	unsigned char etaddr[20] = {};

	Keccak keccak256(Keccak::Keccak256);
	hex2bin(ethashtag,keccak256((char *)etpubkey,64).c_str(),32);
	memcpy(etaddr, ethashtag + 12, 20);
	std::string etaddrstring = "0x" + bin2hex(etaddr,20);
	std::cout << "et pubkey :" << std::endl << "0x" + bin2hex(etpubkey,64) << std::endl << "et address:" << std::endl << etaddrstring << std::endl;


	return 0;
}
