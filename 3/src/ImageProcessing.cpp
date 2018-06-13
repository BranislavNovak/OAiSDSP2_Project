
#include "ImageProcessing.h"
#include "ImageInterpolation.h"

#include <QDebug>

void imageProcessingFun(const QString& progName, QImage* const outImgs, const QImage* const inImgs, const QVector<double>& params) 
{
	int X_SIZE = inImgs->width();
	int Y_SIZE = inImgs->height();
	
	if(progName == "Sample and hold") 
	{	
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
		/* Horizontal scale factor is params[1] */
		
		double horizontal_scale = params[1];
		double vertical_scale = params[0];

		/* Calculating output image resolution and construct output image object */

		// -> Size of X and Y for outImgs. It is equal to inImgs X and Y multiplied 
		//    by horizontal and vertical scale substracted with remaining of module 4.
		
		int x_remaining_part = (int)(X_SIZE * horizontal_scale) % 4;
		int y_remaining_part = (int)(Y_SIZE * vertical_scale) % 4;

		int newXSize = (int)(X_SIZE * horizontal_scale - x_remaining_part);
		int newYSize = (int)(Y_SIZE * vertical_scale - y_remaining_part);

		// -> Initializing the outImgs with constructor of QImage class.
		//    Class is declared in qimage.h as  QImage(int width, int height, Format format);
		new (outImgs) QImage(newXSize, newYSize, inImgs->format());

		/* Performing Sample and hold interpolation  */
		sampleAndHold(inImgs->bits(), X_SIZE, Y_SIZE, outImgs->bits(), newXSize, newYSize);
	}
	else if (progName == "Bilinear") 
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* Vertical scale factor is params[0] */
		/* Horizontal scale factor is params[1] */

		double horizontal_scale = params[1];
		double vertical_scale = params[0];

		/* Calculating output image resolution and construct output image object */

		// -> Size of X and Y for outImgs. It is equal to inImgs X and Y multiplied 
		//    by horizontal and vertical scale substracted with remaining of module 4.

		int x_remaining_part = (int)(X_SIZE * horizontal_scale) % 4;
		int y_remaining_part = (int)(Y_SIZE * vertical_scale) % 4;

		int newXSize = (int)(X_SIZE * horizontal_scale - x_remaining_part);
		int newYSize = (int)(Y_SIZE * vertical_scale - y_remaining_part);

		// -> Initializing the outImgs with constructor of QImage class.
		//    Class is declared in qimage.h as  QImage(int width, int height, Format format);
		new (outImgs) QImage(newXSize, newYSize, inImgs->format());

		/* Performing Bilinear interpolation  */
		bilinearInterpolate(inImgs->bits(), X_SIZE, Y_SIZE, outImgs->bits(), newXSize, newYSize);
	}
	else if(progName == "Transform") 
	{	
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* k1 and k2 parameters are given as params[0] and params[1]*/

		double amplitude = params[0];
		double frequency = params[1];

		/* Constructing output image object */
		new (outImgs) QImage(X_SIZE, Y_SIZE, inImgs->format());

		/* Performing image transformation */
		imageTransform(inImgs->bits(), X_SIZE, Y_SIZE, outImgs->bits(), amplitude, frequency);		  	
	}
	else if (progName == "Transform Bilinear") 
	{
		/* Input image data in RGB format can be obtained with inImgs->bits() */
		/* k1 and k2 parameters are given as params[0] and params[1]*/
		
		double amplitude = params[0];
		double frequency = params[1];

		/* Constructing output image object  */
		new (outImgs) QImage(X_SIZE, Y_SIZE, inImgs->format());

		/* Performing image transformation with bilinear interpolation */
		imageTransformBilinear(inImgs->bits(), X_SIZE, Y_SIZE, outImgs->bits(), amplitude, frequency);
	}

}

