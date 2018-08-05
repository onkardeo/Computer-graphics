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
    if( elemArray ) {
        delete [] elemArray;
        elemArray = 0;
    }
    points.clear();
    numElements = 0;
}


///
// adds a triangle to the current shape
//
// @param p0 first triangle vertex
// @param p1 second triangle vertex
// @param p2 final triangle vertex
///
void Canvas::addTriangle( Vertex p0, Vertex p1, Vertex p2 )
{
    points.push_back( p0.x );
    points.push_back( p0.y );
    points.push_back( p0.z );
    points.push_back( 1.0f );

    points.push_back( p1.x );
    points.push_back( p1.y );
    points.push_back( p1.z );
    points.push_back( 1.0f );

    points.push_back( p2.x );
    points.push_back( p2.y );
    points.push_back( p2.z );
    points.push_back( 1.0f );

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

    // create and fill a new point array
    pointArray = new float[ points.size() ];
    if( pointArray == 0 ) {
        cerr << "point allocation failure" << endl;
        exit( 1 );
    }
    for( int i = 0; i < points.size(); i++ ) {
        pointArray[i] = points[i];
    }

    return pointArray;
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

    // create and fill a new element array
    elemArray = new GLuint[ numElements ];
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
