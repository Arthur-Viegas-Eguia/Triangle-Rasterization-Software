#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "040pixel.h"


/** Does the calculations which result in
 * the interpolated color of the triangle at a specific point.
 * Draws that point with the calculated color modulated by the 
 * rgb value on the screen.
*/
void renderPixel(int i, int j, const double rgb[3], const double a[2], const double m[2][2], const double betaMinusAlpha[3], const double gammaMinusAlpha[3], const double alpha[3]){
    const double x[2] = {i, j};
    double xMinusA[2], pAndQ[2], scaledP[3], scaledQ[3], scaledSum[3], chi[3], modulatedColor[3];
    vecSubtract(2, x, a, xMinusA);
    mat221Multiply(m, xMinusA, pAndQ);
    vecScale(3, pAndQ[0], betaMinusAlpha, scaledP);
    vecScale(3, pAndQ[1], gammaMinusAlpha, scaledQ);
    vecAdd(3, scaledP, scaledQ, scaledSum);
    vecAdd(3, scaledSum, alpha, chi);
    vecModulate(3, chi, rgb, modulatedColor);
    pixSetRGB(i, j, modulatedColor[0], modulatedColor[1], modulatedColor[2]);
}




/** Does the basic calculations to interpolate the
 * color of a triangle. These calculations are used
 * for every single point in it.
 * Renders the triangle on the screen with the desired
 * colors in each vertex and the desired rgb modulation.
*/
void triRenderHelper(const double a[2], const double b[2], const double c[2], const double rgb[3], const double alpha[3], const double beta[3], const double gamma[3]){
        int i, j;

        /*Declares the basic variables to calculate
        the interpolated color of a triangle*/
        double column1[2], column2[2], mat[2][2], m[2][2], betaMinusAlpha[3], gammaMinusAlpha[3];

        /*Does the basic calculations that can be used in
        any given point to interpolate the color of a triangle*/
        vecSubtract(2,b,a,column1);
        vecSubtract(2, c, a, column2);
        mat22Columns(column1, column2, mat);
        mat22Invert(mat, m);
        vecSubtract(3, beta, alpha, betaMinusAlpha);
        vecSubtract(3, gamma, alpha, gammaMinusAlpha);


        /* Checks whether c has lower x coordinates than b,
           if that is the case the triangle points up, and
           the program will draw it from the staight line
           joining a to b, reaching until the line AC, and
           them from the line joining a to b reaching until
           line CB.
        */
        if(c[0] < b[0]){
            //Checks for a division by 0 and handles it if necessary
            if(!(a[0] == b[0] || a[0] == c[0])){
                //Draws the first half of the triangle
                for(i = ceil(a[0]); i <= floor(c[0]); i++){
                    for(j = ceil((a[1] +(((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])))); j <= floor((a[1]+(((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])))); j ++){
                        renderPixel(i, j, rgb, a, m, betaMinusAlpha, gammaMinusAlpha, alpha);
                    }
                }
            }
            //Draws the second half of the triangle
            for(i = floor(c[0]) + 1; i <= floor(b[0]); i++){
                for(j = ceil((a[1] +(((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])))); j <= floor((c[1] + (((b[1] - c[1])/(b[0] - c[0])) * (i - c[0])))); j ++){
                    renderPixel(i, j, rgb, a, m, betaMinusAlpha, gammaMinusAlpha, alpha);
                }
            }
        }
        /* The program enters this code block if b has lower
           coordinates than c, in which case the triangle points
           down. The program will draw the triangle from line
           segment AB until the straight line AC, and then from
           line segment BC until the straight line AB.
        */
        else{
            //Detects possible divisions by 0 and deals with them
            if(!(a[0] == b[0] || a[0] == c[0])){
                //Draws the first half of the triangle
                for(i = ceil(a[0]); i <= floor(b[0]); i++){
                    for(j = ceil((a[1] + (((b[1] - a[1])/(b[0] - a[0])) * (i - a[0])))); j <= floor((a[1] + (((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])))); j++){
                        renderPixel(i, j, rgb, a, m, betaMinusAlpha, gammaMinusAlpha, alpha);
                    }
                }
            }
            //Detects possible divisions by 0 and deals with them
            if(!(b[0] == c[0])){
                //Draws the second half of the triangle
                for(i = floor(b[0]) + 1; i <= floor(c[0]); i++){
                    for(j = ceil((b[1] + (((c[1] - b[1])/(c[0] - b[0])) * (i - b[0])))); j <= floor((a[1] + (((c[1] - a[1])/(c[0] - a[0])) * (i - a[0])))); j++){
                        renderPixel(i, j, rgb, a, m, betaMinusAlpha, gammaMinusAlpha, alpha);
                    }
                }
            }
        }
}



/** Receives the coordinates of the vertices of a triangle
 * in counterclockwise direction and its color, sets a to be the
 * leftmost point in the triangle. Calls helper functions
 * to render it on the screen.
*/
void triRender(const double a[2], const double b[2], const double c[2], const double rgb[3], const double alpha[3], const double beta[3], const double gamma[3]) {
    if(a[0] <= b[0] && a[0] <= c[0]){
        triRenderHelper(a, b, c, rgb, alpha, beta, gamma);
    }
    else if(b[0] <= c[0] && b[0] <= a[0])
        triRenderHelper(b, c, a, rgb, beta, gamma, alpha);
    else{
        triRenderHelper(c, a, b, rgb, gamma, alpha, beta);
    }
}







