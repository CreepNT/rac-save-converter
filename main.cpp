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
	double versionnum = 0.001;
	if (argc > 1)
	{

		std::string argv1 = argv[1];
		bool autoname = 1;
#ifdef  DEBUG
		std::cout << argv1 << std::endl;
#endif //  DEBUG
		if ((argv1 == "-h") || (argv1 == "-help") || (argv1 == "--help") || (argv1 == "--h"))//Just to be sure you don't miss it :D
		{//Testing for help arguement, if it is present, displays help
			std::cout << "PlayStation Save Converter by Jackblue & CreepNT - Version " << versionnum << std::endl;
			std::cout << "Copyright (C) 2019 Jackblue & CreepNT - Provided AS IS, WITHOUT ANY WARRANTY !" << std::endl;
			std::cout << "Used to convert save files between PS3 and Vita.\n" << std::endl;
			std::cout << "Syntax : \n" << argv[0] << " file-to-convert [optional-output-file]\n" << std::endl;
			std::cout << "If no name is provided, the converted file's name will be the same with an added \"CONVERTED-\" prefix." << std::endl;
			std::cout << "In the case the file to convert is named USR-DATA, then the output is named RCX_SAVEDATA_Y.bin." << std::endl;
			//TODO : implement autonaming for Vita=>PS3
			std::cout << "Autonaming can be disabled by passing -na (NoAuto) as the first argument." << std::endl;
			std::cout << "If a custom output file name has been provided, autonaming is automatically ignored." << std::endl;
			return 0;
		}
		std::string filename;
		if (argv1 == "-na")
		{//Testing for NoAuto naming argument
			filename = argv[2];
			autoname = 0;
		}
		else
		{
			filename = argv1;//No correct arguments have been passed, assuming there is none
		}
		if (std::filesystem::exists(filename))
		{
			std::vector<char> data;
			readFileBytes(filename.c_str(), data);

			std::vector<char> finalsave(data.size());
			//Convert endianness by swapping bytes of 4-bytes blocks
			//This method SEEMS to work for RC1, needs to be tested for RC2, and DOESN'T work for RC3
			for (int block = 0; block < data.size() / 4; ++block)
			{
				int pos = block * 4;

				finalsave[pos + 3] = data[pos + 0];
				finalsave[pos + 2] = data[pos + 1];
				finalsave[pos + 1] = data[pos + 2];
				finalsave[pos + 0] = data[pos + 3];
			}
			std::string outputpath = "CONVERTED-" + filename; //If no other test suceed, name will not be changed. This makes code prettier.
			if ((argc > 2 && autoname == 1) || (argc > 3 && autoname == 0)) {
				switch (autoname)
				{
				case 1:
					outputpath = argv[2];//No arguments were passed so second arg should be output path.
					break;
				case 0:
					outputpath = argv[3]; //Surprisingly, -na argument has been passed.  Output path should be third argument.
					break;
				}
			}
			else
			{//Autonaming tests
				if (filename == "USR-DATA" && autoname == 1) {
					outputpath = "RCx_SAVEDATA_y.bin";
				}
				//TODO : Implement Vita --> PS3 autonaming
			}

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