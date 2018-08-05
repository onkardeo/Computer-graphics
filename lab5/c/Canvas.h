//
//  Canvas.h
//
//  Created by Warren R. Carithers 2016/11/03.
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

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif

#include "floatVector.h"
#include "Vertex.h"

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
// adds a triangle to the current shape
//
// @param C The Canvas to be used
// @param p0 first triangle vertex
// @param p1 second triangle vertex
// @param p2 final triangle vertex
///
void Canvas_addTriangle( Canvas *C, Vertex p0, Vertex p1, Vertex p2 );

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
// retrieve the vertex count from this Canvas
//
// @param C The Canvas to be used
///

int Canvas_nVertices( Canvas *C );

#endif
