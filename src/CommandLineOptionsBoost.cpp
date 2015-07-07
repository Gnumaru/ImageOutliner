/*
 * CommandLineOptionsBoost.cpp
 *
 *  Created on: 18/06/2015
 *      Author: lucas
 */

#include <boost/program_options.hpp>
#include <CommandLineOptionsBoost.hpp>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

namespace po = boost::program_options;

namespace imageOutliner {
	
	CommandLineOptionsBoost::CommandLineOptionsBoost(int argc, char* argv[])
	:
			argc(argc), argv(argv), desc(NULL), bpo(NULL), bitMask(255), outlineColor(0xFFFFFFFF), backgroundColor(0x00000000)
	{
		setAvailableCommandLineOptions();
		parseCommandLine();
	}
	
	CommandLineOptionsBoost::~CommandLineOptionsBoost(){
		delete bpo;
		delete desc;
	}

	/**
	 *    Parse the command line arguments. If help was requested, print it and exit.
	 */
	void CommandLineOptionsBoost::parseCommandLine(){
		bpo = new po::basic_parsed_options<char>(po::parse_command_line(argc, argv, *desc));
		po::store(*bpo, vm);
		po::notify(vm);

		if(vm.count("help") > 0){
			// print the help and forcefully terminates the program here
			std::cout << desc << std::endl;
			std::exit(0);
		}else if(vm.count("input") == 0){
			// print the help and forcefully terminates the program here
			std::cout << "At least one source image must be provided." << std::endl;
			std::cout << *desc << std::endl;
			std::exit(0);
		}
		validateOptions();
	}

	void CommandLineOptionsBoost::validateOptions(){
		if(vm.count("mask") > 0){
			std::string mask = vm["mask"].as<std::string>();
			const char* invalidMaskMsg = "'mask' must be a binary sequence (eg: 01001011) or a decimal between 0 and 255";
			if(mask.length() == 8){ // try to parse binary mask
				const char* maskCStr = mask.c_str();
				while(*maskCStr != '\0'){
					if(*maskCStr < '0' || *maskCStr > '1'){
						throw std::invalid_argument(invalidMaskMsg);
					}
					maskCStr++;
				}
				bitMask = (byte) std::strtol(mask.c_str(), NULL, 2);
			}else if(mask.length() < 4){ // if less than 3 digits, then we got a valid decimal mask
				bitMask = (byte) std::atoi(mask.c_str());
			}else{
				throw std::invalid_argument(invalidMaskMsg);
			}
			std::printf("using bitmask %d (%s).\r\n", bitMask, mask.c_str());
		}

		if(vm.count("color") > 0){
			std::string colorString = vm["color"].as<std::string>();
			const char* invalidColorkMsg = "'color' must be provided as a 4 byte hex color string, as in \"A5E6D7FF\"";
			if(colorString.length() == 8){ // try to parse binary mask
				outlineColor = (uint) std::strtol(colorString.c_str(), NULL, 16);
				std::printf("using border color \"%d, %d, %d, %d\" (%s).\r\n",
				        (outlineColor) >> 24,
				        (outlineColor & 0x00FFFFFF) >> 16,
				        (outlineColor & 0x0000FFFF) >> 8,
				        (outlineColor & 0x000000FF),
				        colorString.c_str());
			}else{
				throw std::invalid_argument(invalidColorkMsg);
			}
		}

		if(vm.count("background") > 0){
			std::string backgroundColorString = vm["background"].as<std::string>();
			const char* invalidColorkMsg = "'backgroundColorString' must be provided as a 4 byte hex color string, as in \"A5E6D7FF\"";
			if(backgroundColorString.length() == 8){
				backgroundColor = (uint) std::strtol(backgroundColorString.c_str(), NULL, 16);
				std::printf("using background color \"%d, %d, %d, %d\" (%s).\r\n",
				        (backgroundColor) >> 24,
				        (backgroundColor & 0x00FFFFFF) >> 16,
				        (backgroundColor & 0x0000FFFF) >> 8,
				        (backgroundColor & 0x000000FF),
				        backgroundColorString.c_str());
			}else{
				throw std::invalid_argument(invalidColorkMsg);
			}
		}else{
			std::printf("using completely transparent pixels as background color \"*, *, *, 0\" (******00).\r\n");
		}
	}

	/**
	 * Determine the available commnad line options
	 */
	void CommandLineOptionsBoost::setAvailableCommandLineOptions(){
		desc = new po::options_description("Available options:");
		desc->add_options()
		("help,h", "Bring up this help page.")
		("input,i", po::value<std::string>(), "The image to be outlined. Eg.: -iInputImage")
		("output,o", po::value<std::string>(), "The generated png file name, without extension. Eg.: -oOutputImage")
		("mask,m", po::value<std::string>(),
		        "A bitmask (integer or binary string) telling wich pixels should be taken into account when outlining. Bits start at up neighbour and continue clockwise until upleft neighbour, exactly 8. Eg.: -m10101010 or -m192")
		("color,c", po::value<std::string>(),
		        "The RGBA color to be used as outline, provided in hex RGBA format, as in \"FFCC66FF\". If not provided by command line, defaults to fully opaque completely white (FFFFFFFF). Eg.: -cFEDCBA98")
		("background,b", po::value<std::string>(),
		        "The RGBA color to be used as background, provided in hex RGBA format, as in \"FFCC66FF\". If not provided by command line, defaults to color independente completely transparent (xxxxxx00). Eg.: -cFEDCBA98");
	}
} /* namespace RandomImageTileGenerator */
