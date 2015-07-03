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
		ci::CImg<byte> img_normalized(imgPath.c_str());
		ci::CImg<byte> newImg(img_normalized.width(), img_normalized.height(), 1, 4, 0);
		int width = img_normalized.width();
		int height = img_normalized.height();
		printf("Processing a %dx%d image.\n", width, height);

		rgba color = {0, 0, 0, 0};
		int transparents = 0;

		int z = 0;
		for(int y = 0; y < height; y++){ // traverse rows
			for(int x = 0; x < width; x++){ // traverse columns
				for(int c = 0; c < 4; c++){ // traverse channels
					color[c] = *img_normalized.data(x, y, z, c);
				}
				if(color[3] == 0 && hasOpaqueNeighbour(&img_normalized, x, y)){ // if alpha is totally transparent and at least one neighbour is not totally transparent
					transparents++;
					color[0] = (clo.outlineColor >> 24) & 0x000000FF; // put the red in the least significant byte
					color[1] = (clo.outlineColor >> 16) & 0x000000FF; // put the green in the least significant byte
					color[2] = (clo.outlineColor >> 8) & 0x000000FF; // put the blue in the least significant byte
					color[3] = clo.outlineColor & 0x000000FF; // put the alpha in the least significant byte
					newImg.draw_point(x, y, color);
				}else{
					newImg.draw_point(x, y, color);
				}
			}
		}
		std::cout << transparents << " completely transparent pixels where found." << std::endl;
		std::string imgOutPath = "";
		if(clo.vm.count("output") > 0){
			imgOutPath = clo.vm["output"].as<std::string>() + ".png";
		}else{
			imgOutPath = imgPath + ".mod.png";
		}
		newImg.save(imgOutPath.c_str());
		std::cout << "generated image " + imgOutPath << std::endl;
	}

	bool ImageOutliner::hasOpaqueNeighbour(ci::CImg<byte>* img, int x, int y){
		bool itHas = false;
		// CImg treats the image coordinates starting from up and going downwards. It means that "y == 0" is the topmost column of pixels, whereas "y == height-1" is the bottommost pixel
//		y = img->height() - 1 - y;

		// check up neighbour (colorize down pixel)
		if((clo.bitMask & 128) == 128
		        && y + 1 < img->height()
		        && *img->data(x, y + 1, 0, 3) > 0){
			itHas = true;

			// check down neighbour (colorize up pixel)
		}else if((clo.bitMask & 8) == 8
		        && y - 1 >= 0
		        && *img->data(x, y - 1, 0, 3) > 0){
			itHas = true;

			// check right neighbour (colorize left pixel)
		}else if((clo.bitMask & 2) == 2
		        && x + 1 < img->width()
		        && *img->data(x + 1, y, 0, 3) > 0){
			itHas = true;

			// check left neighbour (colorize right pixel)
		}else if((clo.bitMask & 32) == 32
		        && x - 1 >= 0
		        && *img->data(x - 1, y, 0, 3) > 0){
			itHas = true;

			// check upright neighbour (colorize downleft pixel)
		}else if((clo.bitMask & 1) == 1
		        && x + 1 < img->width() && y + 1 < img->height()
		        && *img->data(x + 1, y + 1, 0, 3) > 0){
			itHas = true;

			// check upleft neighbour (colorize downright pixel)
		}else if((clo.bitMask & 64) == 64
		        && x - 1 >= 0 && y + 1 < img->height()
		        && *img->data(x - 1, y + 1, 0, 3) > 0){
			itHas = true;

			// check downleft neighbour (colorize upright pixel)
		}else if((clo.bitMask & 16) == 16
		        && x - 1 >= 0 && y - 1 >= 0
		        && *img->data(x - 1, y - 1, 0, 3) > 0){
			itHas = true;

			// check downright neighbour (colorize upleft pixel)
		}else if((clo.bitMask & 4) == 4
		        && x + 1 < img->width() && y - 1 >= 0
		        && *img->data(x + 1, y - 1, 0, 3) > 0){
			itHas = true;
		}

		return itHas;
	}

} /* namespace randomImageTileGenerator */
