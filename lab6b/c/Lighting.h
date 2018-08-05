//
//  Lighting.h
//
//  Created by Warren R. Carithers 2016/11/22.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2016 Rochester Institute of Technology.  All rights reserved.
//
//  Simple class for setting up Phong illumination.
//
//  This file should not be modified by students.
//

#ifndef _LIGHTING_H_
#define _LIGHTING_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif

void setUpPhong( GLuint program );

#endif
