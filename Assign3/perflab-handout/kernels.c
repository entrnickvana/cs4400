#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Nicholas Elliott",   /* Full name */
  "u0682219@utah.edu"  /* Email address */
};

/******************************************************
 * PINWHEEL KERNEL
 *
 * Your different versions of the pinwheel kernel go here
 ******************************************************/
char naive2_pinwheel_descr[] = "naive2_pinwheel: MY BASELINE IMPLEMENTATION";
void naive2_pinwheel(pixel *src, pixel *dest)
{
  int qi, qj, i, j;

  /* qi & qj are column and row of quadrant
     i & j are column and row within quadrant */
  

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



/* 
 * naive_pinwheel - The naive baseline version of pinwheel 
 */
char naive_pinwheel_descr[] = "naive_pinwheel: baseline implementation";
void naive_pinwheel(pixel *src, pixel *dest)
{
  int qi, qj, i, j;

  /* qi & qj are column and row of quadrant
     i & j are column and row within quadrant */

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

/* 
 * pinwheel - Your current working version of pinwheel
 * IMPORTANT: This is the version you will be graded on
 */
char pinwheel_descr[] = "pinwheel: Current working version";
void pinwheel(pixel *src, pixel *dest)
{
  naive_pinwheel(src, dest);
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
char motion_descr[] = "motion: Current working version";
void motion(pixel *src, pixel *dst) 
{
 
  int i, j, index;

  index = 0;

  for (i = 0; i < src->dim; i++)
    {
        for (j = 0; j < src->dim; j++)
        {
          if(j + 2 < src->dim  && i + 2 < src->dim)
            dst[index] = weighted_combo4(src->dim, i, j, src);
          else
            dst[index] = weighted_combo2(src->dim, i, j, src, dst);
          
            //static pixel weighted_combo2(int dim, int i, int j, pixel *src, pixel *dest) 

          index++; 
        }
    }
}

/* 
 * weighted_combo - Used specifically for averages of 9x9 only 
 */
static pixel weighted_combo4(int dim, int i, int j, pixel *src) 
{
  
  //printf("Inner i: %d,  j: %d\n", i, j);

  pixel current_pixel;

  int base1, base2, base3;
  base1 = base2 = base3 = 0;


  base1 = RIDX(i,j,dim);
  base2 = RIDX(i+1,j,dim);
  base3 = RIDX(i+2,j,dim);


  current_pixel.red =  (src[base1].red + src[base1 + 1].red + src[base1 + 2].red +
                        src[base2].red + src[base2 + 1].red + src[base2 + 2].red +
                        src[base3].red + src[base3 + 1].red + src[base3 + 2].red)/9;


  current_pixel.green =  (src[base1].green + src[base1 + 1].green + src[base1 + 2].green +
                          src[base2].green + src[base2 + 1].green + src[base2 + 2].green +
                          src[base3].green + src[base3 + 1].green + src[base3 + 2].green)/9;


  current_pixel.blue =  (src[base1].blue + src[base1 + 1].blue + src[base1 + 2].blue +
                        src[base2].blue + src[base2 + 1].blue + src[base2 + 2].blue +
                        src[base3].blue + src[base3 + 1].blue + src[base3 + 2].blue)/9;

  return current_pixel;
}

/*
This helper calulates the variable average of possible dimensions

*/
static pixel weighted_combo2(int dim, int i, int j, pixel *src, pixel *dest) 
{
  
  //printf("Inner i: %d,  j: %d\n", i, j);
  pixel current_pixel;
  
  int red, red1, green, green1, blue, blue1, index;
  red = green = blue = red1 = green1 = blue1 = index =  0;

  int num_neighbors = 0;
  // i ///
    num_neighbors++;
    index = RIDX(i,j,dim);
    red += (int) src[index].red;
    green += (int) src[index].green;
    blue += (int) src[index].blue;
    
    if(j + 1 < dim)
      {
        num_neighbors++;
        index++;
        red += (int) src[index].red;
        green += (int) src[index].green;
        blue += (int) src[index].blue;

        if(j + 2 < dim)
    {
            num_neighbors++;
      index++;
      red += (int) src[index].red;
      green += (int) src[index].green;
      blue += (int) src[index].blue;
    }
      }

 //i + 1 /////////
    if(i + 1 < dim)
      {
        num_neighbors++;
        index = RIDX(i+1,j,dim);
        red += (int) src[index].red;
        green += (int) src[index].green;
        blue += (int) src[index].blue;
    
        if(j + 1 < dim)
    {
      num_neighbors++;
      index++;
      red += (int) src[index].red;
      green += (int) src[index].green;
      blue += (int) src[index].blue;

      if(j + 2 < dim)
        {
          num_neighbors++;
          index++;
          red += (int) src[index].red;
          green += (int) src[index].green;
          blue += (int) src[index].blue;
        }
    }
        if(i + 2 < dim)
          {
        
      num_neighbors++;
      index = RIDX(i+2,j,dim);
      red += (int) src[index].red;
      green += (int) src[index].green;
      blue += (int) src[index].blue;
    
      if(j + 1 < dim)
        {
          num_neighbors++;
          index++;
          red += (int) src[index].red;
          green += (int) src[index].green;
          blue += (int) src[index].blue;

          if(j + 2 < dim)
      {
        num_neighbors++;
        index++;
        red += (int) src[index].red;
        green += (int) src[index].green;
        blue += (int) src[index].blue;
      }
        }
    }
      }

  current_pixel.red = (unsigned short) (red / num_neighbors);
  current_pixel.green = (unsigned short) (green / num_neighbors);
  current_pixel.blue = (unsigned short) (blue / num_neighbors);
  
  return current_pixel;
}


/*
  - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 
char motion_descr[] = "motion: Current working version";
void motion(pixel *src, pixel *dst) 
{
  naive_motion(src, dst);
}
*/


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
