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
		std::string imgPath = clo.input;
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
				bool pixelIsBackground = isBackgroundColor(color) || isWithinBackgroundColorTreshold(color);
				if(pixelIsBackground){
					backgroundPixels++;
				}
				if(pixelIsBackground && hasNonBackgroundNeighbour(&inputImg, x, y)){ // if pixel is a background one and at least one neighbour is not a background one
					borderPixels++;
//					std::memcpy(&color, &clo.outlineColor, sizeof(byte) * 4); // memcpy is just not working, even though it should, leaving here just so anyone knows that
					color[0] = clo.outlineColor >> 24; // put the red in the least significant byte
					color[1] = (clo.outlineColor << 8) >> 24; // put the green in the least significant byte
					color[2] = (clo.outlineColor << 16) >> 24; // put the blue in the least significant byte
					color[3] = (clo.outlineColor << 24) >> 24; // put the alpha in the least significant byte
					newImg.draw_point(x, y, color);
				}else{
					newImg.draw_point(x, y, color);
				}
			}
		}
		std::cout << backgroundPixels << " pixels where of background color." << std::endl;
		std::cout << borderPixels << " of them where border pixels and where coloured." << std::endl;
		std::string imgOutPath = "";
		newImg.save(clo.output.c_str());
		std::cout << "generated image " + imgOutPath << std::endl;
	}

	bool ImageOutliner::isStrictBackgroundColor(ci::CImg<byte>* img, int x, int y){
		rgba color = {0, 0, 0, 0};
		for(int c = 0; c < 4; c++){ // traverse channels
			color[c] = *img->data(x, y, 0, c); // copy the pixel values for the four channels: r, g, b and a
		}
		return isBackgroundColor(color);
	}

	byte ImageOutliner::getChannelValue(uint colorAsUInt32, char channel){
		byte channelValue = 0;
		if(channel == 'r'){
			channelValue = getChannelValue(colorAsUInt32, (uint) 0);
		}else if(channel == 'g'){
			channelValue = getChannelValue(colorAsUInt32, (uint) 1);
		}else if(channel == 'b'){
			channelValue = getChannelValue(colorAsUInt32, (uint) 2);
		}else if(channel == 'a'){
			channelValue = getChannelValue(colorAsUInt32, (uint) 3);
		}else{
			throw std::invalid_argument("\"channel\" should be one of the folowing lowercase characters: 'r', 'g', 'b' or 'a'");
		}
		return channelValue;
	}

	bool ImageOutliner::isWithinBackgroundColorTreshold(rgba color){
		uint deviance = 0;
		for(int i = 0; i < 4; i++){
			deviance += std::abs(color[i] - getChannelValue(clo.backgroundColor, (uint) i));
		}
		return deviance <= clo.backgroundColorMaxDeviance;
	}

	byte ImageOutliner::getChannelValue(uint colorAsUInt32, uint channelIndex){
		if(channelIndex > 3){
			throw std::invalid_argument("\"channel\" should be one 0, 1, 2 or 3.");
		}
		return (colorAsUInt32 << 8 * channelIndex) >> 24;
	}

	bool ImageOutliner::isBackgroundColor(rgba color){ // instead of using rgba wich is a byte[4], it is prefferable to use a byte* so we can get colors from a CImg directly
		bool isBackground = true;
		if(clo.vm.count("background") > 0){ // if no background color was provided check if pixel is equal to the provided background color
			for(int i = 0; i < 4; i++){
				byte channel = getChannelValue(clo.backgroundColor, (uint) i);
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
		        && !isStrictBackgroundColor(img, x, y + 1)){
			itHas = true;

			// check down neighbour (colorize up pixel)
		}else if((clo.bitMask & 8) == 8
		        && y - 1 >= 0
		        && !isStrictBackgroundColor(img, x, y - 1)){
			itHas = true;

			// check right neighbour (colorize left pixel)
		}else if((clo.bitMask & 2) == 2
		        && x + 1 < img->width()
		        && !isStrictBackgroundColor(img, x + 1, y)){
			itHas = true;

			// check left neighbour (colorize right pixel)
		}else if((clo.bitMask & 32) == 32
		        && x - 1 >= 0
		        && !isStrictBackgroundColor(img, x - 1, y)){
			itHas = true;

			// check upright neighbour (colorize downleft pixel)
		}else if((clo.bitMask & 1) == 1
		        && x + 1 < img->width() && y + 1 < img->height()
		        && !isStrictBackgroundColor(img, x + 1, y + 1)){
			itHas = true;

			// check upleft neighbour (colorize downright pixel)
		}else if((clo.bitMask & 64) == 64
		        && x - 1 >= 0 && y + 1 < img->height()
		        && !isStrictBackgroundColor(img, x - 1, y + 1)){
			itHas = true;

			// check downleft neighbour (colorize upright pixel)
		}else if((clo.bitMask & 16) == 16
		        && x - 1 >= 0 && y - 1 >= 0
		        && !isStrictBackgroundColor(img, x - 1, y - 1)){
			itHas = true;

			// check downright neighbour (colorize upleft pixel)
		}else if((clo.bitMask & 4) == 4
		        && x + 1 < img->width() && y - 1 >= 0
		        && !isStrictBackgroundColor(img, x + 1, y - 1)){
			itHas = true;
		}

		return itHas;
	}

} /* namespace randomImageTileGenerator */
