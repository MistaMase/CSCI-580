#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"


GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	 -- set display resolution
	 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	 -- allocate memory for pixel buffer
	 */

	// Resolution
	this->xres = xRes;
	this->yres = yRes;

	// Frame buffer
	framebuffer = new char[3 * xRes * yRes];

	// Pixel Buffer
	pixelbuffer = new GzPixel[xRes * yRes];
}

GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */

	// Deallocate Frame Buffer
	delete[] framebuffer;

	// Deallocate Pixel Buffer
	delete[] pixelbuffer;
}

int GzRender::GzDefault()
{
	/* HW1.3 set pixel buffer to some default values - start a new frame */
	
	// Set default pixel buffer values to white with no alpha
	for (size_t idx = 0; idx < xres * yres; idx++) {
		// Their color (2048, 1792, 1536)

		pixelbuffer[idx].red = 4095;
		pixelbuffer[idx].green = 4095;
		pixelbuffer[idx].blue = 4095;
		pixelbuffer[idx].alpha = 4095;
		pixelbuffer[idx].z = INT_MAX;
	}
	return GZ_SUCCESS;
}


int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.4 write pixel values into the buffer */

	// Index Bounds
	if (i >= 0 && i < xres && j >= 0 && j < yres) {
		int idx = xres * j + i;

		// Bound red to 0-4095
		if (r > 4095)
			pixelbuffer[idx].red = 4095;
		else if (r < 0)
			pixelbuffer[idx].red = 0;
		else
			pixelbuffer[idx].red = r;
		
		// Bound green to 0-4095
		if (g > 4095)
			pixelbuffer[idx].green = 4095;
		else if (g < 0)
			pixelbuffer[idx].green = 0;
		else
			pixelbuffer[idx].green = g;
		
		// Bound blue to 0-4095
		if (b > 4095)
			pixelbuffer[idx].blue = 4095;
		else if (b < 0)
			pixelbuffer[idx].blue = 0;
		else
			pixelbuffer[idx].blue = b;

		// Bound alpha to 0-4095
		if (a > 4095)
			pixelbuffer[idx].alpha = 4095;
		else if (a < 0)
			pixelbuffer[idx].alpha = 0;
		else
			pixelbuffer[idx].alpha = a;

		// Bound Z from 0 - INT_MAX
		if (z > INT_MAX)
			pixelbuffer[idx].z = INT_MAX;
		else if (z < 0)
			pixelbuffer[idx].z = 0;
		else
			pixelbuffer[idx].z = z;
	}

	return GZ_SUCCESS;

	// As per Slack, don't return GZ_FAILURE on out of bounds
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	/* HW1.5 retrieve a pixel information from the pixel buffer */

	// Index Bounds
	if (i >= 0 && i < xres && j >= 0 && j < yres) {
		int idx = xres * i + j;
		*r = pixelbuffer[idx].red;
		*g = pixelbuffer[idx].green;
		*b = pixelbuffer[idx].blue;
		*a = pixelbuffer[idx].alpha;
		*z = pixelbuffer[idx].z;
	}

	return GZ_SUCCESS;

	// As per Slack, don't return GZ_FAILURE on out of bounds
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
	/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */

	// Error check the input file
	// Intentionally return failure as this is the main output of our program
	if (!outfile) {
		return GZ_FAILURE;
	}

	// Write header
	char first_buf[50];
	sprintf(first_buf, "P6 %d %d 255\r", xres, yres);
	fprintf(outfile, first_buf);

	// Write data
	for (size_t idx = 0; idx < xres * yres; idx++) {
		fputc((char)(pixelbuffer[idx].red >> 4), outfile);
		fputc((char)(pixelbuffer[idx].green >> 4), outfile);
		fputc((char)(pixelbuffer[idx].blue >> 4), outfile);
	}
	
	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
	/* HW1.7 write pixels to framebuffer: 
		- put the pixels into the frame buffer
		- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
		- NOT red, green, and blue !!!
	*/

	for (size_t idx = 0; idx < xres * yres; idx++) {
		framebuffer[3 * idx] = (char)(pixelbuffer[idx].blue >> 4); 
		framebuffer[3 * idx + 1] = (char)(pixelbuffer[idx].green >> 4);
		framebuffer[3 * idx + 2] = (char)(pixelbuffer[idx].red >> 4);
	}

	return GZ_SUCCESS;
}