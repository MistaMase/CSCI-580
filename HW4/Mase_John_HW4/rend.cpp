/* CS580 Homework 4 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#define PI (float) 3.14159265358979323846

/* Rotations, Translations, and Scaling Transformations */

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
	/* HW 3.1
		Create rotate matrix : rotate along x axis
		Pass back the matrix using mat value
	*/

	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;

	mat[1][0] = 0.0;
	mat[1][1] = (float)cos((double)degree * PI / 180.0);
	mat[1][2] = -1.0f * (float)sin((double)degree * PI / 180.0);
	mat[1][3] = 0.0;

	mat[2][0] = 0.0;
	mat[2][1] = (float)sin((double)degree * PI / 180.0);
	mat[2][2] = (float)cos((double)degree * PI / 180.0);
	mat[2][3] = 0.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
	/* HW 3.2
		Create rotate matrix : rotate along y axis
		Pass back the matrix using mat value
	*/

	mat[0][0] = (float)cos((double)degree * PI / 180.0);
	mat[0][1] = 0.0;
	mat[0][2] = (float)sin((double)degree * PI / 180.0);
	mat[0][3] = 0.0;

	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;

	mat[2][0] = -1.0f * (float)sin((double)degree * PI / 180.0);
	mat[2][1] = 0.0;
	mat[2][2] = (float)cos((double)degree * PI / 180.0);
	mat[2][3] = 0.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
	/* HW 3.3
		Create rotate matrix : rotate along z axis
		Pass back the matrix using mat value
	*/

	mat[0][0] = (float)cos((double)degree * PI / 180.0);
	mat[0][1] = -1.0f * (float)sin((double)degree * PI / 180.0);
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;

	mat[1][0] = (float)sin((double)degree * PI / 180.0);
	mat[1][1] = (float)cos((double)degree * PI / 180.0);
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;

	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = 0.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
	/* HW 3.4
		Create translation matrix
		Pass back the matrix using mat value
	*/

	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = translate[0];

	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = translate[1];

	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = translate[2];

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
	/* HW 3.5
		Create scaling matrix
		Pass back the matrix using mat value
	*/

	mat[0][0] = scale[0];
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;

	mat[1][0] = 0.0;
	mat[1][1] = scale[1];
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;

	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = scale[2];
	mat[2][3] = 0.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

/* Construction, Destruction, and Initial Renderer Configuration */

GzRender::GzRender(int xRes, int yRes)
{
	// Resolution
	this->xres = xRes;
	this->yres = yRes;

	// Frame buffer
	framebuffer = new char[3 * xRes * yRes];

	// Pixel Buffer
	pixelbuffer = new GzPixel[xRes * yRes];

	// Number of Lights
	numlights = 0;

	// Set default interpolation mode to 0 (Flat shading)
	interp_mode = 0;

	// Set ambient light variable
	ambientlight.color[0] = 0.0;
	ambientlight.color[1] = 0.0;
	ambientlight.color[2] = 0.0;
	ambientlight.direction[0] = 0.0;
	ambientlight.direction[1] = 0.0;
	ambientlight.direction[2] = 0.0;

	// Ambient Light Color - Material properties
	Ka[0] = 0.0;
	Ka[1] = 0.0;
	Ka[2] = 0.0;

	// Diffuse Light Color - Material properties
	Kd[0] = 0.0;
	Kd[1] = 0.0;
	Kd[2] = 0.0;

	// Specular Light Color - Material properties
	Ks[0] = 0.0;
	Ks[1] = 0.0;
	Ks[2] = 0.0;

	// Set default specular power
	spec = 0;

	// Set matlevel to 0
	matlevel = 0;

	// Set flatcolor default to 0
	flatcolor[0] = 0;
	flatcolor[1] = 0;
	flatcolor[2] = 0;

	// Setup Xsp for vertices
	Xsp[0][0] = xres / 2.0f;
	Xsp[0][1] = 0.0;
	Xsp[0][2] = 0.0;
	Xsp[0][3] = xres / 2.0f;

	Xsp[1][0] = 0.0;
	Xsp[1][1] = -1.0f * yres / 2.0f;
	Xsp[1][2] = 0.0;
	Xsp[1][3] = yres / 2.0f;

	Xsp[2][0] = 0.0;
	Xsp[2][1] = 0.0;
	Xsp[2][2] = 1.0f * MAXINT;
	Xsp[2][3] = 0.0;

	Xsp[3][0] = 0.0;
	Xsp[3][1] = 0.0;
	Xsp[3][2] = 0.0;
	Xsp[3][3] = 1.0f;

	// Initialize the default camera
	// Initialize Camera FOV
	m_camera.FOV = DEFAULT_FOV;

	// Initialize Camera Position
	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;

	// Initialize Camera Lookat
	m_camera.lookat[0] = 0.0;
	m_camera.lookat[1] = 0.0;
	m_camera.lookat[2] = 0.0;

	// Initialize Camera Up Vector
	m_camera.worldup[0] = 0.0;
	m_camera.worldup[1] = 1.0f;
	m_camera.worldup[2] = 0.0;

	// Zero out last triangle
	last_triangle[0][0] = 0.0;
	last_triangle[0][1] = 0.0;
	last_triangle[0][2] = 0.0;

	last_triangle[1][0] = 0.0;
	last_triangle[1][1] = 0.0;
	last_triangle[1][2] = 0.0;

	last_triangle[2][0] = 0.0;
	last_triangle[2][1] = 0.0;
	last_triangle[2][2] = 0.0;

	// Zero out last normal, ensure it's a normal just in case
	last_normal[0][0] = 0.0;
	last_normal[0][1] = 0.0;
	last_normal[0][2] = 1.0f;

	last_normal[1][0] = 0.0;
	last_normal[1][1] = 0.0;
	last_normal[1][2] = 1.0f;

	last_normal[2][0] = 0.0;
	last_normal[2][1] = 0.0;
	last_normal[2][2] = 1.0f;

	// Zero out flat color vertex normal, ensure it's a normal just in case
	flatcolor_vertex_normal[0] = 0.0;
	flatcolor_vertex_normal[1] = 0.0;
	flatcolor_vertex_normal[2] = 1.0f;
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

	// Set default pixel buffer values to slightly white with no alpha
	for (int idx = 0; idx < xres * yres; idx++) {
		pixelbuffer[idx].red = (GzIntensity)160 << 4;
		pixelbuffer[idx].green = (GzIntensity)218 << 4;
		pixelbuffer[idx].blue = (GzIntensity)235 << 4;
		pixelbuffer[idx].alpha = 4095;
		pixelbuffer[idx].z = INT_MAX;
	}
	return GZ_SUCCESS;
}

int GzRender::GzBeginRender()
{
	/* HW 3.7
		-- setup for start of each frame - init frame buffer color,alpha,z
		-- compute Xiw and projection xform Xpi from camera definition
		-- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
		-- now stack contains Xsw and app can push model Xforms when needed

	   HW 4
	    -- Push Xsp as identity onto normal matrix stack
		-- Push Xpi as identity onto normal matrix stack
	*/

	// Initialize Xiw for vertices
	m_camera.Xiw[3][0] = 0.0;
	m_camera.Xiw[3][1] = 0.0;
	m_camera.Xiw[3][2] = 0.0;
	m_camera.Xiw[3][3] = 1.0;

	// Create the Z vector
	GzCoord XiwZ, negative_XiwZ;
	vector_subtract(m_camera.lookat, m_camera.position, XiwZ);
	normalize(XiwZ);
	vector_scale(-1.0f, XiwZ, negative_XiwZ);
	m_camera.Xiw[2][0] = XiwZ[0];
	m_camera.Xiw[2][1] = XiwZ[1];
	m_camera.Xiw[2][2] = XiwZ[2];
	m_camera.Xiw[2][3] = dot_product(negative_XiwZ, m_camera.position);

	// Create the Y vector
	GzCoord up_dot_Z_times_Z, XiwY, negative_XiwY;
	float up_dot_Z = dot_product(m_camera.worldup, XiwZ);
	vector_scale(up_dot_Z, XiwZ, up_dot_Z_times_Z);
	vector_subtract(m_camera.worldup, up_dot_Z_times_Z, XiwY);
	normalize(XiwY);
	vector_scale(-1.0f, XiwY, negative_XiwY);
	m_camera.Xiw[1][0] = XiwY[0];
	m_camera.Xiw[1][1] = XiwY[1];
	m_camera.Xiw[1][2] = XiwY[2];
	m_camera.Xiw[1][3] = dot_product(negative_XiwY, m_camera.position);

	// Create the X vector
	GzCoord XiwX, negative_XiwX;
	vector_cross_product(XiwY, XiwZ, XiwX);
	vector_scale(-1.0f, XiwX, negative_XiwX);
	m_camera.Xiw[0][0] = XiwX[0];
	m_camera.Xiw[0][1] = XiwX[1];
	m_camera.Xiw[0][2] = XiwX[2];
	m_camera.Xiw[0][3] = dot_product(negative_XiwX, m_camera.position);

	// Initialize Xpi for vertices
	m_camera.Xpi[0][0] = 1.0;
	m_camera.Xpi[0][1] = 0.0;
	m_camera.Xpi[0][2] = 0.0;
	m_camera.Xpi[0][3] = 0.0;

	m_camera.Xpi[1][0] = 0.0;
	m_camera.Xpi[1][1] = 1.0;
	m_camera.Xpi[1][2] = 0.0;
	m_camera.Xpi[1][3] = 0.0;

	m_camera.Xpi[2][0] = 0.0;
	m_camera.Xpi[2][1] = 0.0;
	m_camera.Xpi[2][2] = (float)tan(1.0 * m_camera.FOV * PI / 180.0 / 2.0);
	m_camera.Xpi[2][3] = 0.0;

	m_camera.Xpi[3][0] = 0.0;
	m_camera.Xpi[3][1] = 0.0;
	m_camera.Xpi[3][2] = (float)tan(1.0 * m_camera.FOV * PI / 180.0 / 2.0);
	m_camera.Xpi[3][3] = 1.0;

	// Initialize Ximage stack head
	matlevel = 0;

	// Put Xsp at base of vertices stack, push Xsp (identity) at base of normal stack
	GzMatrix identity;
	identity_matrix(identity);
	GzPushMatrixUnique(Xsp, identity);

	// Put Xpi on vertex stack, put Xpi (identity) on normal stack
	GzPushMatrixUnique(m_camera.Xpi, identity);

	// Put Xiw on vertex and normal stack
	GzPushMatrix(m_camera.Xiw);

	return GZ_SUCCESS;
}

/* Camera Configuration */

int GzRender::GzPutCamera(GzCamera camera)
{
	/* HW 3.8
		-- overwrite renderer camera structure with new camera definition
	*/

	// Initialize the default camera

	// Initialize Camera FOV
	m_camera.FOV = camera.FOV;

	// Initialize Camera Position
	m_camera.position[0] = camera.position[0];
	m_camera.position[1] = camera.position[1];
	m_camera.position[2] = camera.position[2];

	// Initialize Camera Lookat
	m_camera.lookat[0] = camera.lookat[0];
	m_camera.lookat[1] = camera.lookat[1];
	m_camera.lookat[2] = camera.lookat[2];

	// Initialize Camera Up Vector
	m_camera.worldup[0] = camera.worldup[0];
	m_camera.worldup[1] = camera.worldup[1];
	m_camera.worldup[2] = camera.worldup[2];

	return GZ_SUCCESS;
}

/* Vertex and Normal Transformations */

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
	/* HW 3.9
		-- push a matrix onto the Ximage stack
		-- check for stack overflow
	 
	   HW 4
		-- remove translations from Xnorm matrices
		-- ensure rotations are unitary	
		-- push a matrix onto the Xnorm stack
	*/

	// Compute a scale factor to ensure all rotations are unitary
	float scale_factor = (float)sqrt(pow(matrix[0][0], 2) + pow(matrix[0][1], 2) + pow(matrix[0][2], 2));

	// Remove translations from any matrix going onto Xnorm, apply scale factor
	GzMatrix normals_matrix_parsed = {	{ matrix[0][0] / scale_factor,	matrix[0][1] / scale_factor,	matrix[0][2] / scale_factor,	0.0,		},
										{ matrix[1][0] / scale_factor,	matrix[1][1] / scale_factor,	matrix[1][2] / scale_factor,	0.0,		},
										{ matrix[2][0] / scale_factor,	matrix[2][1] / scale_factor,	matrix[2][2] / scale_factor,	0.0,		},
										{ matrix[3][0],					matrix[3][1],					matrix[3][2],					matrix[3][3]}};


	// Check for stack overflow, return failure
	if (matlevel == MATLEVELS) return GZ_FAILURE;

	// If at the bottom, just add it to the stack
	if (matlevel == 0) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				// Vertex transformations
				Ximage[matlevel][i][j] = matrix[i][j];

				// Normal transformations
				Xnorm[matlevel][i][j] = normals_matrix_parsed[i][j];
			}
		}
	}

	// Otherwise add to the stack by mutiplying the new matrix by the existing top of the stack
	else {
		// Vertex transformations
		matrix_matrix_multiply(Ximage[matlevel - 1], matrix, Ximage[matlevel]);

		// Normal transformations
		matrix_matrix_multiply(Xnorm[matlevel - 1], normals_matrix_parsed, Xnorm[matlevel]);
	}

	// Raise the stack head index by 1
	matlevel++;

	return GZ_SUCCESS;
}

int GzRender::GzPushMatrixUnique(GzMatrix vertex_matrix, GzMatrix normal_matrix) {
	/* HW 3.9
		-- push a matrix onto the Ximage stack
		-- check for stack overflow

	   HW 4
		-- remove translations from Xnorm matrices
		-- ensure rotations are unitary
		-- push a matrix onto the Xnorm stack
	*/

	// Remove translations from any matrix going onto Xnorm
	GzMatrix normals_matrix_parsed = {	normal_matrix[0][0], normal_matrix[0][1], normal_matrix[0][2], 0.0,
										normal_matrix[1][0], normal_matrix[1][1], normal_matrix[1][2], 0.0,
										normal_matrix[2][0], normal_matrix[2][1], normal_matrix[2][2], 0.0,
										normal_matrix[3][0], normal_matrix[3][1], normal_matrix[3][2], normal_matrix[3][3] };

	// Check for stack overflow, return failure
	if (matlevel == MATLEVELS) return GZ_FAILURE;

	// If at the bottom, just add it to the stack
	if (matlevel == 0) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				// Vertex transformations
				Ximage[matlevel][i][j] = vertex_matrix[i][j];

				// Normal transformations
				Xnorm[matlevel][i][j] = normals_matrix_parsed[i][j];
			}
		}
	}

	// Otherwise add to the stack by mutiplying the new matrix by the existing top of the stack
	else {
		// Vertex transformations
		matrix_matrix_multiply(Ximage[matlevel - 1], vertex_matrix, Ximage[matlevel]);

		// Normal transformations
		matrix_matrix_multiply(Xnorm[matlevel - 1], normals_matrix_parsed, Xnorm[matlevel]);
	}

	// Raise the stack head index by 1
	matlevel++;

	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
	/* HW 3.10
		-- pop a matrix off the Ximage stack
		-- check for stack underflow
	*/

	// Decrement the head of the matrix stack if we're not at the base
	if (matlevel != 0) {
		matlevel--;
	}

	return GZ_SUCCESS;
}

/* Pixelbuffer Access */

int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.4 write pixel values into the buffer */

	// Ensure pixel index is within screen-space bounds
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
}

int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	/* HW1.5 retrieve a pixel information from the pixel buffer */

	// Ensure pixel index is within screen space bounds
	if (i >= 0 && i < xres && j >= 0 && j < yres) {
		int idx = xres * i + j;
		*r = pixelbuffer[idx].red;
		*g = pixelbuffer[idx].green;
		*b = pixelbuffer[idx].blue;
		*a = pixelbuffer[idx].alpha;
		*z = pixelbuffer[idx].z;
	}

	// If outside of pixelbuffer bounds return failure
	else return GZ_FAILURE;

	return GZ_SUCCESS;
}

/* Frame I/O */

int GzRender::GzFlushDisplay2File(FILE* outfile)
{
	/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */

	// Error check the input file
	if (!outfile) return GZ_FAILURE;

	// Write header
	char first_buf[50];
	sprintf(first_buf, "P6 %d %d 255\r", xres, yres);
	fprintf(outfile, first_buf);

	// Write data for every pixel
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

	// Loop over the pixelbuffer, convert 12-bit to 8-bit color, place value into the frame buffer
	for (int idx = 0; idx < xres * yres; idx++) {
		framebuffer[3 * idx] = (char)(pixelbuffer[idx].blue >> 4);
		framebuffer[3 * idx + 1] = (char)(pixelbuffer[idx].green >> 4);
		framebuffer[3 * idx + 2] = (char)(pixelbuffer[idx].red >> 4);
	}

	return GZ_SUCCESS;
}

/* Renderer Properties Configuration */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList) 
{
	/* HW 2.1
		-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
		-- In later homeworks set shaders, interpolaters, texture maps, and lights
	
	   HW 4
		== Lighting-Independent Color ==
			GZ_RGB_COLOR				(GzColor)	Sets the color when flat-shading is used

		== Shading Models ==
			GZ_INTERPOLATE				(int)		Sets the shading mode to interpolation
				GZ_FLAT								Flat Shading - Use triangle normal
				GZ_COLOR							Gouroud Shading - Interpolate vertex color
				GZ_NORMALS							Phong Shading - Interpolate vertex normals

		== Lighting Model Parameters ==
			GZ_DIRECTIONAL_LIGHT		(GzLight)	Sets a directional light
			GZ_AMBIENT_LIGHT			(GzLight)	Sets the ambient light color
			GZ_AMBIENT_COEFFICIENT		(GzColor)	Ka ambient reflectance coefficient
			GZ_DIFFUSE_COEFFICIENT		(GzColor)	Kd diffuse reflectance coefficient
			GZ_SPECULAR_COEFFICIENT		(GzColor)	Ks specular reflectance coefficient
			GZ_DISTRIBUTION_COEFFICIENT	(float)		Specular power 's'
	*/

	// Return before looping if either pointer is invalid
	if (!nameList || !valueList) return GZ_FAILURE;

	// Save the status
	int status = 0;

	// Loop over all the parameters
	for (int token_num = 0; token_num < numAttributes; token_num++) {
		if (nameList[token_num] == GZ_RGB_COLOR) 
			status |= GzPutAttribute_RGB_COLOR(valueList[token_num]);
		else if (nameList[token_num] == GZ_INTERPOLATE) 
			status |= GzPutAttribute_INTERPOLATE(valueList[token_num]);
		else if (nameList[token_num] == GZ_DIRECTIONAL_LIGHT) 
			status |= GzPutAttribute_DIRECTIONAL_LIGHT(valueList[token_num]);
		else if (nameList[token_num] == GZ_AMBIENT_LIGHT) 
			status |= GzPutAttribute_AMBIENT_LIGHT(valueList[token_num]);
		else if (nameList[token_num] == GZ_AMBIENT_COEFFICIENT) 
			status |= GzPutAttribute_AMBIENT_COEFFICIENT(valueList[token_num]);
		else if (nameList[token_num] == GZ_DIFFUSE_COEFFICIENT)
			status |= GzPutAttribute_DIFFUSE_COEFFICIENT(valueList[token_num]);
		else if (nameList[token_num] == GZ_SPECULAR_COEFFICIENT) 
			status |= GzPutAttribute_SPECULAR_COEFFICIENT(valueList[token_num]);
		else if (nameList[token_num] == GZ_DISTRIBUTION_COEFFICIENT) 
			status |= GzPutAttribute_DISTRIBUTION_COEFFICIENT(valueList[token_num]);
		else status |= GZ_FAILURE;
	}

	return status;
}

/* Rasterization */

int GzRender::GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList)
{
	/* HW 2.2
		-- Pass in a triangle description with tokens and values corresponding to
			  GZ_NULL_TOKEN:		do nothing - no values
			  GZ_POSITION:		3 vert positions in model space
		-- Invoke the rastrizer/scanline framework
		-- Return error code
		
	   HW 3
		- Apply transformation to vertex coordinates

	   HW 4 
		- Compute Lighting 
		- Compute Shading
		- GZ_NORMAL 
	*/

	// Save the status
	int status = 0;

	// Loop over all the tokens
	for (int token_num = 0; token_num < numParts; token_num++) {
		// Triangle Position
		if (nameList[token_num] == GZ_POSITION) {
			// Get the 3 vertices from the valueList
			GzCoord vertices[3];
			for (size_t tkn = 0; tkn < 3; tkn++) {
				vertices[tkn][0] = (((GzCoord*)valueList[token_num])[tkn])[0];
				vertices[tkn][1] = (((GzCoord*)valueList[token_num])[tkn])[1];
				vertices[tkn][2] = (((GzCoord*)valueList[token_num])[tkn])[2];
			}

			// Save the vertices
			last_triangle[0][0] = vertices[0][0];
			last_triangle[0][1] = vertices[0][1];
			last_triangle[0][2] = vertices[0][2];

			last_triangle[1][0] = vertices[1][0];
			last_triangle[1][1] = vertices[1][1];
			last_triangle[1][2] = vertices[1][2];

			last_triangle[2][0] = vertices[2][0];
			last_triangle[2][1] = vertices[2][1];
			last_triangle[2][2] = vertices[2][2];
		}

		// Triangle Normal - Calls the renderer as now we have a vertex and a normal
		else if (nameList[token_num] == GZ_NORMAL) {
			// Get the 3 normals from the valueList
			GzCoord normals[3];
			for (size_t tkn = 0; tkn < 3; tkn++) {
				// Get the 3 vertices from the valueList
				normals[tkn][0] = (((GzCoord*)valueList[token_num])[tkn])[0];
				normals[tkn][1] = (((GzCoord*)valueList[token_num])[tkn])[1];
				normals[tkn][2] = (((GzCoord*)valueList[token_num])[tkn])[2];
			}

			// Save the normals
			last_normal[0][0] = normals[0][0];
			last_normal[0][1] = normals[0][1];
			last_normal[0][2] = normals[0][2];

			last_normal[1][0] = normals[1][0];
			last_normal[1][1] = normals[1][1];
			last_normal[1][2] = normals[1][2];

			last_normal[2][0] = normals[2][0];
			last_normal[2][1] = normals[2][1];
			last_normal[2][2] = normals[2][2];

			// Save the pre-transformed, pre-sorted first vertex normal for flat shading
			flatcolor_vertex_normal[0] = normals[0][0];
			flatcolor_vertex_normal[1] = normals[0][1];
			flatcolor_vertex_normal[2] = normals[0][2];

			// Sort the vertices
			status |= GzVertexSort(last_triangle, last_normal, Ximage[matlevel - 1]);

			// Apply the current transformation to each vertex for error checking
			GzCoord transformed_vertices[3];
			matrix_vector_multiply(last_triangle[0], Ximage[matlevel - 1], transformed_vertices[0]);
			matrix_vector_multiply(last_triangle[1], Ximage[matlevel - 1], transformed_vertices[1]);
			matrix_vector_multiply(last_triangle[2], Ximage[matlevel - 1], transformed_vertices[2]);

			// Skip this triangle if all x values are less than 0
			if (transformed_vertices[0][0] < 0 && transformed_vertices[1][0] < 0 && transformed_vertices[2][0] < 0) continue;

			// Skip this triangle if all x values are greater than or equal to xres
			if (transformed_vertices[0][0] >= xres && transformed_vertices[1][0] >= xres && transformed_vertices[2][0] >= xres) continue;

			// Skip this triangle if all y values are less than 0
			if (transformed_vertices[0][1] < 0 && transformed_vertices[1][1] < 0 && transformed_vertices[2][1] < 0) continue;

			// Skip this triangle if all y values are greater than or equal to yres
			if (transformed_vertices[0][1] >= yres && transformed_vertices[1][1] >= yres && transformed_vertices[2][1] >= yres) continue;

			// Skip this triangle if any triangle has negative screen-z vertex
			if (transformed_vertices[0][2] < 0 || transformed_vertices[1][2] < 0 || transformed_vertices[2][2] < 0) continue;

			// Rasterize the triangle if it's valid
			status |= GzRasterizeTriangle(transformed_vertices);
		}
	}

	return status;
}

int GzRender::GzVertexSort(GzCoord* vertices, GzCoord* normals, GzMatrix transform_matrix) {
	// Transform the vertices based on the requested transformation
	GzCoord transformed_vertices[3];
	matrix_vector_multiply(vertices[0], transform_matrix, transformed_vertices[0]);
	matrix_vector_multiply(vertices[1], transform_matrix, transformed_vertices[1]);
	matrix_vector_multiply(vertices[2], transform_matrix, transformed_vertices[2]);

	// Sort vertices by Y into (1-2, 2-3) and (1-3)
	// Simple insertion sort
	// The values compared are based on the value of the transformed vertices 
	// Ensures alignment of input vertex with input normal (parallel array sorting)
	int lowestIndex;
	GzCoord temp_vertex, temp_normal, temp_transformed;
	for (size_t i = 0; i < 3; i++) {
		lowestIndex = i;
		for (size_t j = i + 1; j < 3; j++) {
			if (transformed_vertices[j][1] < transformed_vertices[lowestIndex][1]) {
				lowestIndex = j;
			}
		}

		if (lowestIndex != i) {
			// Save vertices[i] into temp_vertex
			temp_vertex[0] = vertices[i][0];
			temp_vertex[1] = vertices[i][1];
			temp_vertex[2] = vertices[i][2];

			// Save normals[i] into temp_vertex
			temp_normal[0] = normals[i][0];
			temp_normal[1] = normals[i][1];
			temp_normal[2] = normals[i][2];

			// Save transformed_vertices[i] into temp_transformed
			temp_transformed[0] = transformed_vertices[i][0];
			temp_transformed[1] = transformed_vertices[i][1];
			temp_transformed[2] = transformed_vertices[i][2];

			// Place new lowest vertex value into vertices[i]
			vertices[i][0] = vertices[lowestIndex][0];
			vertices[i][1] = vertices[lowestIndex][1];
			vertices[i][2] = vertices[lowestIndex][2];

			// Place the cooresponding vertex normal into normals[i]
			normals[i][0] = normals[lowestIndex][0];
			normals[i][1] = normals[lowestIndex][1];
			normals[i][2] = normals[lowestIndex][2];

			// Place the new lowest vertex value into transformed_vertices[i]
			transformed_vertices[i][0] = transformed_vertices[lowestIndex][0];
			transformed_vertices[i][1] = transformed_vertices[lowestIndex][1];
			transformed_vertices[i][2] = transformed_vertices[lowestIndex][2];

			// Place temp vertex value into index of lowest value
			vertices[lowestIndex][0] = temp_vertex[0];
			vertices[lowestIndex][1] = temp_vertex[1];
			vertices[lowestIndex][2] = temp_vertex[2];

			// Place the cooresponding temp normal value into index of lowest value
			normals[lowestIndex][0] = temp_normal[0];
			normals[lowestIndex][1] = temp_normal[1];
			normals[lowestIndex][2] = temp_normal[2];

			// Place temp transformed vertex value into index of lowest value
			transformed_vertices[lowestIndex][0] = temp_transformed[0];
			transformed_vertices[lowestIndex][1] = temp_transformed[1];
			transformed_vertices[lowestIndex][2] = temp_transformed[2];
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzRasterizeTriangle(GzCoord transformed_vertices[3]) {
	// Save status
	int status = GZ_SUCCESS;

	// Skip if no Y change (zero-area triangle in screen space)
	if (transformed_vertices[2][1] - transformed_vertices[0][1] == 0) return GZ_SUCCESS;

	// Compute slopes for the DDA
	float long_edge_slope_xy = (transformed_vertices[2][0] - transformed_vertices[0][0]) / (transformed_vertices[2][1] - transformed_vertices[0][1]);
	float long_edge_slope_zy = (transformed_vertices[2][2] - transformed_vertices[0][2]) / (transformed_vertices[2][1] - transformed_vertices[0][1]);
	float intercept_xy = transformed_vertices[2][0] - long_edge_slope_xy * transformed_vertices[2][1];
	float intercept_zy = transformed_vertices[2][2] - long_edge_slope_zy * transformed_vertices[2][1];
	float edge_check = long_edge_slope_xy * transformed_vertices[1][1] + intercept_xy;

	// Edge Check - Determine if this is a right or left triangle
	bool on_left;

	// Colinear Triangle
	if (transformed_vertices[1][0] == edge_check) return GZ_SUCCESS;

	// Right Triangle
	else if (transformed_vertices[1][0] > edge_check) on_left = false;

	// Left Triangle1
	else on_left = true;

	// Set up DDA for top half Y values
	DDA cur;
	cur.start[0] = transformed_vertices[0][0];
	cur.start[1] = transformed_vertices[0][1];
	cur.start[2] = transformed_vertices[0][2];
	cur.end[0] = transformed_vertices[1][0];
	cur.end[1] = transformed_vertices[1][1];
	cur.end[2] = transformed_vertices[1][2];
	cur.current[0] = transformed_vertices[0][0];
	cur.current[1] = transformed_vertices[0][1];
	cur.current[2] = transformed_vertices[0][2];
	cur.slope_x = (transformed_vertices[1][0] - transformed_vertices[0][0]) / (transformed_vertices[1][1] - transformed_vertices[0][1]);
	cur.slope_z = (transformed_vertices[1][2] - transformed_vertices[0][2]) / (transformed_vertices[1][1] - transformed_vertices[0][1]);

	// Create DDA_Span for X values
	DDA_Span span;

	// Run the scan line algorithm for the top half of the triangle
	status |= GzScanLine(cur, span, on_left, long_edge_slope_xy, long_edge_slope_zy, intercept_xy, intercept_zy);

	// Set up DDA for bottom half Y values
	cur.start[0] = transformed_vertices[1][0];
	cur.start[1] = transformed_vertices[1][1];
	cur.start[2] = transformed_vertices[1][2];
	cur.end[0] = transformed_vertices[2][0];
	cur.end[1] = transformed_vertices[2][1];
	cur.end[2] = transformed_vertices[2][2];
	cur.current[0] = transformed_vertices[1][0];
	cur.current[1] = transformed_vertices[1][1];
	cur.current[2] = transformed_vertices[1][2];
	cur.slope_x = (transformed_vertices[2][0] - transformed_vertices[1][0]) / (transformed_vertices[2][1] - transformed_vertices[1][1]);
	cur.slope_z = (transformed_vertices[2][2] - transformed_vertices[1][2]) / (transformed_vertices[2][1] - transformed_vertices[1][1]);

	// Run the scan line algorithm for the bottom half of the triangle	
	status |= GzScanLine(cur, span, on_left, long_edge_slope_xy, long_edge_slope_zy, intercept_xy, intercept_zy);

	// Return success
	return status;
}

int GzRender::GzScanLine(DDA& cur, DDA_Span& span, bool on_left, float long_edge_slope_xy, float long_edge_slope_zy, float intercept_xy, float intercept_zy) {
	// Save status
	int status = GZ_SUCCESS;

	// Advance to first Y value
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

		// Advance to first X value
		float dx = (float)(ceil(span.start[0]) - span.start[0]);
		span.current[0] = span.start[0] + dx;
		span.current[1] = span.start[1] + dx * span.slope_z;

		// Loop over the X coordinates
		while (span.current[0] < span.end[0]) {
			// Z Buffering - Write current value into the pixel buffer only if it's closer to the camera
			if (span.current[0] >= 0 && span.current[0] < xres && cur.current[1] >= 0 && cur.current[1] < yres && (GzDepth)span.current[1] < pixelbuffer[ARRAY((int)span.current[0], (int)cur.current[1])].z) {
				// Compute the pixel color based on the lighting and shading model
				GzColor color;
				status |= GzLightingShading((int)span.current[0], (int)cur.current[1], color);

				// Place the pixel color on the screen
				status |= GzPut((int)span.current[0], (int)cur.current[1], ctoi(color[0]), ctoi(color[1]), ctoi(color[2]), 4095, (GzDepth)span.current[1]);
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

	return status;
}

/* Shading */

int GzRender::GzLightingShading(int pixel_x, int pixel_y, GzColor& color) {
	// Save the status for error cases
	int status = GZ_SUCCESS;

	// Transform the normals to image space
	GzCoord shading_normals[3];
	matrix_vector_multiply(last_normal[0], Xnorm[matlevel - 1], shading_normals[0]);
	matrix_vector_multiply(last_normal[1], Xnorm[matlevel - 1], shading_normals[1]);
	matrix_vector_multiply(last_normal[2], Xnorm[matlevel - 1], shading_normals[2]);

	// Flat Shading - Compute pixel color using first triangle normal
	if (interp_mode == GZ_FLAT) {
		// For flat shading, we specifically want the pre-transformed, pre-sorted first vertex's normal of the triangle to use for lighting. Only when
		// we use this vertex normal will all the lighting match up between triangles. We then transform this normal into image space light the pixel
		matrix_vector_multiply(flatcolor_vertex_normal, Xnorm[matlevel - 1], shading_normals[0]);

		// Find the triangle color using the first vertex's normal
		status |= GzComputePixelColor(color, shading_normals[0]);

		// Flat shading with no lighting (HW 3)
		//color[0] = flatcolor[0];
		//color[1] = flatcolor[1];
		//color[2] = flatcolor[2];
	}

	// Goroud Shading - Compute pixel color by interpolating vertex colors
	else if (interp_mode == GZ_COLOR) {
		// Find the color at each vertex, here we're using image space for lighting computation
		GzColor vertex_colors[3];
		status |= GzComputePixelColor(vertex_colors[0], shading_normals[0]);
		status |= GzComputePixelColor(vertex_colors[1], shading_normals[1]);
		status |= GzComputePixelColor(vertex_colors[2], shading_normals[2]);

		// Get the screen space vertex positions
		GzCoord vertex_position_screen_space[3];
		matrix_vector_multiply(last_triangle[0], Ximage[matlevel - 1], vertex_position_screen_space[0]);
		matrix_vector_multiply(last_triangle[1], Ximage[matlevel - 1], vertex_position_screen_space[1]);
		matrix_vector_multiply(last_triangle[2], Ximage[matlevel - 1], vertex_position_screen_space[2]);

		// Interpolate the color values at each vertex
		GzCoord vertex_red_channels = { vertex_colors[0][0], vertex_colors[1][0], vertex_colors[2][0] };
		GzCoord vertex_green_channels = { vertex_colors[0][1], vertex_colors[1][1], vertex_colors[2][1] };
		GzCoord vertex_blue_channels = { vertex_colors[0][2], vertex_colors[1][2], vertex_colors[2][2] };
		GzInterpolate(vertex_position_screen_space, vertex_red_channels, (float)pixel_x, (float)pixel_y, color[0]);
		GzInterpolate(vertex_position_screen_space, vertex_green_channels, (float)pixel_x, (float)pixel_y, color[1]);
		GzInterpolate(vertex_position_screen_space, vertex_blue_channels, (float)pixel_x, (float)pixel_y, color[2]);
	}

	// Phong Shading - Interpolating vertex normals and compute color using interpolated normal
	else if (interp_mode == GZ_NORMALS) {
		// Get the screen space vertex positions
		GzCoord vertex_position_screen_space[3];
		matrix_vector_multiply(last_triangle[0], Ximage[matlevel - 1], vertex_position_screen_space[0]);
		matrix_vector_multiply(last_triangle[1], Ximage[matlevel - 1], vertex_position_screen_space[1]);
		matrix_vector_multiply(last_triangle[2], Ximage[matlevel - 1], vertex_position_screen_space[2]);

		// Interpolate the normals at each vertex
		GzCoord interpolated_normal;
		GzCoord vertex_x_values = { shading_normals[0][0], shading_normals[1][0], shading_normals[2][0] };
		GzCoord vertex_y_values = { shading_normals[0][1], shading_normals[1][1], shading_normals[2][1] };
		GzCoord vertex_z_values = { shading_normals[0][2], shading_normals[1][2], shading_normals[2][2] };
		GzInterpolate(vertex_position_screen_space, vertex_x_values, (float)pixel_x, (float)pixel_y, interpolated_normal[0]);
		GzInterpolate(vertex_position_screen_space, vertex_y_values, (float)pixel_x, (float)pixel_y, interpolated_normal[1]);
		GzInterpolate(vertex_position_screen_space, vertex_z_values, (float)pixel_x, (float)pixel_y, interpolated_normal[2]);

		// Find the color at the pixel using the interpolated normal
		status |= GzComputePixelColor(color, interpolated_normal);
	}

	// Internal error for shading mode
	else return GZ_FAILURE;

	return status;
}

/* Lighting */

int GzRender::GzComputePixelColor(GzColor& color, GzCoord normal) {
	/* Ambient Lighting */
	color[0] = Ka[0] * ambientlight.color[0];
	color[1] = Ka[1] * ambientlight.color[1];
	color[2] = Ka[2] * ambientlight.color[2];

	// Loop over all light sources
	for (int light_num = 0; light_num < numlights; light_num++) {
		// Renormalize just in case of fp precision issues
		GzCoord unit_normal = { normal[0], normal[1], normal[2] };
		normalize(unit_normal);

		/* Diffuse Lighting */

		// Find N . L
		GzCoord normal_light_direction = { lights[light_num].direction[0], lights[light_num].direction[1], lights[light_num].direction[2] };
		float NdotL = dot_product(unit_normal, normal_light_direction);

		// The eye vector is simply the direction of our camera (since we're in image space)
		GzCoord eye = { 0.0, 0.0, -1.0 };

		// Find N . E
		float NdotE = dot_product(unit_normal, eye);

		// Determine orientation
		// Both positive (triangle is facing the light) - Compute lighting model

		// Both Negative (compute backside lighting) - Flip normal and compute lighting model on backside of surface
		if (NdotL < 0 && NdotE < 0) {
			// Flip normal and recompute N . L
			GzCoord flipped_normal;
			vector_scale(-1.0, unit_normal, flipped_normal);
			NdotL = dot_product(flipped_normal, normal_light_direction);
		}

		// Alternate Signs (Light and eye are on opposite sides of the surface so the light contributes zero) - Skip it
		else if ((NdotL > 0.0 && NdotE < 0.0) || (NdotL < 0.0 && NdotE > 0.0)) continue;

		// Compute diffuse lighting
		color[0] += Kd[0] * lights[light_num].color[0] * NdotL;
		color[1] += Kd[1] * lights[light_num].color[1] * NdotL;
		color[2] += Kd[2] * lights[light_num].color[2] * NdotL;

		/* Specular Lighting */

		// Compute the reflection vector
		GzCoord reflection_vector;
		vector_scale(NdotL, unit_normal, reflection_vector);
		vector_scale(2.0, reflection_vector, reflection_vector);
		vector_subtract(reflection_vector, normal_light_direction, reflection_vector);

		// Find R . E
		float RdotEtoSpecular = dot_product(reflection_vector, eye);

		// Specifically clamp this value before we raise it to a power
		if (RdotEtoSpecular > 1.0) RdotEtoSpecular = 1.0;
		else if (RdotEtoSpecular < 0.0) RdotEtoSpecular = 0.0;

		// Raise R . E to the specular power
		RdotEtoSpecular = (float)pow(RdotEtoSpecular, spec);

		// Compute specular lighting
		color[0] += Ks[0] * lights[light_num].color[0] * RdotEtoSpecular;
		color[1] += Ks[1] * lights[light_num].color[1] * RdotEtoSpecular;
		color[2] += Ks[2] * lights[light_num].color[2] * RdotEtoSpecular;
	}


	// Check underflow and overflow
	if (color[0] > 1.0) color[0] = 1.0;
	else if (color[0] < 0.0) color[0] = 0.0;

	if (color[1] > 1.0) color[1] = 1.0;
	else if (color[1] < 0.0) color[1] = 0.0;

	if (color[2] > 1.0) color[2] = 1.0;
	else if (color[2] < 0.0) color[2] = 0.0;

	return GZ_SUCCESS;
}

/* Additional Helper Functions */

void GzRender::GzInterpolate(GzCoord xy_values[3], GzCoord z_values, float interpolation_x, float interpolation_y, float& result) {
	// Compute directional vector for A12 (x, y, a) and Color23 (x, y, a)
	// Compute the cross product to get a normal in (x, y, a) space for the plane equation
	GzCoord v1 = { xy_values[0][0], xy_values[0][1], z_values[0] };
	GzCoord v2 = { xy_values[1][0], xy_values[1][1], z_values[1] };
	GzCoord v3 = { xy_values[2][0], xy_values[2][1], z_values[2] };
	GzCoord v12, v23, norm;
	vector_subtract(v2, v1, v12);
	vector_subtract(v3, v2, v23);
	vector_cross_product(v12, v23, norm);

	// Interpolate by solving the plane equation for the a value
	result = v1[2] - (norm[0] * (interpolation_x - v1[0]) + norm[1] * (interpolation_y - v1[1])) / norm[2];
}

/* Attribute Parsing Functions */

int GzRender::GzPutAttribute_RGB_COLOR(GzPointer& token) {
	// Get the GzColor object from the token data
	GzColor* color = (GzColor*)token;

	// Bounds check R value
	if ((*color)[0] > 1.0)
		flatcolor[0] = 1.0;
	else if ((*color)[0] < 0)
		flatcolor[0] = 0;
	else
		flatcolor[0] = (*color)[0];

	// Bounds check G value
	if ((*color)[1] > 1.0)
		flatcolor[1] = 1.0;
	else if ((*color)[1] < 0)
		flatcolor[1] = 1;
	else
		flatcolor[1] = (*color)[1];

	// Bounds check B value
	if ((*color)[2] > 1.0)
		flatcolor[2] = 1.0;
	else if ((*color)[2] < 0)
		flatcolor[2] = 0;
	else
		flatcolor[2] = (*color)[2];

	return GZ_SUCCESS;
}

int GzRender::GzPutAttribute_INTERPOLATE(GzPointer& token) {
	// Parse the generic token
	int* mode = (int*)token;

	// Set the interpolation mode
	if (*mode == GZ_FLAT) interp_mode = GZ_FLAT;
	else if (*mode == GZ_COLOR) interp_mode = GZ_COLOR;
	else if (*mode == GZ_NORMALS) interp_mode = GZ_NORMALS;
	else return GZ_FAILURE;
	return GZ_SUCCESS;
}

int GzRender::GzPutAttribute_DIRECTIONAL_LIGHT(GzPointer& token) {
	// Parse the generic token
	GzLight* directional_light = (GzLight*)token;
	
	// Only support 10 directional lights
	if (numlights >= 10) return GZ_FAILURE;	
	
	// Set direction
	lights[numlights].direction[0] = directional_light->direction[0];
	lights[numlights].direction[1] = directional_light->direction[1];
	lights[numlights].direction[2] = directional_light->direction[2];

	// Error check light color and set
	if (directional_light->color[0] > 1.0) lights[numlights].color[0] = 1.0;
	else if (directional_light->color[0] < 0.0) lights[numlights].color[0] = 0.0;
	else lights[numlights].color[0] = directional_light->color[0];

	if (directional_light->color[1] > 1.0) lights[numlights].color[1] = 1.0;
	else if (directional_light->color[1] < 0.0) lights[numlights].color[1] = 0.0;
	else lights[numlights].color[1] = directional_light->color[1];

	if (directional_light->color[2] > 1.0) lights[numlights].color[2] = 1.0;
	else if (directional_light->color[2] < 0.0) lights[numlights].color[2] = 0.0;
	else lights[numlights].color[2] = directional_light->color[2];

	numlights++;
	return GZ_SUCCESS;
}

int GzRender::GzPutAttribute_AMBIENT_LIGHT(GzPointer& token) {
	// Since only 1 ambient light is supported, we simply overwrite the ambient light on each call to this

	// Parse the generic token
	GzLight* light = (GzLight*)token;

	// Save direction
	ambientlight.direction[0] = light->direction[0];
	ambientlight.direction[1] = light->direction[1];
	ambientlight.direction[2] = light->direction[2];

	// Error check color
	// Red
	if (light->color[0] > 1.0) ambientlight.color[0] = 1.0;
	else if (light->color[0] < 0.0) ambientlight.color[0] = 0.0;
	else ambientlight.color[0] = light->color[0];

	// Green
	if (light->color[1] > 1.0) ambientlight.color[1] = 1.0;
	else if (light->color[1] < 0.0) ambientlight.color[1] = 0.0;
	else ambientlight.color[1] = light->color[1];

	// Blue
	if (light->color[2] > 1.0) ambientlight.color[2] = 1.0;
	else if (light->color[2] < 0.0) ambientlight.color[2] = 0.0;
	else ambientlight.color[2] = light->color[2];

	return GZ_SUCCESS;
}

int GzRender::GzPutAttribute_AMBIENT_COEFFICIENT(GzPointer& token) {
	// Parse the generic token
	GzColor* ambient_color = (GzColor*)token;

	// Error check color values
	// Red
	if ((*ambient_color)[0] > 1.0) Ka[0] = 1.0;
	else if ((*ambient_color)[0] < 0.0) Ka[0] = 0.0;
	else Ka[0] = (*ambient_color)[0];

	// Green
	if ((*ambient_color)[1] > 1.0) Ka[1] = 1.0;
	else if ((*ambient_color)[1] < 0.0) Ka[1] = 0.0;
	else Ka[1] = (*ambient_color)[1];

	// Blue
	if ((*ambient_color)[2] > 1.0) Ka[2] = 1.0;
	else if ((*ambient_color)[2] < 0.0) Ka[2] = 0.0;
	else Ka[2] = (*ambient_color)[2];

	return GZ_SUCCESS;
}

int GzRender::GzPutAttribute_DIFFUSE_COEFFICIENT(GzPointer& token) {
	// Parse the generic token
	GzColor* ambient_diffuse = (GzColor*)token;

	// Error check color values
	// Red
	if ((*ambient_diffuse)[0] > 1.0) Kd[0] = 1.0;
	else if ((*ambient_diffuse)[0] < 0.0) Kd[0] = 0.0;
	else Kd[0] = (*ambient_diffuse)[0];

	// Green
	if ((*ambient_diffuse)[1] > 1.0) Kd[1] = 1.0;
	else if ((*ambient_diffuse)[1] < 0.0) Kd[1] = 0.0;
	else Kd[1] = (*ambient_diffuse)[1];

	// Blue
	if ((*ambient_diffuse)[2] > 1.0) Kd[2] = 1.0;
	else if ((*ambient_diffuse)[2] < 0.0) Kd[2] = 0.0;
	else Kd[2] = (*ambient_diffuse)[2];

	return GZ_SUCCESS;
}

int GzRender::GzPutAttribute_SPECULAR_COEFFICIENT(GzPointer& token) {
	// Parse the generic token
	GzColor* ambient_specular = (GzColor*)token;

	// Error check color values
	// Red
	if ((*ambient_specular)[0] > 1.0) Ks[0] = 1.0;
	else if ((*ambient_specular)[0] < 0.0) Ks[0] = 0.0;
	else Ks[0] = (*ambient_specular)[0];

	// Green
	if ((*ambient_specular)[1] > 1.0) Ks[1] = 1.0;
	else if ((*ambient_specular)[1] < 0.0) Ks[1] = 0.0;
	else Ks[1] = (*ambient_specular)[1];

	// Blue
	if ((*ambient_specular)[2] > 1.0) Ks[2] = 1.0;
	else if ((*ambient_specular)[2] < 0.0) Ks[2] = 0.0;
	else Ks[2] = (*ambient_specular)[2];

	return GZ_SUCCESS;
}

int GzRender::GzPutAttribute_DISTRIBUTION_COEFFICIENT(GzPointer& token) {
	// Parse the generic token
	float* specular_coeff = (float*)token;

	// Save the specular value
	spec = *specular_coeff;

	return GZ_SUCCESS;
}

/* Vector and Matrix Algebra Helper Functions */

void GzRender::identity_matrix(GzMatrix& matrix) {
	matrix[0][0] = 1.0; 	matrix[0][1] = 0.0; 	matrix[0][2] = 0.0;	matrix[0][3] = 0.0;
	matrix[1][0] = 0.0; 	matrix[1][1] = 1.0; 	matrix[1][2] = 0.0;	matrix[1][3] = 0.0;
	matrix[2][0] = 0.0; 	matrix[2][1] = 0.0; 	matrix[2][2] = 1.0;	matrix[2][3] = 0.0;
	matrix[3][0] = 0.0; 	matrix[3][1] = 0.0; 	matrix[3][2] = 0.0;	matrix[3][3] = 1.0;
}

float GzRender::dot_product(GzCoord x, GzCoord y) {
	return (x[0] * y[0] + x[1] * y[1] + x[2] * y[2]);
}

void GzRender::dot_product(GzCoord x, GzCoord y, float& dst) {
	dst = (x[0] * y[0] + x[1] * y[1] + x[2] * y[2]);
}

void GzRender::vector_scale(float scale, GzCoord src, GzCoord& dst) {
	dst[0] = src[0] * scale;
	dst[1] = src[1] * scale;
	dst[2] = src[2] * scale;
}

void GzRender::vector_subtract(GzCoord x, GzCoord y, GzCoord& dst) {
	dst[0] = x[0] - y[0];
	dst[1] = x[1] - y[1];
	dst[2] = x[2] - y[2];
}

void GzRender::normalize(GzCoord& src) {
	float norm = normalization_factor(src);
	src[0] = src[0] / norm;
	src[1] = src[1] / norm; 
	src[2] = src[2] / norm;
}

float GzRender::normalization_factor(GzCoord& src) {
	return (float)sqrt(1.0 * src[0] * src[0] + 1.0 * src[1] * src[1] + 1.0 * src[2] * src[2]);
}

void GzRender::vector_cross_product(GzCoord x, GzCoord y, GzCoord& dst) {
	float a, b, c;
	a = (x[1] * y[2] - x[2] * y[1]);
	b = -1.0f * (x[0] * y[2] - x[2] * y[0]);
	c = (x[0] * y[1] - x[1] * y[0]);
	dst[0] = a;
	dst[1] = b;
	dst[2] = c;
}

void GzRender::matrix_matrix_multiply(GzMatrix x, GzMatrix y, GzMatrix& dst) {
	GzMatrix tmp;
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			tmp[i][j] = x[i][0] * y[0][j] + x[i][1] * y[1][j] + x[i][2] * y[2][j] + x[i][3] * y[3][j];
		}
	}

	// Temporary matrix allows for in-place modifications
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			dst[i][j] = tmp[i][j];
		}
	}
}

void GzRender::matrix_vector_multiply(GzCoord x, GzMatrix y, GzCoord& dst) {
	float w = y[3][0] * x[0] + y[3][1] * x[1] + y[3][2] * x[2] + y[3][3];
	float a = (y[0][0] * x[0] + y[0][1] * x[1] + y[0][2] * x[2] + y[0][3]) / w;
	float b = (y[1][0] * x[0] + y[1][1] * x[1] + y[1][2] * x[2] + y[1][3]) / w;
	float c = (y[2][0] * x[0] + y[2][1] * x[1] + y[2][2] * x[2] + y[2][3]) / w;
	dst[0] = a;
	dst[1] = b;
	dst[2] = c;
}