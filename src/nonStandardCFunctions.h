/*
 * nonStandardFunctions.h
 *
 *  Created on: 8 de jul de 2015
 *      Author: lucas
 */

#ifndef NONSTANDARDCFUNCTIONS_H_
#define NONSTANDARDCFUNCTIONS_H_

#ifdef __cplusplus
extern "C" { // if we are being compiled for c++, use 'extern "C"'
#endif

	char* itoa(int value, char* result, int base);

#ifdef __cplusplus
} // end extern "C" block
#endif

#endif /* NONSTANDARDCFUNCTIONS_H_ */
