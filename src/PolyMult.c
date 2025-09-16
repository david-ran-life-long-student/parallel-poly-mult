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

// same function as PA1. Add your parallelization here for better performance in leaf calls.
void PolyMultGSQ(float *p, float *q, float *r, long d){
  long i, j;

// initialize r to zero
  for(i=0; i <= 2*d; i+=1)
    r[i] = 0;
 
  // computation
  for(i=0; i <= d; i+=1){
    for(j=0; j <= i; j+=1) {
      r[i] = r[i] + p[i-j] * q[j];
    }
  }

  for(i=d+1; i <= 2*d; i+=1){
    for(j=d; j >= i-d; j-=1) {
      r[i] = r[i] + p[i-j] * q[j];
    }
  }
}

//  The other functions simply call the grade school quadratic function.  
//  Your job is to edit/change them

void PolyMultINQ(float *p, float *q, float *r, long d, long tune1, long tune2, long tune3){
  PolyMultGSQ(p, q, r, d);
  //  Same as Grade School, but with j as the outer loop (it's trickier than
  //  just swapping i and j loops).
}

void PolyMultOPQ(float *p, float *q, float *r, long d, long tune1, long tune2, long tune3){
  PolyMultGSQ(p, q, r, d);
  //  Same as Grade School, but iterating over a square -- at each point i, j,
  //  A[i]*q[j] is accumulated into the corret position in C).
}

void PolyMultBLQ(float *p, float *q, float *r, long d, long tune1, long tune2, long tune3){
  PolyMultGSQ(p, q, r, d);
  //  Same as OPQ, but tiled/blocked for better locality
  //  Use tune1 as block size
}

void PolyMultDCQ(float* p, float* q, float* r, long d, long tune1, long tune2, long tune3) {
  PolyMultGSQ(p, q, r, d);
  // Uses divide and conquer algorithm from lecture
  // Use tune2 as leaf size
}

// Ignore these for now

/* void PolyMultDCNew(float* p, float* q, float* r, long d, long tune1, long tune2, long tune3) { */
/*   PolyMultGSQ(p, q, r, d);  */
/*   // Uses divide and conquer algorithm from lecture */
/*   // Use tune2 as leaf size  */
/* }  */


/* void PolyMultDCK(float *p, float *q, float *r, long d, long stA, long stB, long tune1, long tune2, long tune3){ */
/*   PolyMultGSQ(p, q, r, d, 0, 0, 0); */
/* } */

