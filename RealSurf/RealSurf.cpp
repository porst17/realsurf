#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cmath>
#include <ctime>
#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000.0
#endif

//#ifndef WIN32
//	#define WIN32
//#endif

#include <GL/GLee.h>
#include <GL/glut.h>

#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif 

#include <MathGL/GLMatrix.h>

#include "ShaderManager.h"

using namespace std;
using namespace glsl;

#define REAL_SURF_VER "0.1alpha"

/*******************************
 * BEGIN - global variables
 *******************************/
ostream *output = &cout;
GLboolean show_fps = GL_FALSE;
GLint frame_count = 0;
GLdouble fps = 0.0;
ShaderManager *sm;
GLMatrix<GLfloat> bbox_transform;
GLMatrix<GLfloat> surface_transform;
GLint last_x, last_y;
GLboolean show_box = GL_TRUE;
pair<string, string> predefined_surfaces[ 10 ] = {
													pair<string, string>( string( "Barth Decic" ), string( "8*(x^2-1.618033988^4*y^2)*(y^2-1.618033988^4*z^2)*(z^2-1.618033988^4*x^2)*(x^4+y^4+z^4-2*x^2*y^2-2*y^2*z^2-2*z^2*x^2)+(3+5*1.618033988)*(x^2+y^2+z^2-1)^2*(x^2+y^2+z^2-(2-1.618033988))^2" ) ),
													pair<string, string>( string( "Hyperboloid of one sheet" ), string( "x^2+y^2-z^2-1" ) ),
													pair<string, string>( string( "Ding-Dong" ), string( "x^2+y^2+z^3-z^2" ) ),
													pair<string, string>( string( "Cayley Cubic" ), string( "4*(x^3+y^3+z^3+0.5^3)-(x+y+z+0.5)^3" ) ),
													pair<string, string>( string( "Steiner Surface" ), string( "0.0081*x^2*y^2+0.0081*y^2*z^2+0.0081*z^2*x^2-0.027*x*y*z" ) ),
													pair<string, string>( string( "Kummer Surface" ), string( "-x^4-y^4-z^4+4*(x^2+y^2*z^2+y^2+x^2*z^2+z^2+y^2*x^2)-12*1.7320508*x*y*z-1" ) ),
													pair<string, string>( string( "Hollow Cube" ), string( "x^4+y^4+z^4-10*0.4*0.4*(x^2+y^2+z^2)+40.0*0.4^4" ) ),
													pair<string, string>( string( "Klein Bottle" ), string( "(x^2+y^2+z^2+2*y-1)*((x^2+y^2+z^2-2*y-1)^2-8*z^2)+16*x*z*(x^2+y^2+z^2-2*y-1)" ) ),
													pair<string, string>( string( "Heart" ), string( "(2*x^2+y^2+z^2-1)^3-0.1*x^2*z^3-y^2*z^3" ) ),
													pair<string, string>( string( "Barth Sextic" ), string( "4*(1.618033988^2*x^2-y^2)*(1.618033988^2*y^2-z^2)*(1.618033988^2*z^2-x^2)-(1+2*1.618033988)*(x^2+y^2+z^2-1)^2" ) )
												};
/*******************************
 * END - global variables
 *******************************/

void reset_to_defaults()
{
	bbox_transform = GLMatrix<GLfloat>::identity();
	surface_transform.loadScale( 0.25f, 0.25f, 0.25f );
	//surface_transform.loadIdentity();
}

void set_window_title()
{
	ostringstream window_title;
	window_title << "RealSurf " << REAL_SURF_VER;
	if( show_fps )
		window_title << " (" << setprecision( 1 ) << fixed << fps << "fps)";

	ostringstream icon_title;
	icon_title << "RealSurf";

	glutSetWindowTitle( window_title.str().c_str() );
	glutSetIconTitle( icon_title.str().c_str() );
}

GLboolean check_gl_extensions()
{
	GLboolean all_ok = GL_TRUE;
	for( int i = 0; i < 1; i++ )
	{
		GLboolean this_ok = GL_TRUE;
		*output << "Test for ";
		switch( i )
		{
			case 0:
				*output << "OpenGL 2.0";
				this_ok = GLEE_VERSION_2_0;
				break;
		}
		if( this_ok )
			*output << " succeeded" << endl;
		else
			*output << " failed" << endl;
		all_ok = all_ok && this_ok;
	}
	return all_ok;
}

void set_light_and_material()
{
	/*
	// polished Gold
	GLfloat front_emission_color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat front_ambient_color[ 4 ]  = { 0.25f, 0.22f, 0.06f, 1.0f };
	GLfloat front_diffuse_color[ 4 ]  = { 0.35f, 0.31f, 0.09f, 1.0f };
	GLfloat front_specular_color[ 4 ] = { 0.80f, 0.72f, 0.21f, 1.0f };
	GLfloat front_shininess = 83.2f;
	*/

	// Brass
	GLfloat front_emission_color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat front_ambient_color[ 4 ]  = { 0.329412f, 0.223529f, 0.027451f, 1.0f };
	GLfloat front_diffuse_color[ 4 ]  = { 0.780392f, 0.568627f, 0.113725f, 1.0f };
	GLfloat front_specular_color[ 4 ] = { 0.992157f, 0.941176f, 0.807843f, 1.0f };
	GLfloat front_shininess = 0.21794872f * 128.0f;

	glMaterialfv( GL_FRONT, GL_EMISSION, front_emission_color );
	glMaterialfv( GL_FRONT, GL_AMBIENT, front_ambient_color );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, front_diffuse_color );
	glMaterialfv( GL_FRONT, GL_SPECULAR, front_specular_color );
	glMaterialf( GL_FRONT, GL_SHININESS, front_shininess );

	/*
	// polished Silver
	GLfloat back_emission_color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat back_ambient_color[ 4 ]  = { 0.23f, 0.23f, 0.23f, 1.0f };
	GLfloat back_diffuse_color[ 4 ]  = { 0.28f, 0.28f, 0.28f, 1.0f };
	GLfloat back_specular_color[ 4 ] = { 0.77f, 0.77f, 0.77f, 1.0f };
	GLfloat back_shininess = 89.6f;
	*/

	// Ruby
	GLfloat back_emission_color[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat back_ambient_color[ 4 ]  = { 0.1745f, 0.01175f, 0.01175f, 1.0f };
	GLfloat back_diffuse_color[ 4 ]  = { 0.61424f, 0.04136f, 0.04136f, 1.0f };
	GLfloat back_specular_color[ 4 ] = {0.727811f, 0.626959f, 0.626959f, 1.0f };
	GLfloat back_shininess = 0.6f * 128.0f;

	glMaterialfv( GL_BACK, GL_EMISSION, back_emission_color );
	glMaterialfv( GL_BACK, GL_AMBIENT, back_ambient_color );
	glMaterialfv( GL_BACK, GL_DIFFUSE, back_diffuse_color );
	glMaterialfv( GL_BACK, GL_SPECULAR, back_specular_color );
	glMaterialf( GL_BACK, GL_SHININESS, back_shininess );

	GLfloat gl_light0_position[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat gl_light0_ambient[ 4 ]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat gl_light0_diffuse[ 4 ]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat gl_light0_specular[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat gl_light0_constant_attenuation = 1.0f;
	GLfloat gl_light0_linear_attenuation = 0.001f;
	GLfloat gl_light0_quadratic_attenuation = 0.004f;

	GLfloat gl_light0_spot_direection[ 3 ] = { -1.0f, -1.0f, -6.0f };
	GLfloat gl_light0_spot_cutoff = 180.0f;
	GLfloat gl_light0_spot_exponent = 100.0f;

	glLightfv( GL_LIGHT0, GL_POSITION, gl_light0_position );

	glLightfv( GL_LIGHT0, GL_AMBIENT, gl_light0_ambient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, gl_light0_diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, gl_light0_specular );

	glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, gl_light0_constant_attenuation );
	glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, gl_light0_linear_attenuation );
	glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, gl_light0_quadratic_attenuation );

    glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, gl_light0_spot_direection );
    glLightf( GL_LIGHT0, GL_SPOT_CUTOFF, gl_light0_spot_cutoff );
    glLightf( GL_LIGHT0, GL_SPOT_EXPONENT, gl_light0_spot_exponent );

	// disable other lights
	GLfloat gl_lights_ambient[ 4 ]  = { 1.0f, 1.0f, 1.0f, 0.0f };
	glLightfv( GL_LIGHT1, GL_AMBIENT, gl_lights_ambient );
	glLightfv( GL_LIGHT2, GL_AMBIENT, gl_lights_ambient );
	glLightfv( GL_LIGHT3, GL_AMBIENT, gl_lights_ambient );
	glLightfv( GL_LIGHT4, GL_AMBIENT, gl_lights_ambient );
	glLightfv( GL_LIGHT5, GL_AMBIENT, gl_lights_ambient );
	glLightfv( GL_LIGHT6, GL_AMBIENT, gl_lights_ambient );
	glLightfv( GL_LIGHT7, GL_AMBIENT, gl_lights_ambient );
}

void glut_display_window()
{
	// reset modelview transformation
    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// set lights and materials, but ...
	set_light_and_material();

	// ... disable lighting for fixed function pipeline
    glDisable( GL_LIGHTING );

	// enable alpha blending
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );

	// enable backface culling
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	// clear depth buffer
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );

	// move clipping object to desired position
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glTranslated( 0.0, 0.0, -7.5 );

	// apply local transformation (i.e. rotation)
	bbox_transform.glMultMatrix();

	// apply transformation to surface
	if( sm->get_clipping_surface() == ShaderManager::CUBE )
		sm->set_surface_transformation( surface_transform * GLMatrix<GLfloat>().loadScale( 1.73205f, 1.73205f, 1.73205f ) );
	else
		sm->set_surface_transformation( surface_transform );

	// scale surrounding box
	glScaled( 3.55, 3.55, 3.55 );
	if( sm->get_clipping_surface() == ShaderManager::CUBE )		
		glScaled( 1.0 / 1.73205, 1.0 / 1.73205, 1.0 / 1.73205 );

	// draw object background
	sm->enable_fixed_function_pipeline();

	// draw wireframe box
	if( show_box )
	{
		glPushAttrib( GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT );
		glDepthFunc( GL_LESS );
		glPolygonMode( GL_BACK, GL_LINE );
		glCullFace( GL_FRONT );
		glColor4d( 0.5, 0.5, 0.5, 0.5 );
		if( sm->get_clipping_surface() == ShaderManager::CUBE )
			glutSolidCube( 2.0 );
		else
			glutSolidSphere( 1.0, 50, 50 );
		glPopAttrib();
	}

	// start raytracing
	sm->enable_glsl();

	// raytrace in surrounding box ( size: 2x2x2 )
	glDepthFunc( GL_LEQUAL );
	glutSolidCube( 2.0 );

	// reset shading
	sm->enable_fixed_function_pipeline();

	// reset transformation
	glPopMatrix();

	// rendering complete, swap front and back buffer
	glutSwapBuffers();

	frame_count++;
	set_window_title();

	if( show_fps )
		glutPostRedisplay();
}

void glut_timer( GLint value )
{
	static clock_t last_clock = 0;
	clock_t new_clock = clock();

	fps = double( frame_count ) / ( double( new_clock - last_clock ) / CLOCKS_PER_SEC );
	frame_count = 0;
	glutTimerFunc( 2000, glut_timer, 0 );

	last_clock = new_clock;
}

void glut_reshape_window( GLint w, GLint h )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	if( w > h )
	{
		glViewport( 0, 0, (GLsizei) w, (GLsizei) h );
		gluPerspective( 60.0, ( float ) w / ( float ) h, 0.5, 12.0 );
	}
	else
	{
		glViewport( 0, h / 2 - w / 2, (GLsizei) w, (GLsizei) w );
		gluPerspective( 60.0, 1.0, 0.5, 12.0 );
	}

	gluLookAt(	0.0, 0.0, 0.0,
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0 );

	glutPostRedisplay();
}

void glut_left_motion( GLint x, GLint y )
{
	y = glutGet( GLUT_WINDOW_HEIGHT ) - y;

	// rotate clipping object
	GLdouble angle = sqrt( ( double ) ( last_y - y ) * ( last_y - y ) + ( last_x - x ) * ( last_x - x ) );
	GLdouble axis_x = ( last_y - y ) / angle;
	GLdouble axis_y = -( last_x - x ) / angle;

	if( ( axis_x != 0 || axis_y != 0 ) && angle != 0 )
		bbox_transform = GLMatrix<GLfloat>::identity().applyRotate( angle, axis_x, axis_y, 0.0 ) * bbox_transform;

	last_x = x;
	last_y = y;

	glutPostRedisplay();
}

void glut_mouse( GLint button, GLint state, GLint x, GLint y )
{
	if( state == GLUT_DOWN )
	{
		if( button == GLUT_LEFT_BUTTON )
		{
			last_x = x;
			last_y = glutGet( GLUT_WINDOW_HEIGHT ) - y;
			glutMotionFunc( glut_left_motion );
		}
	}
	else if ( state == GLUT_UP )
	{
		if( button == GLUT_LEFT_BUTTON )
			glutMotionFunc( NULL );
		else if ( button == GLUT_WHEEL_UP )
			// zoom in
			surface_transform = GLMatrix<GLfloat>().loadScale( 1.125f, 1.125f, 1.125f ) * surface_transform;
		else if( button == GLUT_WHEEL_DOWN )
			// zoom out
			surface_transform = GLMatrix<GLfloat>().loadScale( 1.0f / 1.125f, 1.0f / 1.125f, 1.0f / 1.125f ) * surface_transform;
		glutPostRedisplay();
	}
}

void glut_keys( unsigned char key, int, int )
{
	if( key >= '0' && key <= '9' ) 
	{
		// select predefined surface
		cout << key << ": \"" << predefined_surfaces[ key - '0' ].first << "\" selected" << endl;
		cout << predefined_surfaces[ key - '0' ].second << endl;
		if(	sm->create_shader( predefined_surfaces[ key - '0' ].second ) == ShaderManager::SUCCESS )
			cout << "ready" << endl;
		else
			cout << sm->get_error_message() << endl;
	}
	else
	{
		switch( key )
		{
			case 27: // ESC
				exit( 0 );
				break;
			case 'r':
				reset_to_defaults();
				break;
			case 'f':
				show_fps = !show_fps;
				break;
			case 'b':
				show_box = !show_box;
				break;
			case 'c':
				if( sm->get_clipping_surface() == ShaderManager::CUBE )
					sm->set_clipping_surface( ShaderManager::SPHERE );
				else
					sm->set_clipping_surface( ShaderManager::CUBE );
				break;
			case 'i':
				{
					ostringstream algebraic_formula;
					char c;
					cout << "enter formula of algebraic surface (without '=0'):" << endl; 
					while( cin.get( c ) && c != '\n' )
						algebraic_formula << c;
					if(	sm->create_shader( algebraic_formula.str() ) == ShaderManager::SUCCESS )
						cout << "ready" << endl;
					else
						cout << "error: " << sm->get_error_message() << endl;
				}
				break;
			case '+': // zoom in
				surface_transform = GLMatrix<GLfloat>().loadScale( 1.125f, 1.125f, 1.125f ) * surface_transform;
				break;
			case '-': // zoom out
				surface_transform = GLMatrix<GLfloat>().loadScale( 1.0f / 1.125f, 1.0f / 1.125f, 1.0f / 1.125f ) * surface_transform;
				break;
			default:
				break;
		}
	}
	glutPostRedisplay();
}

void glut_special_keys( int key, int, int )
{
	GLfloat distance = 0.1f;
    switch( key )
    {
		case GLUT_KEY_F5:
			cout << "reloading shader sources ... ";
			if(	sm->reload() == ShaderManager::SUCCESS )
				cout << "ready" << endl;
			else
				cout << "error: " << endl << sm->get_error_message() << endl;
			break;
		case GLUT_KEY_F2:
			{
				cout << "select method for calculation of polynomial coefficients:" << endl;
				const map< ShaderManager::PolyCoeffMethod, string >* poly_coeff_methods = sm->get_poly_coeff_methods();
				for( map< ShaderManager::PolyCoeffMethod, string >::const_iterator it = poly_coeff_methods->begin(); it != poly_coeff_methods->end(); it++ )
					cout << it->second << ": " << it->first << endl;
				int which;
				cin >> which;
				for( map< ShaderManager::PolyCoeffMethod, string >::const_iterator it = poly_coeff_methods->begin(); it != poly_coeff_methods->end(); it++ )
					if( ( ShaderManager::PolyCoeffMethod ) which == it->first )
						sm->set_poly_coeff_method( ( ShaderManager::PolyCoeffMethod ) which );
				if(	sm->create_shader( sm->get_formula() ) == ShaderManager::SUCCESS )
					cout << "ready" << endl;
				else
					cout << "error: " << sm->get_error_message() << endl;
			}
			break;
		case GLUT_KEY_F3:
			{
				cout << "select method for calculation of polynomial roots:" << endl;
				const map< ShaderManager::RootFinderMethod, string >* root_finder_methods = sm->get_root_finder_methods();
				for( map< ShaderManager::RootFinderMethod, string >::const_iterator it = root_finder_methods->begin(); it != root_finder_methods->end(); it++ )
					cout << it->second << ": " << it->first << endl;
				int which;
				cin >> which;
				for( map< ShaderManager::RootFinderMethod, string >::const_iterator it = root_finder_methods->begin(); it != root_finder_methods->end(); it++ )
					if( ( ShaderManager::RootFinderMethod ) which == it->first )
						sm->set_root_finder_method( ( ShaderManager::RootFinderMethod ) which );
				if(	sm->create_shader( sm->get_formula() ) == ShaderManager::SUCCESS )
					cout << "ready" << endl;
				else
					cout << "error: " << sm->get_error_message() << endl;
			}
			break;
		case GLUT_KEY_UP:
			surface_transform = ( bbox_transform.inverse() * GLMatrix<GLfloat>().loadTranslate( 0.0f, distance, 0.0f ) * bbox_transform ) * surface_transform;
			break;
		case GLUT_KEY_DOWN:
			surface_transform = ( bbox_transform.inverse() * GLMatrix<GLfloat>().loadTranslate( 0.0f, -distance, 0.0f ) * bbox_transform ) * surface_transform;
			break;
		case GLUT_KEY_LEFT:
			surface_transform = ( bbox_transform.inverse() * GLMatrix<GLfloat>().loadTranslate( -distance, 0.0f, 0.0f ) * bbox_transform ) * surface_transform;
			break;
		case GLUT_KEY_RIGHT:
			surface_transform = ( bbox_transform.inverse() * GLMatrix<GLfloat>().loadTranslate( distance, 0.0f, 0.0f ) * bbox_transform ) * surface_transform;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

int main( int argc, char **argv )
{   
	// initialize glut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitWindowPosition( 30, 30 );
    glutCreateWindow( "" );
	set_window_title();

	cout << "RealSurf - Realtime Raytracing of Algebraic Surfaces" << endl;
	cout << "====================================================" << endl;
	cout << endl;
	cout << "running on a " << glGetString( GL_RENDERER ) << " (" << glGetString( GL_VENDOR ) << ")" << endl;
	cout << endl;
	cout << "i   - input new surface equation" << endl;
	cout << "c   - switch between cube and sphere clipping" << endl;
	cout << "b   - show surrounding cube/sphere" << endl;
	cout << "0-9 - some predefined surfaces" << endl;
	cout << "F2  - choose method for calculating polynomial coefficients" << endl;
	cout << "F3  - choose method for calculating polynomial roots" << endl;
	cout << "F5  - reload shader sources" << endl;
	cout << endl;

	// checking required extensions 
	if( !check_gl_extensions() )
	{
		*output << endl << "Please install a compatible grapics card and graphics driver!" << endl;
		return -1;
	}

	// create new ShaderManager, that handles the shaders created for the surface formulas
	sm = new ShaderManager();
	ShaderManager::ErrorCode e = sm->create_shader( "x^2+y^2-x^2*z+y^2*z+z^2-1+x^1" );
	if(	e == ShaderManager::SUCCESS )
		cout << "ready" << endl;
	else
		cout << "error" << endl << sm->get_error_message() << endl;
	//sm->create_shader( "x^3-(x^2*y+x^2*z+y^2*x+y^2*z)+1" );

	// glut callbacks
    glutDisplayFunc( glut_display_window );
    glutReshapeFunc( glut_reshape_window );
	glutMouseFunc( glut_mouse );
    glutKeyboardFunc( glut_keys );
    glutSpecialFunc( glut_special_keys );
	glut_timer( 0 );

	// reset global variables and opengl-state to defaults
	reset_to_defaults();
	
	// start the main loop of the application
    glutMainLoop();

    return 0;
}