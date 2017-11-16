



Initialisation
--------------

Before you can use libbase58 for base58check, you must provide a SHA256
function. The required function signature is:
	bool my_sha256(void *digest, const void *data, size_t datasz)
Simply assign your function to b58_sha256_impl:
	b58_sha256_impl = my_sha256;

This is only required if base58check is used. Raw base58 does not need SHA256.


Decoding Base58
---------------

Simply allocate a buffer to store the binary data in, and set a variable with
the buffer size, and call the b58tobin function:
	bool b58tobin(void *bin, size_t *binsz, const char *b58, size_t b58sz)  
note that a true zero-length base58 string is not supported here.


Validating Base58Check
----------------------

After calling b58tobin, you can validate base58check data using the b58check
function:
	int b58check(const void *bin, size_t binsz, const char *b58, size_t b58sz)
Call it with the same buffers used for b58tobin. If the return value is
negative, an error occurred. Otherwise, the return value is the base58check
"version" byte from the decoded data.


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


Encoding Base58Check
--------------------

Targeting base58check is done similarly to raw base58 encoding, but you must
also provide a version byte:
	bool b58check_enc(char *b58c, size_t *b58c_sz, uint8_t ver,
	                  const void *data, size_t datasz)


### it's visual C++ version
it's not for stand-alone build.
 
linux test:
```
c++ tb.cpp base58_c.cpp -o c
./c
```
sample code:
```c++
char *t=new char[1000];
size_t c=1000;
const unsigned char b[]="uiyiuiuyijiojijjio";  
/* last byte is '\0' coz of b[] dealt with by compiler. 
(size_t)sizeof(b) - 1 as last arg removes it from source binary block. If string related stuff is not used with b there is no '\0' .I mean
 unsigned char z[] = { 0xff, 0xff, 0xff, 0xff };  
.*/   
b58enc(t,&c,(const void *)b,(size_t)sizeof(b));
```
b: 
```
7569796975697579696a696f6a696a6a696f00
```
t:
```
NVhHMCyXNHYRYg5mXw2nSnkLBh
```
