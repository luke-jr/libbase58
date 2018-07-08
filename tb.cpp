#include <iostream>
#include "libbase58.h"

using namespace std;

int main()
{
	
	char *t=new char[1000];
	size_t c=1000;
	const unsigned char b[19]="uiyiuiuyijiojijjio";  /* last byte is '\0' coz of b[] dealt with by compiler. 
(size_t)sizeof(b) - 1 as last arg removes it from source binary block. If string related stuff is not used with b there is no '\0' */   
	b58enc(t,&c,(void *)b,(size_t)sizeof(b));
	
	cout << t << endl;
	unsigned char bc[250]={};
	size_t wq=250;
	bool c5 = b58tobin((void *)bc, &wq, t , strlen(t));
	c5 = b58tobin((void *)bc, &wq, t , strlen(t));
	
	cout << bc << endl << endl<< wq ;
	
	return 0;
}
