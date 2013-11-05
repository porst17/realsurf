%error-verbose

%token <int_val> YY_INTEGER "integer"
%token <float_val> YY_FLOAT "float"
%token <expr> YY_X "x"
%token <expr> YY_Y "y"
%token <expr> YY_Z "z"
%token <expr> YY_NEG '-'
%token <name> YY_IDENTIFIER "identifier"
%token YY_INVALID_CHARACTER "invalid char"
%token YY_MISSING_END_OF_COMMENT

	/* operator tokens */
%token YY_SIN
%token YY_COS
%token YY_TAN
%token YY_ASIN
%token YY_ACOS
%token YY_ATAN
%token YY_ATAN2
%token YY_EXP
%token YY_LOG
%token YY_SQRT
%token YY_POW
%token YY_CEIL
%token YY_FLOOR
%token YY_ABS
%token YY_SIGN

%left '+' '-'
%left '*' '/'
%nonassoc YY_NEG
%left '^'

%type <expr> program polynomial parameter constant

%{
	#include <cstdio>
	#include <cstdlib>
	#include <cmath>
	#include <string>
	#include <sstream>
	#include <set>
	
	#include "Expression.h"
	
	#define YY_NEVER_INTERACTIVE 1
/*
#ifndef NDEBUG
	#define YYDEBUG 1
#endif
*/
	#include "Lexer.hpp"
	
	
	using namespace translator;
	using namespace std;
	
	Expression *root = NULL;
	string error_message;
	
	void yyerror( char const *msg );
	int yylex( void );
	
	static Expression *polyMerge( YYSTYPE x0, YYSTYPE x1 ) { return x0.expr; }
%}

%union
{
	unsigned int int_val;
	float float_val;
	char name[ 30 ];
	Expression * expr;
}
%%
program:
	polynomial									{ root = $1; }
	| parameter									{ root = $1; }

constant:
	YY_FLOAT									{ $$ = new Expression( $1 ); }
	| YY_INTEGER								{ $$ = new Expression( float( $1 ) ); }

parameter:
	constant									{ $$ = $1; }
	| YY_IDENTIFIER								{ $$ = new Expression( std::string( $1 ) ); }
	| parameter '+' parameter					{ $$ = $1->is_constant() && $3->is_constant() ? new Expression( $1->get_number_as_float() + $3->get_number_as_float() ) : new Expression( "+", true, $1, $3 ); if( $1->is_constant() && $3->is_constant() ) { delete $1; delete $3; } }
	| parameter '-' parameter					{ $$ = $1->is_constant() && $3->is_constant() ? new Expression( $1->get_number_as_float() - $3->get_number_as_float() ) : new Expression( "-", true, $1, $3 ); if( $1->is_constant() && $3->is_constant() ) { delete $1; delete $3; } }
	| parameter '*' parameter					{ $$ = $1->is_constant() && $3->is_constant() ? new Expression( $1->get_number_as_float() * $3->get_number_as_float() ) : new Expression( "*", true, $1, $3 ); if( $1->is_constant() && $3->is_constant() ) { delete $1; delete $3; } }
	| parameter '/' parameter					{ $$ = $1->is_constant() && $3->is_constant() ? new Expression( $1->get_number_as_float() / $3->get_number_as_float() ) : new Expression( "/", true, $1, $3 ); if( $1->is_constant() && $3->is_constant() ) { delete $1; delete $3; } }
	| '-' parameter %prec YY_NEG				{ $$ = $2->is_constant() ? new Expression( -( $2->get_number_as_float() ) ) : new Expression( "-", $2 ); if( $2->is_constant() ) { delete $2; } }
	| parameter '^' YY_INTEGER					{
													if( $3 == 0 )
														$$ = new Expression( 0.0f );
													else if( $3 == 1 )
														$$ = $1;
													else
													{
														if( $1->is_constant() )
															$$ = new Expression( pow( $1->get_number_as_float(), float( $3 ) ) );
														else
															$$ = new Expression( "power", false, $1, new Expression( $3 ) );
														
														if( $1->is_constant() )
															delete $1;
													}
												}
	| '(' parameter ')'							{ $$ = $2; }	
	| YY_SIN '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( sin( $3->get_number_as_float() ) ) : new Expression( "sin", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_COS '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( cos( $3->get_number_as_float() ) ) : new Expression( "cos", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_TAN '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( tan( $3->get_number_as_float() ) ) : new Expression( "tan", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_ASIN '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( asin( $3->get_number_as_float() ) ) : new Expression( "asin", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_ACOS '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( acos( $3->get_number_as_float() ) ) : new Expression( "acos", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_ATAN '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( atan( $3->get_number_as_float() ) ) : new Expression( "atan", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_ATAN2 '(' parameter ',' parameter ')'	{ $$ = $3->is_constant() && $5->is_constant() ? new Expression( atan2( $3->get_number_as_float(), $5->get_number_as_float() ) ) : new Expression( "atan", false, $3, $5 ); if( $3->is_constant() && $5->is_constant() ) { delete $3; delete $5; } }
	| YY_EXP '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( exp( $3->get_number_as_float() ) ) : new Expression( "exp", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_LOG '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( log( $3->get_number_as_float() ) ) : new Expression( "log", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_SQRT '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( sqrt( $3->get_number_as_float() ) ) : new Expression( "sqrt", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_POW '(' parameter ',' parameter ')'	{ $$ = $3->is_constant() && $5->is_constant() ? new Expression( pow( $3->get_number_as_float(), $5->get_number_as_float() ) ) : new Expression( "pow", false, $3, $5 ); if( $3->is_constant() && $5->is_constant() ) { delete $3; delete $5; } }
	| YY_CEIL '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( ceil( $3->get_number_as_float() ) ) : new Expression( "ceil", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_FLOOR '(' parameter ')'				{ $$ = $3->is_constant() ? new Expression( floor( $3->get_number_as_float() ) ) : new Expression( "floor", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_ABS '(' parameter ')'					{ $$ = $3->is_constant() ? new Expression( fabs( $3->get_number_as_float() ) ) : new Expression( "abs", $3 ); if( $3->is_constant() ) delete $3; }
	| YY_SIGN '(' parameter ')'					{
													if( $3->is_constant() )
													{
														float sign;
														float val = $3->get_number_as_float();
														if( val == 0.0f )
															sign = 0.0f;
														else if( val < 0.0f )
															sign = -1.0f;
														else if( val > 0.0f )
															sign = 1.0f;
														else
															sign = val;
														$$ = new Expression( sign );
														delete $3;
													}
													else
													{
														$$ = new Expression( "sign", $3 );
													}
												}

polynomial:
	//parameter									{ $$ = $1; }
	YY_X										{ $$ = new Expression( Expression::VAR_X ); }
	| YY_Y										{ $$ = new Expression( Expression::VAR_Y ); }
	| YY_Z										{ $$ = new Expression( Expression::VAR_Z ); }
	| parameter '+' polynomial					{ $$ = new Expression( Expression::POLY_ADD, $1, $3 ); }
	| polynomial '+' polynomial					{ $$ = new Expression( Expression::POLY_ADD, $1, $3 ); }
	| polynomial '+' parameter					{ $$ = new Expression( Expression::POLY_ADD, $1, $3 ); }
	| parameter '-' polynomial					{ $$ = new Expression( Expression::POLY_SUB, $1, $3 ); }
	| polynomial '-' parameter					{ $$ = new Expression( Expression::POLY_SUB, $1, $3 ); }
	| polynomial '-' polynomial					{ $$ = new Expression( Expression::POLY_SUB, $1, $3 ); }
	| parameter '*' polynomial					{ $$ = new Expression( Expression::POLY_MUL, $1, $3 ); }
	| polynomial '*' parameter					{ $$ = new Expression( Expression::POLY_MUL, $1, $3 ); }
	| polynomial '*' polynomial					{ $$ = new Expression( Expression::POLY_MUL, $1, $3 ); }
	| polynomial '/' parameter					{ $$ = new Expression( Expression::POLY_MUL, $1, new Expression( "/", true, new Expression( 1.0f ), $3 ) ); }
	| '-' polynomial %prec YY_NEG				{ $$ = new Expression( Expression::POLY_NEG, $2, NULL ); }
	| polynomial '^' YY_INTEGER					{
													if( $3 == 0 )
														$$ = new Expression( 0.0f );
													else if( $3 == 1 )
														$$ = $1;
													else
													{
														if( $1->is_constant() )
															$$ = new Expression( pow( $1->get_number_as_float(), float( $3 ) ) );
														else if( $1->is_constant_or_param() )
															$$ = new Expression( "power", false, $1, new Expression( $3 ) );
														else
															$$ = new Expression( Expression::POLY_POW, $1, new Expression( $3 ) );
															
														if( $1->is_constant() )
															delete $1;
													}
												}
	| '(' polynomial ')'						{ $$ = $2; }

%%

	void yyerror( char const *msg )
	{
		switch( yychar )
		{
			case YY_MISSING_END_OF_COMMENT:
				error_message = "syntax error, missing end of comment";
				break;
			case YY_INVALID_CHARACTER:
				error_message = "syntax error, invalid character '" + string( yytext ) + "'";
				break;
			default:
				error_message = string( msg );
				break;
		}
	}
	
	namespace translator
	{
		bool translate_formula( const string formula, unsigned int &degree, ostream &glsl_code, set<string> &params, ostream &error_msg )
		{
#if YYDEBUG == 1
			yydebug = 1;
#endif
			YY_BUFFER_STATE my_string_buffer = yy_scan_string( formula.c_str() );
			yyparse();
			yy_delete_buffer( my_string_buffer );
			
			bool success = ( error_message == "" );
			if( success )
			{
				// no error detected
				degree = root->to_glsl( glsl_code, params );
			}
			else
			{
				// error detected
				error_msg << error_message;
				error_message = string( "" );
			}
			
			if( root != NULL )
			{
				delete root;
				root = NULL;
			}
			
			return success;
		}
	};
