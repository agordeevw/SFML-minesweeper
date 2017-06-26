#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>

#define cimg_use_png
#include "CImg.h"
using namespace cimg_library;

void generateFromImage(const char* filePath, const char* resultPath, const char* dataName)
{
	CImg<unsigned char> image(filePath);

	int totalPixels = image.height() * image.width();

	const unsigned char* imageData = image.data();

	std::ofstream dataFile(resultPath);

	if (dataFile)
	{
		dataFile << "#pragma once\n\nconst unsigned char " << dataName << "[] = {\n";
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
		dataFile << std::hex << "0x" << (int)imageData[image.size() - 1];
		dataFile << "\n};\n";
	}
	else
	{
		std::cerr << "Couldn't create output file.\n";
	}
}