# Find-Primes
C++ multi-threaded program to find all prime numbers from 1 to 10^8

From command prompt-
To compile:
g++ -std=c++11 findprimes.cpp -o a.out

To run:
./a.out

Output to file named primes.txt.

Description:

To find all primes from 1 to 10^8 using 8 threads doing approximately the same amount of work, I decided to split the range into smaller sets and distribute them fairly evenly among the threads. Each thread will calculate a set of smaller numbers and a set of larger numbers, the last thread receiving the middle range. This will effectively even out the issue of larger numbers having a higher computation time. 10^8 was divided by 16, and each of 8 threads was assigned two equivalent ranges, from the bottom of the range to the top. For example, thread A calculates the ranges [1, 6250000] and (93750000, 100000000], thread B calculates the ranges [6250001, 12500001] and (81250000, 87500000], etc.

After researching many primality test algorithms, I decided that the most appropriate deterministic way to test a given number in this range for primeness is a reduced iterative approach - sieving out even numbers and only calculating up to the square root of a given number to avoid unnecessary repetition. The given number is tested for even divisibility by each odd number from 3 to its square root.

I implemented this concept by creating a ThreadCalc class to handle each thread and their respective ranges. When all threads are finished, the sums and counts they've accumulated are summed and their arrays of max primes are compared and merged into the top 10 largest primes.

I found this tutorial on threading in C++ to be invaluable! 
(https://baptiste-wicht.com/posts/2012/03/cp11-concurrency-tutorial-part-2-protect-shared-data.html)
