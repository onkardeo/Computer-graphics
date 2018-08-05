//
// Canvas.c
//
//  Created by Warren R. Carithers 2016/11/22.
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
    Canvas *C = (Canvas *) calloc( 1, sizeof(Canvas) );
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
    if( C->normalArray ) {
        free( C->normalArray );
        C->normalArray = 0;
    }
    if( C->uvArray ) {
        free( C->uvArray );
        C->uvArray = 0;
    }
    if( C->elemArray ) {
        free( C->elemArray );
        C->elemArray = 0;
    }
    floatVectorClear( &(C->points) );
    floatVectorClear( &(C->normals) );
    floatVectorClear( &(C->uv) );
    C->numElements = 0;
}


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
    Vertex p0, UVcoord uv0, Vertex p1, UVcoord uv1, Vertex p2, UVcoord uv2 )
{
    // calculate the normal
    float ux = p1.x - p0.x;
    float uy = p1.y - p0.y;
    float uz = p1.z - p0.z;

    float vx = p2.x - p0.x;
    float vy = p2.y - p0.y;
    float vz = p2.z - p0.z;

    Normal nn = { (uy * vz) - (uz * vy),
                  (uz * vx) - (ux * vz),
                  (ux * vy) - (uy * vx) };

    // Attach the normal to all 3 vertices
    Canvas_addTriangleWithNorms( C, p0, nn, p1, nn, p2, nn );

    // Attach the texture coordinates
    floatVectorPushBack( &(C->uv), uv0.x );  // note use of (x,y) vs. (u,v)
    floatVectorPushBack( &(C->uv), uv0.y );  // see Vertex.h for details
    floatVectorPushBack( &(C->uv), uv1.x );
    floatVectorPushBack( &(C->uv), uv1.y );
    floatVectorPushBack( &(C->uv), uv2.x );
    floatVectorPushBack( &(C->uv), uv2.y );
}


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
    Vertex p0, Normal n0, Vertex p1, Normal n1, Vertex p2, Normal n2 )
{
    floatVectorPushBack( &(C->points), p0.x );
    floatVectorPushBack( &(C->points), p0.y );
    floatVectorPushBack( &(C->points), p0.z );
    floatVectorPushBack( &(C->points), 1.0f );

    floatVectorPushBack( &(C->normals), n0.x );
    floatVectorPushBack( &(C->normals), n0.y );
    floatVectorPushBack( &(C->normals), n0.z );

    floatVectorPushBack( &(C->points), p1.x );
    floatVectorPushBack( &(C->points), p1.y );
    floatVectorPushBack( &(C->points), p1.z );
    floatVectorPushBack( &(C->points), 1.0f );

    floatVectorPushBack( &(C->normals), n1.x );
    floatVectorPushBack( &(C->normals), n1.y );
    floatVectorPushBack( &(C->normals), n1.z );

    floatVectorPushBack( &(C->points), p2.x );
    floatVectorPushBack( &(C->points), p2.y );
    floatVectorPushBack( &(C->points), p2.z );
    floatVectorPushBack( &(C->points), 1.0f );

    floatVectorPushBack( &(C->normals), n2.x );
    floatVectorPushBack( &(C->normals), n2.y );
    floatVectorPushBack( &(C->normals), n2.z );

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
    
    int n = floatVectorSize(&(C->points));
    if( n == 0 ) {
        return( NULL );
    }

    // create and fill a new point array
    C->pointArray = (float *) malloc( n * sizeof(float) );
    if( C->pointArray == 0 ) {
        perror( "point allocation failure" );
        exit( 1 );
    }
    for( int i = 0; i < n; i++ ) {
        C->pointArray[i] = C->points.vec[i];
    }

    return C->pointArray;
}

///
// gets the array of normals for the current shape
//
// @param C The Canvas to use
///
float *Canvas_getNormals( Canvas *C )
{
    // delete the old normal array if we have one
    if( C->normalArray ) {
        free( C->normalArray );
    }
    
    int n = floatVectorSize(&(C->normals));
    if( n == 0 ) {
        return( NULL );
    }

    // create and fill a new normal array
    C->normalArray = (float *) malloc( n * sizeof(float) );
    if( C->normalArray == 0 ) {
        perror( "normal allocation failure" );
        exit( 1 );
    }
    for( int i = 0; i < n; i++ ) {
        C->normalArray[i] = C->normals.vec[i];
    }

    return C->normalArray;
}

///
// gets the array of texture coordinates for the current shape
//
// @param C The Canvas to use
///
float *Canvas_getUV( Canvas *C )
{
    // free old texture coordinate array if we have one
    if( C->uvArray ) {
        free( C->uvArray );
    }
    
    int n = floatVectorSize(&(C->uv));
    if( n == 0 ) {
        return( NULL );
    }

    // create and fill a new texture coordinate array
    C->uvArray = (float *) malloc( n * sizeof(float) );
    if( C->uvArray == 0 ) {
        perror( "uv allocation failure" );
	exit( 1 );
    }
    for( int i = 0; i < n; i++ ) {
        C->uvArray[i] = C->uv.vec[i];
    }
    
    return C->uvArray;
}

///
// gets the array of elements for the current shape
///
GLuint *Canvas_getElements( Canvas *C )
{
    // delete the old point array if we have one
    if( C->elemArray ) {
        free( C->elemArray );
    }

    // create and fill a new point array
    C->elemArray = (GLuint *) malloc( C->numElements * sizeof(GLuint) );
    if( C->elemArray == 0 ) {
        perror( "element allocation failure" );
        exit( 1 );
    }
    for( GLuint i = 0; i < C->numElements; i++ ) {
        C->elemArray[i] = i;
    }

    return C->elemArray;
}



/**
 * returns number of vertices in current shape
 */
int Canvas_nVertices( Canvas *C )
{
    return C->numElements;
}
