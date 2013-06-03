#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#define BUFSIZE 1024

#ifndef MIN
/* N.B. will double-evaluate arguments */
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

/**
 * Calculate number of bits which can be used from a single call to rand()
 * given the value of RAND_MAX. If x=bits_per_rand(RAND_MAX) then taking
 * the x least-significant bits from the result of rand() will result in a
 * value which ranges between 0 and (2^x)-1, though it will not be a
 * uniformly distributed pseudorandom value unless RAND_MAX == (2^x)-1 and
 * rand() returns a uniformly distributed pseudorandom number.
 */
int bits_per_rand(int max_value) {
    int i = 0;
    int all_ones = 1;
    unsigned umax_value = max_value;
#if defined(TEST) && defined(DEBUG)
    printf("\nStart: %d %d %u", i, all_ones, umax_value);
#endif
    /* Find the position of the most-significant non-zero bit, and whether
       all bits after this position are 1 too. We could skip the latter check
       by just adding 1 to RAND_MAX and finding the position of the first
       non-zero bit, but that would require use of a type larger than int
       which may not be available on all platforms */
    while (umax_value > 0) {
        if (!(umax_value & 0x01))
            all_ones = 0;
        i++;
        umax_value >>= 1;
#if defined(TEST) && defined(DEBUG)
        printf("\n       %d %d %u", i, all_ones, umax_value);
#endif
    }
    /* Now
       i is the position of the most significant non-zero bit indexed from 1, or
         0 if all bits are zero
       all_ones is true iff all bits after the most significant non-zero bit are
         1
       So the number of bits which are usable are i-1 unless all_ones is true,
       in which case we can use all i. */
    return all_ones?i:(i-1);
}

#ifndef TEST
int main(int argc, char *argv[]) {
    int step, count, nitems, i;
    int bytes_written;
    unsigned current;
    int bytes_remaining=0;
    uint8_t *buf;

    buf = (uint8_t *)malloc(BUFSIZE);
    step = bits_per_rand(RAND_MAX)/8;

    srand(time(NULL));
    count = atoi(argv[1]);

    while (count) {
        nitems = MIN(BUFSIZE, count);
        for (i=0; i<nitems; i++) {
            if (bytes_remaining < 1) {
                /* We have used up all available bytes from the last rand() call */
                current = (unsigned)rand();
                bytes_remaining = step;
            }
            buf[i] = current & 0xff;
            current = current >> 8;
            bytes_remaining--;
        }
        bytes_written = fwrite(buf, 1, nitems, stdout);
        if (bytes_written != nitems) {
            perror("Failed to write to file");
            return 1;
        }

        count -= bytes_written;
    }
    return 0;
}
#endif
