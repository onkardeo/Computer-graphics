#
# This header.mak file will set up all necessary options for compiling
# and linking C and C++ programs which use OpenGL and/or GLUT on the
# Ubuntu systems along with GSL and GLEW
#
# To use it, place it in the same directory as your source code, and
# run the command
#
#	gmakemake > Makefile
#
# To compile and link your program, just run "make".
#
# If you want to take advantage of GDB's extra debugging features,
# change "-g" in the CFLAGS and LIBFLAGS macro definitions to "-ggdb".
#
INCLUDE =
LIBDIRS =

# common linker options
LDLIBS = -lglut -lGL -lm -lGLEW

# language-specific linker options
CLDLIBS = -lgsl -lgslcblas
CCLDLIBS = 

CFLAGS = -std=c99 -g $(INCLUDE) -DGL_GLEXT_PROTOTYPES
CCFLAGS =  $(CFLAGS)
CXXFLAGS = -g $(INCLUDE) -DGL_GLEXT_PROTOTYPES

LIBFLAGS = -g $(LIBDIRS) $(LDLIBS)
CLIBFLAGS = $(LIBFLAGS) $(CLDLIBS)
CCLIBFLAGS = $(LIBFLAGS) $(CCLDLIBS)
