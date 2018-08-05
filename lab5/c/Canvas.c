//
//  Canvas.c
//
//  Created by Warren R. Carithers 2016/11/03.
//  Based on a C++ version created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Routines for adding points to create a new mesh.
//
//  This file should not be modified by students.
//

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Canvas.h"
#include "floatVector.h"

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///
Canvas *Canvas_create( int w, int h ) {
    Canvas *C = calloc( 1, sizeof(Canvas) );
    if( C != NULL ) {
        C->width = w;
        C->height = h;
        // all other fields contain zeroes
    }
    return( C );
}


///
// Destructor
//
// @param C canvas to destroy
///
void Canvas_destroy( Canvas *C ) {
    if( !C )
        return;
    free( C );
}


///
// clear the canvas
//
// @param C which canvas to clear
///
void Canvas_clear( Canvas *C )
{
    if( C->pointArray ) {
        free( C->pointArray );
        C->pointArray = 0;
    }
    if( C->elemArray ) {
        free( C->elemArray );
        C->elemArray = 0;
    }
    floatVectorClear( &(C->points) );
    C->numElements = 0;
}


///
// adds a triangle to the current shape
//
// @param C The Canvas to be used
// @param p0 first triangle vertex
// @param p1 second triangle vertex
// @param p2 final triangle vertex
///
void Canvas_addTriangle( Canvas *C, Vertex p0, Vertex p1, Vertex p2 )
{
    floatVectorPushBack( &(C->points), p0.x );
    floatVectorPushBack( &(C->points), p0.y );
    floatVectorPushBack( &(C->points), p0.z );
    floatVectorPushBack( &(C->points), 1.0f );

    floatVectorPushBack( &(C->points), p1.x );
    floatVectorPushBack( &(C->points), p1.y );
    floatVectorPushBack( &(C->points), p1.z );
    floatVectorPushBack( &(C->points), 1.0f );

    floatVectorPushBack( &(C->points), p2.x );
    floatVectorPushBack( &(C->points), p2.y );
    floatVectorPushBack( &(C->points), p2.z );
    floatVectorPushBack( &(C->points), 1.0f );

    C->numElements += 3;  // three vertices per triangle
}


///
// gets the array of vertices for the current shape
//
// @param C The Canvas to use
///
float *Canvas_getVertices( Canvas *C )
{
    // delete the old point array if we have one
    if( C->pointArray ) {
        free( C->pointArray );
    }

    // create and fill a new point array
    C->pointArray = (float *) malloc(
        floatVectorSize(&(C->points)) * sizeof(float) );
    if( C->pointArray == 0 ) {
    	perror( "point allocation failure" );
	exit( 1 );
    }
    for( int i = 0; i < floatVectorSize(&(C->points)); i++ ) {
        C->pointArray[i] = C->points.vec[i];
    }

    return C->pointArray;
}


///
// gets the array of elements for the current shape
//
// @param C The Canvas to use
///
GLuint *Canvas_getElements( Canvas *C )
{
    // delete the old point array if we have one
    if( C->elemArray ) {
        free( C->elemArray );
    }

    // create and fill a new element array
    C->elemArray = (GLuint *) malloc( C->numElements * sizeof(GLuint) );
    if( C->elemArray == 0 ) {
    	perror( "element allocation failure" );
	exit( 1 );
    }
    for( int i = 0; i < C->numElements; i++ ) {
        C->elemArray[i] = i;
    }

    return C->elemArray;
}


///
// returns number of vertices in current shape
//
// @param C The Canvas to use
///
int Canvas_nVertices( Canvas *C )
{
    return C->numElements;
}
