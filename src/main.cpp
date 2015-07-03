/*
 ============================================================================
 Name        : ImageOutliner.cpp
 Author      : lucas
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C++,
 ============================================================================
 */

#ifndef cimg_use_png
#define cimg_use_png
#endif

#ifndef CIMG_USE_PNG
#define CIMG_USE_PNG
#endif


#include <ImageOutliner.hpp>

namespace imo = imageOutliner;

int main(int argc, char* argv[]) {
	imo::ImageOutliner imgOutl(argc, argv);
	imgOutl.outlineImage();
	return 0;
}
