#include "ShaderManager.h"

#include <sstream>
#include <fstream>
#include <exception>

#include <ctime>
#include <iostream>

#include "Translator.h"

#include "glsl_code.h"

using namespace glsl;
using namespace std;
using namespace translator;

namespace
{
	string read_file( string filename )
	{
		ifstream file( filename.c_str(), ios::in );
		string line;
		ostringstream content;
		while( file.good() )
		{
			getline( file, line );
			content << line << "\n";
		}
		return content.str();
	}
};

map< ShaderManager::PolyCoeffMethod, std::string > ShaderManager::polyCoeffMethods = map< ShaderManager::PolyCoeffMethod, std::string >();
map< ShaderManager::RootFinderMethod, string > ShaderManager::rootFinderMethods = map< ShaderManager::RootFinderMethod, string >();

ShaderManager::ShaderManager()
{
	// apply default shader settings
	this->surface_transform_inverse = GLMatrix<GLfloat>::identity();
	this->clip_to = SPHERE;
	this->use_transparency = GL_FALSE;
	this->polyCoeffMethod = COMPUTER_ALGEBRA;
	this->rootFinderMethod = DESCARTES;

	// reserve glsl shader and program ids
	this->glsl_program = glCreateProgram();
	this->vertex_shader = glCreateShader( GL_VERTEX_SHADER );
	this->fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );

	// attach shaders to program
	glAttachShader( this->glsl_program, this->vertex_shader );
	glAttachShader( this->glsl_program, this->fragment_shader );

	// load source of vertex shader and compile it
	std::string realsurf_glsl_vert_str = std::string( REALSURF_GLSL_VERT );
	compile_glsl_shader( realsurf_glsl_vert_str, this->vertex_shader );

	// load source of fragment shader stub
	this->fragment_shader_stub_code = REALSURF_GLSL_FRAG_STUB;

	// translate standard formula to glsl code and set this as current shader
	create_shader( "0" );
}

ShaderManager::~ShaderManager()
{
	// delete glsl shaders of this manager
	glUseProgram( 0 );
	glDetachShader( this->glsl_program, this->vertex_shader );
	glDeleteShader( this->vertex_shader );
	glDetachShader( this->glsl_program, this->fragment_shader );
	glDeleteShader( this->fragment_shader );
	glDeleteProgram( this->glsl_program );
}

ShaderManager::ErrorCode ShaderManager::reload()
{
	// load source of vertex shader and compile it
	string shader_code = read_file( "realsurf_vert.glsl" );
	if( shader_code == "" )
	{
		this->error_message = "Error reading \"realsurf_vert.glsl\"";
		return FILE_IO_ERROR;
	}
	else
	{
		compile_glsl_shader( shader_code, this->vertex_shader );
	}

	// load source of fragment shader stub
	shader_code = read_file( "realsurf_frag_stub.glsl" );
	if( shader_code == "" )
	{
		this->error_message = "Error reading \"realsurf_frag_stub.glsl\"";
		return FILE_IO_ERROR;
	}
	else
	{
		this->fragment_shader_stub_code = shader_code;
	}

	// backup current surface parameters
	std::map<std::string,GLfloat> tmp_parameters( this->parameters );

	// recreate shader for current formula
	ShaderManager::ErrorCode error_code = create_shader( this->formula );
	if( error_code == SUCCESS )
		this->parameters = tmp_parameters;
	return error_code;
}

// when formula parsing fails, the surface doesn't change!
// on succes: surface parameters are set to zero
ShaderManager::ErrorCode ShaderManager::create_shader( const string &algebraic_formula )
{
	clock_t start_clock = clock();

	// some temporary variables
	set<string> param_names;
	GLuint degree;

	// streams for code and error messages
	ostringstream glsl_code;
	ostringstream error_msg_stream;

	// define glsl version
	glsl_code << "#version 110" << endl;

	// define USE_TRANSPARENCY if needed
	if( this->use_transparency )
		glsl_code << "#define USE_TRANSPARENCY" << endl;

	// macros for method selection
	// method for calculation of polynomial coefficients
	switch( this->polyCoeffMethod )
	{
		case NEWTON_INTERPOLATION:
			glsl_code << "#define METHOD_NEWTON_INTERPOLATION" << endl;
			break;
		case LAGRANGE_INTERPOLATION:
			glsl_code << "#define METHOD_LAGRANGE_INTERPOLATION" << endl;
			break;
		case COMPUTER_ALGEBRA:
			glsl_code << "#define METHOD_COMPUTER_ALGEBRA" << endl;
			break;
	}
	// method for calculation of polynomial roots
	switch( this->rootFinderMethod )
	{
		case DESCARTES:
			glsl_code << "#define METHOD_DESCARTES" << endl;
			break;
		case D_CHAIN_BISECTION:
			glsl_code << "#define METHOD_D_CHAIN" << endl;
			glsl_code << "#define SUB_METHOD_BISECTION" << endl;
			break;
		case D_CHAIN_REGULA_FALSI:
			glsl_code << "#define METHOD_D_CHAIN" << endl;
			glsl_code << "#define SUB_METHOD_REGULA_FALSI" << endl;
			break;
		case STURM_CHAIN:
			glsl_code << "#define METHOD_STURM" << endl;
			break;
		case INTERVAL_ARITHMETIC_BISECTION:
			glsl_code << "#define METHOD_INTERVAL" << endl;
			glsl_code << "#define SUB_METHOD_BISECTION" << endl;
			break;
		case MULLER:
			glsl_code << "#define METHOD_MULLER" << endl;
			break;
		case LAGUERRE:
			glsl_code << "#define METHOD_LAGUERRE" << endl;
			break;
		case FORMULA:
			glsl_code << "#define METHOD_FORMULA" << endl;
			break;
	}
	
	// parse formula, retrieve corresponding glsl fragment shader code and surface parameters
	if( translate_formula( algebraic_formula, degree, glsl_code, param_names, error_msg_stream ) )
	{
		// translation successful:
		// 1. (re-)set state variables
		this->formula = algebraic_formula;
		this->degree = degree;
		this->error_message = string();

		// 2. initialize surface parameters with 0.0f
		this->parameters = map<string,GLfloat>();
		for( set<string>::iterator param_it = param_names.begin(); param_it != param_names.end(); param_it++ )
			this->parameters.insert( pair<string,GLfloat>( *param_it, 0.0f ) );

		// 3. add stub code to generated code
		if( this->degree == 0 )
			glsl_code.str( "uniform mat4 surface_transform_inverse; void main() { discard; }" );
		else
			glsl_code << endl << endl << this->fragment_shader_stub_code;

#ifndef NDEBUG
		// 3.a) save generated code in file for debugging
		fstream debug_output( "debug_frag.glsl", fstream::out );
		debug_output << glsl_code.str();
		debug_output.close();
#endif

		// 4. compile glsl code for the algebraic function; test if compilation of vertex and fragment shaders were successfull
		GLint glsl_vertex_shader_error;
		glGetShaderiv( this->vertex_shader, GL_COMPILE_STATUS, &glsl_vertex_shader_error );
		std::string glsl_code_str = glsl_code.str();
		if( glsl_vertex_shader_error != GL_TRUE || !compile_glsl_shader( glsl_code_str, this->fragment_shader ) )
			return GLSL_COMPILATION_ERROR;

		// 5. link glsl program
		if( !link_glsl_program() )
			return GLSL_LINKING_ERROR;

		// 6. validate glsl program
		if( !validate_glsl_program() )
			return GLSL_VALIDATION_ERROR;

		cout << double( clock() - start_clock ) / CLOCKS_PER_SEC << "s" << endl;

		return SUCCESS;
	}
	else
	{
		this->error_message = error_msg_stream.str();
		return EQUATION_PARSE_ERROR;
	}
}

ShaderManager::ErrorCode ShaderManager::check_for_glsl_errors()
{
	GLint error_code;

	glGetShaderiv( this->vertex_shader, GL_COMPILE_STATUS, &error_code );
	if( error_code != GL_TRUE )
		return GLSL_COMPILATION_ERROR;
	glGetShaderiv( this->fragment_shader, GL_COMPILE_STATUS, &error_code );
	if( error_code != GL_TRUE )
		return GLSL_COMPILATION_ERROR;
	glGetProgramiv( this->glsl_program, GL_LINK_STATUS, &error_code );
	if( error_code != GL_TRUE )
		return GLSL_LINKING_ERROR;
	glGetProgramiv( this->glsl_program, GL_VALIDATE_STATUS, &error_code );
	if( error_code != GL_TRUE )
		return GLSL_VALIDATION_ERROR;

	return SUCCESS;
}

void ShaderManager::enable_glsl()
{
	if( this->check_for_glsl_errors() == SUCCESS ) 
	{
		glUseProgram( this->glsl_program );

		// set surface transformations and clipping
		glUniformMatrix4fv( glGetUniformLocation( this->glsl_program, "surface_transform_inverse" ), 1, GL_FALSE, this->surface_transform_inverse );
		glUniformMatrix4fv( glGetUniformLocation( this->glsl_program, "surface_transform" ), 1, GL_FALSE, this->surface_transform );
		glUniform1i( glGetUniformLocation( this->glsl_program, "clip_to" ), this->clip_to );

		// set surface parameters
		for( map<string,GLfloat>::iterator param_it = this->parameters.begin(); param_it != this->parameters.end(); param_it++ )
		{
			std::string name = "param_" + param_it->first;
			float value = param_it->second;
			glUniform1f( glGetUniformLocation( this->glsl_program, name.c_str() ), param_it->second );
		}
	}
	else
	{
		glUseProgram( 0 );
	}
}

void ShaderManager::enable_fixed_function_pipeline()
{
	glUseProgram( 0 );
}

namespace
{
	void save_shader_info_log( GLuint obj, string &log )
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetShaderiv( obj, GL_INFO_LOG_LENGTH, &infologLength );

		if( infologLength > 0 )
		{
			infoLog = new char[ infologLength ];
			glGetShaderInfoLog( obj, infologLength, &charsWritten, infoLog );
			log.append( infoLog );
			delete infoLog;
		}
	}

	void save_program_info_log( GLuint obj, string &log )
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetProgramiv( obj, GL_INFO_LOG_LENGTH, &infologLength );

		if( infologLength > 0 )
		{
			infoLog = new char[ infologLength ];
			glGetProgramInfoLog( obj, infologLength, &charsWritten, infoLog );
			log.append( infoLog );
			delete infoLog;
		}
	}
};


GLboolean ShaderManager::compile_glsl_shader( string &code, GLuint shader_id )
{
	const char *c_str_code = code.c_str();
	glShaderSource( shader_id, 1, &c_str_code, NULL );
	glCompileShader( shader_id );

	GLint error_code;
	glGetShaderiv( shader_id, GL_COMPILE_STATUS, &error_code );
	if( error_code != GL_TRUE )
		save_shader_info_log( shader_id, this->error_message );
	return error_code;
}

GLboolean ShaderManager::link_glsl_program()
{
	glLinkProgram( this->glsl_program );
	GLint error_code;
	glGetProgramiv( this->glsl_program, GL_LINK_STATUS, &error_code );
	if( error_code != GL_TRUE )
		save_program_info_log( this->glsl_program, this->error_message );
	return GLboolean( error_code );
}

GLboolean ShaderManager::validate_glsl_program()
{
	glValidateProgram( this->glsl_program );
	GLint error_code;
	glGetProgramiv( this->glsl_program, GL_VALIDATE_STATUS, &error_code );
	if( error_code != GL_TRUE )
		save_shader_info_log( this->glsl_program, this->error_message );
	return GLboolean( error_code );
}

void ShaderManager::set_surface_transformation( const GLMatrix<GLfloat> &mat )
{
	this->surface_transform = mat;
	this->surface_transform_inverse = mat.inverse();
}

void ShaderManager::set_clipping_surface( const ClippingSurface clip_to )
{
	this->clip_to = clip_to;
}

GLboolean ShaderManager::set_surface_parameter( const std::string &name, const GLfloat &value )
{
	if( this->parameters.find( name ) != this->parameters.end() )
	{
		this->parameters[ name ] = value;
		return true;
	}
	else
	{
		return false;
	}
}

void ShaderManager::set_transparency_enabled( GLboolean enabled )
{
	this->use_transparency = enabled;
}

string ShaderManager::get_formula() const { return this->formula; }
GLuint ShaderManager::get_degree() const { return this->degree; }
set<string> ShaderManager::get_parameters() const
{
	set<string> param_names;
	for( map<string,GLfloat>::const_iterator param_it = this->parameters.begin(); param_it != this->parameters.end(); param_it++ )
		param_names.insert( param_it->first );
	return param_names;
}
string ShaderManager::get_error_message() const { return this->error_message; }

GLMatrix<GLfloat> ShaderManager::get_surface_transformation() const { return this->surface_transform; }
ShaderManager::ClippingSurface ShaderManager::get_clipping_surface() const { return this->clip_to; }
GLfloat ShaderManager::get_surface_parameter( const std::string &name ) const
{
	map<string,GLfloat>::const_iterator map_it = this->parameters.find( name );
	if( map_it == this->parameters.end() )
		return 0.0f;
	else
		return map_it->second;
}
GLboolean ShaderManager::get_transparency_enabled()
{
	return this->use_transparency;
}

const std::map< ShaderManager::PolyCoeffMethod, string >* ShaderManager::get_poly_coeff_methods()
{
	if( polyCoeffMethods.empty() )
	{
		// store method information
		polyCoeffMethods[ LAGRANGE_INTERPOLATION ] = string( "lagrange interpolation" );
		polyCoeffMethods[ NEWTON_INTERPOLATION ] = string( "newton interpolation" );
		polyCoeffMethods[ COMPUTER_ALGEBRA ] = string( "computer algebra" );
	}

	return &polyCoeffMethods;
}
const std::map< ShaderManager::RootFinderMethod, string >* ShaderManager::get_root_finder_methods()
{
	if( rootFinderMethods.empty() )
	{
		// store method information
		rootFinderMethods[ DESCARTES ] = string( "descartes rule of sign" );
		rootFinderMethods[ D_CHAIN_BISECTION ] = string( "d-chain with bisection" );
		rootFinderMethods[ D_CHAIN_REGULA_FALSI ] = string( "d-chain with regula falsi" );
		rootFinderMethods[ STURM_CHAIN ] = string( "sturm chain" );
		rootFinderMethods[ INTERVAL_ARITHMETIC_BISECTION ] = string( "interval arithmetic with bisection" );
		rootFinderMethods[ MULLER ] = string( "muller-method" );
		rootFinderMethods[ LAGUERRE ] = string( "laguerre-method" );
		rootFinderMethods[ FORMULA ] = string( "formula for degree 1 to 4" );
	}

	return &rootFinderMethods;
}
ShaderManager::PolyCoeffMethod ShaderManager::get_poly_coeff_method() { return this->polyCoeffMethod; }
ShaderManager::RootFinderMethod ShaderManager::get_root_finder_method() { return this->rootFinderMethod; }
void ShaderManager::set_poly_coeff_method( ShaderManager::PolyCoeffMethod pcm ) { this->polyCoeffMethod = pcm; }
void ShaderManager::set_root_finder_method( ShaderManager::RootFinderMethod rfm ) { this->rootFinderMethod = rfm; }
