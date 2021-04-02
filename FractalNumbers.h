/* main.h */
//#include <Carbon/Carbon.h>

#ifndef MAIN_H
#define MAIN_H

#include "Complex.h"

#define FPS		30	/* Frames per second for animations */
#define iterations	50	/* Number of iterations to do for fractal */
#define limitForDiverging 65535	/* What is considered "Infinity" */
#define PanDistance	.5	/* In the imaginary plane, distance to pan */

extern WindowPtr	gMainWindow;	//The main window, duh
extern GWorldPtr	offscreen;
extern PixMapHandle	offscreenPM;

extern ControlRef	zoomIn, zoomOut, pb, gEditTextReal, stopBtn;

extern Rect offworldRect;

extern int 	frameCount;

extern double 	XMAX;
extern double 	YMAX;

extern double	SCREENXMAX;
extern double	SCREENYMAX;

extern double	ZOOM, HOPALONG_ZOOM;
extern bool 	done;

extern complex complexConstant;
extern double const_A, const_B, const_C;

void DrawSelectedFractal();
void DrawHopalong(double x, double y, double a, double b, double c);
void DrawJulia();
void DrawMandlebrot();
void DrawBiomorph();
void  ZoomOut();
void  ZoomIn();
void RandomizeFractal();
double divergingMish(double x, double y); 
double divergingJulia(double x, double y);
double divergingBiomorph(double x, double y);
double divergingMandlebrot(double x, double y);

#endif