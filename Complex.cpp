/*
 *  Complex.cpp
 *  FractalCarbon
 *
 *  Created by Mischa S. on Thu Jan 02 2003.
 *  Copyright (c) 2003 Happy Productions. All rights reserved.
 *
 */
#ifndef COMPLEX_C
#define COMPLEX_C
#include <math.h>

#include "Complex.h"
#include "FractalNumbers.h"

complex::complex(double R, double I)
{
    i=I;
    r=R;
}

void complex::setMe(double R, double I)
{
    this->i=I;
    this->r=R;
}


void complex::operator = (const complex &rhs)
{
    this->i=rhs.i;
    this->r=rhs.r;
}

void complex::subtractReal(double R)
{
    r = r - R;
}

void complex::subtractImaginary(double I)
{
    i = i - I;
}

complex complex::operator * (const complex &rhs)
{
    complex temp(this->r, this->i);

    // do FOIL multiplication, temp.i * rhs.i + temp.r * rhs.r is the real number

    if( temp.i < 0 && rhs.i < 0 ) // -i * -i = -1
    {
        temp.r = (temp.i) * (rhs.i) * (-1) + (temp.r) * (rhs.r);
    }

    else if( (temp.i > 0 && rhs.i < 0) || (temp.i < 0 && rhs.i > 0) ) // i * -i = 1   or     -i * i = 1
    {
        temp.r = (temp.i) * (rhs.i) * (-1) + (temp.r) * (rhs.r);
    }

    else if( temp.i > 0 && rhs.i > 0 ) // i * i = -1
    {
        temp.r = (temp.i) * (rhs.i) * (-1) + (temp.r) * (rhs.r);
    }

    else if( temp.i == 0 || rhs.i == 0 ) // 0 * 0 = 0
    {
        temp.r = (temp.r) * (rhs.r);
    }


    // real1 * i2 + real2 * i1 = i
    // need to use different complex than one used for r, because one above modifies values
    temp.i = this->r * rhs.i + rhs.r * this->i;

    return temp;
}

/*void complex::operator * (const double &rhs)
{
    this->i *= rhs;
    this->r *= rhs;
}*/

complex complex::operator * (const double &rhs)
{
    this->i *= rhs;
    this->r *= rhs;
    return *this;
}

complex complex::operator + (const complex &rhs)
{
    this->i += rhs.i;
    this->r += rhs.r;

    return *this;
}

complex complex::operator - (const complex &rhs)
{
    this->i -= rhs.i;
    this->r -= rhs.r;

    return *this;
}

complex complex::operator + (const double &rhs)
{
    this->i += rhs;
    this->r += rhs;

    return *this;
}

double complex::getR()
{
    return r;
}

double complex::getI()
{
    return i;
}


double abs(complex c)
{
    // a^2 + b^2 = c^2
    return sqrt(c.getR() * c.getR() + c.getI() * c.getI());
}

complex sin(complex c)
{
    complex temp( sin(c.getR()), sin(c.getI()) );
    return temp;
}

#endif