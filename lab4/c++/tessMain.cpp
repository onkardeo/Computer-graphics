//
//  tessMain
//
//  Created by Warren R. Carithers 2016/11/04.
//  Based on code created by Joe Geigel and updated by
//    Vasudev Prasad Bethamcherla.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Main program for tessellation assignment.
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

#ifdef __cplusplus
using namespace std;
#endif

// The shapes that we can draw
#define CUBE 0
#define CYLINDER 1
#define CONE 2
#define SPHERE 3

int currentShape = CUBE;

// dimensions of the drawing window
const int w_width  = 512;
const int w_height = 512;

// subdivisions for tessellation
int division1 = 1;
int division2 = 1;

// are we animating?
bool animating = false;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// our Canvas
Canvas *canvas;

// buffer information
BufferSet shapeBuffers;

// GLSL shader program handle
GLuint program;

// shader arguments
GLuint theta;      // theta uniform location
GLuint vPosition;  // vertex location

// rotation control
float anglesReset[3] = {30.0, 30.0, 0.0};
float angles[3] = {30.0, 30.0, 0.0};
float angleInc = 5.0;

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
void createBuffers( BufferSet &B, Canvas &C ) {

    // get the vertices
    B.numElements = C.nVertices();
    float *points = C.getVertices();
    // #bytes = number of elements * 4 floats/element * bytes/float
    B.vSize = B.numElements * 4 * sizeof(float);

    // get the element data
    GLuint *elements = C.getElements();
    // #bytes = number of elements * bytes/element
    B.eSize = B.numElements * sizeof(GLuint);

    // set up the vertex buffer
    if( B.bufferInit ) {
        // must delete the existing buffers first
        glDeleteBuffers( 1, &(B.vbuffer) );
        glDeleteBuffers( 1, &(B.ebuffer) );
        B.bufferInit = false;
    }

    // first, create the connectivity data
    B.ebuffer = makeBuffer( GL_ELEMENT_ARRAY_BUFFER, elements, B.eSize );

    // next, the vertex buffer, containing vertices only
    B.vbuffer = makeBuffer( GL_ARRAY_BUFFER, points, B.vSize );

    // NOTE:  'points' and 'elements' are dynamically allocated,
    // but we don't free them here because they will be freed at the next
    // call to clear() or the get*() functions

    // finally, mark it as set up
    B.bufferInit = true;

}

///
// create a new shape by tesselating one of
// the 4 basic objects
///
void createNewShape( void ) {

    // clear the old shape
    canvas->clear();

    // create the new shape
    switch( currentShape )
    {
        case CUBE:
	    makeCube( *canvas, division1 );
	    break;
        case CYLINDER:
	    makeCylinder( *canvas, 0.5, division1, division2 );
	    break;
        case CONE:
	    makeCone( *canvas, 0.5, division1, division2 );
	    break;
        case SPHERE:
	    makeSphere( *canvas, 0.5, division1, division2 );
	    break;
    }

    // create our buffers
    createBuffers( shapeBuffers, *canvas );
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
void init( void ) {

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
    theta = glGetUniformLocation( program, "theta" );

    glEnable( GL_DEPTH_TEST );
    glEnable(GL_CULL_FACE);
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // initally create a cube
    createNewShape();
}

#ifdef __cplusplus
extern "C" {
#endif

void display( void ) {

    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // bind our vertex buffer
    glBindBuffer( GL_ARRAY_BUFFER, shapeBuffers.vbuffer );

    // bind our element array buffer
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, shapeBuffers.ebuffer );

    // ensure we have selected the correct shader program
    glUseProgram( program );

    // set up our attribute variables
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                          BUFFER_OFFSET(0) );

    // pass in our rotation as a uniform variable
    glUniform3fv( theta, 1, angles );

    // draw our shape
    glDrawElements( GL_TRIANGLES, shapeBuffers.numElements,
                    GL_UNSIGNED_INT, (void *)0 );

    // swap the framebuffers
    glutSwapBuffers();
}

// Can cause gimbal lock which also happened on Apollo 11
// http://en.wikipedia.org/wiki/Gimbal_lock#Gimbal_lock_on_Apollo_11
// Solution? Use Quaternions (Taught in Comp. Animation: Algorithms)
//
// TIDBIT:
// Quaternion plaque on Brougham (Broom) Bridge, Dublin, which says:
// 
//    "Here as he walked by
//    on the 16th of October 1843
// 
//    Sir William Rowan Hamilton 
//    
//    in a flash of genius discovered
//    the fundamental formula for
//    quaternion multiplication
//    i^2 = j^2 = k^2 = ijk = -1
//    & cut it on a stone of this bridge"

void animate() {
    int i;
    static int level = 0;

    if( level >= 450 ) {
        level = 0;
	animating = false;
    }

    if( !animating ) {
        return;
    }

    if( level < 150 ) {
        angles[0] -= angleInc / 3;
    } else if( level < 300 ) {
        angles[1] -= angleInc / 3;
    } else {
        angles[2] -= angleInc / 3;
    }

    ++level;
    glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y ) {

    switch( key ) {

	// automated animation
        case 'a': case 'A':
	    animating = true;
	    break;

	// incremental rotation along the axes
        case 'x': angles[0] -= angleInc; break;
        case 'y': angles[1] -= angleInc; break;
        case 'z': angles[2] -= angleInc; break;
        case 'X': angles[0] += angleInc; break;
        case 'Y': angles[1] += angleInc; break;
        case 'Z': angles[2] += angleInc; break;

	// shape selection
        case '1':  case 'c': currentShape = CUBE; createNewShape(); break;
        case '2':  case 'C': currentShape = CYLINDER; createNewShape(); break;
        case '3':  case 'n': currentShape = CONE; createNewShape(); break;
        case '4':  case 's': currentShape = SPHERE; createNewShape(); break;

	// tessellation factors
        case '+': division1++; createNewShape(); break;
        case '=': division2++; createNewShape(); break;
        case '-': if( division1 > 1 ) {
                    division1--;
                    createNewShape();
		  }
		  break;
        case '_': if( division2 > 1)  {
                    division2--;
                    if (currentShape != CUBE) createNewShape();
                  }
                  break;

	// reset
	case 'r': case 'R':
	    angles[0] = anglesReset[0];
	    angles[1] = anglesReset[1];
	    angles[2] = anglesReset[2];
	    break;

	// termination
        case 033:  // Escape key
        case 'q': case 'Q':
            exit( 0 );
    }

    glutPostRedisplay();
}

#ifdef __cplusplus
}
#endif

int main( int argc, char **argv ) {

    glutInit( &argc, argv );
    // may need to add GLUT_3_2_CORE_PROFILE on OS X and macOS systems
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( w_width, w_height );
    glutCreateWindow( "Lab 4 - Tessellation" );

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
