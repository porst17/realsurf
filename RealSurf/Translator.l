%{
	#include <cstdlib>
	#include <cstdio>
	#include <cmath>
	#include <string>
	
	#include "Expression.h"
	
	using namespace std;
	using namespace translator;
	
	#include "Parser.hpp"
%}

%x CMNT
%x LINE_CMNT

%%
	/* skip comments */
"/*"										{ BEGIN CMNT; /* switch to comment mode */ }
<CMNT>. |
<CMNT>[ \t\r\n];							/* throw away any comment text */
<CMNT>"*/"									{ BEGIN INITIAL; /* return to regular mode */ }
<CMNT><<EOF>>								{ BEGIN LINE_CMNT; return YY_MISSING_END_OF_COMMENT; }
"//"										{ BEGIN LINE_CMNT; /* switch to line comment mode */ }
<LINE_CMNT>. |
<LINE_CMNT>[ \t\r];							/* throw away any comment text */
<LINE_CMNT><<EOF>> |
<LINE_CMNT>[\n]								{ BEGIN INITIAL; /* return to regular mode */ }

	/* reserved variables */
[xX]										{ return YY_X; }
[yY]										{ return YY_Y; }
[zZ]										{ return YY_Z; }

	/* constant names */ 
"PI"										{ yylval.float_val = 3.14159265358979323846f; return YY_FLOAT; }
"E"											{ yylval.float_val = 2.71828182845904523536f; return YY_FLOAT; }

	/* function names */
"sin"										{ return YY_SIN; }
"cos"										{ return YY_COS; }
"tan"										{ return YY_TAN; }
"asin"										{ return YY_ASIN; }
"acos"										{ return YY_ACOS; }
"atan"										{ return YY_ATAN; }
"atan2"										{ return YY_ATAN2; }
"exp"										{ return YY_EXP; }
"log"										{ return YY_LOG; }
"sqrt"										{ return YY_SQRT; }
"pow"										{ return YY_POW; }
"ceil"										{ return YY_CEIL; }
"floor"										{ return YY_FLOOR; }
"abs"										{ return YY_ABS; }
"sign"										{ return YY_SIGN; }

	/* positive integers */
[0-9]+										{ yylval.int_val = atoi( yytext ); return YY_INTEGER; }

	/* floats */
[0-9]+(\.[0-9]*)?([Ee][-+]?[0-9]+)?				{ yylval.float_val = float( atof( yytext ) ); return YY_FLOAT; }


([a-zA-Z][a-zA-Z0-9_]*[a-zA-Z0-9])|[a-wA-W]	{ strncpy( yylval.name, yytext, 29 ); return YY_IDENTIFIER; }
									
	/* operators */
[-+()/*^,]									return *yytext;

	/* skip whitespace */
[ \t\r\n]									{ /* ignore whitespace */ }

	/* anything else is an error */
.											{ return YY_INVALID_CHARACTER; }
%%

int yywrap()
{
	return 1;
}

