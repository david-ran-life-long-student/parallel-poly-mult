/*/////////////////////////////////////////////////////////////////////////////
//
// File name : PolyMult_wrapper.c
// Author    : Sanjay Rajopdhye
// Date      : 2023/Feb/6
// Desc      : Finds product of two polynomials using schoolbook algorithm
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
#define MAX_COEFF 100.0
// Common Macros
#define mallocCheck(v,s,d) if ((v) == NULL) { printf("Failed to allocate memory for %s : size=%lu\n", "sizeof(d)*(s)", (s) * sizeof(d)); exit(-1); }
//  We never compare floating point nubers with exact equality because of
//  roundoff errors, rather, we check whether the absolute difference is less
//  than some epsilon.  You may nead to tweak this, because floating point operations are
//  not associtive, and Karatsuba, in particular, greatly changes the values
//  leading to some drastic amount of error.
#define EPSILON 1.0E-3

// Signature: PolyMult (A,B,C,deg)
void PolyMultGSQ(float*, float*, float*, long);

//  Function in the provided gold obj 
void PolyMultGold(float*, float*, float*, long);

//main
int main(int argc, char** argv) {
  //Initialization of rand
  srand((unsigned)time(NULL));

  //Check number of args

  if (argc != 2) {
    printf("Error: One argument required.\n");
    printf("Expecting degree, d\n");
    exit(0);
  }

  char *end = 0;
  char *val = 0;
  //Read Parameters
  //Initialization of d, N
  val = argv[1];
  long degree = atoi(val);


  ///Parameter checking
  if (!(degree >= 0)) {
    printf("The degree is not valid.\n");
    exit(-1);
  }


  long N = degree+1;  

  //Timing
  struct timeval time;
  double elapsed_time1, elapsed_time2, elapsed_time3;

  //Memory Allocation
  long n, i, j, k;
  float* A = (float*)malloc(sizeof(float)*N);
  mallocCheck(A, (N), float*);
  float* B = (float*)malloc(sizeof(float)*N);
  mallocCheck(B, (N), float*);
  float* C = (float*)malloc(sizeof(float)*(N*2-1));
  mallocCheck(C, (2*N-1), float*);
  float* D = (float*)malloc(sizeof(float)*(N*2-1));
  mallocCheck(D, (2*N-1), float*);
  
  //Input Initialization
#if defined RANDOM
  float x, y, tmp;
  for(n=0; n <= N-1; n+=1){
    A[n] = ((float) rand()/(float) (RAND_MAX))*MAX_COEFF;
    B[n] = ((float) rand()/(float) (RAND_MAX))*MAX_COEFF;
  }
#else
  // for easy checking, one polynomial counts up, one counts down
  // this way you should catch index offset (i+X) errors and index mismatch (i<->j) errors
  for(n=0; n <= degree; n+=1){
    A[n] = (float)n; 
    B[n] = (float)(-n);
  }
#endif

  for(i=0; i <= 2*degree; i+=1)
    C[i] = D[i] = 0.0;

    
  //Call the main computation

  //**************************************************************************//
  /*          CALL THE APPROPRIATE POLYNOMIAL PRODUCT COMPUTATION             */
  /*             AND COMPARE WITH THE PROVIDED GOLD  STANDARD                 */
  //**************************************************************************//

  gettimeofday(&time, NULL);
  elapsed_time1 = (((double) time.tv_sec) + ((double) time.tv_usec)/(1000*1000));

#if defined GSQ
  PolyMultGSQ(A, B, C, degree);
#endif
  
  gettimeofday(&time, NULL);
  elapsed_time2 = (((double) time.tv_sec) + ((double) time.tv_usec)/(1000*1000));
  elapsed_time1 = elapsed_time2 - elapsed_time1;

#if defined DATA
  // timing information
  
  printf("%lf\n", elapsed_time1);
#else
  // the optimzed seqential algorithm (provided .o file)
  PolyMultGold(A, B, D, degree);
  
  gettimeofday(&time, NULL);
  elapsed_time3 = (((double) time.tv_sec) + ((double) time.tv_usec)/(1000*1000));
  elapsed_time2 = elapsed_time3 - elapsed_time2;

  //**************************************************************************//
  /*               END OF THE POLYNOMIAL PRODUCT COMPUTATION                  */
  /*                                                                          */
  /*                    PRINT OUTPUTS (DEPENDING ON FLAGS)                    */
  //**************************************************************************//
    
#if defined CHECKING
  // Compare the values in C and D and count how many are different
  long error_count = 0;

  for(n=0; n <= 2*degree; n+=1)
    {
      if (fabs((C[n]-D[n])/D[n]) > EPSILON){
  	  error_count += 1;
  	  printf ("C[%ld] = %e, \tD[%ld] = %e\n", n, C[n], n, D[n]);
      }
    }
  printf("The total number of errors is %ld\n", error_count);
#endif
  // timing information
  
  printf("Execution time for Current: \t%lf sec.\n", elapsed_time1);
  printf("Execution time for Gold: \t%lf sec.\n", elapsed_time2);	

#endif

  //Memory Free
  free(A);
  free(B);
  free(C);
  free(D);
	
  return EXIT_SUCCESS;
}

//Common Macro undefs
#undef EPSILON
