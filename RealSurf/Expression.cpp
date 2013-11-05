#include <cstdio>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cmath>
#include "Expression.h"

using namespace translator;
using namespace std;

Expression::Expression( const Expression *expr )
{
	this->type = expr->type;
	this->left_child = ( expr->left_child == NULL ) ? NULL : new Expression( expr->left_child );
	this->right_child = ( expr->right_child == NULL ) ? NULL : new Expression( expr->right_child );

	switch( expr->type )
	{
		case PARAM:
		case PARAM_OPERATOR:
		case PARAM_FUNC_1:
		case PARAM_FUNC_2:
			this->name = new string( *( expr->name ) );
			break;
		case POSINT:
			this->int_value = expr->int_value;
			break;
		case CONSTANT:
			this->float_value = expr->float_value;
			break;
		default:
			break;
	}
}

Expression::Expression( Expression::Var v )
{
	switch( v )
	{
		case VAR_X:
			this->type = POLY_X;
			break;
		case VAR_Y:
			this->type = POLY_Y;
			break;
		case VAR_Z:
			this->type = POLY_Z;
			break;
	}
	this->name = NULL;
	this->left_child = NULL;
	this->right_child = NULL;
}

Expression::Expression( std::string name )
{
	this->type = PARAM;
	this->name = new std::string( name );
	this->left_child = NULL;
	this->right_child = NULL;
}

Expression::Expression( std::string name, Expression *left_child )
{
	this->type = PARAM_FUNC_1;
	this->name = new std::string( name );
	this->left_child = left_child;
	this->right_child = NULL;
}

Expression::Expression( std::string name, bool is_operator, Expression *left_child, Expression *right_child )
{
	this->type = is_operator ? PARAM_OPERATOR : PARAM_FUNC_2;
	this->name = new std::string( name );
	this->left_child = left_child;
	this->right_child = right_child;
}

Expression::Expression( unsigned int int_value )
{
	this->type = POSINT;
	this->int_value = int_value;
	this->left_child = NULL;
	this->right_child = NULL;
}

Expression::Expression( float float_value )
{
	this->type = CONSTANT;
	this->float_value = float_value;
	this->left_child = NULL;
	this->right_child = NULL;
}

Expression::Expression( Expression::ExprType type, Expression *left_child, Expression *right_child )
{
	this->type = type;
	this->left_child = left_child;
	this->right_child = right_child;
}

Expression::~Expression()
{
	if( this->type == PARAM || this->type == PARAM_OPERATOR || this->type == PARAM_FUNC_1 || this->type == PARAM_FUNC_2 )
		delete this->name;

	if( this->left_child != NULL )
		delete this->left_child;
	if( this->right_child != NULL )
		delete this->right_child;
}

Expression::ExprType Expression::get_type() { return this->type; }

std::string Expression::get_name() { return std::string( *( this->name ) ); }
unsigned int Expression::get_int_value() { return this->int_value; }
float Expression::get_float_value() { return this->float_value; }
		
Expression * Expression::get_right_child() { return this->right_child; }
Expression * Expression::get_left_child() { return this->left_child; }
float Expression::get_number_as_float()
{
	if( this->type == CONSTANT )
		return this->float_value;
	else if( this->type == POSINT )
		return float( this->int_value );
	else
		return numeric_limits<float>::quiet_NaN();
}

bool Expression::is_leaf() { return right_child == NULL && left_child == NULL; }
bool Expression::is_constant() { return this->type == CONSTANT || this->type == POSINT; }
bool Expression::is_constant_or_param()
{
	return this->type == POSINT
		|| this->type == CONSTANT
		|| this->type == PARAM
		|| this->type == PARAM_NEG
		|| this->type == PARAM_OPERATOR
		|| this->type == PARAM_FUNC_1
		|| this->type == PARAM_FUNC_2;
}

void Expression::print()
{

	if( !is_leaf() )
		printf( "(" );
	
	switch( this->type )
	{
		case POLY_X:
			printf( "X" );
			break;
		case POLY_Y:
			printf( "Y" );
			break;
		case POLY_Z:
			printf( "Z" );
			break;
		case PARAM:
			printf( this->name->c_str() );
			break;
		case CONSTANT:
			printf( "%f", this->float_value );
			break;
		case POSINT:
			printf( "%i", this->int_value );
			break;
		case PARAM_NEG:
		case POLY_NEG:
			get_left_child()->print();
			printf( " _ " );
			break;
		case PARAM_OPERATOR:
			get_left_child()->print();
			printf( " " );
			get_right_child()->print();
			printf( " %s ", this->name->c_str() );
			break;
		case PARAM_FUNC_1:
			printf( " %s( ", this->name->c_str() );
			get_left_child()->print();
			printf( " ) " );
			break;
		case PARAM_FUNC_2:
			printf( " %s( ", this->name->c_str() );
			get_left_child()->print();
			printf( ", " );
			get_right_child()->print();
			printf( " ) " );
			break;
		case POLY_ADD:
			get_left_child()->print();
			printf( " " );
			get_right_child()->print();
			printf( " + " );
			break;
		case POLY_SUB:
			get_left_child()->print();
			printf( " " );
			get_right_child()->print();
			printf( " - " );
			break;
		case POLY_MUL:
			get_left_child()->print();
			printf( " " );
			get_right_child()->print();
			printf( " * " );
			break;
		case POLY_POW:
			get_left_child()->print();
			printf( " " );
			get_right_child()->print();
			printf( " ^ " );
			break;
		default:
			printf( "unknown" );
	}
	if( !is_leaf() )
		printf( ")" );
}

Expression *Expression::derive( Var var )
{
	switch( this->type )
	{
		case POLY_X:
			return new Expression( ( var == VAR_X ) ? 1.0f : 0.0f );
			break;
		case POLY_Y:
			return new Expression( ( var == VAR_Y ) ? 1.0f : 0.0f );
			break;
		case POLY_Z:
			return new Expression( ( var == VAR_Z ) ? 1.0f : 0.0f );
			break;
		case POLY_NEG:
			{
				Expression *arg_diff = this->left_child->derive( var );
				if( arg_diff->type == CONSTANT )
				{
					arg_diff->float_value = -arg_diff->float_value; 
					return arg_diff;
				}
				else
				{
					return new Expression( POLY_NEG, arg_diff, NULL );
				}
			}
			break;
		case POLY_ADD:
			{
				Expression *u_diff_expr = this->left_child->derive( var );
				Expression *v_diff_expr = this->right_child->derive( var );

				if( u_diff_expr->is_constant() && v_diff_expr->is_constant() )
				{
					float u_val = u_diff_expr->get_number_as_float();
					float v_val = v_diff_expr->get_number_as_float();
					Expression *result = new Expression( u_val + v_val );
					delete u_diff_expr;
					delete v_diff_expr;
					return result;
				}
				else if( u_diff_expr->get_number_as_float() == 0.0f )
				{
					delete u_diff_expr;
					return v_diff_expr;
				}
				else if( v_diff_expr->get_number_as_float() == 0.0f )
				{
					delete v_diff_expr;
					return u_diff_expr;
				}
				else
				{
					return new Expression( POLY_ADD, u_diff_expr, v_diff_expr );
				}
			}
			break;
		case POLY_SUB:
			{
				Expression *u_diff_expr = this->left_child->derive( var );
				Expression *v_diff_expr = this->right_child->derive( var );

				if( u_diff_expr->is_constant() && v_diff_expr->is_constant() )
				{
					float u_val = u_diff_expr->get_number_as_float();
					float v_val = v_diff_expr->get_number_as_float();
					Expression *result = new Expression( u_val - v_val );
					delete u_diff_expr;
					delete v_diff_expr;
					return result;
				}
				else if( u_diff_expr->get_number_as_float() == 0.0f )
				{
					delete u_diff_expr;
					return new Expression( POLY_NEG, v_diff_expr, NULL );
				}
				else if( v_diff_expr->get_number_as_float() == 0.0f )
				{
					delete v_diff_expr;
					return u_diff_expr;
				}
				else
				{
					return new Expression( POLY_SUB, u_diff_expr, v_diff_expr );
				}
			}
			break;
		case POLY_MUL:
			{
				Expression *u_expr = new Expression( this->left_child );
				Expression *v_expr = new Expression( this->right_child );
				Expression *u_diff_expr = u_expr->derive( var );
				Expression *v_diff_expr = v_expr->derive( var );

				Expression *summand_1;
				Expression *summand_2;

				// calculate summand_1
				if( u_expr->get_number_as_float() == 0.0f || v_diff_expr->get_number_as_float() == 0.0f )
				{
					delete u_expr;
					delete v_diff_expr;
					summand_1 = new Expression( 0.0f );
				}
				else if( u_expr->get_number_as_float() == 1.0f )
				{
					delete u_expr;
					summand_1 = v_diff_expr;
				}
				else if( v_diff_expr->get_number_as_float() == 1.0f )
				{
					delete v_diff_expr;
					summand_1 = u_expr;
				}
				else if( u_expr->is_constant() && v_diff_expr->is_constant() )
				{
					summand_1 = new Expression( u_expr->get_number_as_float() * v_diff_expr->get_number_as_float() );
					delete u_expr;
					delete v_diff_expr;
				}
				else
				{
					summand_1 = new Expression( POLY_MUL, u_expr, v_diff_expr );
				}

				// calculate summand_2
				if( v_expr->get_number_as_float() == 0.0f || u_diff_expr->get_number_as_float() == 0.0f )
				{
					delete v_expr;
					delete u_diff_expr;
					summand_2 = new Expression( 0.0f );
				}
				else if( v_expr->get_number_as_float() == 1.0f )
				{
					delete v_expr;
					summand_2 = u_diff_expr;
				}
				else if( u_diff_expr->get_number_as_float() == 1.0f )
				{
					delete u_diff_expr;
					summand_2 = v_expr;
				}
				else if( v_expr->is_constant() && u_diff_expr->is_constant() )
				{
					summand_2 = new Expression( v_expr->get_number_as_float() * u_diff_expr->get_number_as_float() );
					delete v_expr;
					delete u_diff_expr;
				}
				else
				{
					summand_2 = new Expression( POLY_MUL, u_diff_expr, v_expr );
				}

				if( summand_1->get_number_as_float() == 0.0f && summand_2->get_number_as_float() == 0.0f )
				{
					delete summand_1;
					delete summand_2;
					return new Expression( 0.0f );
				}
				else if( summand_1->get_number_as_float() == 0.0f )
				{
					delete summand_1;
					return summand_2;
				}
				else if( summand_2->get_number_as_float() == 0.0f )
				{
					delete summand_2;
					return summand_1;
				}
				else
				{
					return new Expression( POLY_ADD, summand_1, summand_2 );
				}
			}
			break;
		case POLY_POW:
			{
				Expression *base = new Expression( this->left_child );
				Expression *base_diff = base->derive( var );
				unsigned int exponent = this->right_child->get_int_value();

				if( exponent == 0 || base_diff->get_number_as_float() == 0.0f )
				{
					delete base;
					delete base_diff;
					return new Expression( 0.0f );
				}
				else if( exponent == 1 )
				{
					delete base;
					return base_diff;
				}
				else if( base_diff->get_number_as_float() == 1.0f )
				{
					delete base_diff;
					if( exponent == 2 )
						return new Expression( POLY_MUL, new Expression( float( exponent ) ), base );
					else
						return new Expression( POLY_MUL, new Expression( float( exponent ) ), new Expression( POLY_POW, base, new Expression( exponent - 1 ) ) );
				}
				else
				{
					if( exponent == 2 )
						return new Expression( POLY_MUL, new Expression( POLY_MUL, new Expression( float( exponent ) ), base ), base_diff );
					else
						return new Expression( POLY_MUL, new Expression( POLY_MUL, new Expression( float( exponent ) ), new Expression( POLY_POW, base, new Expression( exponent - 1 ) ) ), base_diff );
				}
			}
			break;
		case PARAM:
		case PARAM_NEG:
		case PARAM_OPERATOR:
		case PARAM_FUNC_1:
		case PARAM_FUNC_2:
		case CONSTANT:
			return new Expression( 0.0f );
			break;
		case POSINT:
			return new Expression( ( unsigned int ) 0 );
			break;
		default:
			printf( "unknown" );
			return NULL;
	}
}

int Expression::to_glsl( std::ostream &glsl_code, std::set<std::string> &params )
{
	// buffers for output
	ostringstream predeclarations;
	ostringstream my_glsl_code;
	
	// extract parameter names out of expression
	this->extract_params( params );

	// calculation of coefficients 
	int degree = this->glsl_coefficients( my_glsl_code );
	my_glsl_code << endl << endl;
	
	// calculation of surface gradient
	this->glsl_gradient( my_glsl_code );
	
	// declare uniform float surface parameters
	if( params.begin() != params.end() )
		predeclarations << endl << "// declare surface parameters" << endl;
	for( set<string>::iterator param_it = params.begin(); param_it != params.end(); param_it++ )
		predeclarations << "uniform float param_" << *param_it << ";" << endl;
	if( params.begin() != params.end() )
		predeclarations << endl;

	// forward declaration of arithmetic functions
	predeclarations << "// polynomial of degree " << degree << endl;
	predeclarations << "#define DEGREE " << degree << endl;
	predeclarations << "#define SIZE " << ( degree + 1 ) << endl;
	predeclarations << "struct polynomial { float a[ SIZE ]; };" << endl;
	predeclarations << "polynomial create_poly_0( float a0 );" << endl;
	predeclarations << "polynomial create_poly_1( float a0, float a1 );" << endl;
	predeclarations << "polynomial add( polynomial p1, polynomial p2, int res_degree );" << endl;
	predeclarations << "polynomial sub( polynomial p1, polynomial p2, int res_degree );" << endl;
	predeclarations << "polynomial mult( polynomial p1, polynomial p2, int res_degree );" << endl;
	predeclarations << "polynomial neg( polynomial p, int res_degree );" << endl;
	predeclarations << "polynomial power( polynomial p, int exp, int degree );" << endl;
	predeclarations << "polynomial power_1( polynomial p, int exp );" << endl;
	predeclarations << "float power( float base, int exp );" << endl;
	predeclarations << endl;
	
	glsl_code << predeclarations.str();
	glsl_code << my_glsl_code.str();
	
	return degree;
}

void Expression::extract_params( set<string> &params )
{
	if( this->type == PARAM )
		params.insert( this->get_name() );
	if( this->left_child != NULL )
		this->left_child->extract_params( params );
	if( this->right_child != NULL )
		this->right_child->extract_params( params );
}

int Expression::glsl_coefficients( std::ostringstream &glsl_code )
{
	// degree of resulting polynomial
	int degree;
	
	// begin computer algebra method
	glsl_code << "#ifdef METHOD_COMPUTER_ALGEBRA" << endl;

	// signature
	glsl_code << "polynomial calc_coefficients( vec3 eye, vec3 dir, vec2 clipping_interval )" << endl;
	glsl_code << "{" << endl;
	
	// declarations and definitions
	glsl_code << "\tpolynomial x = create_poly_1( eye.x, dir.x );" << endl;
	glsl_code << "\tpolynomial y = create_poly_1( eye.y, dir.y );" << endl;
	glsl_code << "\tpolynomial z = create_poly_1( eye.z, dir.z );" << endl;
	glsl_code << endl;
	
	// calculation of polynomial coefficients and return
	glsl_code << "\treturn ";
	degree = this->glsl_poly_arith( glsl_code, true );
	glsl_code << ";" << endl;
	glsl_code << "}" << endl;

	// end computer algebra method
	glsl_code << "#endif" << endl << endl;

	// begin lagrange interpolation method
	glsl_code << "#ifdef METHOD_LAGRANGE_INTERPOLATION" << endl;

	// method for evaluation of surface
	glsl_code << "float f( float x, float y, float z )" << endl;
	glsl_code << "{" << endl;
	glsl_code << "\treturn ";
	this->glsl_formula( glsl_code );
	glsl_code << ";" << endl;
	glsl_code << "}" << endl;

	// end lagrange interpolation method
	glsl_code << "#endif" << endl << endl;

	// begin newton interpolation method
	glsl_code << "#ifdef METHOD_NEWTON_INTERPOLATION" << endl;

	// method for evaluation of surface
	glsl_code << "float f( float x, float y, float z )" << endl;
	glsl_code << "{" << endl;
	glsl_code << "\treturn ";
	this->glsl_formula( glsl_code );
	glsl_code << ";" << endl;
	glsl_code << "}" << endl;

	// end newton interpolation method
	glsl_code << "#endif";

	return degree;
}

void Expression::glsl_gradient( std::ostringstream &glsl_code )
{
	// signature
	glsl_code << "vec3 gradient( vec3 p )" << endl;
	glsl_code << "{" << endl;
	
	// declarations and definitions
	glsl_code << "\tfloat x = p.x;" << endl;
	glsl_code << "\tfloat y = p.y;" << endl;
	glsl_code << "\tfloat z = p.z;" << endl;
	glsl_code << endl;
	glsl_code << "\tvec3 res;" << endl;
	
	// calculation of res.x
	glsl_code << "\tres.x = ";
	Expression *derivate_x = this->derive( VAR_X );
	derivate_x->glsl_formula( glsl_code );
	delete derivate_x;
	glsl_code << ";" << endl;
	
	// calculation of res.y
	glsl_code << "\tres.y = ";
	Expression *derivate_y = this->derive( VAR_Y );
	derivate_y->glsl_formula( glsl_code );
	delete derivate_y;
	glsl_code << ";" << endl;

	// calculation of res.z
	glsl_code << "\tres.z = ";
	Expression *derivate_z = this->derive( VAR_Z );
	derivate_z->glsl_formula( glsl_code );
	delete derivate_z;
	glsl_code << ";" << endl;
	glsl_code << endl;
	
	// return
	glsl_code << "\treturn res;" << endl; 
	glsl_code << "}";
}

int Expression::glsl_poly_arith( ostringstream &glsl_code, bool wrap_params_and_constants )
{
	int degree;
	if( wrap_params_and_constants && this->is_constant_or_param() )
		glsl_code << "create_poly_0( ";
	switch( this->type )
	{
		case POLY_X:
			glsl_code << "x";
			degree = 1;
			break;
		case POLY_Y:
			glsl_code << "y";
			degree = 1;
			break;
		case POLY_Z:
			glsl_code << "z";
			degree = 1;
			break;
		case POLY_NEG:
			glsl_code << "neg( ";
			degree = this->get_left_child()->glsl_poly_arith( glsl_code, true );
			glsl_code << ", " << degree << " )";
			break;
		case POLY_ADD:
			{
				glsl_code << "add( ";
				int left_degree = this->get_left_child()->glsl_poly_arith( glsl_code, true );
				glsl_code << ", ";
				int right_degree = this->get_right_child()->glsl_poly_arith( glsl_code, true );
				degree = max( left_degree, right_degree );
				glsl_code << ", " << degree << " )";
			}
			break;
		case POLY_SUB:
			{
				glsl_code << "sub( ";
				int left_degree = this->get_left_child()->glsl_poly_arith( glsl_code, true );
				glsl_code << ", ";
				int right_degree = this->get_right_child()->glsl_poly_arith( glsl_code, true );
				degree = max( left_degree, right_degree );
				glsl_code << ", " << degree << " )";
			}
			break;
		case POLY_MUL:
			{
				glsl_code << "mult( ";
				int left_degree = this->get_left_child()->glsl_poly_arith( glsl_code, true );
				glsl_code << ", ";
				int right_degree = this->get_right_child()->glsl_poly_arith( glsl_code, true );
				degree = left_degree + right_degree;
				glsl_code << ", " << degree << " )";
			}
			break;
		case POLY_POW:
			{
				int exponent = this->get_right_child()->get_int_value();
				if( ( this->get_left_child()->type == POLY_X || this->get_left_child()->type == POLY_Y || this->get_left_child()->type == POLY_Z ) && exponent > 8 )
				{
					glsl_code << "power_1( ";
					int inner_degree = this->get_left_child()->glsl_poly_arith( glsl_code, true );
					glsl_code << ", " << exponent << " )";
					degree = inner_degree * exponent;
				}
				else
				{
					glsl_code << "power( ";
					int inner_degree = this->get_left_child()->glsl_poly_arith( glsl_code, true );
					glsl_code << ", " << exponent <<  ", " << inner_degree << " )";
					degree = inner_degree * exponent;
				}
			}
			break;
		case PARAM:
			glsl_code << "param_" << this->get_name();
			degree = 0;
			break;
		case PARAM_NEG:
			glsl_code << "-( ";
			this->get_left_child()->glsl_poly_arith( glsl_code, false );
			glsl_code << " )";
			degree = 0;
			break;
		case PARAM_OPERATOR:
			glsl_code << "( ";
			this->get_left_child()->glsl_poly_arith( glsl_code, false );
			glsl_code << " ) " << this->get_name() << " ( ";
			this->get_right_child()->glsl_poly_arith( glsl_code, false );
			glsl_code << " )";
			degree = 0;
			break;
		case PARAM_FUNC_1:
			glsl_code << this->get_name() << "( ";
			this->get_left_child()->glsl_poly_arith( glsl_code, false );
			glsl_code << " )";
			degree = 0;
			break;
		case PARAM_FUNC_2:
			glsl_code << this->get_name() << "( ";
			this->get_left_child()->glsl_poly_arith( glsl_code, false );
			glsl_code << ", ";
			this->get_right_child()->glsl_poly_arith( glsl_code, false );
			glsl_code << " )";
			degree = 0;
			break;
		case CONSTANT:
			glsl_code << fmt_float( this->get_float_value() );
			degree = 0;
			break;
		case POSINT:
			glsl_code << this->get_int_value();
			degree = 0;
			break;
		default:
			degree = -1;
	}
	if( wrap_params_and_constants && this->is_constant_or_param() )
		glsl_code << " )";

	return degree;
}

void Expression::glsl_formula( std::ostringstream &glsl_code )
{
	switch( this->type )
	{
		case POLY_X:
			glsl_code << "x";
			break;
		case POLY_Y:
			glsl_code << "y";
			break;
		case POLY_Z:
			glsl_code << "z";
			break;
		case POLY_NEG:
			glsl_code << "-(";
			this->get_left_child()->glsl_formula( glsl_code );
			glsl_code << ")";
			break;
		case POLY_ADD:
			glsl_code << "(";
			this->get_left_child()->glsl_formula( glsl_code );
			glsl_code << ")+(";
			this->get_right_child()->glsl_formula( glsl_code );
			glsl_code << ")";
			break;
		case POLY_SUB:
			glsl_code << "(";
			this->get_left_child()->glsl_formula( glsl_code );
			glsl_code << ")-(";
			this->get_right_child()->glsl_formula( glsl_code );
			glsl_code << ")";
			break;
		case POLY_MUL:
			glsl_code << "(";
			this->get_left_child()->glsl_formula( glsl_code );
			glsl_code << ")*(";
			this->get_right_child()->glsl_formula( glsl_code );
			glsl_code << ")";
			break;
		case POLY_POW:
			if( this->get_right_child()->int_value <= 3 && ( this->get_left_child()->type == POLY_X || this->get_left_child()->type == POLY_Y || this->get_left_child()->type == POLY_Z ) )
			{
				std::ostringstream tmp;
				this->get_left_child()->glsl_formula( tmp );
				glsl_code << "(" << tmp.str();
				for( unsigned int i = 1; i < this->get_right_child()->int_value; i++ )
					glsl_code << "*" << tmp.str();
				glsl_code << ")";
			}
			else
			{
				glsl_code << "power(";
				this->get_left_child()->glsl_formula( glsl_code );
				glsl_code << ",";
				this->get_right_child()->glsl_formula( glsl_code );
				glsl_code << ")";
			}
			break;
		case PARAM:
			glsl_code << "param_" << this->get_name();
			break;
		case PARAM_NEG:
			glsl_code << "-(";
			this->get_left_child()->glsl_formula( glsl_code );
			glsl_code << ")";
			break;
		case PARAM_OPERATOR:
			glsl_code << "(";
			this->get_left_child()->glsl_formula( glsl_code );
			glsl_code << ")" << this->get_name() << "(";
			this->get_right_child()->glsl_formula( glsl_code );
			glsl_code << ")";
			break;
		case PARAM_FUNC_1:
			glsl_code << this->get_name() << "(";
			this->get_left_child()->glsl_formula( glsl_code );
			glsl_code << ")";
			break;
		case PARAM_FUNC_2:
			glsl_code << this->get_name() << "(";
			this->get_left_child()->glsl_formula( glsl_code );
			glsl_code << ", ";
			this->get_right_child()->glsl_formula( glsl_code );
			glsl_code << ")";
			break;
		case CONSTANT:
			glsl_code << fmt_float( this->get_float_value() );
			break;
		case POSINT:
			glsl_code << this->get_int_value();
			break;
		default:
			break;
	}	
}

string Expression::fmt_float( float f )
{
	if( -f == numeric_limits<float>::infinity() )
	{
		return string( "(-1.0/0.0)" );
	}
	else if( f == numeric_limits<float>::infinity()
		|| f == numeric_limits<float>::quiet_NaN()
		|| f == numeric_limits<float>::signaling_NaN() )
	{
		return string( "(1.0/0.0)" );
	}
	else
	{
		ostringstream result;
		result << scientific << f;
		return result.str();
	}
}
