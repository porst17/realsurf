#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_
#include <string>
#include <sstream>
#include <set>

namespace translator
{
	class Expression
	{
		public:
			enum ExprType	{	POLY_X, POLY_Y, POLY_Z, POLY_NEG, POLY_ADD, POLY_SUB, POLY_MUL, POLY_POW,
								PARAM, PARAM_NEG, PARAM_OPERATOR, PARAM_FUNC_1, PARAM_FUNC_2,
								CONSTANT, POSINT };
			enum Var		{ VAR_X, VAR_Y, VAR_Z };
		private:
			ExprType type;
			union
			{
				std::string *name;
				unsigned int int_value;
				float float_value;
			};
			Expression * right_child;
			Expression * left_child;
		
		public:
			/**
			 * Copy constructor.
			 */ 
			Expression( const Expression *expr );

			/**
			 * Construct an expression of type POLY_X, POLY_Y, POLY_Z.
			 */
			Expression( Var v );

			/**
			 * Construct an expression of type PARAM with name 'name'.
			 */
			Expression( std::string name );

			/**
			 * Construct an expression of type PARAM_FUNC_1 with name 'name' and the
			 * subexpression left_child.
			 */
			Expression( std::string name, Expression *left_child );

			/**
			 * Construct an expression of type PARAM_OPERATOR or PARAM_FUNC_2 with name 'name' and the
			 * two subexpressions left_child and right_child.
			 * The parameter 'is_operator' tells, if this expression is an infix operator or a function call.
			 */
			Expression( std::string name, bool is_operator, Expression *left_child, Expression *right_child );

			/**
			 * Constructs an expression of type POSINT with value 'int_value'.
			 */
			Expression( unsigned int int_value );

			/**
			 * Constructs an expression of type CONSTANT with value 'float_value'.
			 */
			Expression( float float_value );

			/**
			 * Constructs an expression of type 'type' with two subexpressions left_child and right_child.
			 */
			Expression( ExprType type, Expression *left_child, Expression *right_child );
			virtual ~Expression();
		
			ExprType get_type();
			std::string get_name();
			unsigned int get_int_value();
			float get_float_value();
			float get_number_as_float();
			
			Expression * get_right_child();
			Expression * get_left_child();
		
			bool is_leaf();
			bool is_constant();
			bool is_constant_or_param();

			Expression *derive( Var v );
		
			void print();
			int to_glsl( std::ostream &glsl_code, std::set<std::string> &params );
			
		private:
			void extract_params( std::set<std::string> &params );
			int glsl_poly_arith( std::ostringstream &glsl_code, bool wrap_params_and_constants );
			int glsl_coefficients( std::ostringstream &glsl_code );
			void glsl_gradient( std::ostringstream &glsl_code );
			void glsl_formula( std::ostringstream &glsl_code );
			std::string fmt_float( float f );
	};
};
#endif
