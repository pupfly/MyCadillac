#ifndef _DHT11_
#define	_DHT11_
#define OK 1
#define ERROR 0

extern unsigned char READ_DHT11(int*, int*, int*, int*);
extern unsigned char READ_DHT11_toStr(unsigned char*, unsigned char*);
#endif