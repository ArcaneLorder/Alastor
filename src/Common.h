#pragma once




/***************************************************[类型]***************************************************/

//
//typedef unsigned int uint32;
//typedef long long int64;
//typedef unsigned long long uint64;
//typedef unsigned long ulong;
//typedef unsigned short ushort;
//typedef int64 llong;
//typedef uint64 ullong;
//typedef unsigned char uchar;
//typedef int64 timestamp;
//typedef uint32 ptr;

#ifdef ALASTOR_DLL
#define ALASTOR_API __declspec(dllexport)
#else
#define ALASTOR_API
#endif

/***************************************************[宏]***************************************************/
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAKELONGLONG
#define MAKELONGLONG(a, b) ((LONGLONG(DWORD(a) & 0xFFFFFFFF) << 32 ) | LONGLONG(DWORD(b) & 0xFFFFFFFF))
#endif
#ifndef MAKEULONGLONG
#define MAKEULONGLONG(a, b) ((ULONGLONG(DWORD(a) & 0xFFFFFFFF) << 32 ) | ULONGLONG(DWORD(b) & 0xFFFFFFFF))
#endif

//1 => '1' / A => 'A'
#ifndef HEX_TO_CHAR
#define HEX_TO_CHAR(x)	((unsigned char)(x) > 9 ? (unsigned char)(x) -10 + 'a': (unsigned char)(x) + '0')
#endif
//'1' => 1 / 'A' => A
#ifndef CHAR_TO_HEX
#define CHAR_TO_HEX(x)	(isdigit((unsigned char)(x)) ? (unsigned char)(x)-'0' : (unsigned char)(tolower(x))-'a'+10)
#endif

#ifndef IN_RANGE
#define IN_RANGE(pos,begin,size) (((ULONGLONG)pos>=(ULONGLONG)begin) && ((ULONGLONG)pos<=((ULONGLONG)begin+size)))
#endif
#ifndef UP_ALIGN
#define UP_ALIGN(x,y)	((x+y-1)& ~(y-1))
#endif
#ifndef DOWN_ALIGN
#define DOWN_ALIGN(x,y) ((x)& ~(y-1))
#endif

