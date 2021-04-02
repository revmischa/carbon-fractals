#include "Imagering.h"
#include "FractalNumbers.h"
#include "Complex.h"
#include "EventStuff.h"
#include "SetupAndKill.h"
#include "ColorFractal.h"

#include <math.h>

const RGBColor MishDarkGreen={0,32766,0};

void SetRadialGradientColor(double x, double y, RGBColor *coler)
{
	Point center={SCREENYMAX/2, SCREENXMAX/2};
	float distance, maxDistance;
	//distance formula = (x2-x1)^2 + (y2-y1)^2 = distance^2
	
	distance =pow(x-center.h, 2) + pow(y-center.v, 2);
	distance=sqrt(distance);
	
	// maximum distance is to the corner
	maxDistance =pow( center.h-SCREENYMAX, 2 ) + pow( center.v-SCREENXMAX, 2 );
	maxDistance=sqrt(maxDistance);
	maxDistance/=2;
	
	coler->blue=fabs(-1 + fabs(distance/maxDistance)) * 32766;
	coler->red=fabs( (distance/maxDistance ) )* 32766;
}	

void SetJuliaColor(double x, double y, RGBColor &coler)
{
	const int intensity=4500;
	int diverge=divergingJulia( x , y );
	coler.red=0;
	coler.green=0;
	coler.blue=0;
	/*switch((diverge % 6) +1)
	{
		case 1:
			coler.red=diverge * intensity;
			break;
		case 2:
			coler.green=diverge * intensity;
			break;
		case 3:
			coler.blue=diverge * intensity;
			break;
		case 4:
			coler.blue=diverge * intensity;
			coler.green=diverge * intensity;
			break;
		case 5:
			coler.red=diverge * intensity;
			coler.green=diverge * intensity;
			break;
		case 6:
			coler.blue=diverge * intensity;
			coler.red=diverge * intensity;
			break;
		case 7:
			coler.blue=diverge * intensity;
			coler.red=diverge * intensity;
			coler.green=diverge * intensity;
			break;
	}*/
	if(diverge>-1)
	{
		coler.red=0;
		coler.blue=diverge*intensity;
		coler.green=(diverge/20)*intensity;
		if(coler.green<0) coler.green=0;
	}
	if(diverge>20)
	{
		coler.red=0;
		coler.blue=diverge*intensity;
		coler.green=diverge*intensity;
	}
}

void SetMishColor(double &x, double &y, RGBColor &coler)
{
	const int intensity=4500;
	int diverge=divergingBiomorph( x, y );
	coler.green=diverge*intensity;
	coler.red=0;
	coler.blue=0;
}

void SetMandlebrotColor(double x, double y, RGBColor &coler)
{
	const int intensity=4500;
	int diverge=divergingMandlebrot( x, y );
	coler.red=diverge*intensity;
	coler.blue=diverge*intensity;
	coler.green=0;
}

void SetBiomorphColor(double x, double y, RGBColor &coler)
{
	const int intensity=10000;
	int diverge=divergingBiomorph( x, y );
	coler.red=diverge*intensity;
	coler.blue=diverge*intensity;
	coler.green=0;
}