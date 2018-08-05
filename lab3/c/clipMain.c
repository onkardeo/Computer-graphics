//
//  clipMain.c
//
//  Created by Warren R. Carithers on 02/27/14.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2011 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

#include "shaderSetup.h"
#include "Vertex.h"
#include "BufferSet.h"
#include "Canvas.h"
#include "Clipper.h"

///
// Object information
///

// our clipping regions (ll and ur corners)

Vertex_t clip1[2] = { { 10, 110 }, {  50, 150 } };
Vertex_t clip2[2] = { { 30,  10 }, {  70,  80 } };
Vertex_t clip3[2] = { { 90,  34 }, { 120,  60 } };
Vertex_t clip4[2] = { { 90,  80 }, { 130, 110 } };

// our polygons (list of vertices)

Vertex_t quad1[] = {
    { 20, 120 }, { 20, 140 }, { 40, 140 }, { 40, 120 }
};
int quad1_nv = sizeof(quad1) / sizeof(Vertex_t);

Vertex_t quad2[] = {
    { 80, 160 }, { 80, 200 }, { 60, 200 }, { 60, 160 }
};
int quad2_nv = sizeof(quad2) / sizeof(Vertex_t);

Vertex_t quad3[] = {
    { 20, 60 }, { 50, 60 }, { 50, 50 }, { 20, 50 }
};
int quad3_nv = sizeof(quad3) / sizeof(Vertex_t);

Vertex_t quad4[] = {
    { 44, 122 }, { 60, 122 }, { 60, 146 }, { 44, 146 }
};
int quad4_nv = sizeof(quad3) / sizeof(Vertex_t);

Vertex_t pent1[] = {
    { 80, 20 }, { 90, 10 }, { 110, 20 }, { 100, 50 }, { 80, 40 }
};
int pent1_nv = sizeof(pent1) / sizeof(Vertex_t);

Vertex_t hept1[] = {
    { 120, 70 }, { 140, 70 }, { 160, 80 }, { 160, 100 },
    { 140, 110 }, { 120, 100 }, { 110, 90 }
};
int hept1_nv = sizeof(hept1) / sizeof(Vertex_t);

// count of vertices in each clipped polygon
int nv[6];

///
// Drawing-related variables
///

// dimensions of the drawing window

const int w_width  = 300;
const int w_height = 300;

#define BUFFER_OFFSET(i)        ((char *)NULL + (i))

// our Canvas
Canvas *canvas;

// variables related to drawing the clipping regions
BufferSet clipBuffers;

// variables related to drawing the original polygons
BufferSet polyOutlineBuffers;

// variables related to drawing the resulting polygons
BufferSet clippedPolyBuffers;

// shader program handle
GLuint program;

///
// Functions
///

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

    // get the color data
    float *colors = Canvas_getColors( C );
    // #bytes = number of elements * 4 floats/element * bytes/float
    B->cSize = B->numElements * 4 * sizeof(float);

    // set up the vertex buffer
    if( B->bufferInit ) {
        // must delete the existing buffers first
        glDeleteBuffers( 1, &(B->vbuffer) );
        glDeleteBuffers( 1, &(B->ebuffer) );
        B->bufferInit = false;
    }

    // first, create the connectivity data
    B->ebuffer = makeBuffer( GL_ELEMENT_ARRAY_BUFFER, elements, B->eSize );

    // next, the vertex buffer, containing vertices and color data
    B->vbuffer = makeBuffer( GL_ARRAY_BUFFER, NULL, B->vSize + B->cSize );
    glBufferSubData( GL_ARRAY_BUFFER, 0, B->vSize, points );
    glBufferSubData( GL_ARRAY_BUFFER, B->vSize, B->cSize, colors );

    // NOTE:  'points', 'elements', and 'colors are dynamically allocated,
    // but we don't free them here because they will be freed at the next
    // call to clearImage() or the get*() functions

    // finally, mark it as set up
    B->bufferInit = true;

}

///
// Support function that draws clip regions as line loops
///
void drawClipRegion( Vertex_t ll, Vertex_t ur, Canvas *C ) {
    Canvas_setPixel( ll.x, ll.y, C );  // LL
    Canvas_setPixel( ur.x, ll.y, C );  // LR
    Canvas_setPixel( ur.x, ur.y, C );  // UR
    Canvas_setPixel( ll.x, ur.y, C );  // UL
}

///
// Draw all the clipping rectangles
///
void makeClipOutlines( Canvas *C ) {

    // we draw the clipping regions as white rectangles.
    // all vertices are put into one vertex buffer, and
    // we use glDrawArrays() instead of glDrawElements()
    // to draw them as line loops

    drawClipRegion( clip1[0], clip1[1], C );
    drawClipRegion( clip2[0], clip2[1], C );
    drawClipRegion( clip3[0], clip3[1], C );
    drawClipRegion( clip4[0], clip4[1], C );
}

///
// Draw a polygon
///
void drawPolygon( Vertex_t v[], int nv, Canvas *C ) {

    // just put the vertices into the vertex buffer, in order

    for( int i = 0; i < nv; ++i ) {
        Canvas_setPixel( v[i].x, v[i].y, C );
    }
}

///
// Create the polygon outlines
///
void makePolygonOutlines( Canvas *C ) {

    // here, we draw the original polygons; these
    // will be rendered using line loops

    Canvas_setColor ( 1.0f, 0.0f, 0.0f, C );	// red
    drawPolygon( quad1, quad1_nv, C );
    Canvas_setColor ( 0.0f, 1.0f, 0.0f, C ); // green
    drawPolygon( quad2, quad2_nv, C );
    Canvas_setColor ( 0.0f, 0.0f, 1.0f, C );	// blue
    drawPolygon( quad3, quad3_nv, C );
    Canvas_setColor ( 1.0f, 0.0f, 1.0f, C );	// magenta
    drawPolygon( quad4, quad4_nv, C );
    Canvas_setColor ( 1.0f, 0.5f, 1.0f, C ); // red-greenish-blue
    drawPolygon( pent1, pent1_nv, C );
    Canvas_setColor ( 0.7f, 0.7f, 0.7f, C );	// gray
    drawPolygon( hept1, hept1_nv, C );
}

///
// Create the filled polygons
///
void makePolygons( Canvas *C ) {
    // temporary vertex array
    Vertex_t tmp[50];
    int wl;

    ///
    // first polygon:  entirely within region
    ///

    Canvas_setColor ( 1.0f, 0.0f, 0.0f, C );	// red
    wl = 0;
    wl = clipPolygon( 4, quad1, tmp, clip1[0], clip1[1] );
    if( wl > 0 ) {
        drawPolygon( tmp, wl, C );
	nv[0] = wl;
    }

    ///
    // second polygon:  entirely outside region
    ///

    Canvas_setColor ( 0.0f, 1.0f, 0.0f, C ); // green
    wl = 0;
    wl = clipPolygon( 4, quad2, tmp, clip1[0], clip1[1] );
    // shouldn't draw anything!
    if( wl > 0 ) {
        drawPolygon( tmp, wl, C );
	nv[1] = wl;
    }

    ///
    // third polygon:  halfway outside on left
    ///

    Canvas_setColor ( 0.0f, 0.0f, 1.0f, C );	// blue
    wl = 0;
    wl = clipPolygon( 4, quad3, tmp, clip2[0], clip2[1] );
    if( wl > 0 ) {
        drawPolygon( tmp, wl, C );
	nv[2] = wl;
    }

    ///
    // fourth polygon:  part outside on right
    ///

    Canvas_setColor ( 1.0f, 0.0f, 1.0f, C );	// magenta
    wl = 0;
    wl = clipPolygon( 4, quad4, tmp, clip1[0], clip1[1] );
    if( wl > 0 ) {
        drawPolygon( tmp, wl, C );
	nv[3] = wl;
    }

    ///
    // fifth polygon:  outside on left and bottom
    ///

    Canvas_setColor ( 1.0f, 0.5f, 1.0f, C ); // red-greenish-blue
    wl = 0;
    wl = clipPolygon( 5, pent1, tmp, clip3[0], clip3[1] );
    if( wl > 0 ) {
        drawPolygon( tmp, wl, C );
	nv[4] = wl;
    }

    ///
    // sixth polygon:  outside on top, right, and bottom
    ///

    Canvas_setColor ( 0.7f, 0.7f, 0.7f, C );	// gray
    wl = 0;
    wl = clipPolygon( 7, hept1, tmp, clip4[0], clip4[1] );
    if( wl > 0 ) {
        drawPolygon( tmp, wl, C );
	nv[5] = wl;
    }

}

///
// Create all our objects
///
void createImage( Canvas *C )
{
    // start with a clean canvas
    Canvas_clear( C );

    // first, create the clipping region buffers
    //
    // start by putting all the vertices for all
    // the regions into a single set of buffers

    // draw all of them in white
    Canvas_setColor( 1.0f, 1.0f, 1.0f, C );
    makeClipOutlines( C );

    // collect the vertex, element, and color data for these
    createBuffers( &clipBuffers, C );

    // next, do the polygon outlines
    Canvas_clear( C );
    makePolygonOutlines( C );
    createBuffers( &polyOutlineBuffers, C );

    // finally, do the polygons
    Canvas_clear( C );
    makePolygons( C );
    createBuffers( &clippedPolyBuffers, C );

}

///
// Verify shader creation
///
void checkShaderError( GLuint program, const char *which )
{
    if( program == 0 ) {
        fprintf( stderr, "Error setting up %s shader - %s\n",
            which, errorString(shaderErrorCode) );
	exit( 1 );
    }
}

///
// OpenGL initialization
///
void init( void )
{
    // Create our Canvas and Rasterizer "objects"
    canvas = Canvas_create( w_width, w_height );

    if( canvas == NULL ) {
        fputs( "error - cannot create Canvas\n", stderr );
        exit( 1 );
    }

    // Load shaders and use the resulting shader program
    program = shaderSetup( "shader.vert", "shader.frag" );
    checkShaderError( program, "basic" );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    // glEnable( GL_CULL_FACE );
    // glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // create the geometry for our shapes.
    createImage( canvas );
}

void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // ensure we have selected the correct shader program
    glUseProgram( program );

    // set up our uniforms
    GLuint wh = glGetUniformLocation( program, "wh" );
    glUniform2f( wh, (float) w_width, (float) w_height );

    ///
    // first, draw the clip region outlines
    ///
    GLuint vPosition, vColor;

    // bind our buffers
    glBindBuffer( GL_ARRAY_BUFFER, clipBuffers.vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, clipBuffers.ebuffer );

    // set up our attribute variables
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(clipBuffers.vSize) );

    // draw our shapes
    glDrawArrays( GL_LINE_LOOP,  0, 4 );
    glDrawArrays( GL_LINE_LOOP,  4, 4 );
    glDrawArrays( GL_LINE_LOOP,  8, 4 );
    glDrawArrays( GL_LINE_LOOP, 12, 4 );

    ///
    // next, draw the polygon outlines
    ///

    // bind our buffers
    glBindBuffer( GL_ARRAY_BUFFER, polyOutlineBuffers.vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, polyOutlineBuffers.ebuffer );

    // set up our attribute variables
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(polyOutlineBuffers.vSize) );

    // draw our shapes
    int skip = 0;
    glDrawArrays( GL_LINE_LOOP, skip, quad1_nv ); skip += quad1_nv;
    glDrawArrays( GL_LINE_LOOP, skip, quad2_nv ); skip += quad2_nv;
    glDrawArrays( GL_LINE_LOOP, skip, quad3_nv ); skip += quad3_nv;
    glDrawArrays( GL_LINE_LOOP, skip, quad4_nv ); skip += quad4_nv;
    glDrawArrays( GL_LINE_LOOP, skip, pent1_nv ); skip += pent1_nv;
    glDrawArrays( GL_LINE_LOOP, skip, hept1_nv ); skip += hept1_nv;

    ///
    // finally, draw the clipped polygons
    ///

    // bind our buffers
    glBindBuffer( GL_ARRAY_BUFFER, clippedPolyBuffers.vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, clippedPolyBuffers.ebuffer );

    // set up our attribute variables
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(clippedPolyBuffers.vSize) );

    // draw our shapes
    //
    // be sure to only draw what's there
    skip = 0;
    if( nv[0] ) {
        glDrawArrays( GL_TRIANGLE_FAN, skip, nv[0] ); skip += nv[0];
    }
    if( nv[1] ) {
    }
        glDrawArrays( GL_TRIANGLE_FAN, skip, nv[1] ); skip += nv[1];
    if( nv[2] ) {
        glDrawArrays( GL_TRIANGLE_FAN, skip, nv[2] ); skip += nv[2];
    }
    if( nv[3] ) {
        glDrawArrays( GL_TRIANGLE_FAN, skip, nv[3] ); skip += nv[3];
    }
    if( nv[4] ) {
        glDrawArrays( GL_TRIANGLE_FAN, skip, nv[4] ); skip += nv[4];
    }
    if( nv[5] ) {
        glDrawArrays( GL_TRIANGLE_FAN, skip, nv[5] ); skip += nv[5];
    }

    // swap the buffers
    glutSwapBuffers();
}

void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 033:  // Escape key
        case 'q': case 'Q':
            Canvas_destroy( canvas );
            exit( 0 );
            break;
    }

    glutPostRedisplay();
}


///
// main program for polygon clip assignment
///
int main( int argc, char *argv[] )
{
    glutInit( &argc, argv );
    // may need to add GLUT_3_2_CORE_PROFILE on OS X and macOS systems
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( w_width, w_height );
    glutCreateWindow( "Lab 3 - Polygon Clipping" );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        fprintf( stderr, "GLEW error: %s\n", glewGetErrorString(err) );
        exit( 1 );
    }
    if( ! GL_VERSION_3_2 ) {
        fprintf( stderr, "GLEW: OpenGL 3.2 not supported!\n" );
        exit( 1 );
    }
#endif

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
