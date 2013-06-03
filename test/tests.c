#include <stdio.h>
#include <stdlib.h>
#include "tinytest.h"
#include "tinytest_macros.h"

int bits_per_rand(int max_value);

static void
test_bytes_per_rand(void *data)
{
    (void)data;

    tt_int_op(bits_per_rand(  0x0000), ==,  0);
    tt_int_op(bits_per_rand(  0x0001), ==,  1);
    tt_int_op(bits_per_rand(  0x00fe), ==,  7);
    tt_int_op(bits_per_rand(  0x00ff), ==,  8);
    tt_int_op(bits_per_rand(  0x0100), ==,  8);
    tt_int_op(bits_per_rand(  0x01ff), ==,  9);
    tt_int_op(bits_per_rand(  0xfffe), ==, 15);
    tt_int_op(bits_per_rand(  0xffff), ==, 16);
    tt_int_op(bits_per_rand(0x010000), ==, 16);

 end:
    ;
}

static void
test_rand_max_size(void *data)
{
    int nbits, nbytes;

    (void)data;
    nbits = bits_per_rand(RAND_MAX);
    nbytes = nbits/8;
    printf("\nRAND_MAX=%d, bits=%d, bytes=%d", (int)RAND_MAX, nbits, nbytes);
    tt_int_op(nbytes, >, 0);

 end:
    ;
}

struct testcase_t fast_random_tests[] = {
    { "bytes_per_rand", test_bytes_per_rand, },
    { "rand_max_size", test_rand_max_size, },

    END_OF_TESTCASES
};

struct testgroup_t groups[] = {
    { "fast-random/", fast_random_tests },

    END_OF_GROUPS
};

int
main(int argc, const char **argv)
{
    return tinytest_main(argc, argv, groups);
}
