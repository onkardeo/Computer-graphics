#version 150

// Phong vertex shader
//
// Contributor:  Onkar Deorukhkar

// INCOMING DATA

// Vertex location (in model space)
in vec4 vPosition;

// Normal vector at vertex (in model space)
in vec3 vNormal;

// Model transformations
uniform vec3 theta;
uniform vec3 trans;
uniform vec3 scale;

// Camera parameters
uniform vec3 cPosition;
uniform vec3 cLookAt;
uniform vec3 cUp;

// View volume boundaries
uniform float left;
uniform float right;
uniform float top;
uniform float bottom;
uniform float near;
uniform float far;

// Add all necessary variables for doing the
// vertex shader portion of lighting and shading here
uniform vec4 light_Source_Position;

// OUTGOING DATA
varying vec3 L;
varying vec3 N;
varying vec3 V;

// Add all necessary variables for communicating with
// the fragment shader here

void main()
{
    // Compute the sines and cosines of each rotation about each axis
    vec3 angles = radians( theta );
    vec3 c = cos( angles );
    vec3 s = sin( angles );

    // Create rotation matrices
    mat4 rxMat = mat4( 1.0,  0.0,  0.0,  0.0,
                       0.0,  c.x,  s.x,  0.0,
                       0.0,  -s.x, c.x,  0.0,
                       0.0,  0.0,  0.0,  1.0 );

    mat4 ryMat = mat4( c.y,  0.0,  -s.y, 0.0,
                       0.0,  1.0,  0.0,  0.0,
                       s.y,  0.0,  c.y,  0.0,
                       0.0,  0.0,  0.0,  1.0 );

    mat4 rzMat = mat4( c.z,  s.z,  0.0,  0.0,
                       -s.z, c.z,  0.0,  0.0,
                       0.0,  0.0,  1.0,  0.0,
                       0.0,  0.0,  0.0,  1.0 );

    mat4 xlateMat = mat4( 1.0,     0.0,     0.0,     0.0,
                          0.0,     1.0,     0.0,     0.0,
                          0.0,     0.0,     1.0,     0.0,
                          trans.x, trans.y, trans.z, 1.0 );

    mat4 scaleMat = mat4( scale.x,  0.0,     0.0,     0.0,
                          0.0,      scale.y, 0.0,     0.0,
                          0.0,      0.0,     scale.z, 0.0,
                          0.0,      0.0,     0.0,     1.0 );

    // Create view matrix
    vec3 nVec = normalize( cPosition - cLookAt );
    vec3 uVec = normalize( cross (normalize(cUp), nVec) );
    vec3 vVec = normalize( cross (nVec, uVec) );

    mat4 viewMat = mat4( uVec.x, vVec.x, nVec.x, 0.0,
                         uVec.y, vVec.y, nVec.y, 0.0,
                         uVec.z, vVec.z, nVec.z, 0.0,
                         -1.0*(dot(uVec, cPosition)),
                         -1.0*(dot(vVec, cPosition)),
                         -1.0*(dot(nVec, cPosition)), 1.0 );

    // Create projection matrix
    mat4 projMat = mat4( (2.0*near)/(right-left), 0.0, 0.0, 0.0,
                         0.0, ((2.0*near)/(top-bottom)), 0.0, 0.0,
                         ((right+left)/(right-left)),
                         ((top+bottom)/(top-bottom)),
                         ((-1.0*(far+near)) / (far-near)), -1.0,
                         0.0, 0.0, ((-2.0*far*near)/(far-near)), 0.0 );

    // Transformation order:
    //    scale, rotate Z, rotate Y, rotate X, translate
    mat4 modelMat = xlateMat * rxMat * ryMat * rzMat * scaleMat;
    mat4 modelViewMat = viewMat * modelMat;

    // Transform the vertex location into clip space
    gl_Position =  projMat * viewMat  * modelMat * vPosition;

    // Add all code here to do the lighting-related calculations
    // that must be done in the vertex shader
    vec4 L_Vec, V_Vec, N_Vec;
    
    L_Vec = viewMat * light_Source_Position;
    V_Vec = modelViewMat * vPosition;
    N_Vec = normalize(modelViewMat * vec4(vNormal, 0.0));
    
    N = normalize(N_Vec.xyz);
    L = normalize(L_Vec.xyz - V_Vec.xyz);
    V = normalize(V_Vec.xyz);
}
