/*
 *  EventStuff.h
 *  FractalCarbon
 *
 *  Created by Mischa S. on Thu Jan 02 2003.
 *  Copyright (c) 2003 Happy Productions. All rights reserved.
 *
 */

//#include <Carbon/Carbon.h>

#define	rMenuBar		128	/* menu bar */

#define	mApple		128	/* Apple menu */
#define	iAbout		1

#define	mFile		129	/* File menu */
#define	iSavePicture	1
#define	iQuit			2

#define	mEdit		130	/* Edit menu */

#define	mFractal		131	/* Fractal menu */
#define	iRedraw		1	/* Redraw Fractal */
#define	iRandomize	2	/* Redraw Fractal */
#define	iAnimateMovie	3	/* Redraw Fractal */
/// menu seperator -----
#define	iHopalong	5	/* Draw a hopalong Fractal */
#define	iJulia		6	/* Draw a Julia set Fractal */
#define	iMandlebrot	7	/* Draw a Mandlebrot Fractal */
#define	iBiomorph	8	/* Draw a Biomorph Fractal */
#define	i3d			9	/* Draw a 3d Fractal */

#define kAboutBoxID	129	/* Dialog resource for About box */
#define kMainWindowID	128	/* Main Window resource ID */
#define kZoomInID		128	/* Zoom in button resource ID */
#define kZoomOutID	129	/* Zoom out button resource ID */
#define kProgressBarID	130	/* Progress bar resource ID */
#define kTextEditID		131	/* Standard textEdit resource ID */
#define kStopBtnID		132	/* Stop button for animation */

enum
{
	kNoFractalSelected=0,
	kDrawHopalong,
	kDrawMandlebrot,
	kDrawJulia,
	kDrawBiomorph,
	kDraw3d
};

extern int currentlySelectedFractal;

void EventLoop();
pascal OSErr QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon );
void DoMenuCommand(long menuResult);