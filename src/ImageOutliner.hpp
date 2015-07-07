/*
 * ImageOutliner.hpp
 *
 *  Created on: 01/07/2015
 *      Author: lucas
 */

#ifndef IMAGEOUTLINER_HPP_
#define IMAGEOUTLINER_HPP_
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <CImg.h>
#include <cstdio>

#include "Types.h"
#include "CommandLineOptionsBoost.hpp"

namespace po = boost::program_options;
namespace ci = cimg_library;

namespace imageOutliner {
	
	/*
	 *
	 */
	class ImageOutliner{
		public:
			ImageOutliner(int argc, char* argv[]);
			virtual ~ImageOutliner();
			void outlineImage();

		private:
			// CONSTRUCTORS


			// FIELDS
			CommandLineOptionsBoost clo;

			// METHODS
			bool hasNonBackgroundNeighbour(ci::CImg<byte>* img, int x, int y);
			bool isBackgroundColor(rgba color);
			bool isStrictBackgroundColor(ci::CImg<byte>* img, int x, int y);
			bool isWithinBackgroundColorTreshold(rgba color);
			byte getChannelValue(uint colorAsUInt32, char channel);
			byte getChannelValue(uint colorAsUInt32, uint channel);
	};

} /* namespace randomImageTileGenerator */

#endif /* IMAGEOUTLINER_HPP_ */
