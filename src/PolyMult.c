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
/* This function takes two 1d array of size d + 1 and convolve them
 * then stores the result in r, which has 2d space allocated
 * algorithm illustration:
 * len(a) or len(b) = d + 1
 * len(r) = 2d + 1
 * time series  j across i down
 *                      sum
 * 0,0                  r0
 * 1,0  0,1             r1
 * 2,0  1,1  0,2        r2
 * ( j iteration is backwards after this point,
 * but logically it should be like below )
 *      2,1  1,2        r3
 *           2,2        r4
 *
 */
void PolyMultGSQ(float *p, float *q, float *r, long d){
  long i, j;  
#pragma omp parallel
  {
#pragma omp for private(j) schdeule(dynamic, 1000) nowait
    for(i=0; i <= d; i++) {
      r[i] = 0;
      for(j=0; j <= i; j++) {
        r[i] = r[i] + p[i-j] * q[j];
      }
    }

#pragma omp for private(j) schdeule(dynamic, 1000)
    for(i=d+1; i <= 2*d; i++) {
      r[i] = 0;
      for(j=d; j >= i-d; j--) {
        r[i] = r[i] + p[i-j] * q[j];
      }
    }
  }
}


