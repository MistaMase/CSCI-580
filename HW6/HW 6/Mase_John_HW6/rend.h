#include	"gz.h"
#ifndef GZRENDER_
#define GZRENDER_


/* Camera defaults */
#define	DEFAULT_FOV			35.0
#define	DEFAULT_IM_Z		(-10.0)		/* world coords for image plane origin */
#define	DEFAULT_IM_Y		(5.0)		/* default look-at point = 0,0,0 */
#define	DEFAULT_IM_X		(-10.0)

#define	DEFAULT_AMBIENT		{0.1, 0.1, 0.1}
#define	DEFAULT_DIFFUSE		{0.7, 0.6, 0.5}
#define	DEFAULT_SPECULAR	{0.2, 0.3, 0.4}
#define	DEFAULT_SPEC		32

#define	MATLEVELS			100			/* how many matrix pushes allowed */
#define	MAX_LIGHTS			10			/* how many lights allowed */

#define AAKERNEL_SIZE		6			/* number of samples for anti-aliasing */

class GzRender{							/* define a renderer */
  

public:
	unsigned short	xres;
	unsigned short	yres;
	GzPixel		*pixelbuffer;			/* frame buffer array */
	char* framebuffer;

	GzCamera		m_camera;
	short		    matlevel;	        /* top of stack - current xform */
	GzMatrix		Ximage[MATLEVELS];	/* stack of xforms (Xsm) */
	GzMatrix		Xnorm[MATLEVELS];	/* xforms for norms (Xim) */
	GzMatrix		Xsp;		        /* NDC to screen (pers-to-screen) */
	GzColor		flatcolor;				/* color state for flat shaded triangles */
	int			interp_mode;
	int			numlights;
	GzLight		lights[MAX_LIGHTS];
	GzLight		ambientlight;
	GzColor		Ka, Kd, Ks;
	float		    spec;				/* specular power */
	GzTexture		tex_fun;			/* tex_fun(float u, float v, GzColor color) */

  	// Constructors
	GzRender(int xRes, int yRes);
	~GzRender();

	// HW1: Display methods
	int GzDefault();
	int GzBeginRender();
	int GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a);
	int GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth	*z);

	int GzFlushDisplay2File(FILE* outfile);
	int GzFlushDisplay2FrameBuffer();

	// HW2: Render methods
	int GzPutAttribute(int numAttributes, GzToken *nameList, GzPointer *valueList);
	int GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList);

	// HW3
	int GzPutCamera(GzCamera camera);
	int GzPushMatrix(GzMatrix	matrix);
	int GzPopMatrix();

	// Extra methods: NOT part of API - just for general assistance */
	inline int ARRAY(int x, int y){return (x+y*xres);}	/* simplify fbuf indexing */
	inline short	ctoi(float color) {return(short)((int)(color * ((1 << 12) - 1)));}		/* convert float color to GzIntensity short */

	// Object Translation
	int GzRotXMat(float degree, GzMatrix mat);
	int GzRotYMat(float degree, GzMatrix mat);
	int GzRotZMat(float degree, GzMatrix mat);
	int GzTrxMat(GzCoord translate, GzMatrix mat);
	int GzScaleMat(GzCoord scale, GzMatrix mat);

// Added additional data structures for the scan-line algorithm
// Added additional helper methods for vector and matrix operations
// Added additional helper methods for lighting and shading
private:
	// Antialiasing kernel weights and offsets
	float AAFilter[AAKERNEL_SIZE][3] = {
		-0.52f,  0.38f, 0.128f,		0.41f,  0.56f, 0.119f,		0.27f,   0.08f, 0.294f,
		-0.17f, -0.29f, 0.249f,		0.58f, -0.55f, 0.104f,		-0.31f, -0.71f, 0.106f
	};

	// Scanline algorithm vertical structure
	typedef struct {
		float start[3], end[3];
		float current[3];
		float slope_x, slope_z;
	} DDA;

	// Scanline algorithm horizontal structure
	typedef struct {
		float start[2], end[2];
		float current[2];
		float slope_z;
	} DDA_Span;

	// Holds the triangle information necessary for rasterization
	typedef struct {
		GzCoord vertices[3];
		GzCoord normals[3];
		GzTextureIndex textures[3];
		GzCoord flatcolor_vertex_normal;
	} RasterTriangle;

	// Small struct to hold the per-pixel results for anti-aliasing
	typedef struct {
		GzPixel* pixel_offset;
	} AAOffsetBuffer;

	// Large buffer to hold all multi-sampled values of the pixel
	AAOffsetBuffer aa_buffer[AAKERNEL_SIZE];

	// Stores the data from the last triangle to be used while rasterizing
	RasterTriangle last_triangle;

	// Adds the pixel values into the AA Buffer
	int GzPutAA(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z, int kernel_offset);

	// Pushes two different matrices onto the vertex stack and normal stack, respectively
	int GzPushMatrixUnique(GzMatrix vertex_matrix, GzMatrix normal_matrix);

	// Calls the renderer when given the necessary information
	int GzRenderTriangle();

	// Vertically sort vertices in a triangle in transform_matrix space, reorders normals based on vertex ordering
	int GzVertexSort(GzCoord* vertices, GzCoord* normals, GzTextureIndex* texture_indices, GzMatrix transform_matrix);

	// Populates the DDAs and invokes the scan line algorithm
	int GzRasterizeTriangle(GzCoord transformed_vertices[3], int aa_num);

	// Carries out the scan line algorithm 
	int GzScanLine(DDA& cur, DDA_Span& span, bool on_left, float long_edge_slope_xy, float long_edge_slope_zy, float intercept_xy, float intercept_zy, int aa_num);

	// Determines and calls the necessary interpolation depending on the current model
	int GzLightingShading(int pixel_x, int pixel_y, float pixel_z, GzColor& color);

	// Computes lighting at each pixel
	int GzComputePixelColor(GzColor& color, GzCoord normal, GzColor texture_color, GzToken shading_mode);

	// Interpolates based on plane equation
	void GzPlanarInterpolate(GzCoord xy_values[3], GzCoord z_values, float interpolation_x, float interpolation_y, float& result);

	// Perspective-correct interpolation based on plane equation
	void GzPerspectiveCorrectInterpolation(GzCoord* vertex_position_screen_space, GzTextureIndex* texture_coordinates, int pixel_x, int pixel_y, float pixel_z, GzTextureIndex& affine_texture_coordinate);

	// Functions for parsing specific tokens
	int GzPutAttribute_RGB_COLOR(GzPointer& token);
	int GzPutAttribute_INTERPOLATE(GzPointer& token);
	int GzPutAttribute_DIRECTIONAL_LIGHT(GzPointer& token);
	int GzPutAttribute_AMBIENT_LIGHT(GzPointer& token);
	int GzPutAttribute_AMBIENT_COEFFICIENT(GzPointer& token);
	int GzPutAttribute_DIFFUSE_COEFFICIENT(GzPointer& token);
	int GzPutAttribute_SPECULAR_COEFFICIENT(GzPointer& token);
	int GzPutAttribute_DISTRIBUTION_COEFFICIENT(GzPointer& token);
	int GzPutAttribute_TEXTURE_MAP(GzPointer& token);

	// Simple vector and matrix operations
	// All are safe for in-place operations
	void identity_matrix(GzMatrix& matrix);
	float dot_product(GzCoord a, GzCoord b);
	void dot_product(GzCoord a, GzCoord b, float& dst);
	void vector_scale(float scale, GzCoord src, GzCoord& dst);
	void vector_subtract(GzCoord a, GzCoord b, GzCoord& dst);
	void normalize(GzCoord& src);
	float normalization_factor(GzCoord& src);
	void vector_cross_product(GzCoord a, GzCoord b, GzCoord& dst);
	void matrix_matrix_multiply(GzMatrix a, GzMatrix b, GzMatrix& dst);
	void matrix_vector_multiply(GzCoord a, GzMatrix b, GzCoord& dst);
	void matrix_vector_multiply_offset(GzCoord a, GzMatrix b, GzCoord& dst, float offset_x, float offset_y);
};

#endif