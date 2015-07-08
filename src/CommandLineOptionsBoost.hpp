/*
 * CommandLineOptionsBoost.h
 *
 *  Created on: 18/06/2015
 *      Author: lucas
 */

#ifndef COMMANDLINEOPTIONSBOOST_HPP_
#define COMMANDLINEOPTIONSBOOST_HPP_

#include "Types.h"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>


namespace po = boost::program_options;
//using namespace std;

namespace imageOutliner {
	
	/*
	 *
	 */
	class CommandLineOptionsBoost{
		public:
			CommandLineOptionsBoost(int argc, char* argv[]);
			virtual ~CommandLineOptionsBoost();

			// FIELDS
			po::variables_map vm;
			byte bitMask;
			uint outlineColor;
			uint backgroundColor;
			uint backgroundColorMaxDeviance;
			std::string output;
			std::string input;
		private:
			// CONSTRUCTORS

			// FIELDS

			int argc;
			char** argv; // char* argv[];
			po::options_description* desc;
			po::basic_parsed_options<char>* bpo;

			// METHODS
			void parseCommandLine();
			void setAvailableCommandLineOptions();
			void validateOptions();
	};

} /* namespace RandomImageTileGenerator */

#endif /* COMMANDLINEOPTIONSBOOST_HPP_ */
