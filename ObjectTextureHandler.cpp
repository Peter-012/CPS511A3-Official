
#pragma once
#ifndef OBJECT_TEXTURE_HANDLER_H
#define OBJECT_TEXTURE_HANDLER_H

typedef unsigned char byte;

typedef struct RGB
{
  byte r, g, b;
} RGB;



typedef struct RGBpixmap
{
	int nRows, nCols;
	RGB* pixel;
} RGBpixmap;

















#endif