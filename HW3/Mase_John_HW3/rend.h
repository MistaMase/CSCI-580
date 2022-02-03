#include	"gz.h"
#ifndef GZRENDER_
#define GZRENDER_

/* Camera defaults */
#define	DEFAULT_FOV	35.0
/* world coords for image plane origin */
#define	DEFAULT_IM_Z	(-10.0)  
#define	DEFAULT_IM_Y	(5.0)    
#define	DEFAULT_IM_X	(-10.0)
/* default look-at point = 0,0,0 */

#define	DEFAULT_AMBIENT	{0.1, 0.1, 0.1}
#define	DEFAULT_DIFFUSE	{0.7, 0.6, 0.5}
#define	DEFAULT_SPECULAR	{0.2, 0.3, 0.4}
#define	DEFAULT_SPEC		32

#define	MATLEVELS	100	/* how many matrix pushes allowed */
#define	MAX_LIGHTS	20	/* how many lights allowed */

/* Dummy definition : change it later */
#ifndef GzLight
#define GzLight		GzPointer
#endif

#ifndef GzTexture
#define GzTexture	GzPointer
#endif

class GzRender{			/* define a renderer */
  

public:
	unsigned short	xres;
	unsigned short	yres;
	GzPixel		*pixelbuffer;		/* frame buffer array */
	char* framebuffer;

	GzCamera		m_camera;
	short		matlevel;	/* top of stack - current xform */
	GzMatrix		Ximage[MATLEVELS];	/* stack of xforms (Xsm) */
	GzMatrix		Xnorm[MATLEVELS];	/* xform for norms (Xim) */
	GzMatrix		Xsp;		/* NDC to screen (pers-to-screen) */
	GzColor		flatcolor;    /* color state for flat shaded triangles */
	int			interp_mode;
	int			numlights;
	GzLight		lights[MAX_LIGHTS];
	GzLight		ambientlight;
	GzColor		Ka, Kd, Ks;
	float		spec;		/* specular power */
	GzTexture		tex_fun;  /* tex_fun(float u, float v, GzColor color) */


	// Constructors
	GzRender(int xRes, int yRes);
	~GzRender();

	// HW1: Display methods
	int GzDefault();
	int GzBeginRender();
	int GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z);
	int GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth	*z);

	int GzFlushDisplay2File(FILE* outfile);
	int GzFlushDisplay2FrameBuffer();

	// HW2: Render methods
	int GzPutAttribute(int numAttributes, GzToken *nameList, GzPointer *valueList);
	int GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList);
	
	// HW3
	int GzDefaultCamera();
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
private:
	typedef struct {
		float start[3], end[3];
		float current[3];
		float slope_x, slope_z;
	} DDA;

	typedef struct {
		float start[2], end[2];
		float current[2];
		float slope_z;
	} DDA_Span;

	// Simple vector and matrix operations
	float dot_product(GzCoord x, GzCoord y) { return (x[0] * y[0] + x[1] * y[1] + x[2] * y[2]); }
	void dot_product(GzCoord x, GzCoord y, float& dst) {  dst = (x[0] * y[0] + x[1] * y[1] + x[2] * y[2]); }
	void vector_scale_inplace(float scale, GzCoord& x) { x[0] = x[0] * scale; x[1] = x[1] * scale; x[2] = x[2] * scale; }
	void vector_scale(float scale, GzCoord& src, GzCoord& dst) { dst[0] = src[0] * scale; dst[1] = src[1] * scale; dst[2] = src[2] * scale; }
	void vector_subtract(GzCoord x, GzCoord y, GzCoord& dst) { dst[0] = x[0] - y[0]; dst[1] = x[1] - y[1]; dst[2] = x[2] - y[2]; }
	void normalize(GzCoord& src) { float norm = normalization_factor(src); src[0] = src[0] / norm; src[1] = src[1] / norm; src[2] = src[2] / norm; }
	float normalization_factor(GzCoord& src) { return (float)sqrt(1.0 * src[0] * src[0] + 1.0 * src[1] * src[1] + 1.0 * src[2] * src[2]); }
	void vector_cross_product(GzCoord x, GzCoord y, GzCoord& dst) { dst[0] = (x[1] * y[2] - x[2] * y[1]); dst[1] = -1.0f * (x[0] * y[2] - x[2] * y[0]); dst[2] = (x[0] * y[1] - x[1] * y[0]); }
	void matrix_matrix_multiply(GzMatrix x, GzMatrix y, GzMatrix& dst) { 
		for (int i = 0; i < 4; i++) { 
			for (int j = 0; j < 4; j++) { 
				dst[i][j] = x[i][0] * y[0][j] + x[i][1] * y[1][j] + x[i][2] * y[2][j] + x[i][3] * y[3][j]; 
			}
		}
	}
	void matrix_vector_multiply(GzCoord x, GzMatrix y, GzCoord& dst) { 
		float w = y[3][0] * x[0] + y[3][1] * x[1] + y[3][2] * x[2] + y[3][3];
		float a = (y[0][0] * x[0] + y[0][1] * x[1] + y[0][2] * x[2] + y[0][3]) / w;
		float b = (y[1][0] * x[0] + y[1][1] * x[1] + y[1][2] * x[2] + y[1][3]) / w;
		float c = (y[2][0] * x[0] + y[2][1] * x[1] + y[2][2] * x[2] + y[2][3]) / w;
		dst[0] = a;
		dst[1] = b;
		dst[2] = c;
	}
};

#endif
