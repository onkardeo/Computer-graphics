//
//  Canvas.h
//
//  Created by Warren R. Carithers 2016/09/30.
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

using namespace std;

#include <vector>

#include "Vertex.h"

///
// Simple canvas class that allows for pixel-by-pixel rendering.
///

class Canvas {

    ///
    // canvas size information
    ///
    int width;
    int height;

    ///
    // point-related data
    ///
    vector<float> points;
    float *pointArray;
    vector<float> normals;
    float *normalArray;
    vector<float> uv;
    float *uvArray;
    int numElements;
    GLuint *elemArray;

public:
    ///
    // Constructor
    //
    // @param w width of canvas
    // @param h height of canvas
    ///
    Canvas( int w, int h );

    ///
    // Destructor
    ///
    ~Canvas( void );

    ///
    // clears the canvas
    ///
    void clear( void );

    ///
    // adds a triangle to the current shape, along with (u,v) data
    //
    // @param p0 first triangle vertex
    // @param uv0 first vertex (u,v) data
    // @param p1 second triangle vertex
    // @param uv1 second vertex (u,v) data
    // @param p2 final triangle vertex
    // @param uv2 final vertex (u,v) data
    ///
    void addTriangleWithUV( Vertex p0, UVcoord uv0,
            Vertex p1, UVcoord uv1, Vertex p2, UVcoord uv2 );

    ///
    // adds a triangle to the current shape, along with normal data
    //
    // @param p0 first triangle vertex
    // @param n0 first vertex normal data
    // @param p1 second triangle vertex
    // @param n1 second vertex normal data
    // @param p2 final triangle vertex
    // @param n2 final vertex normal data
    ///
    void addTriangleWithNorms( Vertex p0, Normal n0,
            Vertex p1, Normal n1, Vertex p2, Normal n2 );

    ///
    // retrieve the array of element data from this Canvas
    ///
    GLuint *getElements( void );

    ///
    // retrieve the array of normal data from this Canvas
    ///
    float *getNormals( void );

    ///
    // retrieve the array of (u,v) data from this Canvas
    ///
    float *getUV( void );

    ///
    // retrieve the array of vertex data from this Canvas
    ///
    float *getVertices( void );

    ///
    // retrieve the vertex count from this Canvas
    ///
    int nVertices( void );

};

#endif
