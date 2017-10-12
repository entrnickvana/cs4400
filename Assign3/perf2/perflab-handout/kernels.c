#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include <math.h>

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Nicholas Elliott",     /* Full name */
  "u0682219@utah.edu",  /* Email address */

};

/******************************************************
 * PINWHEEL KERNEL
 *
 * Your different versions of the pinwheel kernel go here
 ******************************************************/

/* 
 * naive_pinwheel - The naive baseline version of pinwheel 
 */
char naive_pinwheel_descr[] = "naive_pinwheel: baseline implementation";
void naive_pinwheel(pixel *src, pixel *dest)
{
  int qi, qj, i, j;

  /* qi & qj are column and row of quadrant
     i & j are column and row within quadrant

	#define RIDX(i,j,n) (1+(i)*(n)+(j))

      */

  /* Loop over 4 quadrants: */
  for (qi = 0; qi < 2; qi++)
    for (qj = 0; qj < 2; qj++)
      /* Loop within quadrant: */
      for (i = 0; i < src->dim/2; i++)
        for (j = 0; j < src->dim/2; j++) {
          int s_idx = RIDX((qj * src->dim/2) + i,
                           j + (qi * src->dim/2), src->dim);
          int d_idx = RIDX((qj * src->dim/2) + src->dim/2 - 1 - j,
                           i + (qi * src->dim/2), src->dim);
          dest[d_idx].red = (src[s_idx].red
                             + src[s_idx].green
                             + src[s_idx].blue) / 3;
          dest[d_idx].green = (src[s_idx].red
                               + src[s_idx].green
                               + src[s_idx].blue) / 3;
          dest[d_idx].blue = (src[s_idx].red
                              + src[s_idx].green
                              + src[s_idx].blue) / 3;
        }
}



void pinwheel_1(pixel *src, pixel *dest)
{
	int qi, qj, i, j;

  /* qi & qj are column and row of quadrant
     i & j are column and row within quadrant */

	/*
		1) Common Sub expressions
			acessing dim

	*/

	int ii;
	int jj;
	int localDim = src->dim;
	int hlfDim = localDim/2;
	int qj_hlf_dim;
	int qi_hlf_dim;
	int result = 0;
	int W = 16;

  /* Loop over 4 quadrants: */
  for (qi = 0; qi < 2; qi++){

    for (qj = 0; qj < 2; qj++){
      /* Loop within quadrant: */

      for (j = 0; j < hlfDim; j+= W){

        for (i = 0; i < hlfDim; i+= W) {

        	for(jj = j; jj < j + W; jj++){
	        	for(ii = i; ii < i + W; ii++){
	        

		          qj_hlf_dim = qj * hlfDim;
		          qi_hlf_dim = qi * hlfDim;

		          int s_idx = RIDX(qj_hlf_dim + ii, 
		          				   jj + (qi_hlf_dim),
		          				   localDim);

		          int d_idx = RIDX((qj_hlf_dim) + hlfDim- 1 - jj,
		                           ii + (qi_hlf_dim),
		                           localDim);

		          result =  (src[s_idx].red + src[s_idx].green + src[s_idx].blue) / 3;

		          dest[d_idx].red = result;

		          dest[d_idx].green = result;

		          dest[d_idx].blue = result;
		      	}
        	}
		    
	      	
        }
      }

    }
  }
}

/* 
 * pinwheel - Your current working version of pinwheel
 * IMPORTANT: This is the version you will be graded on
 */
char pinwheel_descr[] = "pinwheel: Current working version V1 (for grading)";
void pinwheel(pixel *src, pixel *dest)
{
  pinwheel_1(src, dest);
}

/*********************************************************************
 * register_pinwheel_functions - Register all of your different versions
 *     of the pinwheel kernel with the driver by calling the
 *     add_pinwheel_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_pinwheel_functions() {
  add_pinwheel_function(&pinwheel, pinwheel_descr);
  add_pinwheel_function(&naive_pinwheel, naive_pinwheel_descr);
}


/***************************************************************
 * MOTION KERNEL
 * 
 * Starts with various typedefs and helper functions for the motion
 * function, and you may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
  int red;
  int green;
  int blue;
} pixel_sum;

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
  sum->red = sum->green = sum->blue = 0;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_weighted_sum(pixel_sum *sum, pixel p, double weight) 
{
  sum->red += (int) p.red * weight;
  sum->green += (int) p.green * weight;
  sum->blue += (int) p.blue * weight;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
  current_pixel->red = (unsigned short)sum.red;
  current_pixel->green = (unsigned short)sum.green;
  current_pixel->blue = (unsigned short)sum.blue;
}

/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo(int dim, int i, int j, pixel *src) 
{
  int ii, jj;
  pixel_sum sum;
  pixel current_pixel;
  double weights[3][3] = { { 0.60, 0.03, 0.00 },
                           { 0.03, 0.30, 0.03 },
                           { 0.00, 0.03, 0.10 } };

  initialize_pixel_sum(&sum);
  for (ii=0; ii < 3; ii++)
    for (jj=0; jj < 3; jj++) 
      if ((i + ii < dim) && (j + jj < dim))
        accumulate_weighted_sum(&sum,
                                src[RIDX(i+ii,j+jj,dim)],
                                weights[ii][jj]);
  
  assign_sum_to_pixel(&current_pixel, sum);

  return current_pixel;
}

static pixel weighted_combo2(int dim, int i, int j, pixel *src) 
{

  pixel_sum sum;
  pixel current_pixel;
 
  initialize_pixel_sum(&sum);

/*
  		 sum.red =    (  	(int)(i_base0_0->red * 0.6)
  		 				+  	(int)(i_base1_1->red  * 0.3)
  		 				+  	(int)(i_base2_2->red  * 0.1)
  		 				+  	(int)(i_base1_0->red  * 0.03)
  		 				+  	(int)(i_base0_1->red  * 0.03)
  		 				+  	(int)(i_base1_2->red  * 0.03)
  		 				+  	(int)(i_base2_1->red  * 0.03));

  		 sum.green =    (  	(int)(i_base0_0->green * 0.6)
  		 				+  	(int)(i_base1_1->green  * 0.3)
  		 				+  	(int)(i_base2_2->green  * 0.1)
  		 				+  	(int)(i_base1_0->green  * 0.03)
  		 				+  	(int)(i_base0_1->green * 0.03)
  		 				+  	(int)(i_base1_2->green * 0.03)
  		 				+  	(int)(i_base2_1->green  * 0.03));

  		 sum.blue =    (  	(int)(i_base0_0->blue * 0.6)
  		 				+  	(int)(i_base1_1->blue * 0.3)
  		 				+  	(int)(i_base2_2->blue * 0.1)
  		 				+  	(int)(i_base1_0->blue * 0.03)
  		 				+  	(int)(i_base0_1->blue * 0.03)
  		 				+  	(int)(i_base1_2->blue * 0.03)
  		 				+  	(int)(i_base2_1->blue * 0.03));
*/

  		sum.red = 	(	(int)(src[RIDX(i,j,dim)].red * 6/10)
  					+	(int)(src[RIDX(i+1,j+1,dim)].red * 3/10)
  					+	(int)(src[RIDX(i+2,j+2,dim)].red * 1/10)
  					+	(int)(src[RIDX(i,j+1,dim)].red * 3/100)
  					+	(int)(src[RIDX(i+1,j,dim)].red * 3/100)
  					+	(int)(src[RIDX(i+1,j+2,dim)].red * 3/100)
  					+	(int)(src[RIDX(i+2,j+1,dim)].red * 3/100)

  					);

  		sum.green = (	(int)(src[RIDX(i,j,dim)].green * 6/10)
  					+	(int)(src[RIDX(i+1,j+1,dim)].green * 3/10)
  					+	(int)(src[RIDX(i+2,j+2,dim)].green * 1/10)
  					+	(int)(src[RIDX(i,j+1,dim)].green * 3/100)
  					+	(int)(src[RIDX(i+1,j,dim)].green * 3/100)
  					+	(int)(src[RIDX(i+1,j+2,dim)].green * 3/100)
  					+	(int)(src[RIDX(i+2,j+1,dim)].green * 3/100)

  					);

  		sum.blue = (	(int)(src[RIDX(i,j,dim)].blue * 6/10)
  					+	(int)(src[RIDX(i+1,j+1,dim)].blue * 3/10)
  					+	(int)(src[RIDX(i+2,j+2,dim)].blue * 1/10)
  					+	(int)(src[RIDX(i,j+1,dim)].blue * 3/100)
  					+	(int)(src[RIDX(i+1,j,dim)].blue * 3/100)
  					+	(int)(src[RIDX(i+1,j+2,dim)].blue * 3/100)
  					+	(int)(src[RIDX(i+2,j+1,dim)].blue * 3/100)

  					);

/*
  		 sum.red =  (		(int)(src[RIDX(i,j,dim)].red * (6/10))
  		 				+  	(int)(src[RIDX(i+1,j+1,dim)].red * (3/10))
  		 				+  	(int)(src[RIDX(i+2,j+2,dim)].red * (1/10))
  		 				+   (int)(src[RIDX(i,j+1,dim)].red * (3/100))
  		 				+  	(int)(src[RIDX(i+1,j,dim)].red * (3/100))
  		 				+  	(int)(src[RIDX(i+1,j+2,dim)].red * (3/100))
  		 				+  	(int)(src[RIDX(i+2,j+1,dim)].red * (3/100)));

  		 sum.green =  (		(int)(src[RIDX(i,j,dim)].green * (6/10))
  		 				+  	(int)(src[RIDX(i+1,j+1,dim)].green * (3/10))
  		 				+  	(int)(src[RIDX(i+2,j+2,dim)].green * (1/10))
  		 				+   (int)(src[RIDX(i,j+1,dim)].green * (3/100))
  		 				+  	(int)(src[RIDX(i+1,j,dim)].green * (3/100))
  		 				+  	(int)(src[RIDX(i+1,j+2,dim)].green * (3/100))
  		 				+  	(int)(src[RIDX(i+2,j+1,dim)].green * (3/100)));

  		 sum.blue =  (		(int)(src[RIDX(i,j,dim)].blue * (6/10))
  		 				+  	(int)(src[RIDX(i+1,j+1,dim)].blue * (3/10))
  		 				+  	(int)(src[RIDX(i+2,j+2,dim)].blue * (1/10))
  		 				+   (int)(src[RIDX(i,j+1,dim)].blue * (3/100))
  		 				+  	(int)(src[RIDX(i+1,j,dim)].blue * (3/100))
  		 				+  	(int)(src[RIDX(i+1,j+2,dim)].blue * (3/100))
  		 				+  	(int)(src[RIDX(i+2,j+1,dim)].blue * (3/100)));
 */
  
  assign_sum_to_pixel(&current_pixel, sum);

  return current_pixel;

}




void motion1(pixel *src, pixel *dst) 
{
  int i, j;	
  int dim = src->dim;
  
  for (i = 0; i < dim; i++){

    for (j = 0; j < dim; j++){

      if( (i < (dim -3)) && (j < (dim - 3))){

      	dst[RIDX(i, j, dim)] = weighted_combo2(dim, i, j, src);

      }else{

 	  	dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);

      }

    }
  }
}

/******************************************************
 * Your different versions of the motion kernel go here
 ******************************************************/

/*
 * naive_motion - The naive baseline version of motion 
 */
char naive_motion_descr[] = "naive_motion: baseline implementation";
void naive_motion(pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < src->dim; i++)
    for (j = 0; j < src->dim; j++)
      dst[RIDX(i, j, src->dim)] = weighted_combo(src->dim, i, j, src);
}

/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion version 3 (for grading)";
void motion(pixel *src, pixel *dst) 
{
  motion1(src, dst);

}

/********************************************************************* 
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_motion_functions() {
  add_motion_function(&motion, motion_descr);
  add_motion_function(&naive_motion, naive_motion_descr);
}
