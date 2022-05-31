#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <math.h>

// We define default values for the number of darts to be thrown per thread & the number of threads we have
#define     DEFAULT_DARTS_PER_THREAD 500000
#define     DEFAULT_NUM_THREADS           2

/* Using TLS for the random number generator buffer to avoid data races between threads
   */
__thread unsigned short buff[3];

/*
   Call the following macro in your thread function before any calls to RANDOM_DOUBLE()
   By seeding the random number generator based on the thread ID each thread gets a different
   seed, and the seeds are different in different program runs.
   */

#define RANDOM_INIT()  (buff[0]=buff[1]=buff[2]=(unsigned short)pthread_self())

/*
   RANDOM_DOUBLE() returns a random number uniformly distributed between $[0, 1)$
*/

#define RANDOM_DOUBLE() (erand48(buff))

/*
    The drand48() and erand48() functions return nonnegative double-precision floating-point values uniformly distributed over the interval [0.0, 1.0)
*/


///////////////////////////////////////////////////////////////////////

typedef struct
{
    int darts_per_thread;
    double count;   
} thread_info;

void* determine_dart(void *arg)
{   
    thread_info* tinfo = (thread_info*)arg;
    double x, y;
    int end = tinfo->darts_per_thread;
    RANDOM_INIT();

    for (int i = 0; i < end; i++) {
        x = RANDOM_DOUBLE();
        y = RANDOM_DOUBLE();
        if ((x * x) + (y * y) < 1)
        {
            tinfo->count += 1;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char ** argv)
{
    // here we have the number of threads and the number of darts we want to throw per thread initialized to the default values
    int     num_threads      = DEFAULT_NUM_THREADS;
    int     darts_per_thread = DEFAULT_DARTS_PER_THREAD;

    // Passing in values for the number of threads and the number of darts to throw, executable can still be used alone with default values
    if (argc > 2) {
        num_threads = atoi(argv[1]);
        darts_per_thread = atoi(argv[2]);
        assert( num_threads>0 && darts_per_thread>0 );
    }
    else {
        fprintf(stderr, "Usage: %s <num_threads> <darts_per_thread>\n", argv[0]);
        fprintf(stderr, "Currently using the default values\n");
    }

    // we already have the variable n calculated for us as the number of threads and the number of darts thrown per thread
    long long n = (long long)num_threads * darts_per_thread;

    // we still need to determine the p value (which be the sum of the number of darts landed summed across all the threads)

    printf("Throwing %lld darts using %d thread%s\n", n, num_threads, (num_threads>1 ? "s" : "") );

    pthread_t   tid[num_threads];     // to store thread IDs
    thread_info arg[num_threads];     // to pass thread args

    // start num_threads threads to add up disjoint chunks of the data array
    for(int i=0; i<num_threads; i++)
    {
        arg[i] = (thread_info){darts_per_thread, 0};

        int ret = pthread_create(&tid[i], NULL, determine_dart, (void *)&(arg[i]));
        if (ret) {
            fprintf(stderr, "ERROR creating thread #%d\n"
                         "return code %d: %s\n", i, ret, strerror(ret));
            exit(-1);
        }
    }

    // collect results
    double p = 0;
    for(int i=0; i<num_threads; i++)
    {
        int ret = pthread_join(tid[i], NULL);
        if (ret) {
            fprintf(stderr, "ERROR joining thread #%d\n"
                         "return code %d: %s\n", i, ret, strerror(ret));
            exit(-1);
        }

        p += arg[i].count;
    }

    double my_pi = (double)p / n * 4.0;
    printf("Pi approximation: %.16f Error: %.16f\n", my_pi, fabs(my_pi - M_PI));
    return 0;
}
