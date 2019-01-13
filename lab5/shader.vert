//
// Vertex shader for the transformation assignment
//
// Author:  W. R. Carithers
//
// Contributor:  Onkar Deorukhkar

#version 150

// attribute:  vertex position
in vec4 vPosition;

// add other global shader variables to hold the
// parameters your application code is providing
uniform vec3 E;
uniform vec3 L;
uniform vec3 U;
uniform vec3 S;
uniform vec3 R;
uniform vec3 T;
uniform int proj_type;
uniform float l, r, t, b, n, f;

// function to set the scaling matrix
void scale(out mat4 scaled){
	scaled = mat4(S.x, 0, 0, 0,    0, S.y, 0, 0,     0, 0, S.z, 0,     0, 0, 0, 1);
}

// function to set the rotation matrix
void rotate(out mat4 rotated){
	// referred from Lab 4 shader.vert
	vec3 angles = radians( R );
    vec3 c = cos( angles );
    vec3 s = sin( angles );

    mat4 rotate_x = mat4 ( 1.0,  0.0,  0.0,  0.0,
                     	   0.0,  c.x,  s.x,  0.0,
                     	   0.0, -s.x,  c.x,  0.0,
                           0.0,  0.0,  0.0,  1.0 );

    mat4 rotate_y = mat4 ( c.y,  0.0, -s.y,  0.0,
                           0.0,  1.0,  0.0,  0.0,
                           s.y,  0.0,  c.y,  0.0,
                           0.0,  0.0,  0.0,  1.0 );

    mat4 rotate_z = mat4 ( c.z,  s.z,  0.0,  0.0,
                    	  -s.z,  c.z,  0.0,  0.0,
                     	   0.0,  0.0,  1.0,  0.0,
                     	   0.0,  0.0,  0.0,  1.0 );
                     
    rotated = rotate_z * rotate_y * rotate_x;
}

// function to set the translation matrix
void translate(out mat4 translated){
	translated = mat4(1, 0, 0, 0,    0, 1, 0, 0,    0, 0, 1, 0,     T.x, T.y, T.z, 1);
}

// matrix to convert world coordinates to camera to coordinates
void worldtocamera(out mat4 WC){
	vec3 n, u, v;
	n = normalize(E - L);
	u = normalize(cross(U, n));
	v = normalize(cross(n, u));
	
	WC = mat4(u.x, v.x, n.x, 0,   u.y, v.y, n.y, 0,   u.z, v.z, n.z, 0,  -dot(u, E), -dot(v, E), -dot(n, E), 1);
}

// function to set the orthographic projection matrix
void orthographic(out mat4 O){
	O = mat4( 2/(r-l), 0, 0, 0,    0, 2/(t-b), 0, 0,    0, 0, -2/(f-n), 0,   -((r+l)/(r-l)), -((t+b)/(t-b)), -((f+n)/(f-n)), 1);
}

// function to set the perspective projection matrix
void frustum(out mat4 F){
	F = mat4(((2*n)/(r-l)), 0, 0, 0,    0, ((2*n)/(t-b)), 0, 0,   ((r+l)/(r-l)), ((t+b)/(t-b)), -((f+n)/(f-n)), -1,    0, 0, ((-2*f*n)/(f-n)), 0);
}

void main()
{
    // By default, no transformations are performed.
    
    mat4 scaled, rotated, translated, WC, O, F;
    scale(scaled);
    rotate(rotated);
    translate(translated);
    worldtocamera(WC);
    
    // select perspective projection
    if(proj_type == 1){
    	frustum(F);
    	gl_Position =  F * WC * translated * rotated * scaled * vPosition;
    }
    
    // select orthographic projection
    else{
    	orthographic(O);
    	gl_Position =  O * WC * translated * rotated * scaled * vPosition;
    } 
}
