/*
 * ImageOutliner.cpp
 *
 *  Created on: 01/07/2015
 *      Author: lucas
 */

#include "ImageOutliner.hpp"

namespace imageOutliner {
	
	ImageOutliner::ImageOutliner(int argc, char* argv[])
	:
			clo(argc, argv)
	{
		// TODO Auto-generated constructor stub
		
	}
	
	ImageOutliner::~ImageOutliner(){
		// TODO Auto-generated destructor stub
	}

	void ImageOutliner::outlineImage(){
		std::string imgPath = clo.vm["input"].as<std::string>();
		ci::CImg<byte> inputImg(imgPath.c_str());
		ci::CImg<byte> newImg(inputImg.width(), inputImg.height(), 1, 4, 0);
		int width = inputImg.width();
		int height = inputImg.height();
		printf("Processing a %dx%d image (%d pixels).\n", width, height, width * height);

		rgba color = {0, 0, 0, 0};
		int borderPixels = 0;
		int backgroundPixels = 0;

		int z = 0;
		for(int y = 0; y < height; y++){ // traverse rows
			for(int x = 0; x < width; x++){ // traverse columns
				for(int c = 0; c < 4; c++){ // traverse channels
					color[c] = *inputImg.data(x, y, z, c); // copy the pixel values for the four channels: r, g, b and a
				}
//				std::memcpy(&color, inputImg.data(x, y, z), sizeof(rgba)); // for some reason memcpy is not working
				bool pixelIsBackgroundColor = isBackgroundColor(color);
				if(pixelIsBackgroundColor){
					backgroundPixels++;
				}
				if(pixelIsBackgroundColor && hasNonBackgroundNeighbour(&inputImg, x, y)){ // if pixel is a background one and at least one neighbour is not a background one
					borderPixels++;
					std::memcpy(&color, &clo.outlineColor, sizeof(rgba)); // clo.outlineColor is an unsigned int (32 bits) and color is a rgba, which is a byte[4] which is a unsigned char[4] (32 bits), hence we can just copy the memory from one to another
//					color[0] = (clo.outlineColor >> 24) & 0x000000FF; // put the red in the least significant byte
//					color[1] = (clo.outlineColor >> 16) & 0x000000FF; // put the green in the least significant byte
//					color[2] = (clo.outlineColor >> 8) & 0x000000FF; // put the blue in the least significant byte
//					color[3] = clo.outlineColor & 0x000000FF; // put the alpha in the least significant byte
					newImg.draw_point(x, y, color);
				}else{
					newImg.draw_point(x, y, color);
				}
			}
		}
		std::cout << backgroundPixels << " pixels where of background color." << std::endl;
		std::cout << borderPixels << " of them where border pixels and where coloured." << std::endl;
		std::string imgOutPath = "";
		if(clo.vm.count("output") > 0){
			imgOutPath = clo.vm["output"].as<std::string>() + ".png";
		}else{
			imgOutPath = imgPath + ".mod.png";
		}
		newImg.save(imgOutPath.c_str());
		std::cout << "generated image " + imgOutPath << std::endl;
	}

	bool ImageOutliner::isBackgroundColor(ci::CImg<byte>* img, int x, int y){
		rgba color = {0, 0, 0, 0};
		for(int c = 0; c < 4; c++){ // traverse channels
			color[c] = *img->data(x, y, 0, c); // copy the pixel values for the four channels: r, g, b and a
		}
		return isBackgroundColor(color);
	}

	bool ImageOutliner::isBackgroundColor(rgba color){ // instead of using rgba wich is a byte[4], it is prefferable to use a byte* so we can get colors from a CImg directly
		bool isBackground = true;
		if(clo.vm.count("background") > 0){ // if no background color was provided check if pixel is equal to the provided background color
			for(int i = 0; i < 4; i++){
				byte channel = (clo.backgroundColor << 8 * i) >> 24; // shift left to get rid of unwanted upper bits, then shift right to position the desired byte in the lower 8 bits.
				byte currentColor = color[i];
				if(currentColor != channel){
					isBackground = false;
					break;
				}
			}
		}else{ // if no background color was not provided just account for the alpha channel
			if(color[3] != 0){
				isBackground = false;
			}
		}

		return isBackground;
	}

	bool ImageOutliner::hasNonBackgroundNeighbour(ci::CImg<byte>* img, int x, int y){
		bool itHas = false;
		// CImg treats the image coordinates starting from up and going downwards. It means that "y == 0" is the topmost column of pixels, whereas "y == height-1" is the bottommost pixel
//		y = img->height() - 1 - y;

		// check up neighbour (colorize down pixel)
		if((clo.bitMask & 128) == 128
		        && y + 1 < img->height()
		        && !isBackgroundColor(img, x, y + 1)){
			itHas = true;

			// check down neighbour (colorize up pixel)
		}else if((clo.bitMask & 8) == 8
		        && y - 1 >= 0
		        && !isBackgroundColor(img, x, y - 1)){
			itHas = true;

			// check right neighbour (colorize left pixel)
		}else if((clo.bitMask & 2) == 2
		        && x + 1 < img->width()
		        && !isBackgroundColor(img, x + 1, y)){
			itHas = true;

			// check left neighbour (colorize right pixel)
		}else if((clo.bitMask & 32) == 32
		        && x - 1 >= 0
		        && !isBackgroundColor(img, x - 1, y)){
			itHas = true;

			// check upright neighbour (colorize downleft pixel)
		}else if((clo.bitMask & 1) == 1
		        && x + 1 < img->width() && y + 1 < img->height()
		        && !isBackgroundColor(img, x + 1, y + 1)){
			itHas = true;

			// check upleft neighbour (colorize downright pixel)
		}else if((clo.bitMask & 64) == 64
		        && x - 1 >= 0 && y + 1 < img->height()
		        && !isBackgroundColor(img, x - 1, y + 1)){
			itHas = true;

			// check downleft neighbour (colorize upright pixel)
		}else if((clo.bitMask & 16) == 16
		        && x - 1 >= 0 && y - 1 >= 0
		        && !isBackgroundColor(img, x - 1, y - 1)){
			itHas = true;

			// check downright neighbour (colorize upleft pixel)
		}else if((clo.bitMask & 4) == 4
		        && x + 1 < img->width() && y - 1 >= 0
		        && !isBackgroundColor(img, x + 1, y - 1)){
			itHas = true;
		}

		return itHas;
	}

} /* namespace randomImageTileGenerator */
