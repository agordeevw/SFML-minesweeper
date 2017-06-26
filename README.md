# SFML-minesweeper
Classic Minesweeper game made with SFML.

## Dependencies
[libpng](http://www.libpng.org/pub/png/libpng.html), [CImg](http://cimg.eu/) and [SFML](https://github.com/SFML/SFML).

## Included projects
### SFML-minesweeper
Main project, contains game implementation.

### imageDataGen
Helper project, needed to process game resource files into headers with data for these resources to be embedded in the executable rather that supplied alongside it.

## Tips
Game resources (cell sheet and icon) are stored in .png files inside imageDataGen project folder.

Generate headers containing pixel data with imageDataGen and copy them in SFML-minesweeper project folder.
