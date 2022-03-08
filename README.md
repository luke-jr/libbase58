
Decoding Base58
---------------

Simply allocate a buffer to store the binary data in, and set a variable with
the buffer size, and call the b58tobin function:
	bool b58tobin(void *bin, size_t *binsz, const char *b58, size_t b58sz)  
note that a true zero-length base58 string is not supported here.


Encoding Base58
---------------

Allocate a string to store the base58 content, create a size_t variable with the
size of that allocation, and call:
	bool b58enc(char *b58, size_t *b58sz, const void *data, size_t binsz)
Note that you must pass a pointer to the string size variable, not the size
itself. When b58enc returns, the variable will be modified to contain the actual
number of bytes used (including the null terminator). If encoding fails for any
reason, or if the string buffer is not large enough for the result, b58enc will
return false. Otherwise, it returns true to indicate success.
  

check stuff is not really needed by anyone

### it's visual C++ version
works in any os with `gcc/clang`. it will not work with args which bump internals or out vars out of `LONG_MAX` and/or `SIZE_MAX`.
  
only for devices with LSB memory model
 
linux test:
```
c++ tb.cpp base58_c.cpp -o c
./c
```
sample code:
```c++
#include <iostream>
#include <iomanip>
#include "libbase58.h"

int main()
{
	
	char *t = new char[1000]();
	size_t c = 1000;
	const unsigned char b[19] = "uiyiuiuyijiojijjio";  /* last byte is '\0' coz of b[] dealt with by compiler. 
(size_t)sizeof(b) - 1 as last arg removes it from source binary block. */   
	b58enc(t, &c, (void *)b, (size_t)sizeof(b));
	
	std::cout << t << std::endl;
	unsigned char bc[250] = {};
	size_t wq = 250;
	bool c5 = b58tobin((void *)bc, &wq, t, strlen(t));
	c5 = b58tobin((void *)bc, &wq, t, strlen(t));
	
	std::cout << bc << std::endl << std::endl << wq ;
}
```
b: 
```
7569796975697579696a696f6a696a6a696f00
```
t:
```
NVhHMCyXNHYRYg5mXw2nSnkLBh
```



 # generator 
  
 linux:  
 ```
c++ generator.cpp base58_c.cpp keccak.cpp -o c -lcrypto
./c
 ```
