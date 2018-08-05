    //
// viewParams.java
//
// Created by Joe Geigel on 1/23/13.
//
// Contributor:  MANDAR BADAVE
//
// 20155
//

import javax.media.opengl.*;
import javax.media.opengl.fixedfunc.*;

public class viewParams
{

    ///
    // constructor
    ///
    public viewParams()
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
    //
    // @param gl3 - GL3 object on which all OpenGL calls are to be made
    //
    ///
    public void setUpFrustum (int program, GL3 gl3)
    {
        // Add your code here.
    	// Send all the appropriate parameters to the vertex shader to use a frustum projection.
    	// This includes parameters involved in the model and view transforms.
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "l"), -1);                        a
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "r"), 1);                         b
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "t"), 1);                         c
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "b"), -1);                        d
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "n"), (float) 0.9);               e
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "f"), (float) 4.5);               f
    	gl3.glUniform1i(gl3.glGetUniformLocation(program, "proj_type"), 1);
    }

    ///
    // This function sets up the view and projection parameter for an
    // orthographic projection of the scene. See the assignment description
    // for the values for the projection parameters.
    //
    // You will need to write this function, and maintain all of the values
    // needed to be sent to the vertex shader.
    //
    // @param program - The ID of an OpenGL (GLSL) shader program to which
    //    parameter values are to be sent
    //
    // @param gl3 - GL3 object on which all OpenGL calls are to be made
    //
    ///
    public void setUpOrtho (int program, GL3 gl3)
    {
        // Add your code here.
    	// Send all the appropriate parameters to the vertex shader to use a orthographic projection.
    	// This includes parameters involved in the model and view transforms.
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "l"), -1);
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "r"), 1);
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "t"), 1);
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "b"), -1);
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "n"), (float) 0.9);
    	gl3.glUniform1f(gl3.glGetUniformLocation(program, "f"), (float) 4.5);
    	gl3.glUniform1i(gl3.glGetUniformLocation(program, "proj_type"), 2);
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
        // Add your code here.
    	float[] S = {1, 1, 1};
    	float[] R = {0, 0, 0};
    	float[] T = {0, 0, 0};
    	// gets the location of the uniform variable and assigns it the above array
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "S"), 1, S, 0);
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "R"), 1, R, 0);
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "T"), 1, T, 0);
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
    // @param scaleX - amount of scaling along the x-axis
    // @param scaleY - amount of scaling along the y-axis
    // @param scaleZ - amount of scaling along the z-axis
    // @param rotateX - angle of rotation around the x-axis, in degrees
    // @param rotateY - angle of rotation around the y-axis, in degrees
    // @param rotateZ - angle of rotation around the z-axis, in degrees
    // @param translateX - amount of translation along the x axis
    // @param translateY - amount of translation along the y axis
    // @param translateZ - amount of translation along the z axis
    ///
    public void setUpTransforms( int program, GL3 gl3,
        float scaleX, float scaleY, float scaleZ,
        float rotateX, float rotateY, float rotateZ,
        float translateX, float translateY, float translateZ )
    {
        // Add your code here.
    	float[] S = {scaleX, scaleY, scaleZ};
    	float[] R = {rotateX, rotateY, rotateZ};
    	float[] T = {translateX, translateY, translateZ};
    	// gets the location of the uniform variable and assigns it the above array
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "S"), 1, S, 0);
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "R"), 1, R, 0);
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "T"), 1, T, 0);
    }


    ///
    // This function clears any changes made to camera parameters, setting the
    // values to the defaults: eye (0.0,0.0,0.0), lookat (0,0,0.0,-1.0),
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
        // Add your code here.
    	float[] E = {0, 0, 0};
    	float[] L = {0, 0, -1};
    	float[] U = {0, 1, 0};
    	// gets the location of the uniform variable and assigns it the above array
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "E"), 1, E, 0);
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "L"), 1, L, 0);
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "U"), 1, U, 0);
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
    // @param eyeX - x coordinate of the camera location
    // @param eyeY - y coordinate of the camera location
    // @param eyeZ - z coordinate of the camera location
    // @param lookatX - x coordinate of the lookat point
    // @param lookatY - y coordinate of the lookat point
    // @param lookatZ - z coordinate of the lookat point
    // @param upX - x coordinate of the up vector
    // @param upY - y coordinate of the up vector
    // @param upZ - z coordinate of the up vector
    ///
    void setUpCamera( int program, GL3 gl3,
        float eyeX, float eyeY, float eyeZ,
        float lookatX, float lookatY, float lookatZ,
        float upX, float upY, float upZ )
    {
        // Add your code here.
    	float[] E = {eyeX, eyeY, eyeZ};
    	float[] L = {lookatX, lookatY, lookatZ};
    	float[] U = {upX, upY, upZ};
    	// gets the location of the uniform variable and assigns it the above array
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "E"), 1, E, 0);
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "L"), 1, L, 0);
    	gl3.glUniform3fv(gl3.glGetUniformLocation(program, "U"), 1, U, 0);
    }

}
