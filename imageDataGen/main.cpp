#include "imageDataGen.h"

int main(int argc, const char** argv)
{
	generateFromImage("cells.png", "cellsData.h", "cellsData");
	generateFromImage("icon.png", "iconData.h", "iconData");
}