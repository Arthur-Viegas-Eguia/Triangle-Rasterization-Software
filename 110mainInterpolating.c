


/* On macOS, compile with...
    clang 100mainTesting.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 100mainTesting.c 040pixel.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <math.h>
#include "040pixel.h"

#include "080vector.c"
#include "100matrix.c"
#include "110triangle.c"
/**
 * Declaration of the vertex of the triangl and the alpa,
 * beta and gamma values.
 */
double a[2] = {144.0, -156.0};
double b[2] = {244.0, 244.0};
double c[2] = {-226.0, -226.0};
double rgb[3] = {1.0, 1.0, 0.0};
double angle = 0.0;
double alpha[3] = {1.0, 0.0, 0.0};
double beta[3] = {0.0, 1.0, 0.0};
double gamma[3] = {0.0, 0.0, 1.0};
/**
 * Animates the triangle, prints the framerate on the screen
 */
void handleTimeStep(double oldTime, double newTime) {
    if (floor(newTime) - floor(oldTime) >= 1.0)
        printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
    double transl[2] = {256.0, 256.0};
    double aa[2], bb[2], cc[2], rrggbb[3], rot[2][2];
    angle += (newTime - oldTime) / 10.0;
    mat22Rotation(angle, rot);
    mat221Multiply(rot, a, aa);
    mat221Multiply(rot, b, bb);
    mat221Multiply(rot, c, cc);
    vecAdd(2, transl, aa, aa);
    vecAdd(2, transl, bb, bb);
    vecAdd(2, transl, cc, cc);
    vecScale(3, (2.0 + sin(newTime)) / 3.0, rgb, rrggbb);
    pixClearRGB(0.0, 0.0, 0.0);
    triRender(aa, bb, cc, rrggbb, alpha, beta, gamma);
}
//Runs the program. Tests the triRender
int main(void) {
    if (pixInitialize(512, 512, "Testing") != 0)
        return 1;
    pixSetTimeStepHandler(handleTimeStep);
    //triRender(a, b, c, rgb, alpha, beta, gamma);

    pixRun();
    pixFinalize();
    return 0;
}


