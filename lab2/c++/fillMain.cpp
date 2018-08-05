//
//  fillMain
//
//  Main program for polygon fill assignment
//
//  This file should not be modified by students.
//

#include <cstdlib>
#include <iostream>

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include "shaderSetup.h"
#include "Canvas.h"
#include "Rasterizer.h"

using namespace std;

// dimensions of drawing window

const int w_width  = 900;
const int w_height = 600;

#define	BUFFER_OFFSET(i)	((char *)NULL + (i))

// our Rasterizer
Rasterizer *rasterizer;

// buffer information
bool bufferInit = false;
GLuint vbuffer, ebuffer;
GLuint program;
int numElements;
long vdataSize, edataSize, cdataSize;

///
// Create all the polygons and draw them using the Rasterizer
///
void makePolygons( Rasterizer &R )
{
    // start with a clean canvas
    R.C.clear();

    // make these large enough for any of our polygons
    int x[10];
    int y[10];

    // ########### TEAPOT START ###########
    // BASE
    x[0] = 760;  y[0] =  40;
    x[1] = 600;  y[1] =  40;
    x[2] = 620;  y[2] =  60;
    x[3] = 740;  y[3] =  60;
    R.C.setColor( 1.0f, 0.0f, 0.0f );
    R.drawPolygon( 4, x, y );

    // RIGHT BOTTOM TRIANGLE
    x[0] = 800;  y[0] = 120;
    x[1] = 740;  y[1] =  60;
    x[2] = 620;  y[2] =  60;
    R.C.setColor( 0.9f, 0.0f, 0.0f );
    R.drawPolygon( 3, x, y );

    // SPOUT
    x[0] = 620;  y[0] =  60;
    x[1] = 560;  y[1] = 100;
    x[2] = 500;  y[2] = 180;
    R.C.setColor( 0.8f, 0.0f, 0.0f );
    R.drawPolygon( 3, x, y );

    x[0] = 620;  y[0] =  60;
    x[1] = 500;  y[1] = 180;
    x[2] = 460;  y[2] = 200;
    x[3] = 520;  y[3] = 200;
    x[4] = 580;  y[4] = 160;
    R.C.setColor( 0.7f, 0.0f, 0.0f );
    R.drawPolygon( 5, x, y );

    x[0] = 620;  y[0] =  60;
    x[1] = 580;  y[1] = 160;
    x[2] = 620;  y[2] = 240;
    x[3] = 740;  y[3] = 240;
    x[4] = 800;  y[4] = 120;
    R.C.setColor( 0.6f, 0.0f, 0.0f );
    R.drawPolygon( 5, x, y );

    x[0] = 800;  y[0] = 120;
    x[1] = 840;  y[1] = 160;
    x[2] = 855;  y[2] = 200;
    x[3] = 720;  y[3] = 220;
    x[4] = 720;  y[4] = 200;
    x[5] = 830;  y[5] = 190;
    x[6] = 825;  y[6] = 165;
    x[7] = 780;  y[7] = 120;
    R.C.setColor( 0.5f, 0.0f, 0.0f );
    R.drawPolygon( 8, x, y );

    x[0] = 690;  y[0] = 240;
    x[1] = 710;  y[1] = 260;
    x[2] = 650;  y[2] = 260;
    x[3] = 670;  y[3] = 240;
    R.C.setColor( 0.4f, 0.0f, 0.0f );
    R.drawPolygon( 4, x, y );

    // ######## TRIANGLE #######
    x[0] = 460;  y[0] = 220;
    x[1] = 490;  y[1] = 280;
    x[2] = 420;  y[2] = 280;
    R.C.setColor( 0.0f, 1.0f, 0.0f );
    R.drawPolygon( 3, x, y );

    // ########## QUAD ##########
    x[0] = 380;  y[0] = 280;
    x[1] = 320;  y[1] = 320;
    x[2] = 360;  y[2] = 380;
    x[3] = 420;  y[3] = 340;
    R.C.setColor( 0.0f, 0.8f, 0.8f );
    R.drawPolygon( 4, x, y );

    // ############ STAR #############
    // RIGHT SIDE
    x[0] = 230;  y[0] = 389;
    x[1] = 260;  y[1] = 369;
    x[2] = 254;  y[2] = 402;
    x[3] = 278;  y[3] = 425;
    x[4] = 245;  y[4] = 430;
    x[5] = 230;  y[5] = 460;
    x[6] = 230;  y[6] = 410;
    R.C.setColor( 0.8f, 0.8f, 0.0f );
    R.drawPolygon( 7, x, y );

    // LEFT SIDE
    x[0] = 230;  y[0] = 460;
    x[1] = 216;  y[1] = 430;
    x[2] = 183;  y[2] = 425;
    x[3] = 207;  y[3] = 402;
    x[4] = 201;  y[4] = 369;
    x[5] = 230;  y[5] = 389;
    x[6] = 230;  y[6] = 410;
    R.C.setColor( 0.7f, 0.7f, 0.0f );
    R.drawPolygon( 7, x, y );

    // ########## BORDERS ###############
    // SQUARE BOTTOM LEFT
    x[0] =   0;  y[0] =   0;
    x[1] =   0;  y[1] =  20;
    x[2] =  20;  y[2] =  20;
    x[3] =  20;  y[3] =   0;
    R.C.setColor( 0.0f, 0.0f, 1.0f );
    R.drawPolygon( 4, x, y );

    x[0] =   0;  y[0] =  10;
    x[1] =  10;  y[1] =  10;
    x[2] =  10;  y[2] = 580;
    x[3] =   0;  y[3] = 580;
    R.C.setColor( 0.0f, 0.1f, 0.9f );
    R.drawPolygon( 4, x, y );

    x[0] =   0;   y[0] = 580;
    x[1] =   0;  y[1] = 599;
    x[2] =  20;  y[2] = 599;
    x[3] =  20;  y[3] = 580;
    R.C.setColor( 0.0f, 0.2f, 0.8f );
    R.drawPolygon( 4, x, y );

    //  TRIANGLE TOP:TOP
    x[0] =  10;  y[0] = 590;
    x[1] =  10;  y[1] = 599;
    x[2] = 880;  y[2] = 599;
    R.C.setColor( 0.0f, 0.3f, 0.7f );
    R.drawPolygon( 3, x, y );

    //  TRIANGLE TOP:BOTTOM
    x[0] =  10;  y[0] = 590;
    x[1] = 880;  y[1] = 590;
    x[2] = 880;  y[2] = 599;
    R.C.setColor( 0.0f, 0.4f, 0.6f );
    R.drawPolygon( 3, x, y );

    // SQUARE TOP RIGHT
    x[0] = 899;  y[0] = 599;
    x[1] = 899;  y[1] = 580;
    x[2] = 880;  y[2] = 580;
    x[3] = 880;  y[3] = 599;
    R.C.setColor( 0.1f, 0.4f, 0.5f );
    R.drawPolygon( 4, x, y );

    //  TRIANGLE RIGHT:RIGHT
    x[0] = 890;  y[0] = 580;
    x[1] = 899;  y[1] = 580;
    x[2] = 890;  y[2] =  20;
    R.C.setColor( 0.2f, 0.4f, 0.4f );
    R.drawPolygon( 3, x, y );

    //  TRIANGLE RIGHT:LEFT
    x[0] = 899;  y[0] = 580;
    x[1] = 899;  y[1] =  20;
    x[2] = 890;  y[2] =  20;
    R.C.setColor( 0.3f, 0.4f, 0.3f );
    R.drawPolygon( 3, x, y );

    // SQUARE BOTTOM RIGHT
    x[0] = 899;  y[0] =   0;
    x[1] = 899;  y[1] =  20;
    x[2] = 880;  y[2] =  20;
    x[3] = 880;  y[3] =   0;
    R.C.setColor( 0.4f, 0.4f, 0.2f );
    R.drawPolygon( 4, x, y );

    // QUAD BOTTOM
    x[0] =  20;  y[0] =   0;
    x[1] =  20;  y[1] =  10;
    x[2] = 880;  y[2] =  10;
    x[3] = 880;  y[3] =   0;
    R.C.setColor( 0.4f, 0.5f, 0.1f );
    R.drawPolygon( 4, x, y );
}

///
// create a buffer
///
GLuint makeBuffer( GLenum target, const void *data, GLsizei size )
{
    GLuint buffer;

    glGenBuffers( 1, &buffer );
    glBindBuffer( target, buffer );
    glBufferData( target, size, data, GL_STATIC_DRAW );

    return( buffer );
}

//
// create the shapes and fill all the buffers
//
void createImage( Rasterizer &R )
{
    // draw all our polygons
    makePolygons( R );

    // get the vertices
    numElements = R.C.nVertices();
    float *points = R.C.getVertices();
    // #bytes = number of elements * 4 floats/element * bytes/float
    vdataSize = numElements * 4 * sizeof(float);

    // get the element data
    GLuint *elements = R.C.getElements();
    // #bytes = number of elements * bytes/element
    edataSize = numElements * sizeof(GLuint);

    // get the color data
    float *colors = R.C.getColors();
    // #bytes = number of elements * 4 floats/element * bytes/float
    cdataSize = numElements * 4 * sizeof(float);

    // set up the vertex buffer
    if( bufferInit ) {
        // must delete the existing buffers first
        glDeleteBuffers( 1, &vbuffer );
        glDeleteBuffers( 1, &ebuffer );
        bufferInit = false;
    }

    // first, create the connectivity data
    ebuffer = makeBuffer( GL_ELEMENT_ARRAY_BUFFER, elements, edataSize );

    // next, the vertex buffer, containing vertices and color data
    vbuffer = makeBuffer( GL_ARRAY_BUFFER, NULL, vdataSize + cdataSize );
    glBufferSubData( GL_ARRAY_BUFFER, 0, vdataSize, points );
    glBufferSubData( GL_ARRAY_BUFFER, vdataSize, cdataSize, colors );

    // NOTE:  'points', 'elements', and 'colors are dynamically allocated,
    // but we don't free them here because they will be freed at the next
    // call to clearImage() or the get*() functions

    // note that we've already created buffers once
    bufferInit = true;
}

///
// Verify shader creation
///
void checkShaderError( GLuint program, const char *which )
{
    if( program == 0 ) {
        cerr << "Error setting up " << which << " shader - "
            << errorString(shaderErrorCode) << endl;
        exit(1);
    }
}

///
// OpenGL initialization
///
void init( void )
{
    // Create our Canvas and Rasterizer "objects"
    Canvas *C = new Canvas( w_width, w_height );
    Rasterizer *R = new Rasterizer( w_height, *C );

    if( C == NULL ) {
        cerr << "error - cannot create Canvas" << endl;
        exit( 1 );
    }

    if( R == NULL ) {
        cerr << "error - cannot create Rasterizer" << endl;
        exit( 1 );
    }

    // Load shaders and use the resulting shader program
    program = shaderSetup( "shader.vert", "shader.frag" );
    checkShaderError( program, "basic" );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );

    // create the geometry for our shapes.
    createImage( *R );

    // rememeber where the Rasterizer is so we can clean it up
    rasterizer = R;
}

extern "C" {

void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // ensure we have selected the correct shader program
    glUseProgram( program );

    // bind our vertex buffer
    glBindBuffer( GL_ARRAY_BUFFER, vbuffer );

    // bind our element array buffer
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // set up our attribute variables
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(vdataSize) );

    // set up our uniforms
    GLuint wh = glGetUniformLocation( program, "wh" );
    glUniform2f( wh, (float) w_width, (float) w_height );

    // draw our shape
    glDrawElements( GL_POINTS, numElements,  GL_UNSIGNED_INT, (void *)0 );

    // swap the buffers
    glutSwapBuffers();
}

void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 033:  // Escape key
        case 'q': case 'Q':
            exit( 0 );
            break;
    }

    glutPostRedisplay();
}

} // extern "C"

///
// main program for polygon fill assignment
///
int main( int argc, char *argv[] )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( w_width, w_height );
    glutCreateWindow( "Lab 2 - Polygon Fill" );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        cerr << "GLEW error: " << glewGetErrorString(err) << endl;
        exit( 1 );
    }
    if( ! GL_VERSION_3_2 ) {
        cerr << "GLEW: OpenGL 3.2 not supported!" << endl;
        exit( 1 );
    }
#endif

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
