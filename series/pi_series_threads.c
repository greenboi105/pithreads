#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include "unixtimer.h"

#define DEFAULT_N  1000000
#define DEFAULT_NTHREADS 2

///////////////////////////////////////////////////////////////////////

typedef struct
{
   long start;
   long end;
   double sum;
} thread_info;

void* sum(void* arg)
{
   thread_info* tinfo = (thread_info*)arg;
   long start = tinfo->start;
   long end   = tinfo->end;
   double mysum = 0.0;

   for (long i = start; i < end; i++)
   {
      
      if( i % 2 ==0)
         mysum += (1.0/(2*i+1));
      else
         mysum -= (1.0/(2*i+1));

   }

   tinfo->sum = mysum;
   pthread_exit(NULL);
}
/////////////////////////////////////////////////////////////////
//
// Compute the sum of first n terms of the Madhava-Leibniz series
// using num_threads threads
//
double sum_threads(long n, int num_threads)
{
   pthread_t   tid[num_threads];    
   thread_info arg[num_threads];     

   for(int i=0; i<num_threads; i++)
   {
      long start = i*n/num_threads;                                 
      long end   = (i==num_threads-1 ? n : (i+1)*n/num_threads ); 
      arg[i] = (thread_info){start, end, 0};

      int ret = pthread_create(&tid[i], NULL, sum, (void *)&(arg[i]));
      if (ret) {
         fprintf(stderr, "ERROR creating thread #%d\n"
                         "return code %d: %s\n", i, ret, strerror(ret));
         exit(-1);
      }
   }

   // collect results
   double pi_estimate = 0;
   for(int i=0; i<num_threads; i++)
   {
      int ret = pthread_join(tid[i], NULL);
      if (ret) {
         fprintf(stderr, "ERROR joining thread #%d\n"
                         "return code %d: %s\n", i, ret, strerror(ret));
         exit(-1);
      }

      pi_estimate += arg[i].sum;
   }

   return pi_estimate;

}

int main(int argc, char *argv[])
{
   long n           = (argc < 2 ? DEFAULT_N : atoi(argv[1]) );
   int  num_threads = (argc < 3 ? DEFAULT_NTHREADS : atoi(argv[2]) );
   double PI25 = 3.141592653589793238462643;

   // Compute and print the approximation of pi
   start_clock();
   start_timer();
   double my_pi = 4 * sum_threads(n, num_threads);
   printf("pi approximation: %.16f Error: %.16f\n", my_pi, fabs(my_pi - PI25));
   printf ("Clock time = %.2f CPU time =  %.2f\n", clock_seconds(), cpu_seconds() );

   return 0;
}
