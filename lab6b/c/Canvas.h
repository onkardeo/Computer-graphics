//
// Canvas.h
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on a C++ version created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology. All rights reserved.
//
//  Prototypes for routines for manipulating a simple canvas
//  holding point information along with normal and/or texture data.
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
    floatVector_t normals;
    float *normalArray;
    floatVector_t uv;
    float *uvArray;
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
// adds a triangle to the current shape, along with (u,v) data
//
// @param C The Canvas to be used
// @param p0 first triangle vertex
// @param uv0 first triangle (u,v) data
// @param p1 second triangle vertex
// @param uv1 second triangle (u,v) data
// @param p2 final triangle vertex
// @param uv2 final triangle (u,v) data
///

void Canvas_addTriangleWithUV( Canvas *C,
    Vertex p0, UVcoord uv0, Vertex p1, UVcoord uv1, Vertex p2, UVcoord uv2 );

///
// adds a triangle to the current shape, along with normal data
//
// @param C The Canvas to be used
// @param p0 first triangle vertex
// @param n0 first triangle normal data
// @param p1 second triangle vertex
// @param n1 second triangle normal data
// @param p2 final triangle vertex
// @param n2 final triangle normal data
///

void Canvas_addTriangleWithNorms( Canvas *C,
    Vertex p0, Normal n0, Vertex p1, Normal n1, Vertex p2, Normal n2 );

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
// retrieve the array of normal data from this Canvas
//
// @param C The Canvas to be used
///

float *Canvas_getNormals( Canvas *C );

///
// retrieve the array of (u,v) data from this Canvas
//
// @param C The Canvas to be used
///

float *Canvas_getUV( Canvas *C );

///
// retrieve the vertex count from this Canvas
//
// @param C The Canvas to be used
///

int Canvas_nVertices( Canvas *C );

#endif
