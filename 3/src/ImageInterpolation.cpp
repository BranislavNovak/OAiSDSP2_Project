#include "ImageInterpolation.h"
#include "ColorSpaces.h"
#include <math.h>


void sampleAndHold(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
	double horizontal_scale = ((double)xSize) / newXSize;
	double vertical_scale = ((double)ySize) / newYSize;

	for (int x = 0; x < newXSize; x++){
		for (int y = 0; y < newYSize; y++){

			// Setting the iterators for inImg for X and Y by formula from PDF file.
			int inImg_x = floor((x - 1) * horizontal_scale + 1);
			int inImg_y = floor((y - 1) * vertical_scale + 1);

			// Clipping the edges to avoid going over it.
			if (inImg_x >= xSize) {
				inImg_x = xSize - 1;
			}
			if (inImg_y >= ySize) {
				inImg_y = ySize - 1;
			}

			// Setting every each of RGB colors in outImg from inImg by iterating through both images.
			output[3 * x + y * newXSize * 3] = input[3 * inImg_x + inImg_y * xSize * 3];			// R
			output[3 * x + 1 + y * newXSize * 3] = input[3 * inImg_x + 1 + inImg_y * xSize * 3];	// G
			output[3 * x + 2 + y * newXSize * 3] = input[3 * inImg_x + 2 + inImg_y * xSize * 3];	// B
		}
	}
}

void bilinearInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
	double horizontal_scale = ((double)xSize) / newXSize;
	double vertical_scale = ((double)ySize) / newYSize;
	
	for (int x = 0; x < newXSize; x++){
		for (int y = 0; y < newYSize; y++){
			
			// Setting the iterators for inImg for X and Y by formula from PDF file.
			int inImg_x = x * horizontal_scale;
			int inImg_y = y * vertical_scale;

			// Setting parameters A and B by formula from PDF file.
			double a = y / vertical_scale - floor(y / vertical_scale);
			double b = x / horizontal_scale - floor(x / horizontal_scale);


			// Setting every each of RGB colors in outImg from inImg by iterating through both images.
			// Used a forumlua from PDF file Y = (1-a)*(1-b)*X(m,n).....
			output[3 * x + y * newXSize * 3] =
				(1 - a) * (1 - b) * input[3 * inImg_x + inImg_y * xSize * 3] +
				(1 - a) * b * input[3 * (inImg_x + 1) + inImg_y * xSize * 3] +
				a * (1 - b) * input[3 * inImg_x + (inImg_y + 1) * xSize * 3] +
				a * b * input[3 * (inImg_x + 1) + (inImg_y + 1) * xSize * 3];			// R

			output[3 * x + 1 + y * newXSize * 3] =
				(1 - a) * (1 - b) * input[3 * inImg_x + 1 + inImg_y * xSize * 3] +
				(1 - a) * b * input[3 * (inImg_x + 1) + 1 + inImg_y * xSize * 3] +
				a * (1 - b) * input[3 * inImg_x + 1 + (inImg_y + 1) * xSize * 3] +
				a * b * input[3 * (inImg_x + 1) + 1 + (inImg_y + 1) * xSize * 3];		// G

			output[3 * x + 2 + y * newXSize * 3] =
				(1 - a) * (1 - b) * input[3 * inImg_x + 2 + inImg_y * xSize * 3] +
				(1 - a) * b * input[3 * (inImg_x + 1) + 2 + inImg_y * xSize * 3] +
				a * (1 - b) * input[3 * inImg_x + 2 + (inImg_y + 1) * xSize * 3] +
				a * b * input[3 * (inImg_x + 1) + 2 + (inImg_y + 1) * xSize * 3];		// B

		}
	}
}

void bicubicInterpolate(const uchar input[], int xSize, int ySize, uchar output[], int newXSize, int newYSize)
{
	/* TO DO */
}

void imageTransform(const uchar input[], int xSize, int ySize, uchar output[], double k1, double k2)
{
	/* TO DO */
	for (int x = 0; x < xSize; x++) { 
		for (int y = 0; y < ySize; y++){

			// Setting the new variable newY that is used for iteration through inImg. 
			int newY = y + k1 * ySize * sin(2 * 3.14 * x / (xSize * k2));
		   			
			if (newY >= 0 && newY < ySize && x >= 0 && y < xSize) {
				// Checking if the iterators are out of range in inImg.
				output[3 * x + y * xSize * 3] = input[3 * x + newY * xSize * 3];			// R
				output[3 * x + 1 + y * xSize * 3] = input[3 * x + 1 + newY * xSize * 3];	// G
				output[3 * x + 2 + y * xSize * 3] = input[3 * x + 2 + newY * xSize * 3];	// B
			}
			else {
				// If not, make all pixel in outImg black.
				output[3 * x + y * xSize * 3] = 0;											// R
				output[3 * x + 1 + y * xSize * 3] = 0;										// G
				output[3 * x + 2 + y * xSize * 3] = 0;										// B
			}
		}
	}
}

void imageTransformBilinear(const uchar input[], int xSize, int ySize, uchar output[], double k1, double k2)
{
	/* TO DO */
	for (int x = 0; x < xSize; x++){
		for (int y = 0; y < ySize; y++){

			// Setting the iterators for inImg for X and Y by formula from PDF file.
			
			// For vertical use declarations below.
			//int inImg_x = x + k1 * xSize * sin(2 * 3.14* y / (ySize * k2));
			//int inImg_y = y;

			// For horizontal use declarations below.
			int inImg_x = x;
			int inImg_y = y + k1 * ySize * sin(2 * 3.14* x / (xSize * k2));

			// Setting parameters A and B by formula from PDF file.
			double a = inImg_y - floor(inImg_y);
			double b = inImg_x - floor(inImg_x);

			if (inImg_x >= 0 && inImg_x + 1 < xSize && inImg_y >= 0 && inImg_y + 1 < ySize){
				// Checking if the iterators are out of range in inImg.
				// Setting every each of RGB colors in outImg from inImg by iterating through both images.
				// Used a forumlua from PDF file Y = (1-a)*(1-b)*X(m,n).....

				output[3 * x + y * xSize * 3] =
					(1 - a) * (1 - b) * input[3 * inImg_x + inImg_y * xSize * 3] +
					(1 - a) * b * input[3 * (inImg_x + 1) + inImg_y * xSize * 3] +
					a * (1 - b) * input[3 * inImg_x + (inImg_y + 1) * xSize * 3] +
					a * b * input[3 * (inImg_x + 1) + (inImg_y + 1) * xSize * 3];			// R
				
				output[3 * x + 1 + y * xSize * 3] =
					(1 - a) * (1 - b) * input[3 * inImg_x + 1 + inImg_y * xSize * 3] +
					(1 - a) * b * input[3 * (inImg_x + 1) + 1 + inImg_y * xSize * 3] +
					a * (1 - b) * input[3 * inImg_x + 1 + (inImg_y + 1) * xSize * 3] +
					a * b * input[3 * (inImg_x + 1) + 1 + (inImg_y + 1) * xSize * 3];		// G
				
				output[3 * x + 2 + y * xSize * 3] =
					(1 - a) * (1 - b) * input[3 * inImg_x + 2 + inImg_y * xSize * 3] +
					(1 - a) * b * input[3 * (inImg_x + 1) + 2 + inImg_y * xSize * 3] +
					a * (1 - b) * input[3 * inImg_x + 2 + (inImg_y + 1) * xSize * 3] +
					a * b * input[3 * (inImg_x + 1) + 2 + (inImg_y + 1) * xSize * 3];		// B
			}
			else
			{
				// If not, make all pixel in outImg black.
				output[3 * x + y * xSize * 3] = 0;															// R
				output[3 * x + 1 + y * xSize * 3] = 0;														// G
				output[3 * x + 2 + y * xSize * 3] = 0;														// B
			}
		}
	}
}