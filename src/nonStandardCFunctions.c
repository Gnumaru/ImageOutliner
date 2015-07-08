/*
 * nonStandardFunctions.c
 *
 *  Created on: 8 de jul de 2015
 *      Author: lucas
 */

#include "nonStandardCFunctions.h"

#ifdef __cplusplus
extern "C" { // if we are being compiled for c++, use 'extern "C"'
#endif

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 *
 * http://www.jb.man.ac.uk/~slowe/cpp/itoa.html
 */
char* itoa(int value, char* result, int base){
	// check that the base if valid
	if(base < 2 || base > 36){
		*result = '\0';
		return result;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
//	char* ptr = result;
//	char** ptr1 = result;
//	char* tmp_char;
	int tmp_value;

	do{
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
	}while(value);

	// Apply negative sign
	if(tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr){
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

#ifdef __cplusplus
} // end extern "C" block
#endif
