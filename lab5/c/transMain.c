//
//  transMain
//
//  Created by Warren R. Carithers 2016/11/10.
//  Based on code created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Main program for transformation assignment
//
//  This file should not be modified by students.
//

#ifdef __cplusplus
#include <cstdlib>
#include <iostream>
#else
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "BufferSet.h"
#include "ShaderSetup.h"
#include "Canvas.h"
#include "Shapes.h"
#include "Viewing.h"

#ifdef __cplusplus
using namespace std;
#endif

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// our drawing canvas
Canvas *canvas;

// dimensions of the drawing window
const int w_width  = 512;
const int w_height = 512;

// buffer information
BufferSet shapeBuffers;

// mouse click tracker
int counter = 0;

// flags controlling drawing options
bool cameraAdjust = false;
bool transformsOn = false;
int viewMode = 1;

// program IDs...for program and parameters
GLuint program;

// vertex position attribute variable
GLuint vPosition;

///
// create a vertex or element buffer
///
GLuint makeBuffer( GLenum target, const void *data, GLsizei size )
{
    GLuint buffer;

    glGenBuffers( 1, &buffer );
    glBindBuffer( target, buffer );
    glBufferData( target, size, data, GL_STATIC_DRAW );

    return( buffer );
}

///
// Creat a set of vertex and element buffers
///
void createBuffers( BufferSet *B, Canvas *C ) {

    // get the vertices
    B->numElements = Canvas_nVertices( C );
    float *points = Canvas_getVertices( C );
    // #bytes = number of elements * 4 floats/element * bytes/float
    B->vSize = B->numElements * 4 * sizeof(float);

    // get the element data
    GLuint *elements = Canvas_getElements( C );
    // #bytes = number of elements * bytes/element
    B->eSize = B->numElements * sizeof(GLuint);

    // set up the vertex buffer
    if( B->bufferInit ) {
        // must delete the existing buffers first
        glDeleteBuffers( 1, &(B->vbuffer) );
        glDeleteBuffers( 1, &(B->ebuffer) );
        B->bufferInit = false;
    }

    // first, create the connectivity data
    B->ebuffer = makeBuffer( GL_ELEMENT_ARRAY_BUFFER, elements, B->eSize );

    // next, the vertex buffer, containing vertices only
    B->vbuffer = makeBuffer( GL_ARRAY_BUFFER, points, B->vSize );

    // NOTE:  'points' and 'elements' are dynamically allocated,
    // but we don't free them here because they will be freed at the next
    // call to clear() or the get*() functions

    // finally, mark it as set up
    B->bufferInit = true;
}

///
// Create the current teapot image
///
void createShapes( Canvas *C )
{
    // reset the canvas
    Canvas_clear( C );

    // make the teapot
    makeTeapot( C );

    // create the necessary buffers
    createBuffers( &shapeBuffers, C );
}

///
// Verify shader creation
///
void checkShaderError( GLuint program, const char *which )
{
    if( program == 0 ) {
#ifdef __cplusplus
        cerr << "Error setting up " << which << " shader - "
             << errorString(shaderErrorCode) << endl;
#else
        fprintf( stderr, "Error setting up %s shader - %s\n",
                 which, errorString(shaderErrorCode) );
#endif
        exit( 1 );
    }
}

///
// OpenGL initialization
///
void init()
{
    // Create our Canvas
#ifdef __cplusplus
    canvas = new Canvas( w_width, w_height );
#else
    canvas = Canvas_create( w_width, w_height );
#endif

    if( canvas == NULL ) {
#ifdef __cplusplus
        cerr << "error - cannot create Canvas" << endl;
#else
        fputs( "error - cannot create Canvas\n", stderr );
#endif
        exit( 1 );
    }

    // Load shaders and use the resulting shader program
    program = ShaderSetup( "shader.vert", "shader.frag" );
    checkShaderError( program, "basic" );

    // select this shader program
    glUseProgram( program );

    // get location of shader global variables
    vPosition = glGetAttribLocation( program, "vPosition" );

    // Other OpenGL initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // create the geometry for your shapes.
    createShapes( canvas );
}


#ifdef __cplusplus
extern "C" {
#endif

void display( void )
{
    // clear and draw params..
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // bind our vertex buffer
    glBindBuffer( GL_ARRAY_BUFFER, shapeBuffers.vbuffer );

    // bind our element array buffer
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, shapeBuffers.ebuffer );

    // ensure we have selected the correct shader program
    glUseProgram( program );

    // set up our attribute variable
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    // set up viewing and projection parameters

    if( viewMode == 1 )
        setUpFrustum( program );
    else if( viewMode == 2 )
        setUpOrtho( program );
    else {
        fprintf( stderr, "unknown viewing mode %d - resetting\n", viewMode );
	viewMode = 1;
	setUpFrustum( program );
    }

    // set up the camera
    //
    // changing the camera sets eyepoint to (0,1.3,-0.5), lookat
    // to (0,-0.4,-1.0), and up to (0,1,0)

    if( cameraAdjust )
        setUpCamera( program, 0.0f, 1.3f, -0.5f,
            0.0f, -0.4f, -1.0f, 0.0f, 1.0f, 0.0f );
    else
        clearCamera( program );

    // set up transformations
    //
    // transformations are applied in this order (if you are having
    // trouble recreating the solution image, check your order of
    // matrix multiplication):
    //
    //    scale Y by 2
    //    rotate around Z by 335 degrees
    //    rotate around Y by 10 degrees
    //    translate in X by -0.2
    //    translate in Y by 0.2

    if( transformsOn )
        setUpTransforms( program, 1.0f, 2.0f, 1.0f,
            0.0f, 10.0f, 335.0f,
            -0.2f, 0.2f, 0.0f );
    else
        clearTransforms( program );

    // draw your shape
    glDrawElements( GL_TRIANGLES, shapeBuffers.numElements,
                    GL_UNSIGNED_INT, (void *)0 );

    // swap the buffers
    glutSwapBuffers();
}


void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case '1':
            viewMode = 1;
            break;

        case '2':
            viewMode = 2;
            break;

        case 033:  // Escape key
        case 'q': case 'Q':
            exit( 0 );
            break;
    }

    glutPostRedisplay();
}

void mouse( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
        counter++;

    switch( counter % 4 ) {
        case 0:
            // sets defaults
            cameraAdjust = false;
            transformsOn = false;
            break;
        case 1:
            // turn on transformations
            transformsOn = true;
            break;
        case 2:
            // turn camera adjust on, turn transformations off
            cameraAdjust = true;
            transformsOn = false;
            break;
        case 3:
            // turn transformations back on
            transformsOn = true;
            break;
    }

    glutPostRedisplay();
}

#ifdef __cplusplus
}
#endif

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    // may need to add GLUT_3_2_CORE_PROFILE on OS X and macOS systems
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( w_width, w_height );
    glutCreateWindow( "Lab 5 - Transformations" );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
#ifdef __cplusplus
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
#else
        fprintf( stderr, "GLEW error: %s\n", glewGetErrorString(err) );
#endif
        exit( 1 );
    }
    if( ! GL_VERSION_3_2 ) {
#ifdef __cplusplus
        cerr << "GLEW: OpenGL 3.2 not supported!" << endl;
#else
        fputs( "GLEW: OpenGL 3.2 not supported!\n", stderr );
#endif
        exit( 1 );
    }
#endif

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );

    glutMainLoop();
    return 0;
}
