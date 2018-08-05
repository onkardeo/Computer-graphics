//
//  ShaderSetup
//
//  Set up a GLSL shader based on supplied source files.
//

#ifndef _SHADERSETUP_H_
#define _SHADERSETUP_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif

///
// Error codes returned by ShaderSetup()
///

#define	E_NO_ERROR	0
#define	E_VS_LOAD	1
#define	E_FS_LOAD	2
#define	E_VS_COMPILE	3
#define	E_FS_COMPILE	4
#define	E_SHADER_LINK	5

///
// shaderErrorCode
//
// Contains a code (see ShaderSetup.h) indicating the status
// of the most recent ShaderSetup() call
///

extern GLuint shaderErrorCode;

///
// read_text_file(name)
//
// Read the text file given as 'name'.
//
// @param name   the path to the file to be processed
//
// @return the contents of the file in a dynamically-allocated
//         string buffer, or NULL if an error occurs.
///

GLchar *read_text_file( const char *name );

///
// print_shader_info_log(shader)
//
// Print the information log from a shader compilation attempt
//
// @param shader   which shader object to check
///

void print_shader_info_log( GLuint shader );

///
// print_program_info_log(shader)
//
// Print a program information log
//
// This is identical to print_shader_info_log(), except that it uses
// glGetProgramiv() and glGetProgramInfoLog() instead of the *Shader*()
// versions.
//
// @param shader   which program object to check
///

void print_program_info_log( GLuint shader );

///
// errorString(code)
//
// @param code    the error code to interpret
//
// @return a const char* with a text version of the supplied error code.
///

const char *errorString( GLuint code );

///
// ShaderSetup(vertex,fragment)
//
// Set up a GLSL shader program.
//
// Requires the name of a vertex program and a fragment
// program.  Returns a handle to the created GLSL program
//
// @param vert   vertex shader program source file path
// @param frag   fragment shader program source file path
//
// @return the GLSL shader program handle, or 0 on error
//
// On success:
//    returns the GLSL shader program handle, and sets the global
//    shaderErrorCode to E_NO_ERROR.
//
// On failure:
//    returns 0, and shaderErrorCode contains an error code
///

GLuint ShaderSetup( const char *vert, const char *frag );

#endif
