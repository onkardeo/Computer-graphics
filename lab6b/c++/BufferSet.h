//
//  BufferSet.h
//
//  Representation of the collection of buffers for an object.
//
//  Author:  Warren R. Carithers
//  Date:    2016/10/11 12:34:54
//

#ifndef _BUFFERSET_H_
#define _BUFFERSET_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif

#ifndef __cplusplus
#include <stdbool.h>
#endif

///
// All the relevant information needed to keep
// track of vertex and element buffers
///

typedef struct BufSet {
    GLuint vbuffer, ebuffer;          // buffer handles
    int numElements;                  // total number of vertices
    long vSize, nSize, tSize, eSize;  // component sizes (bytes)
    bool bufferInit;                  // have these already been set up?
} BufferSet;

#endif
