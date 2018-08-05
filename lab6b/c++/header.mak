#
# This header.mak file will set up all necessary options for compiling
# and linking C and C++ programs which use OpenGL, GLUT, GLEW, and/or
# SOIL on the Ubuntu systems.
#
# If you want to take advantage of GDB's extra debugging features,
# change "-g" in the CFLAGS and LIBFLAGS macro definitions to "-ggdb".
#
INCLUDE = -I/usr/include/SOIL
LIBDIRS =

# common linker options
LDLIBS = -lSOIL -lglut -lGL -lm -lGLEW

# language-specific linker options
CLDLIBS = -lgsl -lgslcblas
CCLDLIBS =

CFLAGS = -std=c99 -g $(INCLUDE) -DGL_GLEXT_PROTOTYPES
CCFLAGS = $(CFLAGS)
CXXFLAGS = -g $(INCLUDE) -DGL_GLEXT_PROTOTYPES

LIBFLAGS = -g $(LIBDIRS) $(LIBDIRS) $(LDLIBS)
CLIBFLAGS = $(LIBFLAGS) $(CLDLIBS)
CCLIBFLAGS = $(LIBFLAGS) $(CCLDLIBS)
