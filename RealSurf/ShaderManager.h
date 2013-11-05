#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include <GL/GLee.h>
#include <MathGL/GLMatrix.h>

#include <string>
#include <map>
#include <set>
#include <utility>

namespace glsl
{
	class ShaderManager
	{
		public:
			enum ClippingSurface { SPHERE = 1, CUBE = 2 };
			enum ErrorCode { SUCCESS, EQUATION_PARSE_ERROR, GLSL_COMPILATION_ERROR, GLSL_LINKING_ERROR, GLSL_VALIDATION_ERROR, FILE_IO_ERROR };
			enum PolyCoeffMethod { COMPUTER_ALGEBRA, LAGRANGE_INTERPOLATION, NEWTON_INTERPOLATION };
			enum RootFinderMethod { DESCARTES, D_CHAIN_BISECTION, D_CHAIN_REGULA_FALSI, STURM_CHAIN, INTERVAL_ARITHMETIC_BISECTION, MULLER, LAGUERRE, FORMULA };

		private:
			// glsl-ids
			GLuint vertex_shader;
			GLuint fragment_shader;
			GLuint glsl_program;

			// fragment code stub
			std::string fragment_shader_stub_code;

			// current state
			std::string formula;
			GLuint degree;
			std::map<std::string,GLfloat> parameters;
			std::string error_message;

			// shader settings
			GLMatrix<GLfloat> surface_transform;
			GLMatrix<GLfloat> surface_transform_inverse;
			ClippingSurface clip_to;
			ErrorCode error_code;
			GLboolean use_transparency;

			// method information
			static std::map< PolyCoeffMethod, std::string > polyCoeffMethods;
			static std::map< RootFinderMethod, std::string > rootFinderMethods;
			PolyCoeffMethod polyCoeffMethod;
			RootFinderMethod rootFinderMethod;

		public:
			// bei Aufruf muss OpenGL/GLee bereits initialisiert sein!!
			ShaderManager();
			virtual ~ShaderManager();

			ErrorCode reload();

			void enable_glsl();
			void enable_fixed_function_pipeline();

			ErrorCode create_shader( const std::string &algebraic_formula );
			ErrorCode check_for_glsl_errors();

			void set_surface_transformation( const GLMatrix<GLfloat> &matrix );
			void set_clipping_surface( const ClippingSurface clip_to );
			GLboolean set_surface_parameter( const std::string &name, const GLfloat &value );
			void set_transparency_enabled( GLboolean enabled );
			void set_poly_coeff_method( PolyCoeffMethod pcm );
			void set_root_finder_method( RootFinderMethod rfm );

			std::string get_formula() const;
			GLuint get_degree() const;
			std::set<std::string> get_parameters() const;
			std::string get_error_message() const;
			static const std::map< PolyCoeffMethod, std::string >* get_poly_coeff_methods();
			static const std::map< RootFinderMethod, std::string >* get_root_finder_methods();
			PolyCoeffMethod get_poly_coeff_method();
			RootFinderMethod get_root_finder_method();

			GLMatrix<GLfloat> get_surface_transformation() const;
			GLfloat get_surface_parameter( const std::string &name ) const;
			ClippingSurface get_clipping_surface() const;
			GLboolean get_transparency_enabled();

		private:
			GLboolean compile_glsl_shader( std::string &code, GLuint shader_id );
			GLboolean link_glsl_program();
			GLboolean validate_glsl_program();
	};
}
#endif