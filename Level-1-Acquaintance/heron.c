// Script to compute the approximation of the inverse of a positive number. With
// accuracy specified as tol, using Heron's algorithm (equivalent to Newton's
// method

#include <stdio.h>
#include <stdlib.h>

// 0x1 == 1 in hexadecimal, P-01 == times 2^(-1)
static double const eps1m01 = 1.0 - 0x1P-01; // 0x1P-01 == 0.5
static double const eps1p01 = 1.0 + 0x1P-01;
static double const eps1m24 = 1.0 - 0x1P-24;
static double const eps1p24 = 1.0 + 0x1P-24;

int main(int argc, char* argv[argc + 1]) {
    // First, make sure only one argument was supplied and process it
    if (argc != 2) {
        printf("Too many/too few arguments! You must supply only one argument\n");
        return EXIT_FAILURE;
    }
    
    double const a = strtod(argv[1], 0);
    if (a <= 0) {
        printf("The supplied argument has to be a positive number!\n");
        return EXIT_FAILURE;
    }
    
    double x = 1.0;
    // First, we bring the initial estimate closer to 1/a
    for (;;) {
        double prod = a*x;
        if (prod < eps1m01)
            x *= 2.0;
        else if (prod > eps1p01)
            x *= 0.5;
        else
            break;
    }

    for (;;) {
        double prod = a*x;
        if (prod < eps1m24 || prod > eps1p24)
            x *= (2.0 - prod);
        else
            break;
    }

   printf("Heron's algorithm: a=%.5e,\tx=%.5e,\ta*x=%.12f\n", a, x, a*x); 
    return EXIT_SUCCESS;
}
