//
//  Canvas.cpp
//
//  Created by Warren R. Carithers 2016/09/30.
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

#include <cstdlib>
#include <iostream>
#include <iomanip>

#include "Canvas.h"

///
// Constructor
//
// @param w width of canvas
// @param h height of canvas
///

Canvas::Canvas( int w, int h ) : width(w), height(h) {
    pointArray = 0;
    normalArray = 0;
    uvArray = 0;
    elemArray = 0;
    clear();
}


///
// Destructor
///

Canvas::~Canvas( void ) {
    clear();
}


///
// clear the canvas
///
void Canvas::clear( void )
{
    if( pointArray ) {
        delete [] pointArray;
        pointArray = 0;
    }
    if( normalArray ) {
        delete [] normalArray;
        normalArray = 0;
    }
    if( uvArray ) {
        delete [] uvArray;
        uvArray = 0;
    }
    if( elemArray ) {
        delete [] elemArray;
        elemArray = 0;
    }
    points.clear();
    normals.clear();
    uv.clear();
    numElements = 0;
}


///
// adds a triangle to the current shape, along with (u,v) data
//
// @param p0 first triangle vertex
// @param uv0 first triangle (u,v) data
// @param p1 second triangle vertex
// @param uv1 second triangle (u,v) data
// @param p2 final triangle vertex
// @param uv2 final triangle (u,v) data
///
void Canvas::addTriangleWithUV( Vertex p0, UVcoord uv0,
        Vertex p1, UVcoord uv1, Vertex p2, UVcoord uv2 )
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
    addTriangleWithNorms( p0, nn, p1, nn, p2, nn );

    // Attach the texture coordinates
    uv.push_back( uv0.x );  // note use of (x,y) vs. (u,v)
    uv.push_back( uv0.y );  // see Vertex.h for details
    uv.push_back( uv1.x );
    uv.push_back( uv1.y );
    uv.push_back( uv2.x );
    uv.push_back( uv2.y );
}


///
// adds a triangle to the current shape, along with normal data
//
// @param p0 first triangle vertex
// @param n0 first triangle normal data
// @param p1 second triangle vertex
// @param n1 second triangle normal data
// @param p2 final triangle vertex
// @param n2 final triangle normal data
///
void Canvas::addTriangleWithNorms( Vertex p0, Normal n0,
             Vertex p1, Normal n1, Vertex p2, Normal n2 )
{
    points.push_back( p0.x );
    points.push_back( p0.y );
    points.push_back( p0.z );
    points.push_back( 1.0f );

    normals.push_back( n0.x );
    normals.push_back( n0.y );
    normals.push_back( n0.z );

    points.push_back( p1.x );
    points.push_back( p1.y );
    points.push_back( p1.z );
    points.push_back( 1.0f );

    normals.push_back( n1.x );
    normals.push_back( n1.y );
    normals.push_back( n1.z );

    points.push_back( p2.x );
    points.push_back( p2.y );
    points.push_back( p2.z );
    points.push_back( 1.0f );

    normals.push_back( n2.x );
    normals.push_back( n2.y );
    normals.push_back( n2.z );

    numElements += 3;  // three vertices per triangle

}


///
// gets the array of vertices for the current shape
///
float *Canvas::getVertices( void )
{
    // delete the old point array if we have one
    if( pointArray ) {
        delete [] pointArray;
    }

    int n = points.size();
    if( n == 0 ) {
        return( NULL );
    }

    // create and fill a new point array
    pointArray = new float[ n ];
    if( pointArray == 0 ) {
        cerr << "point allocation failure" << endl;
        exit( 1 );
    }
    for( int i = 0; i < n; i++ ) {
        pointArray[i] = points[i];
    }

    return pointArray;
}


///
// gets the array of normals for the current shape
///
float *Canvas::getNormals( void )
{
    // delete the old point array if we have one
    if( normalArray ) {
        delete [] normalArray;
    }

    int n = normals.size();
    if( n == 0 ) {
        return( NULL );
    }

    // create and fill a new point array
    normalArray = new float[ n ];
    if( normalArray == 0 ) {
        cerr << "normal allocation failure" << endl;
        exit( 1 );
    }
    for( int i = 0; i < n; i++ ) {
        normalArray[i] = normals[i];
    }

    return normalArray;
}


///
// gets the array of texture coordinates for the current shape
///
float *Canvas::getUV( void )
{
    // delete the old texture coordinate array if we have one
    if( uvArray ) {
        delete [] uvArray;
    }

    int n = uv.size();
    if( n == 0 ) {
        return( NULL );
    }

    // create and fill a new texture coordinate array
    uvArray = new float[ n ];
    if( uvArray == 0 ) {
        cerr << "uv allocation failure" << endl;
        exit( 1 );
    }
    for( int i = 0; i < n; i++ ) {
        uvArray[i] = uv[i];
    }

    return uvArray;
}


///
// gets the array of elements for the current shape
///
GLuint *Canvas::getElements( void )
{
    // delete the old element array if we have one
    if( elemArray ) {
        delete [] elemArray;
    }

    int n = points.size();
    if( n == 0 ) {
        return( NULL );
    }

    // create and fill a new element array
    elemArray = new GLuint[ n ];
    if( elemArray == 0 ) {
        cerr << "element allocation failure" << endl;
        exit( 1 );
    }
    for( int i = 0; i < numElements; i++ ) {
        elemArray[i] = i;
    }

    return elemArray;
}


///
// returns number of vertices in current shape
///
int Canvas::nVertices( void )
{
    return numElements;
}
