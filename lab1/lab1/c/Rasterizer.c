//
//  Rasterizer.c
//
//  Created by Warren R. Carithers on 01/28/14.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#include <stdlib.h>

#include "Rasterizer.h"
#include "Canvas.h"

///
//
// Simple class that performs rasterization algorithms
//
///

///
// Constructor
//
// @param n number of scanlines
//
///

Rasterizer *Rasterizer_create( int n, Canvas *C )
{
    Rasterizer *new = calloc( 1, sizeof(Rasterizer) );
    if( new != NULL ) {
        new->n_scanlines = n;
        new->C = C;
    }
    return( new );
}


///
// Destructor
//
// @param R Rasterizer to destroy
///

void Rasterizer_destroy( Rasterizer *R )
{
    if( R )
        free( R );
}


///
// Set the canvas drawing color
//
// @param r The red component of the new color (between 0-1)
// @param g The green component of the new color (between 0-1)
// @param b The blue component of the new color (between 0-1)
///

void Rasterizer_setColor( float r, float g, float b, Rasterizer *R ) {
    Canvas_setColor( r, g, b, R->C );
}


///
// Draw my initials
//
// Draw my own initials using calls to drawLine(), in the same
// manner that makeLines() in the driver program draws 'CG'.
//
// @param R the Rasterizer to be used
///

void myInitials( Rasterizer *R ) {

    // ######## Use blue color (0,0.5,1) to write your initials ######## 

    Canvas_setColor( 0.0, 0.5, 1.0, R->C );

    //
    // add code here to draw your initials
    // with calls to your drawLine() function
    // 


}

///
// Draw a line from (x0,y0) to (x1, y1)
//
// Implementation should be using the Midpoint Line Algorithm
//
// You are to add the implementation here using only calls to setPixel()
//
// @param x0 x coord of first endpoint
// @param y0 y coord of first endpoint
// @param x1 x coord of second endpoint
// @param y1 y coord of second endpoint
// @param R  The Rasterizer to be used
///

void drawLine (int x0, int y0, int x1, int y1, Rasterizer *R )
{
    //
    // add code here to implement drawLine()
    // using the midpoint line algorithm
    //


}
