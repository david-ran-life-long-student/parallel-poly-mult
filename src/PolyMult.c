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
    // zero out output array
#pragma omp for
    for(i=0; i <= 2*d; i++)  // i really don't like this <= iteration checker, huge source of off by one error
      r[i] = 0;

#pragma omp for private(j) schedule(dynamic, 1000) nowait
    for(i=0; i <= d; i++) {
      for(j=0; j <= i; j++) {
        r[i] = r[i] + p[i-j] * q[j];
      }
    }

#pragma omp for private(j) schedule(dynamic, 1000)
    for(i=d+1; i <= 2*d; i++) {
      for(j=d; j >= i-d; j-=1) {
        r[i] = r[i] + p[i-j] * q[j];
      }
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
  // square iteration, store results diagonally
  long i;
  #pragma omp parallel
  {
    #pragma omp for
    for (i=0; i < 2*d + 1; i++) {
      r[i] = 0;
    }

    #pragma omp for reduction(+:r[: 2 * d + 1]) collapse(2)
    for (i = 0; i < d + 1; i++) {
      for (long j = 0; j < d + 1; j++) {
        r[i + j] += p[i] * q[j];
      }
    }
  }
}

void PolyMultBLQ(float *p, float *q, float *r, long d, long block_size, long tune2, long tune3){
  // square iteration, store results diagonally
  long block_i, block_j;

  // catch the divide by zero error
  if (block_size == 0) {
    block_size = 1024; // 1024 is a large-ish number but guaranteed to fit inside the l1 cache
  }

  #pragma omp parallel
  {
    // initialize return array
    #pragma omp for
    for (long i=0; i <= 2*d; i++) {
      r[i] = 0;
    }

    // block level iteration
    #pragma omp for reduction(+:r[: 2 * d + 1]) collapse(2)
    for (block_i = 0; block_i < (d + 1) / block_size; block_i++) {
      for (block_j = 0; block_j < (d + 1) / block_size; block_j++) {

        // within block iteration
        for (long inner_i = 0; inner_i < block_size; inner_i++) {
          for (long inner_j = 0; inner_j < block_size; inner_j++) {
            // this is r[i + j] += p[i] * q[j] just wrapped in block indexing
            r[block_i * block_size + inner_i + block_j * block_size + inner_j] += \
                p[block_i * block_size + inner_i] * q[block_j * block_size + inner_j];
          }
        }
      }
    }

  }
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

