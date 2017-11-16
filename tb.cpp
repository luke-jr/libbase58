#include <iostream>
#include "libbase58.h"

using namespace std;

main()
{
	
	char *t=new char[1000];
	size_t c=1000;
	const unsigned char b[]="uiyiuiuyijiojijjio";  
/* last byte is '\0' coz of b[] dealt with by compiler. 
(size_t)sizeof(b) - 1 as last arg removes it from source binary block. If string related stuff is not used with b there is no '\0' */   
	b58enc(t,&c,(const void *)b,(size_t)sizeof(b));
	
	cout << t << endl;
	return 0;
}
