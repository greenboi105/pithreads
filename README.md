# threadcomputations

In this pair of projects threading is used via POSIX threads to improve the performance of two different methods of estimating the value of pi.

Monte-Carlo Method:

One particularly simple method for approximating pi is based on Monte-Carlo sampling. 
The idea is to throw n darts uniformly at random at the [0,1) x [0,1) square.

Suppose that out of the n darts, p of them fall inside the circle of radius 1 centered at (0, 0),
i.e., the (x, y) coordinates of p of the darts satisfy the inequality x^2 + y^2 < 1. 
Pi can then be approximated as pi is approximately equal to 4 * p / n. The quality of the approximation depends on how large n is.

Project writes a multi-threaded program that approximates pi by having multiple threads throwing darts concurrently. 
Program takes two command-line arguments: the number of threads num_threads and the number of darts per thread darts_per_thread. 
Each thread proceeds with throwing darts uniformly at random and counting how many of the darts
fall within the circle. When all threads have completed, results are collated and an approximation is obtained.

Two macros, RANDOM INIT() and RANDOM DOUBLE() are used to independently pick random values x and y that are uniformly distributed 
between [0,1). 
RANDOM INIT() initializes a state buffer while RANDOM DOUBLE returns a pseudo-random double-precision floating point value.

Essentially, we want to have each individual thread throw darts and determine if they fall within the circle of radius 1. 
We later tabulate the results of each thread throwing darts and estimate the value of pi.

A dart is within the circle if it satisfies the inequality x^2+y^2 < 1.

Madhava-Leibniz Method:

The Madhava-Leibniz series is known to converge to pi/4.

Arbitrarily good approximations of pi can be obtained by computing the sum of the first n terms of the series for increasingly large values of n.
Unfortunately, convergence is quite slow: estimating pi with d-digit accuracy requires summing up roughly 10^d+1 terms. For this problem, threads are used
to speed up the computation. 

The program pi_series_threads.c takes n and the number of threads as command-line arguments, then calls the function
sum_threads() to compute the sum using threads. 

The program improves the performance of pi_series_serial by using threads to compute portions of the sum independently then combining their 
results, making it more efficient to compute up to extremely large values of n.
