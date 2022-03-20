/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

#define FRACTAL_DEPTH 8

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

// Function prototype
int GzBilinearInterpolation(float u, float v, GzColor& interpolated_color, int image_size_x, int image_size_y, GzColor* image);

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i;
  FILE			*fd;

  if (reset) {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0f / 255.0f);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0f / 255.0f);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0f / 255.0f);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

  // Invalid texture coordinates - Set to black
  if (u > 1.0 || v > 1.0 || u < 0.0 || v < 0.0) {
    color[RED] = 0.0;
    color[GREEN] = 0.0;
    color[BLUE] = 0.0;
    return GZ_FAILURE;
  }

  // Bilinear interpolation
  int status = GZ_SUCCESS;
  GzColor interpolated_color;
  status |= GzBilinearInterpolation(u, v, interpolated_color, xs, ys, image);
  color[RED] = interpolated_color[RED];
  color[GREEN] = interpolated_color[GREEN];
  color[BLUE] = interpolated_color[BLUE];

  return status;
}

/* Procedural Checkerboard Texture Function */
int procedural_checkerboard(float u, float v, GzColor color)
{
    // Invalid texture coordiantes - Set to black
    if (u > 1.0 || v > 1.0 || u < 0.0 || v < 0.0) {
        color[RED] = 0.0;
        color[GREEN] = 0.0;
        color[BLUE] = 0.0;
        return GZ_FAILURE;
    }

    // Checkerboard texture
    float NUM_U_SQUARES = 12;
    float NUM_V_SQUARES = 12;
    float u_square_division = 1.0f / NUM_U_SQUARES;
    float v_square_division = 1.0f / NUM_V_SQUARES;
    int u_square = (int)floor(u / u_square_division);
    int v_square = (int)floor(v / v_square_division);
    if ((u_square + v_square) % 2 == 0) {
        color[RED] = 0;
        color[GREEN] = 0;
        color[BLUE] = 0;
    }
    else {
        color[RED] = 1.0f;
        color[GREEN] = 1.0f;
        color[BLUE] = 1.0f;
    }

	return GZ_SUCCESS;
}

/* Recursive Function for generating fractal texture */
int fractal_helper(int depth, float u, float v, float xs, float xe, float ys, float ye) {
    if (!depth) return 1;
    
    float x_step = (xe - xs) / 3;
    float x_mid_start = xs + x_step;
    float x_mid_end = xs + x_step * 2;
    float y_step = (ye - ys) / 3;
    float y_mid_start = ys + y_step;
    float y_mid_end = ys + y_step * 2;
    if (u > x_mid_start && u < x_mid_end && v > y_mid_start && v < y_mid_end) return 0;

    float new_xs, new_xe, new_ys, new_ye;
    if (u < x_mid_start) {
        new_xs = xs;
        new_xe = xs + x_step;
    }
    else if (u >= x_mid_start && u < x_mid_end) {
        new_xs = xs + x_step;
        new_xe = xs + x_step * 2;
    }
    else {
        new_xs = xs + x_step * 2;
        new_xe = xe;
    }
    if (v < y_mid_start) {
        new_ys = ys;
        new_ye = ys + y_step;
    }
    else if (v >= y_mid_start && v < y_mid_end) {
        new_ys = ys + y_step;
        new_ye = ys + y_step * 2;
    }
    else {
        new_ys = ys + y_step * 2;
        new_ye = ye;
    }
   
    return fractal_helper(depth - 1, u, v, new_xs, new_xe, new_ys, new_ye);
}

/* Procedular Fractal Texture Function */
int procedural_fractal(float u, float v, GzColor color) {
    // Evaluate the fractal
    int value = fractal_helper(FRACTAL_DEPTH, u, v, 0, 1, 0, 1);

    color[RED] = (float)value;
    color[GREEN] = (float)value;
    color[BLUE] = (float)value;

    return GZ_SUCCESS;
}

/* Selects the procedural texture to use */
int ptex_fun(float u, float v, GzColor color) {
#if 1
    return procedural_fractal(u, v, color);
#else
    return procedural_checkerboard(u, v, color);
#endif
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

int GzBilinearInterpolation(float u, float v, GzColor& interpolated_color, int image_size_x, int image_size_y, GzColor* image) {
    // Scale to size of texture
    float scaled_u = u * (image_size_x - 1);
    float scaled_v = v * (image_size_y - 1);

    // Take the ceiling for the "right" pixel and the floor for the "left" pixel
    int u_ceil = (int)ceil(scaled_u);
    int v_ceil = (int)ceil(scaled_v);
    int u_floor = (int)floor(scaled_u);
    int v_floor = (int)floor(scaled_v);

    // Bilinear interpolation based on floor and ceil of u and v
    float s_value = scaled_u - u_floor;
    float t_value = scaled_v - v_floor;
    interpolated_color[RED] =   s_value          * t_value           * image[image_size_x * v_ceil + u_ceil][RED] +
                                (1.0f - s_value) * t_value           * image[image_size_x * v_ceil + u_floor][RED] + 
                                s_value          * (1.0f - t_value)   * image[image_size_x * v_floor + u_ceil][RED] +
                                (1.0f - s_value) * (1.0f - t_value)   * image[image_size_x * v_floor + u_floor][RED];

    interpolated_color[GREEN] = s_value          * t_value           * image[image_size_x * v_ceil + u_ceil][GREEN] +
                                (1.0f - s_value) * t_value           * image[image_size_x * v_ceil + u_floor][GREEN] +
                                s_value          * (1.0f - t_value)   * image[image_size_x * v_floor + u_ceil][GREEN] +
                                (1.0f - s_value) * (1.0f - t_value)   * image[image_size_x * v_floor + u_floor][GREEN];

    interpolated_color[BLUE] =  s_value          * t_value           * image[image_size_x * v_ceil + u_ceil][BLUE] +
                                (1.0f - s_value) * t_value           * image[image_size_x * v_ceil + u_floor][BLUE] +
                                s_value          * (1.0f - t_value)   * image[image_size_x * v_floor + u_ceil][BLUE] +
                                (1.0f - s_value) * (1.0f - t_value)   * image[image_size_x * v_floor + u_floor][BLUE];

    return GZ_SUCCESS;
}

