//
//  Canvas.h
//
//  Created by Warren R. Carithers 2016/09/23.
//  Based on a C++ version created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology. All rights reserved.
//
//  Prototypes for routines for manipulating a simple canvas
//  holding point information along with color data.
//
//  This file should not be modified by students.
//

#ifndef _CANVAS_H_
#define _CANVAS_H_

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "floatVector.h"

///
// Simple canvas structure that allows for pixel-by-pixel rendering.
///

typedef struct st_Canvas {

    ///
    // canvas size information
    ///
    int width;
    int height;

    ///
    // point-related data
    ///
    floatVector_t points;
    float *pointArray;
    int numElements;
    GLuint *elemArray;

    ///
    // color-related data
    ///
    floatVector_t colors;
    float *colorArray;
    GLfloat currentColor[3]; // RGB only - A is fixed

} Canvas;


///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///

Canvas *Canvas_create( int w, int h );

///
// Destructor
//
// @param C canvas to destroy
///

void Canvas_destroy( Canvas *C );

///
// clears the canvas
//
// @param C The Canvas to use
///

void Canvas_clear( Canvas *C );

///
// Sets the current color
//
// @param r The red component of the new color (between 0-1)
// @param g The green component of the new color (between 0-1)
// @param b The blue component of the new color (between 0-1);
// @param C The Canvas to use
///

void Canvas_setColor( float r, float g, float b, Canvas *C );

///
// writes a pixel using the current color
//
// @param x The x coord of the pixel to be set
// @param y The y coord of the pixel to be set
// @param C The Canvas to use
///

void Canvas_setPixel( int x, int y, Canvas *C );

///
// retrieve the array of element data from this Canvas
//
// @param C The Canvas to be used
///

GLuint *Canvas_getElements( Canvas *C );

///
// retrieve the array of vertex data from this Canvas
//
// @param C The Canvas to be used
///

float *Canvas_getVertices( Canvas *C );

///
// retrieve the array of color data from this Canvas
//
// @param C The Canvas to be used
///

float *Canvas_getColors( Canvas *C );

///
// retrieve the vertex count from this Canvas
//
// @param C The Canvas to be used
///

int Canvas_nVertices( Canvas *C );

#endif
