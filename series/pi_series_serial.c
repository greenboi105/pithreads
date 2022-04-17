#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "unixtimer.h"

#define DEFAULT_N  1000000

/////////////////////////////////////////////////////////////////
//
// Compute the sum of first n terms of the Madhava-Leibniz series
//
// in the serial case the task is extremely simple, just run the algorithm for the number specified
double sum_serial(long n)
{
   double sum = 0.0;

   for(long i=0; i<=n; i++) {
      if(i%2==0)
         sum += 1.0/(2*i+1);
      else
         sum -= 1.0/(2*i+1);
   }

   return sum;
}



int main(int argc, char *argv[])
{
    long n     = (argc < 2 ? DEFAULT_N : atoi(argv[1]) ); // pass in the number of times we want to approximate it to 
    double PI25 = 3.141592653589793238462643;
    double my_pi;


   // Compute and print the approximation of pi
   start_clock();
   start_timer();
   my_pi = 4 * sum_serial(n);
   printf("pi approximation: %.16f Error: %.16f\n", my_pi, fabs(my_pi - PI25));
   printf ("Clock time = %.2f CPU time =  %.2f\n", clock_seconds(), cpu_seconds() );

   return 0;
}
