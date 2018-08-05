//
//  Canvas.c
//
//  Routines for adding points to create a new mesh.
//
//  Students are not to modify this file.
//

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
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
// @param C The Canvas to use
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
    if( C->colorArray ) {
        free( C->colorArray );
        C->colorArray = 0;
    }
    floatVectorClear( &(C->points) );
    floatVectorClear( &(C->colors) );
    C->numElements = 0;
    C->currentColor[0] = 0.0;
    C->currentColor[1] = 0.0;
    C->currentColor[2] = 0.0;
}


///
// change the current drawing color
//
// @param r The red component of the new color (between 0-1)
// @param g The green component of the new color (between 0-1)
// @param b The blue component of the new color (between 0-1);
// @param C The Canvas to use
///
void Canvas_setColor( float r, float g, float b, Canvas *C )
{
    C->currentColor[0] = r;
    C->currentColor[1] = g;
    C->currentColor[2] = b;
}


///
// set a pixel to the current drawing color
//
// @param x The x coord of the pixel to be set
// @param y The y coord of the pixel to be set
// @param C The Canvas to use
///
void Canvas_setPixel( int x0, int y0, Canvas *C )
{
    floatVectorPushBack( &(C->points), (float) x0 );
    floatVectorPushBack( &(C->points), (float) y0 );
    floatVectorPushBack( &(C->points), -1.0 );  // fixed Z depth
    floatVectorPushBack( &(C->points), 1.0 );

    floatVectorPushBack( &(C->colors), C->currentColor[0] );
    floatVectorPushBack( &(C->colors), C->currentColor[1] );
    floatVectorPushBack( &(C->colors), C->currentColor[2] );
    floatVectorPushBack( &(C->colors), 1.0 );   // alpha channel

    C->numElements += 1;
}


///
// gets the array of vertices for the current shape
//
// @param C The Canvas to use
///
float *Canvas_getVertices( Canvas *C )
{
    int i;

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
    for( i = 0; i < floatVectorSize(&(C->points)); i++ ) {
        C->pointArray[i] = C->points.vec[i];
    }

    return C->pointArray;
}


///
// gets the array of elements for the current shape
//
// @param C The Canvas to use
///
GLushort *Canvas_getElements( Canvas *C )
{
    int i;

    // delete the old point array if we have one
    if( C->elemArray ) {
        free( C->elemArray );
    }

    // create and fill a new point array
    C->elemArray = (GLushort *) malloc(
        floatVectorSize(&(C->points)) * sizeof(GLushort) );
    if( C->elemArray == 0 ) {
    	perror( "element allocation failure" );
	exit( 1 );
    }
    for( i = 0; i < floatVectorSize(&(C->points)); i++ ) {
        C->elemArray[i] = i;
    }

    return C->elemArray;
}


///
// gets the array of colors for the current shape
//
// @param C The Canvas to use
///
float *Canvas_getColors( Canvas *C )
{
    int i;

    // delete the old color array if we have one
    if( C->colorArray ) {
        free( C->colorArray );
    }

    // create and fill a new color array
    C->colorArray = (float *) malloc(
        floatVectorSize(&(C->colors)) * sizeof(float) );
    if( C->colorArray == 0 ) {
    	perror( "color allocation failure" );
	exit( 1 );
    }
    for( i = 0; i < floatVectorSize(&(C->colors)); i++ ) {
        C->colorArray[i] = C->colors.vec[i];
    }

    return C->colorArray;
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
