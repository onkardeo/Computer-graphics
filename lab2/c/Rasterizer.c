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
// Simple class that performs rasterization algorithms
///

///
// Constructor
//
// @param n number of scanlines
// @param C The Canvas to use
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
    if( R->C )
        Canvas_destroy( R->C );
    if( R )
        free( R );
}

///
// Set the canvas drawing color
//
// @param r The red component of the new color (between 0-1)
// @param g The green component of the new color (between 0-1)
// @param b The blue component of the new color (between 0-1)
// @param R The Rasterizer to use
///

void Rasterizer_setColor( float r, float g, float b, Rasterizer *R ) {
    Canvas_setColor( r, g, b, R->C );
}

///
// Draw a filled polygon
//
// The polygon has n distinct vertices.  The coordinates of the vertices
// making up the polygon are stored in the x and y arrays.  The ith
// vertex will have coordinate (x[i],y[i]).
//
// You are to add the implementation here using only calls to setPixel()
//
// @param n - number of vertices
// @param x - x coordinates
// @param y - y coordinates
// @param R - The Rasterizer to be used
///

void drawPolygon( int n, int x[], int y[], Rasterizer *R )
{
    // YOUR IMPLEMENTATION GOES HERE

}
