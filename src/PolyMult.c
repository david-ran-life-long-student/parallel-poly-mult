/*/////////////////////////////////////////////////////////////////////////////
//
// File name : PolyMult.c
// Author    : Sanjay Rajopdhye
// Date      : 2023/Feb/6
// Desc      : Finds the product of two polynomials of degree d
//
/////////////////////////////////////////////////////////////////////////////*/

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <omp.h>

#define max(x, y)   ((x)>(y) ? (x) : (y))
#define min(x, y)   ((x)>(y) ? (y) : (x))

// TODO: Parallelize this function
/* This function takes two one d array of size d and convolve them
 * then stores the result in r, which has 2d space allocated
 *
 */
void PolyMultGSQ(float *p, float *q, float *r, long d){
  long i, j;  

  // zero out output array
  for(i=0; i <= 2*d; i++)
    r[i] = 0;

  for(i=0; i <= d; i++) {
    for(j=0; j <= i; j++) {
      r[i] = r[i] + p[i-j] * q[j];
    }
  }

  for(i=d+1; i <= 2*d; i++) {
    for(j=d; j >= i-d; j-=1) {
      r[i] = r[i] + p[i-j] * q[j];
    }
  }
}


