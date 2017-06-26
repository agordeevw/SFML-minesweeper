#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#define cimg_use_png
#include "CImg.h"
using namespace cimg_library;

// Generate headers containing height, width and array of pixel data of a .png image located at filePath.
void generateFromImage(const char* filePath)
{
	std::string s(filePath);
	size_t nameBegLoc = s.find_last_of('/') + 1;
	size_t nameEndLoc = s.find_last_of('.');

	std::string name(&filePath[nameBegLoc], nameEndLoc - nameBegLoc);
	std::string folder;
	if (nameBegLoc > 0)
		folder = std::string(filePath, nameBegLoc - 1);

	CImg<unsigned char> image(filePath);

	int totalPixels = image.height() * image.width();

	const unsigned char* imageData = image.data();

	std::ofstream dataFile(folder + name + "Data.h");

	if (dataFile)
	{
		dataFile << "#pragma once\n\n";
		dataFile << "const unsigned int " << name << "Width = " << image.width() << ";\n";
		dataFile << "const unsigned int " << name << "Height = " << image.height() << ";\n";
		dataFile << "\nconst unsigned char " << name << "Data[] = {\n";
		for (int i = 0; i < totalPixels - 1; i++)
		{
			dataFile << std::hex << "0x" << (int)imageData[i]					<< "," <<	// Red
						std::hex << "0x" << (int)imageData[totalPixels + i]		<< "," <<	// Green
						std::hex << "0x" << (int)imageData[2 * totalPixels + i]	<< "," <<	// Blue
						std::hex << "0x" << (int)imageData[3 * totalPixels + i] << ",\n";	// Alpha
		}
		dataFile << std::hex << "0x" << (int)imageData[totalPixels - 1] << "," <<	// Red
			std::hex << "0x" << (int)imageData[totalPixels + totalPixels - 1] << "," <<	// Green
			std::hex << "0x" << (int)imageData[2 * totalPixels + totalPixels - 1] << "," <<	// Blue
			std::hex << "0x" << (int)imageData[3 * totalPixels + totalPixels - 1] << ",\n";	// Alpha
		dataFile << "\n};\n";
	}
	else
	{
		std::cerr << "Couldn't create output file.\n";
	}
}