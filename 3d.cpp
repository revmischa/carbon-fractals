#include "Imagering.h"
#include "FractalNumbers.h"
#include "Complex.h"
#include "EventStuff.h"
#include "SetupAndKill.h"
#include "ColorFractal.h"
#include "3d.h"

/*
#include <OpenGL/OpenGL.h> 
#include <OpenGL/gl.h> 
#include <OpenGL/glu.h> 
*/
#include "gl.h"
#include "glu.h"
#include "glut.h"
#include "agl.h"
#include "glext.h"
#include <math.h>
#include <stdio.h>

void Draw3d()
{
	Rect rect;
	WindowRef win = NULL;
	GLint attrib [32], i = 0;
	AGLPixelFormat fmt;
	AGLContext ctx;
	unsigned long temp;
	Rect FDrawRect;
	GDHandle oldGDeviceH;
	GrafPtr savePort2;
	PixMapHandle pmh;
	
	pmh=GetPortPixMap(offscreen);
	
    GetGWorld (&savePort2, &oldGDeviceH);
	
	GetPortBounds( offscreen, &FDrawRect );
	rect=FDrawRect;
	OffsetRect(&rect, 50, 50);
	
	/* Create a window */
	/*CreateNewWindow (kDocumentWindowClass,
	kWindowStandardDocumentAttributes, &rect, &win);
	ShowWindow (win);
	SetPortWindowPort(win);*/
	
	//SetGWorld(offscreen, NULL);
	
	/* Choose pixel format */
	attrib [i++] = AGL_RGBA;
	attrib [i++] = AGL_DOUBLEBUFFER;
	attrib [i++] = AGL_NONE; // end parameter list
	
	fmt = aglChoosePixelFormat(NULL, 0, attrib);
	CheckAGLError();
	if(!fmt) 
		DieError("\paglChoosePixelFormat Error!");
	
	/* Create an AGL context */
	ctx = aglCreateContext(fmt, NULL);
	CheckAGLError();
	
	/* Attach the context to the window and set context current */
	//if( !aglSetDrawable (ctx, GetWindowPort(win)) ) DieError("\paglSetDrawable Error!");
	aglSetDrawable (ctx, offscreen);
	CheckAGLError();
	
	aglSetOffScreen(ctx, SCREENXMAX, SCREENYMAX, (**pmh).rowBytes, (*pmh)->baseAddr);
	CheckAGLError();
	
	aglSetCurrentContext (ctx);
	CheckAGLError();
	
	/* Clear buffer */
	glClearColor(1.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,0,1);
	glRecti(-1,-1,.5,.5);
	MoveTo(50,50);
	DrawString("\pHello World!");
	CheckAGLError();
	
	/* swap to front */
	aglSwapBuffers(ctx);
	CheckAGLError();
	
	//aglDestroyContext(ctx);
	//CopyBits( GetPortBitMapForCopyBits(GetWindowPort(win)), GetPortBitMapForCopyBits(offscreen) , &FDrawRect, &FDrawRect, srcCopy, NULL );
	SetPortWindowPort(gMainWindow);
	GetPortBounds(GetWindowPort(win), &rect);
	DrawIntoMain(offscreen);
	//CopyBits( GetPortBitMapForCopyBits(GetWindowPort(win)), GetPortBitMapForCopyBits(offscreen) , &rect, &FDrawRect, srcCopy, NULL );
	QDFlushPortBuffer( GetWindowPort(gMainWindow), NULL );
	SetGWorld (savePort2, oldGDeviceH);
}

void CheckAGLError()
{
	GLint gle;
	
	gle=aglGetError();
	if(gle) 
	{
		SetPortWindowPort(gMainWindow);
		DieError( "\pGLError: ", gle );
	}
}