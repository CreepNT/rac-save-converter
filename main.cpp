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

//Special zones addresses declaration for RAC3: 
/*Ranges : 0x4A8-0x543 | 0x54C-0x5E7 (half sure) | 0x2C14-0x2C2F (half sure) | 0x2C38-0x2CD7
		   0x2D14-0x2D17 (always 0x0000000F ??)*/
#define UNLOCK_TABLE_START 1192
#define UNLOCK_TABLE_END 1347 //unsure but 543-547 are swapped even tho they should be part of the unlock table
#define MAGIC_1_START 1356
#define MAGIC_1_END 1511
#define MAGIC_2_START 11284
#define MAGIC_2_END 11311
#define ITEM_TABLE_START 11320
#define ITEM_TABLE_END 11479
#define MAGIC_3_START 11540
#define MAGIC_3_END 11543

double versionnum = 0.3;

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

	if (argc > 1){
		std::string argv1 = argv[1];
		std::string filename;
		int gameversion = 0;
		std::string outputpath;

		if ((argv1 == "-h") || (argv1 == "-help") || (argv1 == "--help") || (argv1 == "--h"))
		{//Testing for help arguement, if it is present, displays help
			std::cout << "Ratchet & Clank Save Converter by Jackblue & CreepNT - Version " << versionnum << std::endl;
			std::cout << "Copyright (C) 2019 Jackblue & CreepNT - Provided AS IS, WITHOUT ANY WARRANTY !" << std::endl;
			std::cout << "Used to convert save files of RAC HD between PS3 and Vita.\n" << std::endl;
			std::cout << "Syntax : \n" << argv[0] << " <-rc1/-rc2/-rc3> file-to-convert [output-file-name]\n" << std::endl;
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
		if ((argv1 == "-rc1") || (argv1 =="-rc2") || (argv1 == "-rc3")) {//If argv[1] is the game version then the savefile path is argv[2]
			filename = argv[2];
			if (argc > 3) {//If there are more than 3 args (program, game version and savefile)
				outputpath = argv[3];//then assume 4th argument is output file name
			}
			else {
				outputpath = "CONVERTED-" + filename;
			}
		}
		else {//it's argv[1]
			filename = argv[1]; 
			if (argc > 2) {//If there are more than 2 args (program and savefile)
				outputpath = argv[2];//then assume 3rd argument is output file name
			}
			else {
				outputpath = "CONVERTED-" + filename;
			}
		}

		if (std::filesystem::exists(filename))
		{	std::vector<char> data;
			readFileBytes(filename.c_str(), data); //We get the data from the original save
			std::vector<char> finalsave(data.size()); //and make another vector of the same size to recieve the modified data


			switch (gameversion) {
				default:
				case 0:	std::cout << "Could not identify the game the save is from. Please specify with the -rc1/-rc2/-rc3 argument." << std::endl;
						return 0;

				//RC1 Conversion : A I-haven't-RE'ed-RC1-saves method
				case 1:	std::cout << "Warning : RAC1 conversion is WIP and may break saves.\nUse with caution and backup your saves." << std::endl;
						for (int block = 0; block < data.size() / 4; ++block) {//RC1 method. Semi-working a.k.a. WIP.
						int pos = block * 4;
						finalsave[pos + 3] = data[pos + 0];
						finalsave[pos + 2] = data[pos + 1];
						finalsave[pos + 1] = data[pos + 2];
						finalsave[pos + 0] = data[pos + 3];}
						break;

				//RC2 Conversion : not done. try RC1 one if you want.
				case 2:	std::cout << "RC2 conversion is not implemented yet.\nExiting..." << std::endl;
						return 0;

				//RC3 Conversion :
				case 3:	std::cout << "Starting conversion using RC3 method..." << std::endl;
						for (int block = 0; block < data.size() / 4; ++block) {
						int pos = block * 4;
						if (pos == UNLOCK_TABLE_START || pos == MAGIC_1_START || pos == MAGIC_2_START || pos == ITEM_TABLE_START || pos == MAGIC_3_START) {
							for (block; block < data.size() / 4; ++block) { //If we reach a special zone, enter the 1:1 copy loop
								pos = block * 4;
								int testpos = pos - 1;
								if (testpos == UNLOCK_TABLE_END || testpos == MAGIC_1_END || testpos == MAGIC_2_END || testpos == ITEM_TABLE_END || testpos == MAGIC_3_END) {
										finalsave[pos + 3] = data[pos + 0];//End of special zone reached, going back to "normal" mode
										finalsave[pos + 2] = data[pos + 1];
										finalsave[pos + 1] = data[pos + 2];
										finalsave[pos + 0] = data[pos + 3];
										break;
								}
								else {
									//1:1 copy
									finalsave[pos + 0] = data[pos + 0];
									finalsave[pos + 1] = data[pos + 1];
									finalsave[pos + 2] = data[pos + 2];
									finalsave[pos + 3] = data[pos + 3];
									}
								}
							}
							else {
							//normal "endian-conversion" copy
								finalsave[pos + 3] = data[pos + 0];
								finalsave[pos + 2] = data[pos + 1];
								finalsave[pos + 1] = data[pos + 2];
								finalsave[pos + 0] = data[pos + 3];
							}
						}
						break;

			} 
			
			std::ofstream outfile(outputpath, std::ios::out | std::ios::binary);
			if (outfile.is_open()) {
				outfile.write(finalsave.data(), finalsave.size());
				outfile.close();
				std::cout << "Save successfuly converted !" << std::endl;
				std::cout << "Outputed to " << outputpath << std::endl;
			}
			else {
				std::cout << "Error : couldn't open the output file (" << filename << ").\nExiting..." << std::endl;
			}

		}
		else {
			std::cout << "Error: Specified file (" << filename << ") doesn't exists !" << std::endl;
			std::cout << "Use \"-h\" argument to get help.\nExiting..." << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid arguments." << std::endl;
		std::cout << "Syntax : " << argv[0] << " file-to-convert" << std::endl;
		std::cout << "Use \"-h\" argument to get help.\nExiting..." << std::endl;
	}
	return 0;
}