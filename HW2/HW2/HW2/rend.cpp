#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include	<stdlib.h>

/***********************************************/
/* HW1 methods: copy here the methods from HW1 */

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
	for (int idx = 0; idx < xres * yres; idx++) {
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
	for (int idx = 0; idx < xres * yres; idx++) {
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

	for (int idx = 0; idx < xres * yres; idx++) {
		framebuffer[3 * idx] = (char)(pixelbuffer[idx].blue >> 4);
		framebuffer[3 * idx + 1] = (char)(pixelbuffer[idx].green >> 4);
		framebuffer[3 * idx + 2] = (char)(pixelbuffer[idx].red >> 4);
	}

	return GZ_SUCCESS;
}


/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList) 
{
	/* HW 2.1
	-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
	-- In later homeworks set shaders, interpolaters, texture maps, and lights
	*/

	// Return before looping if either pointer is invalid
	if (!nameList || !valueList)
		return GZ_SUCCESS;


	// Loop per attribute
	for (int idx = 0; idx < numAttributes; idx++) {

		// Change renderer color
		if (nameList[idx] == GZ_RGB_COLOR) {
			GzColor* color = (GzColor*)valueList[idx];
			
			// Bounds check R value
			if ((*color)[0] > 1.0)
				flatcolor[0] = 4095;
			else if ((*color)[0] < 0)
				flatcolor[0] = 0;
			else
				flatcolor[0] = (*color)[0];

			// Bounds check G value
			if ((*color)[1] > 1.0)
				flatcolor[1] = 4095;
			else if ((*color)[1] < 0)
				flatcolor[0] = 0;
			else
				flatcolor[1] = (*color)[1];

			// Bounds check B value
			if ((*color)[2] > 1.0)
				flatcolor[2] = 4095;
			else if ((*color)[2] < 0)
				flatcolor[2] = 0;
			else
				flatcolor[2] = (*color)[2];
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int	numParts, GzToken *nameList, GzPointer *valueList) 
	/* numParts - how many names and values */
	{
	/* HW 2.2
	-- Pass in a triangle description with tokens and values corresponding to
		  GZ_NULL_TOKEN:		do nothing - no values
		  GZ_POSITION:		3 vert positions in model space
	-- Invoke the rastrizer/scanline framework
	-- Return error code
	*/

	GzPointer valueListIndexPointer = valueList[0];
	for (int token_num = 0; token_num < numParts; token_num++) {
		// Add the triangle
		if (nameList[token_num] == GZ_POSITION) {
			// Get the 3 vertices from the valueList
			GzCoord vertices[3];
			for (size_t tkn = 0; tkn < 3; tkn++) {
				// Cast the generic pointer to a GzCoord pointer
				vertices[tkn][0] = (*(GzCoord*)valueListIndexPointer)[0];
				vertices[tkn][1] = (*(GzCoord*)valueListIndexPointer)[1];
				vertices[tkn][2] = (*(GzCoord*)valueListIndexPointer)[2];

				// Advance the generic pointer to the next GzCoord using pointer logic
				valueListIndexPointer = (GzCoord*)valueListIndexPointer + 1;
			}

			// Skip this triangle if all x values are less than 0
			if (vertices[0][0] < 0 && vertices[1][0] < 0 && vertices[2][0] < 0) 
				continue;

			// Skip this triangle if all x values are greater than or equal to xres
			if (vertices[0][0] >= xres && vertices[1][0] >= xres && vertices[2][0] >= xres) 
				continue;

			// Skip this triangle if all y values are less than 0
			if (vertices[0][1] < 0 && vertices[1][1] < 0 && vertices[2][1] < 0) 
				continue;

			// Skip this triangle if all y values are greater than or equal to yres
			if (vertices[0][1] >= yres && vertices[1][1] >= yres && vertices[2][1] >= yres) 
				continue;

			// Sort vertices by Y into (1-2, 2-3) and (1-3)
			// Simple insertion sort
			int lowestIndex;
			GzCoord temp;
			for (size_t i = 0; i < 3; i++) {
				lowestIndex = i;
				for (size_t j = i+1; j < 3; j++) {
					if (vertices[j][1] < vertices[lowestIndex][1]) {
						lowestIndex = j;
					}
				}

				if (lowestIndex != i) {
					// Save vertices[i] into temp
					temp[0] = vertices[i][0];
					temp[1] = vertices[i][1];
					temp[2] = vertices[i][2];

					// Place new lowest value into vertices[i]
					vertices[i][0] = vertices[lowestIndex][0];
					vertices[i][1] = vertices[lowestIndex][1];
					vertices[i][2] = vertices[lowestIndex][2];

					// Place temp value into index of lowest value
					vertices[lowestIndex][0] = temp[0];
					vertices[lowestIndex][1] = temp[1];
					vertices[lowestIndex][2] = temp[2];
				}
			}

			// Set up DDA
			DDA cur;
			cur.start[0] = vertices[0][0];
			cur.start[1] = vertices[0][1];
			cur.start[2] = vertices[0][2];
			cur.end[0] = vertices[1][0];
			cur.end[1] = vertices[1][1];
			cur.end[2] = vertices[1][2];
			cur.current[0] = vertices[0][0];
			cur.current[1] = vertices[0][1];
			cur.current[2] = vertices[0][2];
			cur.slope_x = (vertices[1][0] - vertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			cur.slope_z = (vertices[1][2] - vertices[0][2]) / (vertices[1][1] - vertices[0][1]);


			// 3 - Check if V2 is on left or right
			// TODO Double Check for Negative Values
			bool on_left;


			// Skip if no y change
			if (vertices[2][1] - vertices[0][1] == 0) continue;
			float long_edge_slope_xy = (vertices[2][0] - vertices[0][0]) / (vertices[2][1] - vertices[0][1]);

			float long_edge_slope_zy = (vertices[2][2] - vertices[0][2]) / (vertices[2][1] - vertices[0][1]);
			float intercept_xy = vertices[2][0] - long_edge_slope_xy * vertices[2][1];
			float intercept_zy = vertices[2][2] - long_edge_slope_zy * vertices[2][1];

			float edge_check = long_edge_slope_xy * vertices[1][1] + intercept_xy;
			// Colinear Triangle
			if (vertices[1][0] == edge_check) {
				continue;
			}
			// V2 on Right
			else if (vertices[1][0] > edge_check) {
				on_left = false;
			}
			// V2 on Left
			else {
				on_left = true;
			}
		
			// Top Half
			DDA_Span span;

			// Advance to first value
			float dy = (float)(ceil(cur.start[1]) - cur.start[1]);
			cur.current[0] = cur.start[0] + cur.slope_x * dy;
			cur.current[1] = cur.start[1] + dy;
			cur.current[2] = cur.start[2] + cur.slope_z * dy;

			// Loop over the top half Y coordinates
			while (cur.current[1] < cur.end[1]) {
				// V2 on Left - Go left-to-right from X value of E(1,2) to X value of E(1,3)
				if (on_left) {
					// Configure span values
					span.start[0] = cur.current[0];
					span.start[1] = cur.current[2];
					span.end[0] = long_edge_slope_xy * cur.current[1] + intercept_xy;
					span.end[1] = long_edge_slope_zy * cur.current[1] + intercept_zy;
				}
				
				// V2 on Right - Go left-to-right from X value of E(1,3) to X value of E(1,2)
				else {
					// Configure span values
					span.start[0] = long_edge_slope_xy * cur.current[1] + intercept_xy;
					span.start[1] = long_edge_slope_zy * cur.current[1] + intercept_zy;
					span.end[0] = cur.current[0];
					span.end[1] = cur.current[2];
				}

				// Configure span values
				span.current[0] = span.start[0];
				span.current[1] = span.start[1];
				span.slope_z = (span.end[1] - span.start[1]) / (span.end[0] - span.start[0]);

				// Advance to first value
				float dx = (float)(ceil(span.start[0]) - span.start[0]);
				span.current[0] = span.start[0] + dx;
				span.current[1] = span.start[1] + dx * span.slope_z;

				// Loop over the X coordinates
				while (span.current[0] < span.end[0]) {
					// Write current value into the pixel buffer only if it's closer to the camera
					if (span.current[0] >= 0 && span.current[0] < xres && cur.current[1] >= 0 && cur.current[1] < yres && (GzDepth)span.current[1] < pixelbuffer[ARRAY((int)span.current[0], (int)cur.current[1])].z) {
						GzPut(span.current[0], cur.current[1], ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 4095, (GzDepth)span.current[1]);
						//GzPut((int)span.current[0], (int)cur.current[1], 4095, 0, 0, 4095, (GzDepth)span.current[1]);
					}

					// Step the current X
					span.current[0]++;
					span.current[1] = span.current[1] + span.slope_z;
				}

				// Step the current Y
				cur.current[0] += cur.slope_x;
				cur.current[1]++;
				cur.current[2] += cur.slope_z;
				
			}

			// Bottom Half
			cur.start[0] = vertices[1][0];
			cur.start[1] = vertices[1][1];
			cur.start[2] = vertices[1][2];
			cur.end[0] = vertices[2][0];
			cur.end[1] = vertices[2][1];
			cur.end[2] = vertices[2][2];
			cur.current[0] = vertices[1][0];
			cur.current[1] = vertices[1][1];
			cur.current[2] = vertices[1][2];
			cur.slope_x = (vertices[2][0] - vertices[1][0]) / (vertices[2][1] - vertices[1][1]);
			cur.slope_z = (vertices[2][2] - vertices[1][2]) / (vertices[2][1] - vertices[1][1]);

			// Advance to first value
			dy = (float)(ceil(cur.start[1]) - cur.start[1]);
			cur.current[0] = cur.start[0] + cur.slope_x * dy;
			cur.current[1] = cur.start[1] + dy;
			cur.current[2] = cur.start[2] + cur.slope_z * dy;

			// Loop over the bottom half Y coordinates
			while (cur.current[1] < cur.end[1]) {
				// V2 on Left - Go left-to-right from X value of E(2,3) to X value of E(1,3)
				if (on_left) {
					// Configure span values
					span.start[0] = cur.current[0];
					span.start[1] = cur.current[2];
					span.end[0] = long_edge_slope_xy * cur.current[1] + intercept_xy;
					span.end[1] = long_edge_slope_zy * cur.current[1] + intercept_zy;
				}

				// V2 on Right - Go left-to-right from X value E(1,3) to X value E(2,3)
				else {
					// Configure span values
					span.start[0] = long_edge_slope_xy * cur.current[1] + intercept_xy;
					span.start[1] = long_edge_slope_zy * cur.current[1] + intercept_zy;
					span.end[0] = cur.current[0];
					span.end[1] = cur.current[2];
				}

				// Configure span values
				span.current[0] = span.start[0];
				span.current[1] = span.start[1];
				span.slope_z = (span.end[1] - span.start[1]) / (span.end[0] - span.start[0]);

				// Advance to first value
				float dx = (float)(ceil(span.start[0]) - span.start[0]);
				span.current[0] = span.start[0] + dx;
				span.current[1] = span.start[1] + dx * span.slope_z;

				// Loop over the X coordinates
				while (span.current[0] < span.end[0]) {
					// Write current value into the pixel buffer only if it's closer to the camera
					if (span.current[0] >= 0 && span.current[0] < xres && cur.current[1] >= 0 && cur.current[1] < yres && (GzDepth)span.current[1] < pixelbuffer[ARRAY((int)span.current[0], (int)cur.current[1])].z) {
						GzPut((int)span.current[0], (int)cur.current[1], ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 4095, (GzDepth)span.current[1]);
						//GzPut((int)span.current[0], (int)cur.current[1], 0, 4095, 0, 4095, (GzDepth)span.current[1]);
					}

					// Step the current X
					span.current[0]++;
					span.current[1] = span.current[1] + span.slope_z;
				}

				// Step the current Y
				cur.current[0] += cur.slope_x;
				cur.current[1]++;
				cur.current[2] += cur.slope_z;
			}
		}
	}

	return GZ_SUCCESS;
}
