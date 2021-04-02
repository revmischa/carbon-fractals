/*
 *  EventStuff.cpp
 *  FractalCarbon
 *
 *  Created by Mischa S. on Thu Jan 02 2003.
 *  Copyright (c) 2003 Happy Productions. All rights reserved.
 *
 */
 
#include "Imagering.h"
#include "EventStuff.h"
#include "Complex.h"
#include "FractalNumbers.h"

#include <math.h>

int currentlySelectedFractal=kNoFractalSelected;

void EventLoop()
{
    short			part,hit;
    WindowPtr			myWindow;
    ControlHandle		control;
    short 			ControlPart;
    char			key;
    Point 			p;
    Rect 			FDrawRect, tempRect;
    EventRecord			evt;
	Point 		center={SCREENYMAX/2, SCREENXMAX/2};
	float 			distanceX, distanceY;

    while(!done)
    {
        if (WaitNextEvent(everyEvent, &evt, 30, NULL))
        {
            switch (evt.what)
            {
                case updateEvt:
                    BeginUpdate((WindowPtr) evt.message);
                    DrawIntoMain( offscreen );
                    SetPortWindowPort( (WindowPtr) evt.message );
                    EndUpdate((WindowPtr) evt.message);
                    DrawMenuBar();
                    break;

                case diskEvt:
                    break;
                case osEvt:
                    break;
                case kHighLevelEvent:
                    AEProcessAppleEvent(&evt);
                    break;

                case mouseDown:
                    part = FindWindow(evt.where, &myWindow);
                    switch (part)
                    {
                        case inMenuBar:  			// process a moused menu command
                            DoMenuCommand(MenuSelect(evt.where));
                            break;
                        case inSysWindow:			// let system handle it
                            break;
                        case inContent:
                            if (myWindow != FrontWindow())
                                SelectWindow(myWindow);

                            GetMouse(&p);

                            //GlobalToLocal( &p );
                            ControlPart = FindControl( p, gMainWindow, &control );
                            if( control == zoomIn )
                                if( TrackControl( control, p, nil ) ) ZoomIn();
                                if( control == zoomOut )
                                    if( TrackControl( control, p, nil ) ) ZoomOut();

                                    SetRect( &FDrawRect, 0, 0, SCREENXMAX, SCREENYMAX );
                            if( (currentlySelectedFractal != kDraw3d) && (currentlySelectedFractal != kDrawHopalong) && (p.h < FDrawRect.right) && (p.h > FDrawRect.left) && (p.v < FDrawRect.bottom) && (p.v > FDrawRect.top) )
                            {
                                //pan the mandlebrot, julia and biomorph
                                
                           	//distance formula = (x2-x1)^2 + (y2-y1)^2 = distance^2

			distanceX =pow(p.h-center.h, 2);
			distanceX=sqrt(distanceX);
			distanceY =pow(p.v-center.v, 2);
			distanceY=sqrt(distanceY);
                                
                                if( p.h<center.h )
                                	XMAX += distanceX/(SCREENXMAX/2);
                                else
                                	XMAX -= distanceX/(SCREENXMAX/2);

                                 if( p.v<center.v )
                                	YMAX += distanceY/(SCREENYMAX/2);
                                else
                                	YMAX -= distanceY/(SCREENYMAX/2);
                                //XMAX*=2;
                                //YMAX*=2;

                                const int limit=3;
                                if( XMAX>limit ) XMAX=limit;
                                if( YMAX>limit ) YMAX=limit;
                                DrawWindo( offscreen );
                            }
                                break;
                        case inDrag:
                            GetRegionBounds(GetGrayRgn(), &tempRect);
                            DragWindow(myWindow, evt.where, &tempRect);
                            break;
                        case inGrow:
                            break;
                        case inGoAway:
                            //done=true;
                            if(myWindow!=gMainWindow) //openGL window
                            	DisposeWindow(myWindow);
                            else
                            	done=true;
                            break;
                        case inZoomIn:
                        case inZoomOut:
                            hit = TrackBox(myWindow, evt.where, part);
                            if (hit)
                            {
                                SetPort(GetWindowPort(myWindow));   // window must be current port
                                EraseRect(GetWindowPortBounds(myWindow, &tempRect));   // inval/erase because of ZoomWindow bug
                                ZoomWindow(myWindow, part, true);
                                InvalWindowRect(myWindow, GetWindowPortBounds(myWindow, &tempRect));
                            }
                                break;
                    }
                        break;

                case keyDown:
                    key = evt.message & charCodeMask;
                    if( key == kLeftArrowCharCode )
                    {
                        //Pan Left:
                        XMAX += PanDistance;// / ZOOM;
                        DrawWindo( offscreen );
                    }
                        if( key == kRightArrowCharCode )
                        {
                            //Pan Right:
                            XMAX -= PanDistance;// / ZOOM;
                            DrawWindo( offscreen );
                        }
                        if( key == kUpArrowCharCode )
                        {
                            //Pan Up:
                            YMAX += PanDistance;// / ZOOM;
                            DrawWindo( offscreen );
                        }
                        if( key == kDownArrowCharCode )
                        {
                            //Pan Down:
                            YMAX -= PanDistance;// / ZOOM;
                            DrawWindo( offscreen );
                        }

                        if (evt.modifiers & cmdKey)
                            if (evt.what == keyDown)
                                DoMenuCommand(MenuKey(key));
                        break;
            }
        }
    }
}

pascal OSErr QuitAppleEventHandler( const AppleEvent *appleEvt, AppleEvent* reply, UInt32 refcon )
{
    ExitToShell();
    return false;
}

void DoMenuCommand(long menuResult)
{
    short		menuID;				// resource ID of selected menu
    short		menuItem;			// item number of selected menu

    menuID = HiWord(menuResult);
    menuItem = LoWord(menuResult);

    switch (menuID)
    {
        case mApple:
            switch (menuItem)
            {
                case iAbout:
                    NoteAlert(kAboutBoxID, nil);
                    break;
                default:
                    break;
            }
            break;
        case mFile:
            switch (menuItem)
            {
                case iSavePicture:
                    SaveAsPicture();
                    break;
                case iQuit:
                    done=true;
                    break;
            }
            break;
        case mEdit:
            break;
        case mFractal:
            switch (menuItem)
            {
                case iRedraw: //redraw
                    DrawWindo( offscreen );
                    break;

                case iRandomize: //recalc
                    RandomizeFractal();
                    DrawWindo( offscreen );
                    break;

                case iAnimateMovie: //Create movie
                    StartMakingMovie();
                    break;
                    
                case iHopalong:
                	currentlySelectedFractal=kDrawHopalong;
                	CheckMenuItem( GetMenuHandle(menuID), iHopalong, true );
                	CheckMenuItem( GetMenuHandle(menuID), iJulia, false );
                	CheckMenuItem( GetMenuHandle(menuID), iMandlebrot, false );
                	CheckMenuItem( GetMenuHandle(menuID), iBiomorph, false );
                	CheckMenuItem( GetMenuHandle(menuID), i3d, false );
                	DrawWindo(offscreen);
                	break;
                	
                case iJulia:
               	XMAX=1;
               	YMAX=1;
               	ZOOM=1;
                	currentlySelectedFractal=kDrawJulia;
                	CheckMenuItem( GetMenuHandle(menuID), iHopalong, false );
                	CheckMenuItem( GetMenuHandle(menuID), iJulia, true );
                	CheckMenuItem( GetMenuHandle(menuID), iMandlebrot, false );
                	CheckMenuItem( GetMenuHandle(menuID), iBiomorph, false );
                	CheckMenuItem( GetMenuHandle(menuID), i3d, false );
                	DrawWindo(offscreen);
                	break;
                	
                case iMandlebrot:
                	XMAX=1.5;
                	YMAX=1;
                	ZOOM=1;
                	currentlySelectedFractal=kDrawMandlebrot;
                	CheckMenuItem( GetMenuHandle(menuID), iHopalong, false );
                	CheckMenuItem( GetMenuHandle(menuID), iJulia, false );
                	CheckMenuItem( GetMenuHandle(menuID), iMandlebrot, true );
                	CheckMenuItem( GetMenuHandle(menuID), iBiomorph, false );
                	CheckMenuItem( GetMenuHandle(menuID), i3d, false );
                	DrawWindo(offscreen);
                	break;
                	
                case iBiomorph:
                	XMAX=.5;
                	YMAX=1;
                	ZOOM=2;
                	currentlySelectedFractal=kDrawBiomorph;
                	CheckMenuItem( GetMenuHandle(menuID), iHopalong, false );
                	CheckMenuItem( GetMenuHandle(menuID), iJulia, false );
                	CheckMenuItem( GetMenuHandle(menuID), iMandlebrot, false );
                	CheckMenuItem( GetMenuHandle(menuID), iBiomorph, true );
                	CheckMenuItem( GetMenuHandle(menuID), i3d, false );
                	DrawWindo(offscreen);
                	break;
                	
               case i3d:
                	currentlySelectedFractal=kDraw3d;
                	CheckMenuItem( GetMenuHandle(menuID), iHopalong, false );
                	CheckMenuItem( GetMenuHandle(menuID), iJulia, false );
                	CheckMenuItem( GetMenuHandle(menuID), iMandlebrot, false );
                	CheckMenuItem( GetMenuHandle(menuID), iBiomorph, false );
                	CheckMenuItem( GetMenuHandle(menuID), i3d, true );
                	DrawWindo(offscreen);
                	break;
                	
            }
            break;
    }
    HiliteMenu(0);
}

Boolean CheckKey (unsigned char *PtrToKeyMap, short theKey)
{
    unsigned char	theByte, theBit;
    short			byteIndex;

    byteIndex = theKey >> 3;
    theByte = *(unsigned char *)(PtrToKeyMap + byteIndex);
    theBit = 1L<<(theKey & 7);

    return (theByte & theBit);
}
