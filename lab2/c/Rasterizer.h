//
//  Rasterizer.h
//
//  Created by Warren R. Carithers on 01/28/14.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2014 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include "Canvas.h"

///
// Simple structure that performs rasterization
///

typedef struct st_Rasterizer {

    ///
    // number of scanlines
    ///

    int n_scanlines;

    ///
    // Drawing canvas
    ///

    Canvas *C;

} Rasterizer;

///
// Constructor
//
// @param n number of scanlines
// @param C drawing canvas to use
///

Rasterizer *Rasterizer_create( int n, Canvas *C );

///
// Destructor
//
// @param R Rasterizer to destroy
///

void Rasterizer_destroy( Rasterizer *R );

///
// Set the canvas drawing color
//
// @param r The red component of the new color (between 0-1)
// @param g The green component of the new color (between 0-1)
// @param b The blue component of the new color (between 0-1)
///

void Rasterizer_setColor( float r, float g, float b, Rasterizer *R );

///
// Draw a filled polygon in the the Canvas C.
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
// @param R - The relevant rasterizer to draw with
///

void drawPolygon( int n, int x[], int y[], Rasterizer *R );

#endif
