//
//  Textures.h
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Simple class for setting up texture mapping parameters.
//  
//  This file should not be modified by students.
//

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif

void loadTexture( void );
void setUpTexture( GLuint program );

#endif 
