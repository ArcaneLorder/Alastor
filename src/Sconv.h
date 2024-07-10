#pragma once

typedef unsigned short  wchar;

#ifdef __cplusplus
extern "C" {
#endif

	// @slen: string lenth, in characters. 
	//        if this parameter is -1, the function processes the entire input string, 
	//          excluding the terminating null character. 
	// @osize: the size of outbuf, in bytes.
	// return : if outbuf is null, return needed buf size, in bytes.
	//          otherwise return the size of converted data filled in outbuf, in bytes.
	int gbk_to_unicode(const char* gbkstr, int slen, wchar* outbuf, int osize);

	// @wlen: string len of wstr, in characters.
	int unicode_to_gbk(const wchar* wstr, int wlen, char* outbuf, int osize);

	int utf8_to_unicode(const char* utf8str, int slen, wchar* outbuf, int osize);
	int unicode_to_utf8(const wchar* wstr, int wlen, char* outbuf, int osize);

#ifdef __cplusplus
}
#endif
