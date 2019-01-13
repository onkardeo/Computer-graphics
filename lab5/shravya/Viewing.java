//
//  Viewing.java
//
//  Created by Warren R. Carithers 2016/11/11.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Simple class for setting up the viewing and projection transforms
//  for the Transformation Assignment.
//
//  Contributor:  Onkar Deorukhkar
//

import javax.media.opengl.*;
import javax.media.opengl.fixedfunc.*;

public class Viewing
{

    ///
    // constructor
    ///
    public Viewing()
    {
    }


    ///
    // This function sets up the view and projection parameter for a frustum
    // projection of the scene. See the assignment description for the values
    // for the projection parameters.
    //
    // You will need to write this function, and maintain all of the values
    // needed to be sent to the vertex shader.
    //
    // @param program - The ID of an OpenGL (GLSL) shader program to which
    //    parameter values are to be sent
    // @param gl3 - GL3 object on which all OpenGL calls are to be made
    ///
    public void setUpFrustum( int program, GL3 gl3 )
    {
        // Add your code here.
    	//int id = gl3.glGetAttribLocation( program, "projType");
        //gl3.glVertexAttrib1f( id, 0.0f );
        
    	int id = gl3.glGetAttribLocation( program, "projType");
    	int id1 = gl3.glGetUniformLocation( program, "left");
    	int id2 = gl3.glGetUniformLocation( program, "right");
    	int id3 = gl3.glGetUniformLocation( program, "top");
    	int id4 = gl3.glGetUniformLocation( program, "bottom");
    	int id5 = gl3.glGetUniformLocation( program, "far");
    	int id6 = gl3.glGetUniformLocation( program, "near");
     	gl3.glUniform1f(id1, -1);
     	gl3.glUniform1f(id2, 1);
     	gl3.glUniform1f(id3, 1);
     	gl3.glUniform1f(id4, -1);
     	
     	gl3.glUniform1f(id5, (float) 4.5);
     	gl3.glUniform1f(id6, (float) 0.9);
     	gl3.glUniform1i(id, 1);
    }


    ///
     //This function sets up the view and projection parameter for an
    // orthographic projection of the scene. See the assignment description
    // for the values for the projection parameters.
    //
    // You will need to write this function, and maintain all of the values
    // needed to be sent to the vertex shader.
    //
    // @param program - The ID of an OpenGL (GLSL) shader program to which
    //    parameter values are to be sent
    // @param gl3 - GL3 object on which all OpenGL calls are to be made
    ///
    public void setUpOrtho( int program, GL3 gl3 )
    {
        // Add your code here.
    	int id = gl3.glGetAttribLocation( program, "projType");
    	int id1 = gl3.glGetUniformLocation( program, "left");
    	int id2 = gl3.glGetUniformLocation( program, "right");
    	int id3 = gl3.glGetUniformLocation( program, "top");
    	int id4 = gl3.glGetUniformLocation( program, "bottom");
    	int id5 = gl3.glGetUniformLocation( program, "far");
    	int id6 = gl3.glGetUniformLocation( program, "near");
     	gl3.glUniform1f(id1, -1);
     	gl3.glUniform1f(id2, 1);
     	gl3.glUniform1f(id3, 1);
     	gl3.glUniform1f(id4, -1);
     	
     	gl3.glUniform1f(id5, (float) 4.5);
     	gl3.glUniform1f(id6, (float) 0.9);
     	gl3.glUniform1i(id, 2);
    }


    ///
    // This function clears any transformations, setting the values to the
    // defaults: no scaling (scale factor of 1), no rotation (degree of
    // rotation = 0), and no translation (0 translation in each direction).
    //
    // You will need to write this function, and maintain all of the values
    // which must be sent to the vertex shader.
    //
    // @param program - The ID of an OpenGL (GLSL) shader program to which
    //    parameter values are to be sent
    // @param gl3 - GL3 object on which all OpenGL calls are to be made
    ///
    public void clearTransforms( int program, GL3 gl3 )
    {
    	
    	float[] sc = { 1, 1, 1 };
    	float[] ro = { 0, 0, 0 };
    	float[] tr = { 0, 0, 0 };
    	
    	int id1 = gl3.glGetUniformLocation( program, "sc");
    	int id2 = gl3.glGetUniformLocation( program, "ro");
    	int id3 = gl3.glGetUniformLocation( program, "tr");
    	gl3.glUniform3fv( id1, 1,sc,0 );
    	gl3.glUniform3fv( id2, 1,ro,0 );
    	gl3.glUniform3fv( id3, 1,tr,0 );
    	
    	
    	// send all values of transformation to shader
    		
    	
    }

    ///
    // This function sets up the transformation parameters for the vertices
    // of the teapot.  The order of application is specified in the driver
    // program.
    //
    // You will need to write this function, and maintain all of the values
    // which must be sent to the vertex shader.
    //
    // @param program - The ID of an OpenGL (GLSL) shader program to which
    //    parameter values are to be sent
    // @param gl3 - GL3 object on which all OpenGL calls are to be made
    // @param xScale - amount of scaling along the x-axis
    // @param yScale - amount of scaling along the y-axis
    // @param zScale - amount of scaling along the z-axis
    // @param xRotate - angle of rotation around the x-axis, in degrees
    // @param yRotate - angle of rotation around the y-axis, in degrees
    // @param zRotate - angle of rotation around the z-axis, in degrees
    // @param xTranslate - amount of translation along the x axis
    // @param yTranslate - amount of translation along the y axis
    // @param zTranslate - amount of translation along the z axis
    ///
    public void setUpTransforms( int program, GL3 gl3,
        float xScale, float yScale, float zScale,
        float xRotate, float yRotate, float zRotate,
        float xTranslate, float yTranslate, float zTranslate )
    {
    	float[] sc = { xScale, yScale, zScale };
    	float[] ro = { xRotate, yRotate, zRotate };
    	float[] tr = { xTranslate, yTranslate, zTranslate };
    	
    	int id1 = gl3.glGetUniformLocation( program, "sc");
    	int id2 = gl3.glGetUniformLocation( program, "ro");
    	int id3 = gl3.glGetUniformLocation( program, "tr");
    	gl3.glUniform3fv( id1, 1,sc,0 );
    	gl3.glUniform3fv( id2, 1,ro,0 );
    	gl3.glUniform3fv( id3, 1,tr,0 );
    	
    		// send all values of transformation to shader
    		//glUniform3fv(glGetUniformLocation(program, "sc"), 1, scale);
    		//glUniform3fv(glGetUniformLocation(program, "ro"), 1, rotate);
    		//glUniform3fv(glGetUniformLocation(program, "tr"), 1, translate);
        // Add your code here.
    }


    ///
    // This function clears any changes made to camera parameters, setting the
    // values to the defaults: eyepoint (0.0,0.0,0.0), lookat (0,0,0.0,-1.0),
    // and up vector (0.0,1.0,0.0).
    //
    // You will need to write this function, and maintain all of the values
    // which must be sent to the vertex shader.
    //
    // @param program - The ID of an OpenGL (GLSL) shader program to which
    //    parameter values are to be sent
    // @param gl3 - GL3 object on which all OpenGL calls are to be made
    ///
    void clearCamera( int program, GL3 gl3 )
    {
    	
    	float[] ep = { 0, 0, 0 };
    	float[] lo = { 0, 0, -1 };
    	float[] up = { 0, 1, 0 };
    	
    	int id1 = gl3.glGetUniformLocation( program, "ep");
    	int id2 = gl3.glGetUniformLocation( program, "lo");
    	int id3 = gl3.glGetUniformLocation( program, "up");
    	gl3.glUniform3fv( id1, 1, ep, 0 );
    	gl3.glUniform3fv( id2, 1, lo, 0 );
    	gl3.glUniform3fv( id3, 1, up, 0 );
    	
    	//glUniform3fv(glGetUniformLocation(program, "ey"), 1, eye);
    	//glUniform3fv(glGetUniformLocation(program, "la"), 1, lookat);
    	//glUniform3fv(glGetUniformLocation(program, "up"), 1, up);
        // Add your code here.
    }


    ///
    // This function sets up the camera parameters controlling the viewing
    // transformation.
    //
    // You will need to write this function, and maintain all of the values
    // which must be sent to the vertex shader.
    //
    // @param program - The ID of an OpenGL (GLSL) shader program to which
    //    parameter values are to be sent
    // @param gl3 - GL3 object on which all OpenGL calls are to be made
    // @param eyepointX - x coordinate of the camera location
    // @param eyepointY - y coordinate of the camera location
    // @param eyepointZ - z coordinate of the camera location
    // @param lookatX - x coordinate of the lookat point
    // @param lookatY - y coordinate of the lookat point
    // @param lookatZ - z coordinate of the lookat point
    // @param upX - x coordinate of the up vector
    // @param upY - y coordinate of the up vector
    // @param upZ - z coordinate of the up vector
    ///
    void setUpCamera( int program, GL3 gl3,
        float eyepointX, float eyepointY, float eyepointZ,
        float lookatX, float lookatY, float lookatZ,
        float upX, float upY, float upZ )
    {
    	
    	float[] ep = { eyepointX, eyepointY, eyepointZ };
    	float[] lo = { lookatX, lookatY, lookatZ };
    	float[] up = { upX, upY, upZ };

    	// send all values of transformation to shader
    	int id1= gl3.glGetUniformLocation(program, "ep");
    	int id2= gl3.glGetUniformLocation(program, "lo");
    	int id3= gl3.glGetUniformLocation(program, "up");
    	gl3.glUniform3fv(id1,1,ep,0);
    	gl3.glUniform3fv(id2,1,lo,0);
    	gl3.glUniform3fv(id3,1,up,0);
    		
        
    }

}
