/*
 *  Complex.h
 *  FractalCarbon
 *
 *  Created by Mischa S. on Thu Jan 02 2003.
 *  Copyright (c) 2003 Happy Productions. All rights reserved.
 *
 */
#ifndef COMPLEX_H
#define COMPLEX_H

//#include <Carbon/Carbon.h>

class complex
{
	public:
	complex(double, double);
	void setMe(double R, double I);
	
	complex operator * (const complex &rhs);
	complex operator * (const double &rhs);
        
	complex operator + (const complex &rhs);
	complex operator - (const complex &rhs);
         complex operator + (const double &rhs);
	void operator = (const complex &);
	void subtractReal(double R);
	void subtractImaginary(double I);
	
	// accessor functions:
	double getI();
	double getR();
	
	private:
	double i, r;
};

// Math functions overloaded for Complex Numbers
complex sin(complex c);
double abs(complex c);

#endif