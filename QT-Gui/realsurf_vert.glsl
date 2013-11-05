uniform mat4 surface_transform_inverse;

varying vec3 eye_space_eye;
varying vec3 eye_space_dir;
varying vec3 clipping_space_eye;
varying vec3 clipping_space_dir;
varying vec3 surface_space_eye;
varying vec3 surface_space_dir;

void main( void )
{
	// calculate raster position by standard opengl procedure
	gl_Position = ftransform();
	
	// calculate ray in different coordinate systems
	eye_space_eye = ( gl_ModelViewMatrix * gl_Vertex ).xyz;
	eye_space_dir = ( eye_space_eye );
	
	clipping_space_eye = gl_Vertex.xyz;
	clipping_space_dir = ( clipping_space_eye - ( gl_ModelViewMatrixInverse * vec4( 0.0, 0.0, 0.0, 1.0 ) ).xyz );
	
	surface_space_eye = ( surface_transform_inverse * gl_Vertex ).xyz;
	surface_space_dir = ( surface_space_eye - ( surface_transform_inverse * gl_ModelViewMatrixInverse * vec4( 0.0, 0.0, 0.0, 1.0 ) ).xyz );
}