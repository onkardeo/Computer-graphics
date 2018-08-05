//
//  textingMain
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Main program for lighting/shading/texturing assignment
//
//  This code can be compiled as either C or C++.
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
#include "Lighting.h"
#include "Textures.h"

#ifdef __cplusplus
using namespace std;
#endif

// How to calculate an offset into the vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// our drawing canvas
Canvas *canvas;

// dimensions of the drawing window
const int w_width  = 600;
const int w_height = 600;

//
// We need two vertex buffers and two element buffers:
// one set for the quad (texture mapped), and one set
// for the teapot (Phong shaded)
//
BufferSet quadBuffers;
BufferSet teapotBuffers;

// Animation flag
bool animating = false;

// Initial animation rotation angles
float angles[2] = { 0.0f, 0.0f };

// program IDs...for shader programs
GLuint pshader, tshader;

//
// makeBuffer() - create a vertex or element array buffer
//
// @param target - which type of buffer to create
// @param data   - source of data for buffer (or NULL)
// @param size   - desired length of buffer
//
GLuint makeBuffer( GLenum target, const void *data, GLsizei size )
{
    GLuint buffer;

    glGenBuffers( 1, &buffer );
    glBindBuffer( target, buffer );
    glBufferData( target, size, data, GL_STATIC_DRAW );

    return( buffer );
}

///
// Create a set of vertex and element buffers
//
// @param B   - the BufferSet to be modified
// @param C   - the Canvas we'll use for drawing
///
void createBuffers( BufferSet *B, Canvas *C ) {

    // get the vertices
    B->numElements = Canvas_nVertices( C );
    float *points = Canvas_getVertices( C );
    // #bytes = number of elements * 4 floats/element * bytes/float
    B->vSize = B->numElements * 4 * sizeof(float);
    int vbufferSize = B->vSize;

    // get the normals
    float *normals = Canvas_getNormals( C );
    B->nSize = B->numElements * 3 * sizeof(float);
    vbufferSize += B->nSize;

    // get the UV data (if any)
    float *uv = Canvas_getUV( C );
    B->tSize = 0;
    if( uv != NULL ) {
	B->tSize = B->numElements * 2 * sizeof(float);
    }
    vbufferSize += B->tSize;

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

    // next, the vertex buffer, containing vertices and "extra" data
    B->vbuffer = makeBuffer( GL_ARRAY_BUFFER, NULL, vbufferSize );
    glBufferSubData( GL_ARRAY_BUFFER, 0, B->vSize, points );
    glBufferSubData( GL_ARRAY_BUFFER, B->vSize, B->nSize, normals );
    if( uv != NULL ) {
	// add the texture coordinates after the normals
        glBufferSubData( GL_ARRAY_BUFFER, B->vSize + B->nSize,
	                 B->tSize, uv );
    }

    // NOTE:  'points', 'extra', and 'elements' are dynamically allocated,
    // but we don't free them here because they will be freed at the next
    // call to clear() or the get*() functions

    // finally, mark it as set up
    B->bufferInit = true;
}

//
// createShape() - create vertex and element buffers for a shape
//
// @param obj - which shape to create
// @param C   - the Canvas to use
//
void createShape( int obj, Canvas *C )
{
    // clear any previous shape
    Canvas_clear( C );

    // make the shape
    makeShape( obj, C );

    // create the necessary buffers
    createBuffers( obj == OBJ_TEAPOT ? &teapotBuffers : &quadBuffers, C );
}

//
// selectBuffers() - bind the correct vertex and element buffers
//
// @param program - GLSL program object
// @param obj     - which object to select
// @param B       - the BufferSet to update
//
void selectBuffers( GLuint program, int obj, BufferSet *B ) {

    // bind the buffers
    glBindBuffer( GL_ARRAY_BUFFER, B->vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, B->ebuffer );

    // set up the vertex attribute variables
    GLuint vPosition = glGetAttribLocation( program , "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(B->vSize) );

    // if we're doing the quad, also need to set texture coords
    if( obj == OBJ_QUAD ) {
        GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
        glEnableVertexAttribArray( vTexCoord );
        glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(B->vSize+B->nSize) );
    }
}


// Verify shader program creation
static void checkShaderError( GLuint program, const char *which )
{
    if( !program ) {
#ifdef __cplusplus
        cerr << "Error setting " << which << " shader - "
             << errorString(shaderErrorCode) << endl;
#else
        fprintf( stderr, "Error setting up %s shader - %s\n",
            which, errorString(shaderErrorCode) );
#endif
        exit(1);
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

    // Load texture image(s)
    loadTexture();

    // Load shaders, verifying each
    tshader = ShaderSetup( "texture.vert", "texture.frag" );
    checkShaderError( tshader, "texture" );

    pshader = ShaderSetup( "phong.vert", "phong.frag" );
    checkShaderError( pshader, "phong" );

    // Create all our objects
    createShape( OBJ_QUAD, canvas );
    createShape( OBJ_TEAPOT, canvas );

    // Other OpenGL initialization
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );
}


#ifdef __cplusplus
extern "C" {
#endif

void display( void )
{
    // clear and draw params..
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // first, the quad
    glUseProgram( tshader );

    // set up viewing and projection parameters
    setUpFrustum( tshader );

    // set up the texture information
    setUpTexture( tshader );

    // set up the camera
    setUpCamera( tshader,
        0.2f, 3.0f, 6.5f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );

    // set up transformations for the quad
    setUpTransforms( tshader,
        1.5f, 1.5f, 1.5f,
        angles[OBJ_QUAD], angles[OBJ_QUAD], angles[OBJ_QUAD],
        -1.25f, 1.0f, -1.5f
    );

    // draw it
    selectBuffers( tshader, OBJ_QUAD, &quadBuffers );
    glDrawElements( GL_TRIANGLES, quadBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );

    // now, draw the teapot

    glUseProgram( pshader );

    // set up viewing and projection parameters
    setUpFrustum( pshader );

    // set up the Phong shading information
    setUpPhong( pshader );

    // set up the camera
    setUpCamera( pshader,
        0.2f, 3.0f, 6.5f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    );

    setUpTransforms( pshader,
        2.0f, 2.0f, 2.0f,
        angles[OBJ_TEAPOT], angles[OBJ_TEAPOT], angles[OBJ_TEAPOT],
        1.5f, 0.5f, -1.5f );

    // draw it
    selectBuffers( pshader, OBJ_TEAPOT, &teapotBuffers );
    glDrawElements( GL_TRIANGLES, teapotBuffers.numElements,
        GL_UNSIGNED_INT, (void *)0 );

    // swap the buffers
    glutSwapBuffers();
}


void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {

        case 'a':    // animate
            animating = true;
            break;

        case 's':    // stop animating
            animating = false;
            break;

        case 'r':    // reset rotations
            angles[0] = 0.0f;
	    angles[1] = 0.0f;
            break;

        case 033:   // terminate the program
        case 'q': case 'Q':
            exit( 0 );
            break;
    }

    glutPostRedisplay();
}

// Animate the objects (maybe)
void animate( void ) {
    if( animating ) {
        angles[OBJ_QUAD]   += 1;
        angles[OBJ_TEAPOT] += 1;
        glutPostRedisplay();
    }
}

#ifdef __cplusplus
}
#endif

int main (int argc, char **argv)
{
    glutInit( &argc, argv );
    // may need to add GLUT_3_2_CORE_PROFILE on OS X and macOS systems
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( w_width, w_height );
    glutCreateWindow( "Lab 6 - Shading and Texturing" );

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
    glutIdleFunc( animate );

    glutMainLoop();
    return 0;
}
