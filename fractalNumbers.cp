//#include <Carbon/Carbon.h>
#include <QuicktimeComponents.h>
#include <math.h>

#include "Imagering.h"
#include "FractalNumbers.h"
#include "Complex.h"
#include "EventStuff.h"
#include "SetupAndKill.h"
#include "ColorFractal.h"
#include "3d.h"

//using namespace std;


Rect offworldRect;

int 	frameCount=1;

double 	XMAX=	1;
double 	YMAX=	1;

double	SCREENXMAX=	460;
double	SCREENYMAX=	SCREENXMAX*.75;

double	ZOOM=		2;
double 	HOPALONG_ZOOM=	20;
bool 	done=		false;

complex complexConstant( 0.695, .498 );
double const_A, const_B, const_C;

int main()
{
    Setup();

    EventLoop();

    Cleanup();

    return 1; //Cleanup exits the program, if it doesn't, uhhh, bad.
    
}

void ZoomIn()
{
    ZOOM *= .5;
    HOPALONG_ZOOM *= .5;

    DrawWindo( offscreen );
}

void  ZoomOut()
{
    ZOOM *= 2;
    HOPALONG_ZOOM *= 2;
    
    DrawWindo( offscreen );
}

// DrawSelectedFractal:
// Draws the fractal selected as defined by currentlySelectedFractal
// Precondition: Allredy is doing drawing in the currently desired GWorld
// Postcondition: Selected fractal has been drawn
void DrawSelectedFractal()
{
	switch( currentlySelectedFractal )
	{
		case kNoFractalSelected:
			RGBColor oldColor;
			Rect curPortBounds;
			GrafPtr thePort;
			    
			GetForeColor(&oldColor);
    		ForeColor(greenColor);
			
			ForeColor(redColor);
			
			GetPort( &thePort );
			GetPortBounds( thePort, &curPortBounds );
			
			EraseRect( &curPortBounds );
			Str255 selectText="\pSelect a fractal from the menu.";
			TextFont( GetAppFont() );
			SetAntiAliasedTextEnabled( true, 12 );
			TextSize( 24 );
			MoveTo( SCREENXMAX/2 - StringWidth(selectText)/2, SCREENYMAX/2);
			DrawString( selectText );
			RGBForeColor(&oldColor);
			break;
			
		case kDrawHopalong:
			DrawHopalong( SCREENXMAX, SCREENYMAX, const_A, const_B, const_C );
			break;
			
		case kDrawMandlebrot:
			DrawMandlebrot();
			break;
			
		case kDrawJulia:
			DrawJulia();
			break;
			
		case kDrawBiomorph:
			DrawBiomorph();
			break;
			
		case kDraw3d:
			Draw3d();
	}
}

void DrawHopalong(double x, double y, double a, double b, double c)
{
	RGBColor coler;
	GrafPtr oldPort;
	
	GetPort(&oldPort);
	
	double nextX, nextX2, nextY, nextY2;
	double frame=0;
	
	coler.red=0;
	coler.green=0;
	coler.blue=0;
	Rect FDrawRect;
	GetPortBounds( offscreen, &FDrawRect );
	
	while(!Button())
	{
		frame++;
	          
        nextX=y - sin(x) * pow( fabs(a * x - a), .5 );
		nextY=a*c-x;
		x=nextX;
		y=nextY;
		
		nextX2=nextX/HOPALONG_ZOOM+SCREENXMAX/2;
		nextY2=nextY/HOPALONG_ZOOM+SCREENYMAX/2;
		
		SetRadialGradientColor(nextX2,nextY2,&coler);
      	SetCPixel( (int) nextX2,(int) nextY2, &coler );
      	
         if( (unsigned long int)frame % 2000 == 1 )
         {
         	SetPortWindowPort(gMainWindow);
         	//DrawIntoMain(offscreen);
         	CopyBits( GetPortBitMapForCopyBits(offscreen), GetPortBitMapForCopyBits(GetWindowPort(gMainWindow)), &FDrawRect, &FDrawRect, srcCopy, NULL );
         	QDFlushPortBuffer( GetWindowPort(gMainWindow), NULL );
         	//SetGWorld(offscreen, NULL);
         	//SetPort(offscreen);
         	SetPort(oldPort);
         }
	}
}

void DrawJulia()
{
	RGBColor coler;
	
        for( int x=0; x<SCREENXMAX; x++ )
        {
            for( int y=0; y<SCREENYMAX; y++ )
            {
                SetJuliaColor( (x/SCREENXMAX*2-XMAX)*ZOOM , (y/SCREENYMAX*2-YMAX)*ZOOM, coler);
                SetCPixel(x,y, &coler);
            }
            
            SetControlValue( pb, x/SCREENXMAX * 100 );
            QDFlushPortBuffer( GetWindowPort(gMainWindow), NULL ); //update scrollbar changed
        }
}

void DrawMandlebrot()
{
	RGBColor coler;
	
        for( int x=0; x<SCREENXMAX; x++ )
        {
            for( int y=0; y<SCREENYMAX; y++ )
            {
                SetMandlebrotColor( (x/SCREENXMAX*2-XMAX)*ZOOM, (y/SCREENYMAX*2-YMAX)*ZOOM, coler);
                SetCPixel(x,y, &coler);
            }
            
            SetControlValue( pb, x/SCREENXMAX * 100 );
            QDFlushPortBuffer( GetWindowPort(gMainWindow), NULL ); //update scrollbar changed
        }
}

void DrawBiomorph()
{
	RGBColor coler;
	
        for( int x=0; x<SCREENXMAX; x++ )
        {
            for( int y=0; y<SCREENYMAX; y++ )
            {
                SetBiomorphColor( (x/SCREENXMAX*2-XMAX)*ZOOM, (y/SCREENYMAX*2-YMAX)*ZOOM, coler);
                SetCPixel(x,y, &coler);
            }
            
            SetControlValue( pb, x/SCREENXMAX * 100 );
            QDFlushPortBuffer( GetWindowPort(gMainWindow), NULL ); //update scrollbar changed
        }
}


void RandomizeFractal()
{
	complexConstant.setMe( (double)Random()/32767, (double)Random()/32767 );
	const_A=(double)Random()/32767;
	const_B=(double)Random()/32767;
	const_C=(double)Random()/32767;
}

double divergingMish(double x, double y)
{
    complex Z(x,y), ZSubZero(x,y), ZInverse(-x,-y);

    for( double a=1; a<iterations; a++ )
    {
    	if( ((int)a % 2) == 1 )
    	{
        		Z = Z*Z + complexConstant;
        		if( abs(Z) > limitForDiverging )
	        {
	            return fabs(a);
	        }
        	}
        	else
        	{
        		ZInverse = ZInverse*ZInverse - complexConstant;
        		if( abs(ZInverse) > limitForDiverging )
	        {
	            return fabs(a);
	        }
        	}
    }

    return false;
}


double divergingJulia(double x, double y)
{
    complex c(x,y);

    for( double a=1; a<iterations; a++ )
    {
        c = c*c + complexConstant;

        if( abs(c) > limitForDiverging )
        {
            return a;
        }
    }

    return false;
}

double divergingMandlebrot(double x, double y)
{
    complex c(x,y), cSubZero(x,y);

    for( double a=1; a<iterations; a++ )
    {
        c = c*c + cSubZero;

        if( abs(c) > limitForDiverging )
        {
            return a;
        }
    }

    return false;
}


double divergingBiomorph(double x, double y)
{
    complex c(x,y), tempComplex(0,0), tempComplex2(0,0);
    const float e=2.817;

    for( double a=1; a<iterations; a++ )
    {
        
        tempComplex.setMe( cos(c.getI()), sin(c.getI()) ); //cos(x) + i sin(x)
        tempComplex2=tempComplex * pow( (double)e, (double)c.getR() ); // e^ (a+bi) = e^a * e^bi
        
        //eulers law: e^ix = cos(x) + i sin(x)
        //biomorphs: f(z sub n)Ý=Ýsin(z sub n)Ý+Ýe^zÝ+Ýc
        
        c = sin(c) + tempComplex2 + complexConstant;

        if( abs(c) > limitForDiverging )
        {
            return a;
        }
    }

    return false;
}
