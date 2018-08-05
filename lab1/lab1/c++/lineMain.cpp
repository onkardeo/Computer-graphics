//
//  lineMain.cpp
//
//  Main program for line drawing assignment
//
//  Students should not be modifying this file.
//

#include <cstdlib>
#include <iostream>
#include <iomanip>

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

#define	W_WIDTH		600
#define	W_HEIGHT	600

#define BUFFER_OFFSET(i)	((char *)NULL + (i))

// Our Rasterizer
Rasterizer *rasterizer;

// buffer information
bool bufferInit;
GLuint vbuffer, ebuffer;
GLuint program;
int numElements;
int vdataSize, edataSize, cdataSize;

//
// draw all the lines
//
// Idea for lettering style from:
// http://patorjk.com/software/taag/#p=display&f=Star%20Wars&t=Type%20Something
//          _______   ______   
//         /  ____|  /  __  \
//        |  |  __  |  |  |  | 
//        |  | |_ | |  |  |  | 
//        |  |__| | |  `--'  | 
//         \______|  \______/
//
void makeLines( Rasterizer &R )
{
    // start with a clean canvas
    R.C.clear();

    // ######## The letter 'G' in green ########

    R.C.setColor( 0.0, 1.0, 0.0 );

    R.drawLine(  80, 340, 220, 340 );   // Horizontal left to right 
    R.drawLine(  40, 380,  80, 340 );   // 315 degree slope        
    R.drawLine( 220, 340, 260, 380 );  // 45 degree slope          
    R.drawLine( 260, 380, 260, 440 );  // Vertical bottom to top
    R.drawLine( 260, 440, 180, 440 );  // Horizontal right to left
    R.drawLine( 180, 440, 180, 400 );
    R.drawLine( 180, 400, 220, 400 );
    R.drawLine( 220, 400, 200, 380 );
    R.drawLine( 200, 380, 100, 380 );
    R.drawLine( 100, 380,  80, 400 );
    R.drawLine(  80, 400,  80, 500 );
    R.drawLine(  80, 500, 100, 520 );
    R.drawLine( 100, 520, 200, 520 );
    R.drawLine( 200, 520, 220, 500 );
    R.drawLine( 220, 500, 220, 480 );
    R.drawLine( 220, 480, 260, 480 );
    R.drawLine( 260, 480, 260, 520 );
    R.drawLine( 260, 520, 220, 560 );  // 135 degree slope
    R.drawLine( 220, 560,  80, 560 );
    R.drawLine(  80, 560,  40, 520 );  // 225 degree slope
    R.drawLine(  40, 520,  40, 380 );  // Vertical top to bottom

    // ######## The letter 'O' in red ########

    R.C.setColor( 1.0, 0.0, 0.0 );

    R.drawLine( 450, 320, 520, 340 );  // 16.6 degree slope
    R.drawLine( 520, 340, 540, 360 );  // 45 degree slope
    R.drawLine( 540, 360, 560, 450 );  // 77.47 degree slope
    R.drawLine( 560, 450, 540, 540 );  // 102.83 degree slope
    R.drawLine( 540, 540, 520, 560 );  // 135 degree slope
    R.drawLine( 520, 560, 450, 580 );  // 163.3 degree slope
    R.drawLine( 450, 580, 380, 560 );  // 196.71 degree slope
    R.drawLine( 380, 560, 360, 540 );  // 225 degree slope
    R.drawLine( 360, 540, 340, 450 );  
    R.drawLine( 340, 450, 360, 360 );
    R.drawLine( 360, 360, 380, 340 );
    R.drawLine( 380, 340, 450, 320 );
    R.drawLine( 420, 380, 480, 380 );
    R.drawLine( 480, 380, 520, 420 );
    R.drawLine( 520, 420, 520, 480 );
    R.drawLine( 520, 480, 480, 520 );
    R.drawLine( 480, 520, 420, 520 );
    R.drawLine( 420, 520, 380, 480 );
    R.drawLine( 380, 480, 380, 420 );
    R.drawLine( 380, 420, 420, 380 );

    // now, draw the student's initials

    R.myInitials();

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
    // draw all our lines
    makeLines( R );

    // get the vertices
    numElements = R.C.nVertices();
    float *points = R.C.getVertices();
    vdataSize = numElements * 4 * sizeof(float);

    // get the element data
    GLushort *elements = R.C.getElements();
    edataSize = numElements * sizeof(GLushort);

    // get the color data
    float *colors = R.C.getColors();
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


// OpenGL initialization
void init( void )
{
    // Create our Canvas and Rasterizer "objects"
    Canvas *C = new Canvas( W_WIDTH, W_HEIGHT );
    rasterizer = new Rasterizer( W_HEIGHT, *C );

    // Load shaders and use the resulting shader program
    program = shaderSetup( "shader.vert", "shader.frag" );
    if (!program) {
        cerr << "Error setting up shaders - "
            << errorString(shaderErrorCode) << endl;
        exit(1);
    }

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );

    // create the geometry for our shapes.
    createImage( *rasterizer );
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
    glUniform2f( wh, (float) W_WIDTH, (float) W_HEIGHT );

    // draw our shape
    // glDrawElements( GL_POINTS, numElements,  GL_UNSIGNED_SHORT, (void *)0 );
    glDrawArrays( GL_POINTS, 0, numElements );

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

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( W_HEIGHT, W_WIDTH );
    glutCreateWindow( "Lab 1 - Line Drawing" );

#ifndef __APPLE__
    GLenum err = glewInit();
    if( err != GLEW_OK ) {
        cerr << "GLEW error: "
             << glewGetErrorString(err) << endl;
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
