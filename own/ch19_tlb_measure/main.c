#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

/* The idea is you're timing how long access to an array entry takes when
 * the pages you're accessing are in TLB or not.
 * I've disabled compiler optimization, and running it on one CPU
 * However... the results aren't as expected.
 * NUMPAGES = 1, avg = 5
 * NUMPAGES = 16, avg = 2
 * NUMPAGES = 64, avg = ~4
 * NUMPAGES = 1024, avg = ~4
 * NUMPAGES = 3072, avg = ~6
 * NUMPAGES = 4096, avg = 8
 * I'm not seeing this drop-off in performance as we move from L1 TLB to L2 TLB
 * to having to go to the Page Table...
*/

#define PAGESIZE 4096  // page size on the machine - >> getconf PAGESIZE
#define NUMPAGES 3072 // when this is more than what can be stored in TLB,
                    // performance should drop

int main(int argc, char* argv[])
{
    int arr_size = 100000000;
    int* a = malloc(sizeof(int) * arr_size);
    for (int i=0; i<arr_size; ++i)
        a[i] = 0;

    int jump = PAGESIZE / sizeof(int); // each jump moves to next page

    struct timespec start, end;
    uint64_t avg = 0;
    

    int loops = 1000;
    for (int i = 0; i<loops; ++i)
    {
        clock_gettime(CLOCK_REALTIME, &start);
        for (int j = 0; j<NUMPAGES * jump; j += jump)
        {
            a[j] += 1; 
        }
        clock_gettime(CLOCK_REALTIME, &end);
        avg += ((end.tv_nsec - start.tv_nsec) / NUMPAGES);
        // printf("%ld\n", (end.tv_nsec - start.tv_nsec) / NUMPAGES);
        // printf("%ld\n", avg);
    }
    avg /= loops;

    printf("%ld\n", avg);
    
    free(a);

    return 0;
}
