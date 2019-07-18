/*Ratchet & Clank Save Converter
Copyright(C) 2019 CreepNT & Jackblue

This program is free software; you can redistribute itand /or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110 - 1301 USA.*/

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#define DEBUG
void readFileBytes(const char* name, std::vector<char>& data)
{
	std::ifstream ifs(name, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	data = std::vector<char>(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&data[0], pos);
	ifs.close();
}


int main(int argc, char* argv[])
{
	double versionnum = 0.002;//Increment this each time. Thanks.
	if (argc > 1){
		std::string argv1 = argv[1];
		std::string filename;
		bool argv1isfile = 1;
		int gameversion = 0;

#ifdef  DEBUG
		std::cout << argv1 << std::endl;
#endif //DEBUG

		if ((argv1 == "-h") || (argv1 == "-help") || (argv1 == "--help") || (argv1 == "--h"))//Undocumented arguments for lightheads.
		{//Testing for help arguement, if it is present, displays help
			std::cout << "PlayStation Save Converter by Jackblue & CreepNT - Version " << versionnum << std::endl;
			std::cout << "Copyright (C) 2019 Jackblue & CreepNT - Provided AS IS, WITHOUT ANY WARRANTY !" << std::endl;
			std::cout << "Used to convert save files between PS3 and Vita.\n" << std::endl;
			std::cout << "Syntax : \n" << argv[0] << " <-rc1/-rc2/-rc3> file-to-convert [optional-output-file]\n" << std::endl;
			std::cout << "If no output name is provided, the converted file's name will be the same with an added \"CONVERTED-\" prefix." << std::endl;
			std::cout << "If no game version argument is passed, will try to detect it in the file name." << std::endl;
			std::cout << "The auto-detect ONLY works if the file has RCx in its name, with x being the number of the game." << std::endl;
			std::cout << "(i.e. when converting from Vita). If the program can't find, then it will stop." << std::endl;
			return 0;}


		if ((argv1.find("RC1") != std::string::npos) || (argv1 == "-rc1")) {//Trying to find the game version.
			gameversion = 1;
		}
		else if ((argv1.find("RC2") != std::string::npos) || (argv1 == "-rc2")) {
			gameversion = 2;
		}
		else if ((argv1.find("RC3") != std::string::npos) || (argv1 == "-rc3")) {
			gameversion = 3;
		}
		if ((argv1 == "-rc1") || (argv1 =="-rc2") || (argv1 == "-rc3")) {//If first argument is not a filename then we need to shift the index of argv[]
			argv1isfile = 0;
		}

		if (argv1isfile) {
			filename = argv1;
		}
		else {
			filename = argv[2];
		}

		if (std::filesystem::exists(filename))
		{	std::vector<char> data;
			readFileBytes(filename.c_str(), data);
			std::vector<char> finalsave(data.size());

			if (gameversion == 0) {//No version, no CONversion :^)
				std::cout << "Could not identify the game the save is from. Please specify with the -rc1/-rc2/-rc3 argument." << std::endl;
				return 0;
			}
			
			if (gameversion == 1) {//Convert "endianness" by swapping bytes of 4-bytes blocks
				//This method SEEMS to work for RC1, needs to be tested for RC2, and DOESN'T work for RC3
				for (int block = 0; block < data.size() / 4; ++block){
					int pos = block * 4;

					finalsave[pos + 3] = data[pos + 0];
					finalsave[pos + 2] = data[pos + 1];
					finalsave[pos + 1] = data[pos + 2];
					finalsave[pos + 0] = data[pos + 3];
				}
			}
			if (gameversion == 2) {//To be implemented
					std::cout << "RC2 conversion is not implemented yet. Exiting..." << std::endl;
					return 0;
			}
			if (gameversion == 3) {//To be implemented
				std::cout << "RC3 conversion is not implemented yet. Exiting..." << std::endl;
				return 0;
			}
			std::string outputpath = "CONVERTED-" + filename; //If no other test succeed, name will not be changed.
		    //TODO : Implement the naming process


			
			std::ofstream outfile(outputpath, std::ios::out | std::ios::binary);
			outfile.write(finalsave.data(), finalsave.size());
			outfile.close();

			std::cout << "Save successfuly converted !" << std::endl;
			std::cout << "Outputed to " << outputpath << std::endl;
			}
		else {
			std::cout << "Error: Specified file (" << filename << ") doesn't exists !" << std::endl;
			std::cout << "Use \"-h\" argument to get help." << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid arguments." << std::endl;
		std::cout << "Syntax : " << argv[0] << " file-to-convert" << std::endl;
		std::cout << "Use \"-h\" argument to get help." << std::endl;
	}
	return 0;
}