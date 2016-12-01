#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include <GL/glew.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Joe Graphics

// NOTE: There are a lot of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch( ) statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.


// title of these windows:

const char *WINDOWTITLE = { "CS550-Yifan" };
const char *GLUITITLE   = { "User Interface Window" };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 800 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };



// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
	RESET,
	QUIT
};


struct rotateParameter
{
	float degree;
	float x;
	float y;
	float z;
};

// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// the color numbers:
// this order must match the radio button order

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};

char * ColorNames[ ] =
{
	"Red",
	"Yellow",
	"Green",
	"Cyan",
	"Blue",
	"Magenta",
	"White",
	"Black"
};


// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};

void turnRed() {
	glColor3f(1., 0., 0.);
}
void turnGreen() {
	glColor3f(0., 1., 0.);
}
void turnBlue() {
	glColor3f(0., 0., 1.);
}
void turnYellow() {
	glColor3f(1., 1., 0.);
}
void turnCyan() {
	glColor3f(0., 1., 1.);
}
void turnWhite() {
	glColor3f(1., 1., 1.);
}
void turnBlack() {
	glColor3f(0., 0., 0.);
}

void chooseColor(int choice){
	
	switch (choice)
	{
		//top
	case 1:
		turnWhite();
		break;
		//btm
	case 2:
		turnCyan();
		break;
		//left
	case 3:
		turnBlue();
		break;
		//right
	case 4:
		turnGreen();
		break;
		//front
	case 5:
		turnRed();
		break;
		//back
	case 6:
		turnYellow();
		break;
		//inner
	case 7:
		turnBlack();
		break;
	}
}



// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int     ShowInstOn;
GLuint BoxList1, BoxList2, BoxList3, BoxList4, BoxList5, BoxList6, BoxList7, BoxList8, BoxList9, BoxList10, 
BoxList11, BoxList12, BoxList13, BoxList14, BoxList15, BoxList16, BoxList17, BoxList18, BoxList19, BoxList20, BoxList21, BoxList22, BoxList23, BoxList24, BoxList25, BoxList26, BoxList27;				// object display list
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
float Time;						// used in animation
int cubeIndex[3][3][3];			// used to save the small cube info
rotateParameter rotateInfo[27][500];  // 27 means 27 cubics 200 means record 200 times rotation 
int clickTimes = 0; //the times of click
bool timeRestart = false;
float rotateTime = 0;
int rotateTimeAdd = 0;
int cal = 0;
bool contourClock = true;
// the size of small cubes
float dx = BOXSIZE / 2.f;
float dy = BOXSIZE / 2.f;
float dz = BOXSIZE / 2.f;
bool Frozen = false;
// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	Axes( float );
void	HsvRgb( float[3], float [3] );

void    rotateX0();
void    rotateX1();
void    rotateX2();
void    rotateY0();
void    rotateY1();
void    rotateY2();
void    rotateZ0();
void    rotateZ1();
void    rotateZ2();

struct vec3
{
	float x;
	float y;
	float z;
};


void initRotateInfo() {
	for (int i = 0; i < 27; i++) {
		for (int j = 0; j < 500; j++) {
			rotateInfo[i][j].degree = 0;
			rotateInfo[i][j].x = 0;
			rotateInfo[i][j].y = 0;
			rotateInfo[i][j].z = 0;
		}
	}
}

//save the information of rotated  slice to the information matrix 
void pushRotateInfo(int cube1, int cube2, int cube3, int cube4, int cube5, int cube6, int cube7, int cube8, int cube9, int xValue, int yValue, int zValue) {
	//printf("1 is %d, 2 is %d, 3 is %d, 4 is %d\n ", cube1, cube2, cube3, cube4);
	for (int i = 0; i < 27; i++) {
		if (i == (cube1 - 1) || i == (cube2 - 1) || i == (cube3 - 1) || i == (cube4 - 1)|| i == (cube5 - 1) || i == (cube6 - 1) || i == (cube7 - 1) || i == (cube8 - 1) || i == (cube9 - 1)) {
			if (contourClock == true)
			{ 
				rotateInfo[i][clickTimes].degree = 90; // contourclockwise rotation
			}else{
				rotateInfo[i][clickTimes].degree = -90; // clockwise rotation
			}
			
			rotateInfo[i][clickTimes].x = xValue;
			rotateInfo[i][clickTimes].y = yValue;
			rotateInfo[i][clickTimes].z = zValue;
		}
		else {
			rotateInfo[i][clickTimes].degree = 0;
			rotateInfo[i][clickTimes].x = xValue;
			rotateInfo[i][clickTimes].y = yValue;
			rotateInfo[i][clickTimes].z = zValue;
		}
	}
}

void rotateByArrayInfo(int cubeIndex, int Times){
	//Times++;
	int degree = 0;
	if (rotateTimeAdd <= 450) {
		degree = rotateTimeAdd / 5.0;
		//printf("degree is %d\n", degree);
	}
	else {
		degree = 90;
		//printf("90 degree is %d\n", degree);
	}

	/*glPushMatrix();
	glRotatef(degree, rotateInfo[cubeIndex-1][Times - 1].x, rotateInfo[cubeIndex-1][Times - 1].y, rotateInfo[cubeIndex-1][Times - 1].z);*/
	for (int i = Times - 1; i >=0; i--) {
		if (i == Times - 1) {
			glPushMatrix();
			if (rotateInfo[cubeIndex - 1][i].degree == 90 || rotateInfo[cubeIndex - 1][i].degree == -90) {
				if (contourClock == true) {
					glRotatef(degree, rotateInfo[cubeIndex - 1][Times - 1].x, rotateInfo[cubeIndex - 1][Times - 1].y, rotateInfo[cubeIndex - 1][Times - 1].z);
				}
				else {
					glRotatef(-1*degree, rotateInfo[cubeIndex - 1][Times - 1].x, rotateInfo[cubeIndex - 1][Times - 1].y, rotateInfo[cubeIndex - 1][Times - 1].z);
						}
			}
			else if(rotateInfo[cubeIndex - 1][i].degree==0){
				glRotatef(rotateInfo[cubeIndex - 1][i].degree, rotateInfo[cubeIndex - 1][i].x, rotateInfo[cubeIndex - 1][i].y, rotateInfo[cubeIndex - 1][i].z);
					}
			//printf("time is %d, degree is %f, rotate of %d cube, x is %f, y is %f, z is %f\n",Times,degree, cubeIndex, rotateInfo[cubeIndex - 1][Times - 1].x, rotateInfo[cubeIndex - 1][Times - 1].y, rotateInfo[cubeIndex - 1][Times - 1].z);
		}
		else {
			glPushMatrix();
			glRotatef(rotateInfo[cubeIndex - 1][i].degree, rotateInfo[cubeIndex - 1][i].x, rotateInfo[cubeIndex - 1][i].y, rotateInfo[cubeIndex - 1][i].z);
//			printf("time is %d, degree is %f, rotate of %d cube, x is %f, y is %f, z is %f\n", Times, rotateInfo[cubeIndex - 1][i].degree, cubeIndex, rotateInfo[cubeIndex - 1][Times - 1].x, rotateInfo[cubeIndex - 1][Times - 1].y, rotateInfo[cubeIndex - 1][Times - 1].z);
		}
	}
}
void pushPopMatrix(int Times) {
	for (int i = 0; i < Times; i++) {
		glPopMatrix();
	}

}
  
void clickX0() {
	printf("%d,%d,%d,%d,%d,%d,%d,%d,%d\n", cubeIndex[0][0][0], cubeIndex[0][0][1], cubeIndex[0][0][2], cubeIndex[0][1][0], cubeIndex[0][1][1], cubeIndex[0][1][2], cubeIndex[0][2][0], cubeIndex[0][2][1], cubeIndex[0][2][2]);
	pushRotateInfo(cubeIndex[0][0][0], cubeIndex[0][0][1], cubeIndex[0][0][2], cubeIndex[0][1][0], cubeIndex[0][1][1], cubeIndex[0][1][2], cubeIndex[0][2][0], cubeIndex[0][2][1], cubeIndex[0][2][2], 1, 0, 0);
	//rotateX0;
}
void clickX1() {
	pushRotateInfo(cubeIndex[1][0][0], cubeIndex[1][0][1], cubeIndex[1][0][2], cubeIndex[1][1][0], cubeIndex[1][1][1], cubeIndex[1][1][2], cubeIndex[1][2][0], cubeIndex[1][2][1], cubeIndex[1][2][2], 1, 0, 0);
	//rotateX1;
}
void clickX2() {
	pushRotateInfo(cubeIndex[2][0][0], cubeIndex[2][0][1], cubeIndex[2][0][2], cubeIndex[2][1][0], cubeIndex[2][1][1], cubeIndex[2][1][2], cubeIndex[2][2][0], cubeIndex[2][2][1], cubeIndex[2][2][2], 1, 0, 0);
	//rotateX2;
}
void clickY0() {
	pushRotateInfo(cubeIndex[0][0][0], cubeIndex[0][0][1], cubeIndex[0][0][2], cubeIndex[1][0][0], cubeIndex[1][0][1], cubeIndex[1][0][2], cubeIndex[2][0][0], cubeIndex[2][0][1], cubeIndex[2][0][2], 0,1,0);
	//rotateY0;
}
void clickY1() {
	pushRotateInfo(cubeIndex[0][1][0], cubeIndex[0][1][1], cubeIndex[0][1][2], cubeIndex[1][1][0], cubeIndex[1][1][1], cubeIndex[1][1][2], cubeIndex[2][1][0], cubeIndex[2][1][1], cubeIndex[2][1][2], 0, 1, 0);
	//rotateY1;
}
void clickY2() {
	pushRotateInfo(cubeIndex[0][2][0], cubeIndex[0][2][1], cubeIndex[0][2][2], cubeIndex[1][2][0], cubeIndex[1][2][1], cubeIndex[1][2][2], cubeIndex[2][2][0], cubeIndex[2][2][1], cubeIndex[2][2][2], 0, 1, 0);
	//rotateY1;
}
void clickZ0() {
	pushRotateInfo(cubeIndex[0][0][0], cubeIndex[0][1][0], cubeIndex[0][2][0], cubeIndex[1][0][0], cubeIndex[1][1][0], cubeIndex[1][2][0], cubeIndex[2][0][0], cubeIndex[2][1][0], cubeIndex[2][2][0], 0, 0, 1);
	//rotateZ0;
}
void clickZ1() {
	pushRotateInfo(cubeIndex[0][0][1], cubeIndex[0][1][1], cubeIndex[0][2][1], cubeIndex[1][0][1], cubeIndex[1][1][1], cubeIndex[1][2][1], cubeIndex[2][0][1], cubeIndex[2][1][1], cubeIndex[2][2][1],0,0,1);
	//rotateZ1;
}
void clickZ2() {
	pushRotateInfo(cubeIndex[0][0][2], cubeIndex[0][1][2], cubeIndex[0][2][2], cubeIndex[1][0][2], cubeIndex[1][1][2], cubeIndex[1][2][2], cubeIndex[2][0][2], cubeIndex[2][1][2], cubeIndex[2][2][2], 0, 0, 1);
	//rotateZ1;
}

void swapFourCubeCounterClock(int* a, int* b, int* c, int* d) {
	printf("previous %d, %d, %d, %d\n", *a, *b, *c, *d);
	int temp = *a;
	*a = *d;
	*d = *c;
	*c = *b;
	*b = temp;
	printf("after %d, %d, %d, %d\n", *a, *b, *c, *d);
}

void swapFourCubeClock(int* a, int* b, int* c, int* d) {
	int temp = *a;
	*a = *b;
	*b = *c;
	*c = *d;
	*d = temp;
}


void rotateX0() {
	swapFourCubeCounterClock(&cubeIndex[0][0][0], &cubeIndex[0][2][0], &cubeIndex[0][2][2], &cubeIndex[0][0][2]);
	swapFourCubeCounterClock(&cubeIndex[0][1][0], &cubeIndex[0][2][1], &cubeIndex[0][1][2], &cubeIndex[0][0][1]);
}

void clockWiseRotateX0() {
	swapFourCubeClock(&cubeIndex[0][0][0], &cubeIndex[0][2][0], &cubeIndex[0][2][2], &cubeIndex[0][0][2]);
	swapFourCubeClock(&cubeIndex[0][1][0], &cubeIndex[0][2][1], &cubeIndex[0][1][2], &cubeIndex[0][0][1]);
}

void rotateX1() {
	swapFourCubeCounterClock(&cubeIndex[1][0][0], &cubeIndex[1][2][0], &cubeIndex[1][2][2], &cubeIndex[1][0][2]);
	swapFourCubeCounterClock(&cubeIndex[1][1][0], &cubeIndex[1][2][1], &cubeIndex[1][1][2], &cubeIndex[1][0][1]);

}

void clockWiseRotateX1() {
	swapFourCubeClock(&cubeIndex[1][0][0], &cubeIndex[1][2][0], &cubeIndex[1][2][2], &cubeIndex[1][0][2]);
	swapFourCubeClock(&cubeIndex[1][1][0], &cubeIndex[1][2][1], &cubeIndex[1][1][2], &cubeIndex[1][0][1]);


}

void rotateX2() {
	swapFourCubeCounterClock(&cubeIndex[2][0][0], &cubeIndex[2][2][0], &cubeIndex[2][2][2], &cubeIndex[2][0][2]);
	swapFourCubeCounterClock(&cubeIndex[2][1][0], &cubeIndex[2][2][1], &cubeIndex[2][1][2], &cubeIndex[2][0][1]);
}

void clockWiseRotateX2() {
	swapFourCubeClock(&cubeIndex[2][0][0], &cubeIndex[2][2][0], &cubeIndex[2][2][2], &cubeIndex[2][0][2]);
	swapFourCubeClock(&cubeIndex[2][1][0], &cubeIndex[2][2][1], &cubeIndex[2][1][2], &cubeIndex[2][0][1]);

}


void rotateY0() {
	swapFourCubeCounterClock(&cubeIndex[2][0][2], &cubeIndex[2][0][0], &cubeIndex[0][0][0], &cubeIndex[0][0][2]);
	swapFourCubeCounterClock(&cubeIndex[2][0][1], &cubeIndex[1][0][0], &cubeIndex[0][0][1], &cubeIndex[1][0][2]);
}

void clockWiseRotateY0() {
	swapFourCubeClock(&cubeIndex[2][0][2], &cubeIndex[2][0][0], &cubeIndex[0][0][0], &cubeIndex[0][0][2]);
	swapFourCubeClock(&cubeIndex[2][0][1], &cubeIndex[1][0][0], &cubeIndex[0][0][1], &cubeIndex[1][0][2]);
}

void rotateY1() {
	swapFourCubeCounterClock(&cubeIndex[2][1][2], &cubeIndex[2][1][0], &cubeIndex[0][1][0], &cubeIndex[0][1][2]);
	swapFourCubeCounterClock(&cubeIndex[2][1][1], &cubeIndex[1][1][0], &cubeIndex[0][1][1], &cubeIndex[1][1][2]);

}

void clockWiseRotateY1() {
	swapFourCubeClock(&cubeIndex[2][1][2], &cubeIndex[2][1][0], &cubeIndex[0][1][0], &cubeIndex[0][1][2]);
	swapFourCubeClock(&cubeIndex[2][1][1], &cubeIndex[1][1][0], &cubeIndex[0][1][1], &cubeIndex[1][1][2]);

}

void rotateY2() {
	swapFourCubeCounterClock(&cubeIndex[2][2][2], &cubeIndex[2][2][0], &cubeIndex[0][2][0], &cubeIndex[0][2][2]);
	swapFourCubeCounterClock(&cubeIndex[2][2][1], &cubeIndex[1][2][0], &cubeIndex[0][2][1], &cubeIndex[1][2][2]);

}

void clockWiseRotateY2() {
	swapFourCubeClock(&cubeIndex[2][2][2], &cubeIndex[2][2][0], &cubeIndex[0][2][0], &cubeIndex[0][2][2]);
	swapFourCubeClock(&cubeIndex[2][2][1], &cubeIndex[1][2][0], &cubeIndex[0][2][1], &cubeIndex[1][2][2]);

}

void rotateZ0() {
	swapFourCubeCounterClock(&cubeIndex[2][0][0], &cubeIndex[2][2][0], &cubeIndex[0][2][0], &cubeIndex[0][0][0]);
	swapFourCubeCounterClock(&cubeIndex[2][1][0], &cubeIndex[1][2][0], &cubeIndex[0][1][0], &cubeIndex[1][0][0]);
}

void clockWiseRotateZ0() {
	swapFourCubeClock(&cubeIndex[2][0][0], &cubeIndex[2][2][0], &cubeIndex[0][2][0], &cubeIndex[0][0][0]);
	swapFourCubeClock(&cubeIndex[2][1][0], &cubeIndex[1][2][0], &cubeIndex[0][1][0], &cubeIndex[1][0][0]);
}

void rotateZ1() {

	swapFourCubeCounterClock(&cubeIndex[2][0][1], &cubeIndex[2][2][1], &cubeIndex[0][2][1], &cubeIndex[0][0][1]);
	swapFourCubeCounterClock(&cubeIndex[2][1][1], &cubeIndex[1][2][1], &cubeIndex[0][1][1], &cubeIndex[1][0][1]);
	
}

void clockWiseRotateZ1() {

	swapFourCubeClock(&cubeIndex[2][0][1], &cubeIndex[2][2][1], &cubeIndex[0][2][1], &cubeIndex[0][0][1]);
	swapFourCubeClock(&cubeIndex[2][1][1], &cubeIndex[1][2][1], &cubeIndex[0][1][1], &cubeIndex[1][0][1]);

}

void rotateZ2() {

	swapFourCubeCounterClock(&cubeIndex[2][0][2], &cubeIndex[2][2][2], &cubeIndex[0][2][2], &cubeIndex[0][0][2]);
	swapFourCubeCounterClock(&cubeIndex[2][1][2], &cubeIndex[1][2][2], &cubeIndex[0][1][2], &cubeIndex[1][0][2]);

}

void clockWiseRotateZ2() {

	swapFourCubeClock(&cubeIndex[2][0][2], &cubeIndex[2][2][2], &cubeIndex[0][2][2], &cubeIndex[0][0][2]);
	swapFourCubeClock(&cubeIndex[2][1][2], &cubeIndex[1][2][2], &cubeIndex[0][1][2], &cubeIndex[1][0][2]);
}

// set the initial states of cube, xyz fron 0; counter-clockwise
void initCubeIndex() {

	//y0 slice
	cubeIndex[0][0][0] = 1;
	cubeIndex[0][0][1] = 4;
	cubeIndex[0][0][2] = 7;
	
	cubeIndex[1][0][0] = 2;
	cubeIndex[1][0][1] = 5;
	cubeIndex[1][0][2] = 8;

	cubeIndex[2][0][0] = 3;
	cubeIndex[2][0][1] = 6;
	cubeIndex[2][0][2] = 9;

	//y1 slice
	cubeIndex[0][1][0] = 10;
	cubeIndex[0][1][1] = 13;
	cubeIndex[0][1][2] = 16;

	cubeIndex[1][1][0] = 11;
	cubeIndex[1][1][1] = 14;
	cubeIndex[1][1][2] = 17;

	cubeIndex[2][1][0] = 12;
	cubeIndex[2][1][1] = 15;
	cubeIndex[2][1][2] = 18;

	//y2 slice
	cubeIndex[0][2][0] = 19;
	cubeIndex[0][2][1] = 22;
	cubeIndex[0][2][2] = 25;

	cubeIndex[1][2][0] = 20;
	cubeIndex[1][2][1] = 23;
	cubeIndex[1][2][2] = 26;

	cubeIndex[2][2][0] = 21;
	cubeIndex[2][2][1] = 24;
	cubeIndex[2][2][2] = 27;

	
}




// main program:

int
main( int argc, char *argv[ ] )
{

	initCubeIndex();
	initRotateInfo();

	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit( &argc, argv );



	// setup all the graphics stuff:

	InitGraphics( );


	// create the display structures that will not change:

	InitLists( );


	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );


	// setup all the user interface stuff:

	InitMenus( );


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );


	// this is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

int start;
int end;

void
Animate( )
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:

	// force a call to Display( ) next time it is convenient:
	int ms = glutGet(GLUT_ELAPSED_TIME);	// milliseconds
	ms %= 1000;
	Time = (float)ms / (float)1000;        // [ 0., 1. )
	rotateTime++;
	rotateTimeAdd++;

	if (timeRestart == true) {
		while (rotateTime < 0.9) {
			rotateTime = Time*0.9;
		}
		timeRestart = false;
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if( DebugOn != 0 )
	{
		fprintf( stderr, "Display\n" );
	}


	// set which window we want to do the graphics into:

	glutSetWindow( MainWindow );


	// erase the background:

	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );


	// specify shading to be flat:

	glShadeModel( GL_FLAT );


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
	else
		gluPerspective( 90., 1.,	0.1, 1000. );


	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );


	// set the eye position, look-at position, and up-vector:

	gluLookAt( 0., 0., 8.,     0., 0., 0.,     0., 1., 0. );


	// rotate the scene:

glRotatef( (GLfloat)Yrot, 0., 1., 0. );
glRotatef( (GLfloat)Xrot, 1., 0., 0. );


	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );


	// set the fog parameters:

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}


	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glColor3fv( &Colors[WhichColor][0] );
		glCallList( AxesList );
	}


	// since we are using glScalef( ), be sure normals get unitized:

	glEnable( GL_NORMALIZE );

	//slice y0
	// draw the current object:
	rotateByArrayInfo(1, clickTimes);
	glPushMatrix();
	glTranslatef(-2.05, -2.05, -2.05);
	glCallList(BoxList1);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(2, clickTimes);
	glPushMatrix();
	glTranslatef(0, -2.05, -2.05);
	glCallList(BoxList2);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(3, clickTimes);
	glPushMatrix();
	glTranslatef(2.05, -2.05, -2.05);
	glCallList(BoxList3);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(4, clickTimes);
	glPushMatrix();
	glTranslatef(-2.05, -2.05, 0);
	glCallList(BoxList4);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(5, clickTimes);
	glPushMatrix();
	glTranslatef(0, -2.05, 0);
	glCallList(BoxList5);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(6, clickTimes);
	glPushMatrix();
	glTranslatef(2.05, -2.05, 0);
	glCallList(BoxList6);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(7, clickTimes);
	glPushMatrix();
	glTranslatef(-2.05, -2.05, 2.05);
	glCallList(BoxList7);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(8, clickTimes);
	glPushMatrix();
	glTranslatef(0, -2.05, 2.05);
	glCallList(BoxList8);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(9, clickTimes);
	glPushMatrix();
	glTranslatef(2.05, -2.05, 2.05);
	glCallList(BoxList9);
	glPopMatrix();
	pushPopMatrix(clickTimes);


	//slice y1
	rotateByArrayInfo(10, clickTimes);
	glPushMatrix();
	glTranslatef(-2.05, 0, -2.05);
	glCallList(BoxList10);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(11, clickTimes);
	glPushMatrix();
	glTranslatef(0, 0, -2.05);
	glCallList(BoxList11);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(12, clickTimes);
	glPushMatrix();
	glTranslatef(2.05,0, -2.05);
	glCallList(BoxList12);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(13, clickTimes);
	glPushMatrix();
	glTranslatef(-2.05, 0, 0);
	glCallList(BoxList13);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(14, clickTimes);
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glCallList(BoxList14);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(15, clickTimes);
	glPushMatrix();
	glTranslatef(2.05, 0, 0);
	glCallList(BoxList15);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(16, clickTimes);
	glPushMatrix();
	glTranslatef(-2.05, 0, 2.05);
	glCallList(BoxList16);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(17, clickTimes);
	glPushMatrix();
	glTranslatef(0, 0, 2.05);
	glCallList(BoxList17);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(18, clickTimes);
	glPushMatrix();
	glTranslatef(2.05, 0, 2.05);
	glCallList(BoxList18);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	//slice y2
	rotateByArrayInfo(19, clickTimes);
	glPushMatrix();
	glTranslatef(-2.05, 2.05, -2.05);
	glCallList(BoxList19);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(20, clickTimes);
	glPushMatrix();
	glTranslatef(0, 2.05, -2.05);
	glCallList(BoxList20);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(21, clickTimes);
	glPushMatrix();
	glTranslatef(2.05, 2.05, -2.05);
	glCallList(BoxList21);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(22, clickTimes);
	glPushMatrix();
	glTranslatef(-2.05, 2.05, 0);
	glCallList(BoxList22);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(23, clickTimes);
	glPushMatrix();
	glTranslatef(0, 2.05, 0);
	glCallList(BoxList23);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(24, clickTimes);
	glPushMatrix();
	glTranslatef(2.05, 2.05, 0);
	glCallList(BoxList24);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(25, clickTimes);
	glPushMatrix();
	glTranslatef(-2.05, 2.05, 2.05);
	glCallList(BoxList25);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(26, clickTimes);
	glPushMatrix();
	glTranslatef(0, 2.05, 2.05);
	glCallList(BoxList26);
	glPopMatrix();
	pushPopMatrix(clickTimes);

	rotateByArrayInfo(27, clickTimes);
	glPushMatrix();
	glTranslatef(2.05, 2.05, 2.05);
	glCallList(BoxList27);
	glPopMatrix();
	pushPopMatrix(clickTimes);




	// draw some gratuitous text that just rotates on top of the scene:

	glDisable( GL_DEPTH_TEST );
	glColor3f( 0., 1., 1. );
	DoRasterString( 0., 1., 0., "" );


	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( 0., 100.,     0., 100. );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	if (ShowInstOn == 1) {
		glColor3f(1., 1., 1.);
		DoRasterString(5., 5., 0., "Yifan's cube");
		DoRasterString(5., 95., 0., "useage instruction:");
		DoRasterString(5., 90., 0., "x0,x1,x2:a,s,d(counter clockwise) z,x,c(clockwise)");
		DoRasterString(5., 85., 0., "y0,y1,y2:f,g,h(counter clockwise) v,b,n(clockwise)");
		DoRasterString(5., 80., 0., "y0,y1,y2:j,k,l(counter clockwise) m,,,.(clockwise)");
		DoRasterString(5., 75., 0., "reset: i, freeze: e");
	}
	// swap the double-buffered framebuffers:

	glutSwapBuffers( );


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	WhichColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}

void
DoShowInstMenu(int id)
{
	ShowInstOn = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int instmenu = glutCreateMenu(DoShowInstMenu);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Colors",        colormenu);
	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu("Show Instruction", instmenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics( )
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	//glutTimerFunc( -1, NULL, 0 );
	glutIdleFunc( Animate );

	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void drawCube(int top, int btm, int left, int right, int front, int back) {
	glBegin(GL_QUADS);
	printf("%d,%d,%d,%d,%d,%d\n", top, btm, left, right, front, back);
	//surface 1 front (y points top, x points right, z points out of screen)
	chooseColor(front);
	glNormal3f(0., 0., 1.);
	glVertex3f(-dx, -dy, dz);
	glVertex3f(dx, -dy, dz);
	glVertex3f(dx, dy, dz);
	glVertex3f(-dx, dy, dz);
	//surface 2 back 
	chooseColor(back);
	glNormal3f(0., 0., -1.);
	glTexCoord2f(0., 0.);
	glVertex3f(-dx, -dy, -dz);
	glTexCoord2f(0., 1.);
	glVertex3f(-dx, dy, -dz);
	glTexCoord2f(1., 1.);
	glVertex3f(dx, dy, -dz);
	glTexCoord2f(1., 0.);
	glVertex3f(dx, -dy, -dz);
	//surface 3 right 
	chooseColor(right);
	glNormal3f(1., 0., 0.);
	glVertex3f(dx, -dy, dz);
	glVertex3f(dx, -dy, -dz);
	glVertex3f(dx, dy, -dz);
	glVertex3f(dx, dy, dz);
	//surface 4 left
	chooseColor(left);
	glNormal3f(-1., 0., 0.);
	glVertex3f(-dx, -dy, dz);
	glVertex3f(-dx, dy, dz);
	glVertex3f(-dx, dy, -dz);
	glVertex3f(-dx, -dy, -dz);
	//surface 5 top
	chooseColor(top);
	glNormal3f(0., 1., 0.);
	glVertex3f(-dx, dy, dz);
	glVertex3f(dx, dy, dz);
	glVertex3f(dx, dy, -dz);
	glVertex3f(-dx, dy, -dz);
	//surface 6 btm
	chooseColor(btm);
	glNormal3f(0., -1., 0.);
	glVertex3f(-dx, -dy, dz);
	glVertex3f(-dx, -dy, -dz);
	glVertex3f(dx, -dy, -dz);
	glVertex3f(dx, -dy, dz);

	glEnd();
}

void
InitLists( )
{
	//float dx = BOXSIZE / 2.f;
	//float dy = BOXSIZE / 2.f;
	//float dz = BOXSIZE / 2.f;
	glutSetWindow( MainWindow );

	// create the object:
	//glPushMatrix();
	//glTranslatef(2.0, 0, 0);
	BoxList1 = glGenLists( 1 );
	printf("1\n");
	glNewList( BoxList1, GL_COMPILE );
	drawCube(7,2,3,7,7,6 );
	glEndList( );


	BoxList2 = glGenLists(1);
	printf("2\n");
	glNewList(BoxList2, GL_COMPILE);
	drawCube(7,2,7,7,7,6);
	glEndList();

	BoxList3 = glGenLists(1);
	printf("3\n");
	glNewList(BoxList3, GL_COMPILE);
	drawCube(7,2,7,4,7,6 );
	glEndList();

	BoxList4 = glGenLists(1);
	glNewList(BoxList4, GL_COMPILE);
	drawCube(7,2,3,7,7,7);
	glEndList();

	BoxList5 = glGenLists(1);
	glNewList(BoxList5, GL_COMPILE);
	drawCube(7,2,7,7,7,7);
	glEndList();

	BoxList6 = glGenLists(1);
	glNewList(BoxList6, GL_COMPILE);
	drawCube(7,2,7,4,7,7 );
	glEndList();

	BoxList7 = glGenLists(1);
	glNewList(BoxList7, GL_COMPILE);
	drawCube(7,2,3,7,5,7 );
	glEndList();

	BoxList8 = glGenLists(1);
	printf("8\n");
	glNewList(BoxList8, GL_COMPILE);
	drawCube(7,2,7,7,5,7 );
	//drawCube(5,5,5,5,5,5);
	glEndList();

	BoxList9 = glGenLists(1);
	glNewList(BoxList9, GL_COMPILE);
	drawCube(7,2,7,4,5,7);
	glEndList();


	BoxList10 = glGenLists(1);
	glNewList(BoxList10, GL_COMPILE);
	drawCube(7,7,3,7,7,6);
	glEndList();

	BoxList11 = glGenLists(1);
	printf("11\n");
	glNewList(BoxList11, GL_COMPILE);
	drawCube(7,7,7,7,7,6 );
	glEndList();

	BoxList12 = glGenLists(1);
	glNewList(BoxList12, GL_COMPILE);
	drawCube(7,7,7,4,7,6 );
	glEndList();

	BoxList13 = glGenLists(1);
	glNewList(BoxList13, GL_COMPILE);
	drawCube(7,7,3,7,7,7);
	glEndList();

	BoxList14 = glGenLists(1);
	glNewList(BoxList14, GL_COMPILE);
	drawCube(7,7,7,7,7,7 );
	glEndList();

	BoxList15 = glGenLists(1);
	glNewList(BoxList15, GL_COMPILE);
	drawCube(7,7,7,4,7,7);
	glEndList();

	BoxList16 = glGenLists(1);
	glNewList(BoxList16, GL_COMPILE);
	drawCube(7,7,3,7,5,7);
	glEndList();

	BoxList17 = glGenLists(1);
	printf("17\n");
	glNewList(BoxList17, GL_COMPILE);
	drawCube(7,7,7,7,5,7 );
	glEndList();


	BoxList18 = glGenLists(1);
	glNewList(BoxList18, GL_COMPILE);
	drawCube(7,7,7,4,5,7);
	glEndList();

	BoxList19 = glGenLists(1);
	glNewList(BoxList19, GL_COMPILE);
	drawCube(1,7,3,7,7,6 );
	glEndList();

	BoxList20 = glGenLists(1);
	printf("20\n");
	glNewList(BoxList20, GL_COMPILE);
	drawCube(1,7,7,7,7,6 );
	glEndList();

	BoxList21 = glGenLists(1);
	glNewList(BoxList21, GL_COMPILE);
	drawCube(1,7,7,4,7,6 );
	glEndList();

	BoxList22 = glGenLists(1);
	glNewList(BoxList22, GL_COMPILE);
	drawCube(1,7,3,7,7,7);
	glEndList();

	BoxList23 = glGenLists(1);
	glNewList(BoxList23, GL_COMPILE);
	drawCube(1,7,7,7,7,7 );
	glEndList();

	BoxList24 = glGenLists(1);
	glNewList(BoxList24, GL_COMPILE);
	drawCube(1,7,7,4,7,7 );
	glEndList();

	BoxList25 = glGenLists(1);
	glNewList(BoxList25, GL_COMPILE);
	drawCube(1,7,3,7,5,7 );
	glEndList();

	BoxList26 = glGenLists(1);
	printf("26\n");
	glNewList(BoxList26, GL_COMPILE);
	drawCube(1,7,7,7,5,7 );
	glEndList();

	BoxList27 = glGenLists(1);
	glNewList(BoxList27, GL_COMPILE);
	drawCube(1,7,7,4,5,7 );
	glEndList();






	// create the axes:

	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes(4);
		glLineWidth( 1. );
	glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			WhichProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler


		case 'e':
			Frozen = !Frozen;
			if (Frozen)
				glutIdleFunc(NULL);
			else
				glutIdleFunc(Animate);
			break;


		//rotate x0 slice
		case 'a':
			contourClock = true;
			clickX0();
			rotateX0();
			clickTimes++;
			rotateTimeAdd = 0;
			break;
		//rotate x1 slice
		case 's':
			contourClock = true;
			clickX1();
			rotateX1();
			clickTimes++;
			rotateTimeAdd = 0;
			break;
			//rotate y0 slice
		case 'd':
			contourClock = true;
			clickX2();
			rotateX2();
			clickTimes++;
			rotateTimeAdd = 0;
			break;
			//rotate y1 slice
		case 'f':
			contourClock = true;
			clickY0();
			rotateY0();
			clickTimes++;
			rotateTimeAdd = 0;
			break;
			//rotate z0 slice
		case 'g':
			contourClock = true;
			clickY1();
			 rotateY1();
			 clickTimes++;
			 rotateTimeAdd = 0;
			break;
			//rotate z1 slice
		case 'h':
			contourClock = true;
			clickY2();
			rotateY2();
			clickTimes++;
			rotateTimeAdd = 0;
			break;

		case 'j':
			contourClock = true;
			clickZ0();
			rotateZ0();
			clickTimes++;
			rotateTimeAdd = 0;
			break;
			//rotate z0 slice
		case 'k':
			contourClock = true;
			clickZ1();
			rotateZ1();
			clickTimes++;
			rotateTimeAdd = 0;
			break;
			//rotate z1 slice
		case 'l':
			contourClock = true;
			clickZ2();
			rotateZ2();
			clickTimes++;
			rotateTimeAdd = 0;
			break;



			//rotate x0 slice clock wise
		case 'z':
			contourClock = false;
	/*		for (int i = 0; i < 3; i++) {
				clickX0();
				rotateX0();
				clickTimes++;
				rotateTimeAdd = 0;
			}*/
			clickX0();
			clockWiseRotateX0();
			clickTimes++;
			rotateTimeAdd = 0;
			break;
			//rotate x1 slice  clock wise
		case 'x':
			contourClock = false;
			clickX1();
			clockWiseRotateX1();
			clickTimes++;
			rotateTimeAdd = 0;
			break;

			//rotate x1 slice  clock wise
		case 'c':
			contourClock = false;
			clickX2();
			clockWiseRotateX2();
			clickTimes++;
			rotateTimeAdd = 0;
			break;


			//rotate y0 slice  clock wise
		case 'v':
			contourClock = false;
			clickY0();
			clockWiseRotateY0();
			clickTimes++;
			rotateTimeAdd = 0;
			break;
			//rotate y1 slice  clock wise
		case 'b':
			contourClock = false;
			clickY1();
			clockWiseRotateY1();
			clickTimes++;
			rotateTimeAdd = 0;
			break;

		case 'n':
			contourClock = false;
			clickY2();
			clockWiseRotateY2();
			clickTimes++;
			rotateTimeAdd = 0;
			break;

			//rotate z0 slice  clock wise
		case 'm':
			contourClock = false;
			clickZ0();
			clockWiseRotateZ0();
			clickTimes++;
			rotateTimeAdd = 0;
			break;
			//rotate z1 slice  clock wise
		case ',': 
			contourClock = false;
			clickZ1();
			clockWiseRotateZ1();
			clickTimes++;
			rotateTimeAdd = 0;
			break;
			//rotate z2 slice  clock wise
		case '.':
			contourClock = false;
			clickZ2();
			clockWiseRotateZ2();
			clickTimes++;
			rotateTimeAdd = 0;
			break;










		case 'i':
			initCubeIndex();
			initRotateInfo();
			clickTimes = 0;
			break;

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}



	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	ShowInstOn = 1;
	DepthCueOn = 0;
	Scale  = 1.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
		0.f, 1.f, 0.f, 1.f
	      };

static float xy[ ] = {
		-.5f, .5f, .5f, -.5f
	      };

static int xorder[ ] = {
		1, 2, -3, 4
		};

static float yx[ ] = {
		0.f, 0.f, -.5f, .5f
	      };

static float yy[ ] = {
		0.f, .6f, 1.f, 1.f
	      };

static int yorder[ ] = {
		1, 2, 3, -2, 4
		};

static float zx[ ] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };

static float zy[ ] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };

static int zorder[ ] = {
		1, 2, 3, 4, -5, 6
		};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r, g, b;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}
