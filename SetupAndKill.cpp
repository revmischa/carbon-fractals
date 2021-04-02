/*
 *  SetupAndKill.cpp
 *  FractalCarbon
 *
 *  Created by Mischa S. on Fri Jan 03 2003.
 *  Copyright (c) 2003 Happy Productions. All rights reserved.
 *
 */
//#include <Carbon.h>
//#include <QuickTime.h>

#include "Imagering.h"
#include "FractalNumbers.h"
#include "Complex.h"
#include "EventStuff.h"
#include "SetupAndKill.h"
#include "ColorFractal.h"

#include <CoreFoundation.h>

//#include "Profiler.h"

GrafPtr pgpSave;

void Setup()
{
    OSErr 			err;
    unsigned long		theDateTime;

    InitCursor();
    FlushEvents( 0, everyEvent );
    
/*    	#if __profile__ && DO_PROFILE
		ProfilerInit(collectDetailed, bestTimeBase, 20, 5);
		ProfilerSetStatus(false);
	#endif
*/
    err = AEInstallEventHandler( kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP(QuitAppleEventHandler), 0, 0 );
    if (err != noErr)
        ExitToShell();

    if( !NavServicesCanRun() )
        DieError( "\pNavServicesCanRun() Error!" );
    err=NavLoad();
    if( err != noErr )
        DieError( "\pCould not load Nav Services!", err );

    GetPort (&pgpSave);

    gMainWindow=GetNewCWindow(kMainWindowID, nil, (WindowPtr)-1L);
    if(!gMainWindow)
        DieError("\pCould not create main window!");

    MakeMenu();

    SetRect( &offworldRect, 0, 0, (int)SCREENXMAX, (int)SCREENYMAX );

    err = NewGWorld(&offscreen,  0, &offworldRect, 0, 0, 0);
    if( err != noErr )
        DieError( "\pCould not create GWorld!", err );

    GetPortBounds(GetWindowPort(gMainWindow), &offworldRect);

    offscreenPM=GetGWorldPixMap(offscreen);

    SetPortWindowPort (gMainWindow);

    zoomIn=GetNewControl(kZoomInID, gMainWindow);
    if( !zoomIn )
        DieError( "\pCould not load control resources" );
    MoveControl( zoomIn, offworldRect.left+2, offworldRect.bottom-50 );

    zoomOut=GetNewControl(kZoomOutID, gMainWindow);
     if( !zoomOut )
        DieError( "\pCould not load control resources" );
    MoveControl( zoomOut, offworldRect.left+100, offworldRect.bottom-50 );

    pb=GetNewControl(kProgressBarID, gMainWindow);
    if( !zoomOut )
        DieError( "\pCould not load control resources" );
    MoveControl( pb, offworldRect.left, offworldRect.bottom-16 );
    SizeControl( pb, offworldRect.right, 16 );
    
   // stopBtn=GetNewControl(kStopBtnID, gMainWindow);
   // MoveControl( stopBtn, offworldRect.right-90, offworldRect.bottom-40 );
   // SetControlVisibility( stopBtn, false, false );

    Rect textRect;
    
    /*gEditTextReal=GetNewControl( kTextEditID, gMainWindow );
    if( !gEditTextReal )
        DieError( "\pCould not load control resources" );
        
        RGBColor MishRed={32766,0,0};
        
        ControlFontStyleRec theRec;
        theRec.flags=NULL;
        theRec.font=kControlFontSmallBoldSystemFont;
        theRec.size=12;
        //theRec.style=kNormalStyle;
        theRec.backColor=MishRed;
        theRec.foreColor=MishDarkGreen;
        
     err=SetControlFontStyle( gEditTextReal, &theRec );
    if( err!=noErr )
        DieError( "\pSetControlFontStyle() Error", err );
        
                SetUpControlTextColor( gEditTextReal, 16, true );
        SetUpControlBackground( gEditTextReal, 16, true );
        
    SetRect( &textRect, 10, SCREENYMAX+100, 95, SCREENYMAX+115 );
    SetControlBounds(gEditTextReal, &textRect );
    DrawControlInCurrentPort(gEditTextReal);
	
    //short baseLine=1;
    //GetBestControlRect(gEditTextReal, &textRect, &baseLine );
    //SetControlBounds(gEditTextReal, &textRect );
    
    SetControlTitle( gEditTextReal, "\pCreating Fractal..." );*/
    SetRect( &textRect, 10, SCREENYMAX+100, 95, SCREENYMAX+115 );
    //CreateStaticTextControl();
    
    DrawControls( gMainWindow );

    GetDateTime( &theDateTime );
    SetQDGlobalsRandomSeed( theDateTime );
    
    RandomizeFractal();
    DrawWindo( offscreen );
}

void Cleanup()
{
/*	#if __profile__ && DO_PROFILE
		ProfilerDump("\pFractalCarbon.profile");
		ProfilerTerm();
	#endif*/
	
    if(offscreen) DisposeGWorld(offscreen);
    if(zoomIn) DisposeControl(zoomIn);
    if(zoomOut) DisposeControl(zoomOut);
    if(gMainWindow) DisposeWindow (gMainWindow);
    NavUnload();
    ExitMovies();
    SetPort(pgpSave);
    ExitToShell();
}