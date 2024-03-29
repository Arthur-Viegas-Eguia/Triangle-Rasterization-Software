


/* On macOS, compile with...
    clang 160mainAbstracted.c 040pixel.o -lglfw -framework OpenGL -framework Cocoa -framework IOKit
On Ubuntu, compile with...
    cc 160mainAbstracted.c 040pixel.o -lglfw -lGL -lm -ldl
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLFW/glfw3.h>

#include "040pixel.h"

#include "080vector.c"
#include "100matrix.c"
#include "150texture.c"
#include "170shading.c"

/**These constansts represent what information each index
 * of the arrays sent from trirender store.
 */
#define ATTRX 0
#define ATTRY 1
#define ATTRS 2
#define ATTRT 3
#define ATTRR 4
#define ATTRG 5
#define ATTRB 6
#define UNIFR 0
#define UNIFG 1
#define UNIFB 2
#define TEXR 0
#define TEXG 1
#define TEXB 2

/** This functions takes st coordinates sent from
 *tri-render to take 3 different textures from tex,
 * It modulates two of the textures together,
 * and also modulates them with the interpolated
 * color from each vertex. Then it adds the colors
 * from the third texture to it, subtracting 1 to each
 * pixel which has an rgb value greater than 1
 */
void shadeFragment(
        int unifDim, const double unif[], int texNum, const texTexture *tex[], 
        int attrDim, const double attr[], double rgb[3]) {
		double texColor[3], texUnif[3], twoTex[3], tex2RGB[3], addColor[3];
    	texSample(tex[0], attr[ATTRS], attr[ATTRT], texColor);
		vecModulate(3, &attr[ATTRR], texColor, texUnif);
		texSample(tex[1], attr[ATTRS], attr[ATTRT], tex2RGB);
		vecModulate(3, tex2RGB, texUnif, twoTex);
		texSample(tex[2], attr[ATTRS], attr[ATTRT], addColor);
		vecAdd(3, twoTex, addColor, rgb);
		if(rgb[0] > 1)
			rgb[0] = rgb[0] - 1;
		if(rgb[1] > 1)
			rgb[0] = rgb[1] / 2;
		if(rgb[2] > 1)
			rgb[2] = rgb[2] - 1;
}

/* We have to include triangle.c after defining shadeFragment, because triRender 
refers to shadeFragment. (Later in the course we handle this issue better.) */
#include "170triangle.c"

/* This struct is initialized in main() below. */
shaShading sha;
/* Here we make an array of one texTexture pointer, in such a way that the 
const qualifier can be enforced throughout the surrounding code. C is confusing 
for stuff like this. Don't worry about mastering C at this level. It doesn't 
come up much in our course. */
texTexture texture, texture2, texture3;
const texTexture *textures[3] = {&texture, &texture2, &texture3};
const texTexture **tex = textures;


/** Renders the triangle,  tests the filter
 *
 */
void render(void) {
	pixClearRGB(0.0, 0.0, 0.0);
	double a[7] = {400.0, 100.0, 1.0, 1.0, 1.0, 0.0, 0.0};
	double b[7] = {500.0, 500.0, 0.0, 1.0, 0.0, 1.0, 0.0};
	double c[7] = {30.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0};
	double unif[3] = {1.0, 1.0, 1.0};
	triRender(&sha, unif, tex, a, b, c);
}



/** Changes the type of filter from linear to
 * nearest neighbor or vice versa if enter is
 * pressed

 */
void handleKeyUp(int key, int shiftIsDown, int controlIsDown, 
		int altOptionIsDown, int superCommandIsDown) {
	if (key == GLFW_KEY_ENTER) {
		if (texture.filtering == texLINEAR)
			texSetFiltering(&texture, texNEAREST);
		else
			texSetFiltering(&texture, texLINEAR);
		render();
	}
}


//Prints the framerate of the program on the screen
void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0)
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
}


//Runs the program, initializes the
//files
int main(void) {
	if (pixInitialize(512, 512, "Abstracted") != 0)
		return 1;
	if (texInitializeFile(&texture, "test.jpg") != 0 || texInitializeFile(&texture2, "test2.jpg") != 0 || texInitializeFile(&texture3, "test3.png") != 0) {
	    pixFinalize();
		return 2;
	}
    texSetFiltering(&texture, texNEAREST);
    texSetLeftRight(&texture, texREPEAT);
    texSetTopBottom(&texture, texREPEAT);
    sha.unifDim = 3;
    sha.attrDim = 2 + 2 + 3;
    sha.texNum = 1;
    render();
    pixSetKeyUpHandler(handleKeyUp);
    pixSetTimeStepHandler(handleTimeStep);
    pixRun();
    texFinalize(&texture);
    pixFinalize();
    return 0;
}


