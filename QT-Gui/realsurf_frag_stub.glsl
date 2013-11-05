#define MAX_LIGHTS 3

uniform mat4 surface_transform_inverse;
uniform mat4 surface_transform;

uniform int clip_to;

varying vec3 eye_space_eye;
varying vec3 eye_space_dir;
varying vec3 surface_space_eye;
varying vec3 surface_space_dir;
varying vec3 clipping_space_eye;
varying vec3 clipping_space_dir;

vec3 ese_opt, esd_opt, cse_opt, csd_opt, sse_opt, ssd_opt;

polynomial calc_coefficients( vec3 eye, vec3 dir );
vec3 gradient( vec3 p );

#define CLIP_TO_SPHERE 1
#define CLIP_TO_CUBE 2

#ifdef METHOD_LAGRANGE_INTERPOLATION
/******************************************************
 * Die hier verwendete Interpolationsmethode ist eine
 * Portierung des optimierten Lagrange-Algorithmus aus
 *
 *  Numerical Recipes in C: The Art of Scientific Computing
 *  William H. Press et al.
 *  Cambridge University Press
 *  2. Auflage (1996)
 *  ISBN 0-521-43108-5
 *  Seite 121
 ******************************************************/
polynomial calc_coefficients( vec3 eye, vec3 dir, vec2 clipping_interval )
{
	// DEGREE + 1 Stützpunkte auf Strahl eye + t * pos berechnen
	float y[ SIZE ];
	float x[ SIZE ];
	float ci_width = clipping_interval[ 1 ] - clipping_interval[ 0 ];
	for( int i = 0; i <= DEGREE; i++ )
	{
		x[ i ] = clipping_interval[ 0 ] + ( float( i ) / float( DEGREE ) ) * ci_width;
		vec3 p = eye + x[ i ] * dir;
		y[ i ] = f( p.x, p.y, p.z );
	}

	polynomial p;
	float s[ SIZE ];
	for( int i = 0; i < SIZE; i++ )
		s[ i ] = p.a[ i ] = 0.0;
	s[ SIZE - 1 ] = -x[ 0 ];
	for( int i = 1; i < SIZE; i++ )
	{
		for( int j = SIZE - 1 - i; j < SIZE - 1; j++ )
			s[ j ] -= x[ i ] * s[ j + 1 ];
		s[ SIZE - 1 ] -= x[ i ];
	}
	for( int j = 0; j < SIZE; j++ )
	{
		float phi = float( SIZE );
		for( int k = SIZE - 1; k >= 1; k-- )
			phi = float( k ) * s[ k ] + x[ j ] * phi;
		float ff = y[ j ] / phi;
		float b = 1.0;
		for( int k = SIZE - 1; k >= 0; k-- )
		{
			p.a[ k ] += b * ff;
			b = s[ k ] + x[ j ] * b;
		}
	}
	return p;
}
#endif

#ifdef METHOD_NEWTON_INTERPOLATION
polynomial calc_coefficients( vec3 eye, vec3 dir, vec2 clipping_interval )
{
	// DEGREE + 1 Stützpunkte auf Strahl eye + t * pos berechnen
	float y[ SIZE ];
	float x[ SIZE ];
	float ci_width = clipping_interval[ 1 ] - clipping_interval[ 0 ];
	for( int i = 0; i <= DEGREE; i++ )
	{
		x[ i ] = clipping_interval[ 0 ] + ( float( i ) / float( DEGREE ) ) * ci_width;
		vec3 p = eye + x[ i ] * dir;
		y[ i ] = f( p.x, p.y, p.z );
	}

	// dividierte Differenzen berechen
	for( int i = 1; i <= DEGREE; i++ )
		for( int j = DEGREE; j >= i; j-- )
			y[ j ] = ( y[ j ] - y[ j - 1 ] ) / ( x[ j ] - x[ j - i ] );

	// schrittweise Koeffizienten mit Newton-Interpolationsformel berechnen
	polynomial p;
	float newton_basis[ SIZE ];

	newton_basis[ DEGREE ] = 1.0;
	p.a[ 0 ] = y[ 0 ];
	
	for( int i = 1; i <= DEGREE; i++ )
	{
		// ( ai*x^i + ... + a0 ) + ( ai*x^i + ... + a0 ) * ( x - x[ i ] ) * y[ i ] = ( ai*x^i + ... + a0 ) * x - x[ i ] * y[ i ] * ( ai*x^i + ... + a0 ) berechnen
		// 1. Koeffizienten der Newton-Basis um eine Potenz erhöhen (=shiften)
		newton_basis[ DEGREE - i ] = 0.0;
		p.a[ i ] = 0.0;
		
		// 2. alte Koeffizienten der Newton-Basis multipliziert mit x[ i - 1 ] subtrahieren
		for( int j = DEGREE - i; j < DEGREE; j++ )
			newton_basis[ j ] = newton_basis[ j ] - newton_basis[ j + 1 ] * x[ i - 1 ];

		// 3. y[ i ] * ( neue Newton-Basis ) auf alte Koeffizienten addieren
		for( int j = 0; j <= i; j++ )
			p.a[ j ] += newton_basis[ DEGREE - i + j ] * y[ i ];
	}
	return p;
}
#endif

#ifdef METHOD_COMPUTER_ALGEBRA
polynomial create_poly_0( float a0 )
{
	polynomial res;
	for( int i = 1; i <= DEGREE; i++ )
		res.a[ i ] = 0.0;
	res.a[ 0 ] = a0;
	return res;
}

polynomial create_poly_1( float a0, float a1 )
{
	polynomial res;
#if DEGREE > 1
	for( int i = 2; i <= DEGREE; i++ )
		res.a[ i ] = 0.0;
#endif
	res.a[ 0 ] = a0;
	res.a[ 1 ] = a1;
	return res;
}

polynomial add( polynomial p1, polynomial p2, int res_degree )
{
	for( int i = 0; i <= res_degree; i++ )
		p1.a[ i ] += p2.a[ i ];
	return p1;
}

polynomial sub( polynomial p1, polynomial p2, int res_degree )
{
	for( int i = 0; i <= res_degree; i++ )
		/* works with this line: */ // p1.a[ i ] = -( p2.a[ i ] - p1.a[ i ] );
		p1.a[ i ] = p1.a[ i ] - p2.a[ i ];
		
	return p1;
}

polynomial mult( polynomial p1, polynomial p2, int res_degree )
{
	polynomial res = p1;
	for( int i = 0; i <= res_degree; i++ )
	{
		res.a[ i ] = 0.0;
		for( int j = 0; j <= i; j++ )
			res.a[ i ] += p1.a[ j ] * p2.a[ i - j ];
	}
	return res;
}

polynomial neg( polynomial p, int res_degree )
{
	for( int i = 0; i <= res_degree; i++ )
		p.a[ i ] = -p.a[ i ];
	return p;
}
/*
// in Theorie schneller .. in Praxis bei kleinen Potenzen nicht ;-)
// beruht auf Idee: a^n = (a^(n/2))^2
polynomial power( polynomial p, int exp, int degree )
{
	polynomial result = create_poly_0( 1.0 );

	if( exp != 0 )
	{
		polynomial y = p;
		int n = exp;
		int m = exp;
		for( int i = 0; i < int( log2( float( exp ) ) ); i++ )
		{
			m = m / 2;
			degree = degree + degree;
			if( n > 2 * m )
				result = mult( result, y, degree );
			y = mult( y, y, degree );
			n = m;
		}
		result = mult( result, y, degree );
	}
	return result;
}
*/
polynomial power( polynomial p, int exp, int degree )
{
	polynomial res = create_poly_0( 1.0 );
	for( int res_degree = degree; res_degree < degree * exp + 1; res_degree += degree )
		res = mult( res, p, res_degree );
	return res;
}

polynomial power_1( polynomial p, int exp )
{
//	return power( p, exp, 1 );

	// compute powers of p.a[ 0 ] and p.a[ 1 ]
	float a0 = p.a[ 0 ];
	float a1 = p.a[ 1 ];
	float powers_0[ SIZE ];
	float powers_1[ SIZE ];
	powers_0[ 0 ] = 1.0;
	powers_0[ 1 ] = a0;
	powers_1[ 0 ] = 1.0;
	powers_1[ 1 ] = a1;
	for( int i = 2; i <= exp; i++ )
	{
		powers_0[ i ] = powers_0[ i - 1 ] * a0;
		powers_1[ i ] = powers_1[ i - 1 ] * a1;
	}
	
	// compute coefficients of polynomials by binomial expansion
	polynomial res = create_poly_0( 0.0 );
	int a1_exp = exp;
	int a0_exp = 0;
	int bin_coeff = 1;
	for( int deg = exp; deg >= 0; deg-- )
	{
		res.a[ deg ] = float( bin_coeff ) * powers_1[ a1_exp ] * powers_0[ a0_exp ];
		a0_exp++;
		bin_coeff = ( bin_coeff * a1_exp ) / a0_exp;
		a1_exp--;
	}
	return res;
}
#endif

// Ersatz für fehlerhafte NVidia-pow-Funktion ...
float power( float base, int exp )
{
	float res = 1.0;
	for( int i = 0; i < exp; i++ )
		res *= base;
	return res;
}

/**
 * methods, that operate with that algebraic function
 */
#ifdef METHOD_DESCARTES
#extension GL_EXT_gpu_shader4 : enable
float epsilon = 0.0001;
struct roots
{
	float x[ DEGREE + 2 ];
	bool valid[ DEGREE + 2 ];
};

float eval_p( const in polynomial p, float x )
{
	float fx = p.a[ SIZE - 1 ];
	for( int i = SIZE - 2; i >= 0; i-- )
		fx = fx * x + p.a[ i ];
	return fx;
}

float bisect( const in polynomial p, float lowerBound, float upperBound )
{
	float center = lowerBound;
	float old_center = upperBound;
	float fl = eval_p( p, lowerBound );
	float fu = eval_p( p, upperBound );

	while( abs( upperBound - lowerBound ) > epsilon )
	{
		old_center = center;
		center = 0.5 * ( lowerBound + upperBound );
		float fc = eval_p( p, center );
		
		if( fc * fl < 0.0 )
		{
			upperBound = center;
			fu = fc;
		}
		else if( fc == 0.0 )
		{
			break;
		}
		else
		{
			lowerBound = center;
			fl = fc;
		}
	}
	return ( upperBound + lowerBound ) * 0.5;
}
/*
int stretchShiftDescartesRuleOfSignReverseShift1( polynomial p, float scale, float shift, inout polynomial tmpCoeffs )
{
	float multiplier = 1.0;//pow( scale, -float( DEGREE ) );
	for( int i = 0; i < SIZE; i++ )
	{
		tmpCoeffs.a[ SIZE - 1 - i ] = multiplier * p.a[ i ];
		multiplier *= scale;
	}
	for( int i = 1; i <= SIZE; i++ )
		for( int j = SIZE - 2; j >= i - 1; j-- )
			tmpCoeffs.a[ SIZE - 1 - j ] = tmpCoeffs.a[ SIZE - 1 - j ] + shift * tmpCoeffs.a[ SIZE - 1 - ( j + 1 ) ];    
	
	int signChanges = 0;
	
	float lastNonZeroCoeff = 0.0;
	for( int i = 1; i <= SIZE; i++ )
	{
		for( int j = SIZE - 2; j >= i - 1; j-- )
			tmpCoeffs.a[ j ] = tmpCoeffs.a[ j ] + tmpCoeffs.a[ j + 1 ];
		if( tmpCoeffs.a[ i - 1 ] != 0.0 )
		{
			if( tmpCoeffs.a[ i - 1 ] * lastNonZeroCoeff < 0.0 )
				signChanges++;
			if( signChanges > 1 )
				return signChanges;
			lastNonZeroCoeff = tmpCoeffs.a[ i - 1 ];
		}
	}
	
	return signChanges;
}

polynomial stretchNormalize0_5( polynomial p )
{
	polynomial result;
	result.a[ SIZE - 1 ] = p.a[ SIZE - 1 ];
	float multiplier = 2.0;
	for( int i = SIZE - 2; i >= 0; i-- )
	{
		result.a[ i ] = p.a[ i ] * multiplier;
		multiplier *= 2.0;
	}
	
	return result;
}

polynomial shift1( polynomial p )
{
	for( int i = 1; i <= SIZE; i++ )
		for( int j = SIZE - 2; j >= i - 1; j-- )
			p.a[ j ] = p.a[ j ] + p.a[ j + 1 ];
	return p;
}

int descartesRuleOfSignReverseShift1( polynomial p )
{
	int signChanges = 0;
	float[ SIZE ] hornerCoeffs;
	for( int i = 0; i < SIZE; i++ )
		hornerCoeffs[ i ] = p.a[ SIZE - i - 1 ];
	
	float lastNonZeroCoeff = 0.0;
	for( int i = 1; i <= SIZE; i++ )
	{
		for( int j = SIZE - 2; j >= i - 1; j-- )
			hornerCoeffs[ j ] = hornerCoeffs[ j ] + hornerCoeffs[ j + 1 ];
		if( hornerCoeffs[ i - 1 ] != 0.0 )
		{
			if( hornerCoeffs[ i - 1 ] * lastNonZeroCoeff < 0.0 )
				signChanges++;
			if( signChanges > 1 )
				return signChanges;
			lastNonZeroCoeff = hornerCoeffs[ i - 1 ];
		}
	}
	
	return signChanges;
}

polynomial stretchShift( polynomial p, float scale, float shift )
{
	polynomial result;
	float multiplier = 1.0;
	for( int i = 0; i < SIZE; i++ )
	{
		result.a[ i ] = multiplier * p.a[ i ];
		multiplier *= scale;
	}
	for( int i = 1; i <= SIZE; i++ )
		for( int j = SIZE - 2; j >= i - 1; j-- )
			result.a[ j ] = result.a[ j ] + shift * result.a[ j + 1 ];    
	return result;
}

polynomial stretch( polynomial p, float scale )
{
	polynomial result;
	float multiplier = pow( scale, -0.5 * float( DEGREE ) );
	for( int i = 0; i < SIZE; i++ )
	{
		result.a[ i ] = multiplier * p.a[ i ];
		multiplier *= scale;
	}
	return result;
}

polynomial shift( polynomial p, float shift )
{
	polynomial result;
	for( int i = 0; i < SIZE; i++ )
		result.a[ i ] = p.a[ i ];
	for( int i = 1; i <= SIZE; i++ )
		for( int j = SIZE - 2; j >= i - 1; j-- )
			result.a[ j ] = result.a[ j ] + shift * result.a[ j + 1 ];    
	return result;
}
*/

int shiftStretchDescartesRuleOfSignReverseShift1( in polynomial p, float shift, float scale, inout polynomial tmpCoeffs )
{
	for( int i = 0; i < SIZE; i++ )
		tmpCoeffs.a[ SIZE - 1 - i ] = p.a[ i ];
	
	for( int i = 1; i <= SIZE; i++ )
		for( int j = SIZE - 2; j >= i - 1; j-- )
			tmpCoeffs.a[ SIZE - 1 - j ] = tmpCoeffs.a[ SIZE - 1 - j ] + shift * tmpCoeffs.a[ SIZE - 1 - ( j + 1 ) ];    
	
	float multiplier = 1.0;
	for( int i = 0; i < SIZE; i++ )
		multiplier *= 1.0 / scale;
	for( int i = 0; i < SIZE; i++ )
	{
		tmpCoeffs.a[ SIZE - 1 - i ] = multiplier * tmpCoeffs.a[ SIZE - 1 - i ];
		multiplier *= scale;
	}
	
	if( tmpCoeffs.a[ SIZE - 1 ] == 0.0 )
		return -1;

	int signChanges = 0;
	
	float lastNonZeroCoeff = 0.0;
	for( int i = 1; i <= SIZE; i++ )
	{
		for( int j = SIZE - 2; j >= i - 1; j-- )
			tmpCoeffs.a[ j ] = tmpCoeffs.a[ j ] + tmpCoeffs.a[ j + 1 ];
		if( tmpCoeffs.a[ i - 1 ] != 0.0 )
		{
			if( tmpCoeffs.a[ i - 1 ] * lastNonZeroCoeff < 0.0 )
				signChanges++;
			if( signChanges > 1 )
				return signChanges;
			lastNonZeroCoeff = tmpCoeffs.a[ i - 1 ];
		}
	}
	
	return signChanges;
}

float findSmallestPositveRootBelow( in polynomial p, const float upperBound, inout polynomial tmp )
{
	if( shiftStretchDescartesRuleOfSignReverseShift1( p, 0.0, upperBound, tmp ) > 0 )
	{
		float size = 0.5;
		int id = 0;
		while( size < 1.0 )
		{
			float scale = upperBound * size;
			int v = shiftStretchDescartesRuleOfSignReverseShift1( p, scale * float( id ), scale, tmp );
			
			if( v > 1 )
			{
				// go deeper on left side
				id *= 2;
				size /= 2.0;
			}
			else if( v == 0 )
			{
				// go right
				while( id % 2 == 1 )
				{
					id /= 2;
					size *= 2.0;
				}
				id++;
			}
			else if( v == 1 )
			{
				return bisect( p, scale * float( id ), scale * float( id + 1 ) );
			}
			else
			{
				return scale * float( id );
			}
		}
	}
	return upperBound + 1.0;
}

/**
 * uses nearly the same algorithm as for positive roots:
 *  1. transforms x |-> -x (the new task is to find the largest positive root below -lowerBound)
 *  2. use bisection as in findSmallestPositveRootBelow, but start from the upper bound (=-lowerBound) of the search interval
 */
float findSmallestNegativeRootAbove( in polynomial p, const float lowerBound, inout polynomial tmp )
{
	for( int i = 0; i < SIZE; i++ )
		if( i % 2 == 1 )
			p.a[ i ] = -p.a[ i ];

	float upperBound = -lowerBound;
	if( shiftStretchDescartesRuleOfSignReverseShift1( p, 0.0, upperBound, tmp ) > 0 )
	{
		float size = 0.5;
		int id = 0;
		while( size < 1.0 )
		{
			float intervalStart = upperBound * ( 1.0 - size * float( id + 1 ) );
			int v = shiftStretchDescartesRuleOfSignReverseShift1( p, intervalStart, size * upperBound, tmp );
			
			if( v > 1 )
			{
				// go deeper on left side
				id *= 2;
				size /= 2.0;
			}
			else if( v == 0 )
			{
				// go right
				while( id % 2 == 1 )
				{
					id /= 2;
					size *= 2.0;
				}
				id++;
			}
			else if( v == 1 )
			{
				return -bisect( p, intervalStart, intervalStart + upperBound * size );
			}
			else
			{
				return intervalStart;
			}
		}
	}
	return lowerBound - 1.0;
}

float findFirstRootIn( in polynomial p, float lowerBound, float upperBound )
{
	polynomial tmp;
	if( lowerBound < 0.0 )
	{
		float root = findSmallestNegativeRootAbove( p, lowerBound, tmp );
		if( root >= lowerBound )
			return root;
	}
	if( p.a[ 0 ] == 0.0 )
		return 0.0;
	if( upperBound > 0.0 )
	{
		return findSmallestPositveRootBelow( p, upperBound, tmp );
	}
	return lowerBound - 1.0;
}

roots solve( in polynomial p, const in vec2 trace_interval )
{
	float intervalSize = trace_interval[ 1 ] - trace_interval[ 0 ];
	
	// init result array
	roots res;
	res.x[ 0 ] = trace_interval[ 0 ];
	res.valid[ 0 ] = false;
	for( int i = 1; i < DEGREE + 2; i++ )
	{
		res.x[ i ] = trace_interval[ 1 ];
		res.valid[ i ] = false;
	}

	res.x[ 1 ] = findFirstRootIn( p, trace_interval[ 0 ], trace_interval[ 1 ] );
	if( res.x[ 1 ] >= trace_interval[ 0 ] && res.x[ 1 ] <= trace_interval[ 1 ] )
		res.valid[ 1 ] = true;
	
	return res;
}
#endif

#ifdef METHOD_STURM
float epsilon = 0.0001;
float sturm_chain[ ( SIZE * ( SIZE + 1 ) ) / 2 ];

int sc_index( int f_index, int c_index )
{

	//~ int result = 0;
	//~ for( int i = DEGREE; i > f_index; i-- )
		//~ result += i;
	//~ return result + c_index;

	return ( SIZE * ( SIZE + 1 ) ) / 2 - 1 - ( ( f_index + 1 ) * ( f_index + 2 ) ) / 2 + c_index;
}

float eval_f( float where )
{
	float res = 0.0;
	for( int i = DEGREE; i >= 0; i-- )
		res = sturm_chain[ sc_index( DEGREE, i ) ] + where * res;
	return res;
}

float bisection( float x0, float x1 )
{
	float f0 = eval_f( x0 );
	float f1 = eval_f( x1 );
	float x2 = x0;
	while( abs( x0 - x1 ) > epsilon )
	{
		x2 = 0.5 * ( x0 + x1 );
		float f2 = eval_f( x2 );
		if( f2 * f0 < 0.0 )
		{
			x1 = x2;
			f1 = f2;
		}
		else
		{
			x0 = x2;
			f0 = f2;
		}
	}
	return x2;
}

bool contains( const in vec2 interval, const in float value )
{
	return ( interval[ 0 ] < value && value < interval[ 1 ] ) || ( interval[ 0 ] > value && value > interval[ 1 ] );
}

void polynom_div( int dividend, int divisor, int remainder )
{
	// copy dividend to be the current remainder
	int i;
	for( i = 0; i <= dividend; i++ )
		sturm_chain[ sc_index( remainder, i )] = sturm_chain[ sc_index( dividend, i ) ];

	int degree_diff = dividend - divisor;
	for( i = dividend; i >= divisor; i-- )
	{
		// calculate quotient of highest coefficient
		float quotient = sturm_chain[ sc_index( remainder, i ) ] / sturm_chain[ sc_index( divisor, divisor ) ];
		
		// after this step the highest coeff. of the old remainder is actually zero
		//sturm_chain[ remainder * SIZE + i ] = 0.0; // unnecessary calculation, because value is known
		
		// calculate new coeffs. of the remainder
		for( int j = 0; j < divisor; j++ )
			sturm_chain[ sc_index( remainder, j + degree_diff ) ] = sturm_chain[ sc_index( remainder, j + degree_diff ) ] - sturm_chain[ sc_index( divisor, j ) ] * quotient;

		degree_diff--;
	}
}

void construct_sturm_chain()
{
	// calculate first derivate of f
	int i;
	//sturm_chain[ DEGREE - 1 ][ DEGREE ] = 0.0f;
	for( i = 1; i <= DEGREE; i++ )
		sturm_chain[ sc_index( DEGREE - 1, i - 1 ) ] = float( i ) * sturm_chain[ sc_index( DEGREE, i ) ];// / ( DEGREE * sturm_chain[ DEGREE ][ DEGREE ] );
	
	// calculate sturm chain
	for( i = DEGREE - 2; i >= 0; i-- )
	{
		// polynom division, which outputs the remainder to the sturm_chain-array
		polynom_div( i + 2, i + 1, i );
		
		// flip the sign of the remainder and normalize polynom
		for( int j = 0; j <= i; j++ )
			sturm_chain[ sc_index( i, j ) ] = -sturm_chain[ sc_index( i, j ) ];
	}
}

float f_sturm( int num, float t )
{
	float res = 0.0;
	for( int i = num; i >= 0; i-- )
		res = sturm_chain[ sc_index( num, i ) ] + t * res;
	return res;
}

int sign_change( float t )
{
	int sign_sum = 0;
	float last_sign, cur_sign;
	
	// #Vorzeichenwechsel an t berechnen
	last_sign = sign( f_sturm( DEGREE, t ) );

	for( int i = DEGREE - 1; i >= 0; i-- )
	{
		cur_sign = sign( f_sturm( i, t ) );
		sign_sum += ( last_sign != cur_sign ) ? 1 : 0;
		if( cur_sign != 0.0 )
			last_sign = cur_sign;
	}
	
	return sign_sum;
}

float bisection_sturm( float x0, float x1 )
{
	float x2 = x0 - 1.0;
	int sign_change_0 = sign_change( x0 );
	int sign_change_1 = sign_change( x1 );
	
	if( sign_change_0 - sign_change_1 != 0 )
	{
		{
			float f0 = eval_f( x0 );
			float f1 = eval_f( x1 );
			
			while( !( sign_change_0 - sign_change_1 == 1 && f0 * f1 < 0.0 ) )
			{
				x2 = 0.5 * ( x0 + x1 );
				
				int sign_change_2 = sign_change( x2 );
				float f2 = eval_f( x2 );
				
				if( sign_change_0 - sign_change_2 > 0 )
				{
					// there is a root in the first interval -> search in first
					x1 = x2;
					f1 = f2;
					sign_change_1 = sign_change_2;
				}
				else
				{
					// there is no root in the first interval -> search in second
					x0 = x2;
					f0 = f2;
					sign_change_0 = sign_change_2;
				}
			}
		}
		x2 = bisection( x0, x1 );
	}
	
	return x2;
}

struct roots
{
	float x[ DEGREE + 2 ];
	bool valid[ DEGREE + 2 ];
};

roots solve( const in polynomial p, const in vec2 trace_interval )
{
#if DEGREE > 1
	// fill	sturm chain array
	for( int i = 0; i < SIZE; i++ )
		sturm_chain[ sc_index( DEGREE, i ) ] = p.a[ i ];
	construct_sturm_chain();
#endif

	// init result array
	roots res;
	res.x[ 0 ] = trace_interval[ 0 ];
	res.valid[ 0 ] = false;
	for( int i = 1; i < DEGREE + 2; i++ )
	{
		res.x[ i ] = trace_interval[ 1 ];
		res.valid[ i ] = false;
	}

#if DEGREE > 1
	// apply sturm's algorithm
	res.x[ 1 ] = bisection_sturm( trace_interval[ 0 ], trace_interval[ 1 ] );
#else
	// solve linear equation directly
	res.x[ 1 ] = -p.a[ 0 ] / p.a[ 1 ];
#endif
	res.valid[ 1 ] = contains( trace_interval, res.x[ 1 ] );

	return res;
}
#endif
#ifdef METHOD_D_CHAIN
float epsilon = 0.000001;
polynomial derivatives[ DEGREE ];

float eval_f( int which, float where )
{
	float res = 0.0;
	for( int i = DEGREE - which; i >= 0; i-- )
		res = derivatives[ which ].a[ i ] + where * res;
	return res;
}

void calc_derivatives()
{
	for( int derivate = 1; derivate < DEGREE; derivate++ )
		for( int j = 0; j <= DEGREE - derivate; j++ )
			derivatives[ derivate ].a[ j ] = float( j + 1 ) * derivatives[ derivate - 1 ].a[ j + 1 ];
}

#ifdef SUB_METHOD_BISECTION
float bisection( int f_index, float x0, float f0, float x1, float f1 )
{
	float x2 = x0;
	while( abs( x0 - x1 ) > epsilon )
	{
		x2 = 0.5 * ( x0 + x1 );
		float f2 = eval_f( f_index, x2 );
		if( f2 * f0 < 0.0 )
		{
			x1 = x2;
			f1 = f2;
		}
		else
		{
			x0 = x2;
			f0 = f2;
		}
	}
	return x2;
}
#endif

#ifdef SUB_METHOD_REGULA_FALSI
float regula_falsi( int f_index, float x0, float f0, float x1, float f1 )
{
	float x2 = x0;
	while( abs( x0 - x1 ) > epsilon )
	{
		x2 =  x0 - f0 * ( x1 - x0 ) / ( f1 - f0 );
		float f2 = eval_f( f_index, x2 );
		if( f2 * f0 < 0.0 )
		{
			x1 = x2;
			f1 = f2;
		}
		else
		{
			x0 = x2;
			f0 = f2;
		}
	}
	return x2;
}
#endif

struct roots
{
	float x[ DEGREE + 2 ];
	bool valid[ DEGREE + 2 ];
};

bool contains( const in vec2 interval, const in float value )
{
	return ( interval[ 0 ] < value && value < interval[ 1 ] ) || ( interval[ 0 ] > value && value > interval[ 1 ] );
}

void calc_roots( const in int derivate_num, const in roots read_from, inout roots write_to )
{
	// for each root pair of the current derivate: look for roots of previous derivate
	for( int interval_num = 0; interval_num < DEGREE + 2 - 1 - derivate_num; interval_num++ )
	{
		write_to.x[ interval_num + 1 ] = write_to.x[ interval_num ];
		write_to.valid[ interval_num + 1 ] = false;
		
		if( read_from.valid[ interval_num + 1 ] )
		{
			float f0 = eval_f( derivate_num, read_from.x[ interval_num ] );
			float f1 = eval_f( derivate_num, read_from.x[ interval_num + 1 ] );
			if( write_to.valid[ interval_num + 1 ] = ( f0 * f1 < 0.0 ) )
				// there is one root in current interval and root finder will converge
#ifdef SUB_METHOD_BISECTION
				write_to.x[ interval_num + 1 ] = bisection( derivate_num, read_from.x[ interval_num ], f0, read_from.x[ interval_num + 1 ], f1 );
#endif
#ifdef SUB_METHOD_REGULA_FALSI
				write_to.x[ interval_num + 1 ] = regula_falsi( derivate_num, read_from.x[ interval_num ], f0, read_from.x[ interval_num + 1 ], f1 );
#endif
		}
	}
	
	write_to.x[ DEGREE - derivate_num + 1 ] = read_from.x[ DEGREE - derivate_num + 1 ];
	write_to.valid[ DEGREE - derivate_num + 1 ] = true;
}

roots solve( in polynomial p, const in vec2 trace_interval )
{
	
	// copy coefficients in polynom array
	derivatives[ 0 ] = p;
	// fill derivatives array
	calc_derivatives();
	
	// arrays of roots which are swaped after each iteration
	roots read_from, write_to;
	read_from.x[ 0 ] = write_to.x[ 0 ] = trace_interval[ 0 ];
	read_from.valid[ 0 ] = write_to.valid[ 0 ] = true;
	read_from.x[ 1 ] = write_to.x[ 1 ] = trace_interval[ 0 ];
	read_from.valid[ 1 ] = write_to.valid[ 1 ] = false;
	read_from.x[ 2 ] = write_to.x[ 2 ] = trace_interval[ 1 ];
	read_from.valid[ 2 ] = write_to.valid[ 2 ] = true;
	for( int i = 3; i < DEGREE + 2; i++ )
	{
		read_from.x[ i ] = write_to.x[ i ] = trace_interval[ 1 ];
		read_from.valid[ i ] = write_to.valid[ i ] = false;
	}
	
	// basic case: calculate root of degree 1 derivate
	float lin_root = -derivatives[ DEGREE - 1 ].a[ 0 ] / derivatives[ DEGREE - 1 ].a[ 1 ];
	if( contains( trace_interval, lin_root ) )
	{
		read_from.x[ 1 ] = lin_root;
		read_from.valid[ 1 ] = true;
	}

#if DEGREE > 1
	for( int i = DEGREE - 2; i >= 0; i-- )
	{
		calc_roots( i, read_from, write_to );
		
		// copy result to the input buffer of the next iteration
		read_from = write_to;
	}
#endif
		
	return read_from;
}
#endif
#ifdef METHOD_INTERVAL
polynomial derivatives[ DEGREE + 1 ];

float eval_f( int which, float where )
{
	float res = 0.0;
	for( int i = which; i >= 0; i-- )
		res = derivatives[ which ].a[ i ] + where * res;
	return res;
}

void calc_derivatives()
{
	for( int derivate = DEGREE - 1; derivate >= 0; derivate-- )
		for( int j = 0; j <= derivate; j++ )
			derivatives[ derivate ].a[ j ] = float( j + 1 ) * derivatives[ derivate + 1 ].a[ j + 1 ];
}

// interval operations
vec2 mult_01( vec2 i ) { return vec2( min( 0.0, i[ 0 ] ), max( 0.0, i[ 1 ] ) ); }
vec2 make_n11( float f ) { return vec2( min( -f, f ), max( -f, f ) ); }

#ifdef SUB_METHOD_BISECTION
float epsilon = 0.003;
vec2 taylor_bound( vec2 i_x )
{
	#if DEGREE == 1
		float f0 = eval_f( DEGREE, i_x[ 0 ] );
		float f1 = eval_f( DEGREE, i_x[ 1 ] );
		
		return vec2( min( f0, f1 ), max( f0, f1 ) );
	#else
		float x_0 = ( i_x[ 0 ] + i_x[ 1 ] ) * 0.5;
		float x_1 = ( i_x[ 1 ] - i_x[ 0 ] ) * 0.5;
		
		#if DEGREE / 2 == SIZE / 2
			// base case
			vec2 bound = vec2( eval_f( 0, x_0 ) );
			// iterative variant of recursive taylor method
			for( int derivate = 2; derivate <= DEGREE; derivate += 2 )
				bound = vec2( eval_f( derivate, x_0 ) ) + make_n11( x_1 * eval_f( derivate - 1, x_0 ) ) + ( 0.5 * x_1 * x_1 ) * mult_01( bound );
		#else
			// base case
			vec2 bound = vec2( eval_f( 1, x_0 ) ) + make_n11( x_1 * eval_f( 0, x_0 ) );
			// iterative variant of recursive taylor method
			for( int derivate = 3; derivate <= DEGREE; derivate += 2 )
				bound = vec2( eval_f( derivate, x_0 ) ) + make_n11( x_1 * eval_f( derivate - 1, x_0 ) ) + ( 0.5 * x_1 * x_1 ) * mult_01( bound );
		#endif
		return bound;
	#endif
}

float bisection_interval( float x0, float x1 )
{
	float x_upper_bound = x1;
	float x2 = x1;
	
	vec2 bound = taylor_bound( vec2( x0, x1 ) );
	while( ( bound[ 0 ] <= 0.0 && bound[ 1 ] >= 0.0 ) && x1 - x0 > epsilon && x0 < x_upper_bound )
	{
		x2 = 0.5 * ( x0 + x1 );
		bound = taylor_bound( vec2( x0, x2 ) );
		
		if( bound[ 0 ] <= 0.0 && bound[ 1 ] >= 0.0 )
		{
			// there is a root in the first interval -> search in first
			x1 = x2;
		}
		else
		{
			// there is no root in the first interval -> search in second
			x0 = x2;
			x1 = x_upper_bound;
			bound = taylor_bound( vec2( x0, x1 ) );
		}
	}

	if( !( bound[ 0 ] <= 0.0 && bound[ 1 ] >= 0.0 ) )
		x2 = x_upper_bound + 1.0;
	
	return x2;
}
#endif

#ifdef SUB_METHOD_SPHERE_TRACING
float epsilon = 0.00001;
vec2 taylor_bound_df( vec2 i_x )
{
	#if DEGREE == 1
		return vec2( eval_f( 0, 0.0 ) );
	#else
		#define DF_DEGREE ( DEGREE - 1 )
		#define DF_SIZE ( SIZE - 1 )
			#if DF_DEGREE == 1
				float f0 = eval_f( DF_DEGREE, i_x[ 0 ] );
				float f1 = eval_f( DF_DEGREE, i_x[ 1 ] );
				
				return vec2( min( f0, f1 ), max( f0, f1 ) );
			#else
				float x_0 = ( i_x[ 0 ] + i_x[ 1 ] ) * 0.5;
				float x_1 = ( i_x[ 1 ] - i_x[ 0 ] ) * 0.5;
				
				#if DF_DEGREE / 2 == DF_SIZE / 2
					// base case
					vec2 bound = vec2( eval_f( 0, x_0 ) );
					// iterative variant of recursive taylor method
					for( int derivate = 2; derivate <= DF_DEGREE; derivate += 2 )
						bound = vec2( eval_f( derivate, x_0 ) ) + make_n11( x_1 * eval_f( derivate - 1, x_0 ) ) + ( 0.5 * x_1 * x_1 ) * mult_01( bound );
				#else
					// base case
					vec2 bound = vec2( eval_f( 1, x_0 ) ) + make_n11( x_1 * eval_f( 0, x_0 ) );
					// iterative variant of recursive taylor method
					for( int derivate = 3; derivate <= DF_DEGREE; derivate += 2 )
						bound = vec2( eval_f( derivate, x_0 ) ) + make_n11( x_1 * eval_f( derivate - 1, x_0 ) ) + ( 0.5 * x_1 * x_1 ) * mult_01( bound );
				#endif
				return bound;
			#endif
		#undef DF_SIZE
		#undef DF_DEGREE
	#endif
}

float sphere_tracing( float x0, float x1 )
{
	vec2 df_bound = abs( taylor_bound_df( vec2( x0, x1 ) ) );
	float lipschitz = max( df_bound[ 0 ], df_bound[ 1 ] );
	
	float x = x0;
	float last_x = x - 1.0;
	float f = abs( eval_f( DEGREE, x ) );
	while( x - last_x > epsilon && x < x1 )
	{
		last_x = x;
		x = x + f / lipschitz;
		f = abs( eval_f( DEGREE, x ) );
		
		df_bound = abs( taylor_bound_df( vec2( x, x1 ) ) );
		lipschitz = max( df_bound[ 0 ], df_bound[ 1 ] );
	}
	return x;
}
#endif

struct roots
{
	float x[ DEGREE + 2 ];
	bool valid[ DEGREE + 2 ];
};

bool contains( const in vec2 interval, const in float value )
{
	return ( interval[ 0 ] < value && value < interval[ 1 ] ) || ( interval[ 0 ] > value && value > interval[ 1 ] );
}

roots solve( const in polynomial p, const in vec2 trace_interval )
{
	// copy coefficients in polynom array
	derivatives[ DEGREE ] = p;
	// fill derivatives array
	calc_derivatives();
	
	// init result array
	roots res;
	res.x[ 0 ] = trace_interval[ 0 ];
	res.valid[ 0 ] = false;
	for( int i = 1; i < DEGREE + 2; i++ )
	{
		res.x[ i ] = trace_interval[ 1 ];
		res.valid[ i ] = false;
	}

	// apply interval algorithm
	#ifdef SUB_METHOD_BISECTION
		res.x[ 1 ] = bisection_interval( trace_interval[ 0 ], trace_interval[ 1 ] );
	#endif
	#ifdef SUB_METHOD_SPHERE_TRACING
		res.x[ 1 ] = sphere_tracing( trace_interval[ 0 ], trace_interval[ 1 ] );
	#endif
	res.valid[ 1 ] = contains( trace_interval, res.x[ 1 ] );
	
	return res;
}
#endif
#ifdef METHOD_LAGUERRE
float epsilon = 0.0001;
struct complex
{
	float re;
	float im;
};

complex conj( complex z ) { z.im = -z.im; return z; }
float sabs( complex z ) { return abs( z.re ) + abs( z.im ); }
float magnitude( complex z ) { return sqrt( z.re * z.re + z.im * z.im ); }

complex add( float x, complex z ) { z.re = x + z.re; return z; }
complex add( complex z, float x ) { z.re = z.re + x; return z; }
complex add( complex z1, complex z2 ) { return complex( z1.re + z2.re, z1.im + z2.im ); }

complex sub( float x, complex z ) { z.re = x - z.re; return z; }
complex sub( complex z, float x ) { z.re = z.re - x; return z; }
complex sub( complex z1, complex z2 ) { return complex( z1.re - z2.re, z1.im - z2.im ); }

complex mult( float x, complex z ) { return complex( x * z.re, x * z.im ); }
complex mult( complex z, float x ) { return complex( x * z.re, x * z.im ); }
complex mult( complex z1, complex z2 ) { return complex( z1.re * z2.re - z1.im * z2.im, z1.re * z2.im + z1.im * z2.re ); }

complex div( complex z, float x ) { return complex( z.re / x, z.im / x ); }
complex div( float x, complex z ) { float tmp = z.re * z.re + z.im * z.im; return complex( ( x * z.re ) / tmp, -( x * z.im ) / tmp ); }
complex div( complex z1, complex z2 ) { float tmp = z2.re * z2.re + z2.im * z2.im; return complex( ( z1.re * z2.re + z1.im * z2.im ) / tmp, ( z1.im * z2.re - z1.re * z2.im ) / tmp ); }

complex square( complex z ) { return complex( z.re * z.re - z.im * z.im, 2.0 * z.re * z.im ); }

complex complex_sqrt( complex z )
{
	complex c;
	if( ( z.re == 0.0 ) && ( z.im == 0.0 ) )
	{
		c = z;
	}
	else
	{
		float x, y, w, r;
		x = abs( z.re );
		y = abs( z.im );
		if( x >= y )
		{
			r = y / x;
			w = sqrt( x ) * sqrt( 0.5 * ( 1.0 + sqrt( 1.0 + r * r ) ) );
		}
		else
		{
			r = x / y;
			w = sqrt( y ) * sqrt( 0.5 * ( r + sqrt( 1.0 + r * r ) ) );
		}
		if( z.re >= 0.0 )
		{
			c.re = w;
			c.im = z.im / ( 2.0 * w );
		}
		else
		{
			if( z.im >= 0.0 ) 
				c.im = w;
			else
				c.im = -w;
			c.re = z.im / ( 2.0 * c.im );
		}
	}
	return c;
}

float c[ SIZE ];

complex laguerre( complex x, int degree )
{
	complex a = complex( 1.0, 0.0 );
	float f_degree = float( degree );
	int iterations = 0;
	
	complex f, df, ddf;
	bool root_found = false;
	while( !root_found )
	{
		x = sub( x, a );

		f = complex( c[ degree ], 0.0 );
		df = complex( 0.0, 0.0 );
		ddf = complex( 0.0, 0.0 );
		
		// efficient computation of f, first derivate of f and second derivate of f 
		for( int i = degree - 1; i >= 0; i-- )
		{
			ddf = add( mult( x, ddf ), df );
			df = add( mult( x, df ), f );
			f = add( mult( x, f ), c[ i ] );
		}
		
		// eigentlich würde es reichen, wenn diese Bedingung in der while-Schleife geprüft wird,
		// aber dann klappt's nicht mehr -> compiler bug
		if( sabs( f ) != 0.0 && magnitude( a ) > 0.00001 * magnitude( x ) )
		{
			complex G = ( div( df, f ) );
			complex G2 = square( G );
			complex H = ( sub( G2, div( ddf, f ) ) );
			
			complex sqrt_term = complex_sqrt( mult( f_degree - 1.0, sub( mult( f_degree, H ), G2 ) ) );
			complex denom1 = add( G, sqrt_term );
			complex denom2 = sub( G, sqrt_term );
			
			if( magnitude( denom1 ) > magnitude( denom2 ) )
				a = div( f_degree, denom1 );
			else
				a = div( f_degree, denom2 );
		}
		else
		{
			root_found = true;
		}
	}

	return x;
}

void bubble_sort( inout float a[ DEGREE + 2 ] )
{
	for( int i = 1; i < DEGREE + 2; i++ )
	{
		for( int j = 0; j < DEGREE + 2 - i; j++ )
		{
			if( a[ j ] > a[ j + 1 ] )
			{
				float temp = a[ j ];
				a[ j ] = a[ j + 1 ];
				a[ j + 1 ] = temp;
			}
		}
	}
}

struct roots
{
	float x[ DEGREE + 2 ];
	bool valid[ DEGREE + 2 ];
};

bool contains( const in vec2 interval, const in float value )
{
	return interval[ 0 ] < value && value < interval[ 1 ];
}

float solve_linear() { return -c[ 0 ] / c[ 1 ]; }
void solve_quadric( out float x1, out float x2, const in vec2 trace_interval )
{
	float diskriminante = c[ 1 ] * c[ 1 ] - 4.0 * c[ 2 ] * c[ 0 ];
	if( diskriminante >= 0.0 )
	{
		float q = -0.5 * ( c[ 1 ] + sign( c[ 1 ] ) * sqrt( diskriminante ) );
		x1 = q / c[ 2 ];
		x2 = c[ 0 ] / q;
	}
	else
	{
		x1 = x2 = trace_interval[ 0 ] - 1.0;
	}
}

roots solve( const in polynomial p, const in vec2 trace_interval )
{
	// create complex coefficient array from real coefficient array
	for( int i = 0; i < SIZE; i++ )
		c[ i ] = p.a[ i ];
	
	// init result array
	roots res;
	res.x[ 0 ] = trace_interval[ 0 ];
	res.valid[ 0 ] = false;
	res.x[ DEGREE + 1 ] = trace_interval[ 1 ];	
	res.valid[ DEGREE + 1 ] = false;

	// solve individual root successively
	complex initial = complex( 0.5 * ( trace_interval[ 0 ] + trace_interval[ 1 ] ), 0.0 );
	bool skip_next = false;
	for( int i = 0; i < DEGREE; i++ )
	{
		if( skip_next )
		{
			skip_next = false;
		}
		else
		{
			if( DEGREE - i == 1 )
			{
				res.x[ i + 1 ] = solve_linear();
			}
			else if( DEGREE - i == 2 )
			{
				solve_quadric( res.x[ i + 1 ], res.x[ i + 2 ], trace_interval );
				skip_next = true;
			}
			else
			{
				complex root = laguerre( initial, DEGREE - i );
		
				if( abs( root.im ) < 1e-04 * abs( root.re ) )
				{
					// deflate real root 'root.re'
					res.x[ i + 1 ] = root.re;
					
					float oldc;
					float newc = c[ DEGREE - i ];
					for( int j = DEGREE - i - 1; j > -1; j-- )
					{
						oldc = c[ j ];
						c[ j ] = newc;
						newc = oldc + newc * root.re;
					}
				}
				else
				{
					// deflate complex root 'root' and 'conj(root)'
					res.x[ i + 1 ] = trace_interval[ 0 ] - 1.0;
					skip_next = true;
					
					if( i < DEGREE - 1 )
					{
						res.x[ i + 2 ] = trace_interval[ 0 ] - 1.0;
						
						float p = 2.0 * root.re;
						float q = -( root.re * root.re + root.im * root.im );
						
						c[ DEGREE - i - 0 ] += 0.0;
						c[ DEGREE - i - 1 ] += c[ DEGREE - i - 0 ] * p;
						c[ DEGREE - i - 2 ] += c[ DEGREE - i - 0 ] * q + c[ DEGREE - i - 1 ] * p;
						for( int j = DEGREE - i - 2; j > -1; j-- )
							if( j <= DEGREE - i - 3 && j > 1 )
								c[ j ] += c[ j + 2 ] * q + c[ j + 1 ] * p;
						for( int j = 2; j <= DEGREE - i; j++ )
								c[ j - 2 ] = c[ j ];
					}
				}
			}
		}
	}

	// sort roots
	bubble_sort( res.x );
	
	// validate roots
	for( int i = 0; i < DEGREE + 2; i++ )
		res.valid[ i ] = contains( trace_interval, res.x[ i ] );
	
	return res;
}
#endif
#ifdef METHOD_MULLER
float epsilon = 0.0001;
struct complex
{
	float re;
	float im;
};

complex conj( complex z ) { z.im = -z.im; return z; }
float sabs( complex z ) { return abs( z.re ) + abs( z.im ); }
float magnitude( complex z ) { return sqrt( z.re * z.re + z.im * z.im ); }

complex add( float x, complex z ) { z.re = x + z.re; return z; }
complex add( complex z, float x ) { z.re = z.re + x; return z; }
complex add( complex z1, complex z2 ) { return complex( z1.re + z2.re, z1.im + z2.im ); }

complex sub( float x, complex z ) { z.re = x - z.re; return z; }
complex sub( complex z, float x ) { z.re = z.re - x; return z; }
complex sub( complex z1, complex z2 ) { return complex( z1.re - z2.re, z1.im - z2.im ); }

complex mult( float x, complex z ) { return complex( x * z.re, x * z.im ); }
complex mult( complex z, float x ) { return complex( x * z.re, x * z.im ); }
complex mult( complex z1, complex z2 ) { return complex( z1.re * z2.re - z1.im * z2.im, z1.re * z2.im + z1.im * z2.re ); }

complex div( complex z, float x ) { return complex( z.re / x, z.im / x ); }
complex div( float x, complex z ) { float tmp = z.re * z.re + z.im * z.im; return complex( ( x * z.re ) / tmp, -( x * z.im ) / tmp ); }
complex div( complex z1, complex z2 ) { float tmp = z2.re * z2.re + z2.im * z2.im; return complex( ( z1.re * z2.re + z1.im * z2.im ) / tmp, ( z1.im * z2.re - z1.re * z2.im ) / tmp ); }

complex square( complex z ) { return complex( z.re * z.re - z.im * z.im, 2.0 * z.re * z.im ); }

complex complex_sqrt( complex z )
{
	complex c;
	if( ( z.re == 0.0 ) && ( z.im == 0.0 ) )
	{
		c = z;
	}
	else
	{
		float x, y, w, r;
		x = abs( z.re );
		y = abs( z.im );
		if( x >= y )
		{
			r = y / x;
			w = sqrt( x ) * sqrt( 0.5 * ( 1.0 + sqrt( 1.0 + r * r ) ) );
		}
		else
		{
			r = x / y;
			w = sqrt( y ) * sqrt( 0.5 * ( r + sqrt( 1.0 + r * r ) ) );
		}
		if( z.re >= 0.0 )
		{
			c.re = w;
			c.im = z.im / ( 2.0 * w );
		}
		else
		{
			if( z.im >= 0.0 ) 
				c.im = w;
			else
				c.im = -w;
			c.re = z.im / ( 2.0 * c.im );
		}
	}
	return c;
}

float ca[ SIZE ];

complex muller( int degree )
{
	// Startwerte einstellen
	complex a = complex( 1.0, 0.0 );
	complex x0 = complex( -1.0, 0.0 ), f0 = complex( ca[ 0 ] - ca[ 1 ] + ca[ 2 ], 0.0 );
	complex x1 = complex( 1.0, 0.0 ), f1 = complex( ca[ 0 ] + ca[ 1 ] + ca[ 2 ], 0.0 );
	complex x2 = complex( 0.0, 0.0 );
	complex f2 = complex( ca[ 0 ], 0.0 );
	
	bool root_found = false;
	while( !root_found )
	{
		// eigentlich würde es reichen, wenn diese Bedingung in der while-Schleife geprüft wird,
		// aber dann klappt's nicht mehr -> compiler bug
		if( sabs( f2 ) != 0.0 && magnitude( a ) > 0.00001 * magnitude( x2 ) )
		{
			// neue Näherung generieren
			complex q = div( sub( x2, x1 ), sub( x1, x0 ) );
			
			complex A = add( sub( mult( q, f2 ), mult( mult( q, add( 1.0, q ) ), f1 ) ), mult( square( q ), f0 ) );
			complex B = add( sub( mult( add( mult( 2.0, q ), 1.0 ), f2 ), mult( square( add( 1.0, q ) ), f1 ) ), mult( square( q ), f0 ) );
			complex C = mult( add( 1.0, q ), f2 );
			
			complex sqrt_term = complex_sqrt( sub( square( B ), mult( 4.0, mult( A, C ) ) ) );
			complex denom1 = add( B, sqrt_term );
			complex denom2 = sub( B, sqrt_term );
			
			if( magnitude( denom1 ) > magnitude( denom2 ) )
				a = mult( sub( x2, x1 ), div( mult( 2.0, C ), denom1 ) );
			else
				a = mult( sub( x2, x1 ), div( mult( 2.0, C ), denom2 ) );
			
			complex x2_tmp = sub( x2, a );
			complex f2_tmp = complex( ca[ degree ], 0.0 );
			for( int i = degree - 1; i >= 0; i-- )
				f2_tmp = add( mult( f2_tmp, x2_tmp ), ca[ i ] );
			
			x0 = x1; f0 = f1;
			x1 = x2; f1 = f2;
			x2 = x2_tmp; f2 = f2_tmp;
		}
		else
		{
			root_found = true;
		}
	}
	
	return x2;
}

void bubble_sort( inout float a[ DEGREE + 2 ] )
{
	for( int i = 1; i < DEGREE + 2; i++ )
	{
		for( int j = 0; j < DEGREE + 2 - i; j++ )
		{
			if( a[ j ] > a[ j + 1 ] )
			{
				float temp = a[ j ];
				a[ j ] = a[ j + 1 ];
				a[ j + 1 ] = temp;
			}
		}
	}
}

struct roots
{
	float x[ DEGREE + 2 ];
	bool valid[ DEGREE + 2 ];
};

bool contains( const in vec2 interval, const in float value )
{
	return interval[ 0 ] < value && value < interval[ 1 ];
}

float solve_linear() { return -ca[ 0 ] / ca[ 1 ]; }
void solve_quadric( out float x1, out float x2, const in vec2 trace_interval )
{
	float diskriminante = ca[ 1 ] * ca[ 1 ] - 4.0 * ca[ 2 ] * ca[ 0 ];
	if( diskriminante >= 0.0 )
	{
		float q = -0.5 * ( ca[ 1 ] + sign( ca[ 1 ] ) * sqrt( diskriminante ) );
		x1 = q / ca[ 2 ];
		x2 = ca[ 0 ] / q;
	}
	else
	{
		x1 = x2 = trace_interval[ 0 ] - 1.0;
	}
}

roots solve( const in polynomial p, const in vec2 trace_interval )
{
	// create complex coefficient array from real coefficient array
	float scale = abs( p.a[ DEGREE ] );
	if( scale <= 0.0000001 )
		scale = 1.0;
	for( int i = 0; i < SIZE; i++ )
		ca[ i ] = p.a[ i ] / scale;
	
	// init result array
	roots res;
	res.x[ 0 ] = trace_interval[ 0 ];
	res.valid[ 0 ] = false;
	res.x[ DEGREE + 1 ] = trace_interval[ 1 ];	
	res.valid[ DEGREE + 1 ] = false;

	// solve individual root successively
	bool skip_next = false;
	for( int i = 0; i < DEGREE; i++ )
	{
		if( skip_next )
		{
			skip_next = false;
		}
		else
		{
			if( DEGREE - i == 1 )
			{
				res.x[ i + 1 ] = solve_linear();
			}
			else if( DEGREE - i == 2 )
			{
				solve_quadric( res.x[ i + 1 ], res.x[ i + 2 ], trace_interval );
				skip_next = true;
			}
			else
			{
				complex root = muller( DEGREE - i );
	
				if( abs( root.im ) < 1e-04 * abs( root.re ) )
				{
					// deflate real root 'root.re'
					res.x[ i + 1 ] = root.re;
					
					float oldc;
					float newc = ca[ DEGREE - i ];
					for( int j = DEGREE - i - 1; j > -1; j-- )
					{
						oldc = ca[ j ];
						ca[ j ] = newc;
						newc = oldc + newc * root.re;
					}
				}
				else
				{
					// deflate complex root 'root' and 'conj(root)'
					res.x[ i + 1 ] = trace_interval[ 0 ] - 1.0;
					skip_next = true;
					
					if( i < DEGREE - 1 )
					{
						res.x[ i + 2 ] = trace_interval[ 0 ] - 1.0;
						
						float p = 2.0 * root.re;
						float q = -( root.re * root.re + root.im * root.im );
						
						ca[ DEGREE - i - 0 ] += 0.0;
						ca[ DEGREE - i - 1 ] += ca[ DEGREE - i - 0 ] * p;
						ca[ DEGREE - i - 2 ] += ca[ DEGREE - i - 0 ] * q + ca[ DEGREE - i - 1 ] * p;
						for( int j = DEGREE - i - 2; j > -1; j-- )
							if( j <= DEGREE - i - 3 && j > 1 )
								ca[ j ] += ca[ j + 2 ] * q + ca[ j + 1 ] * p;
						for( int j = 2; j <= DEGREE - i; j++ )
								ca[ j - 2 ] = ca[ j ];
					}
				}
			}
		}
	}
	
	// sort roots
	bubble_sort( res.x );
	
	// validate roots
	for( int i = 0; i < DEGREE + 2; i++ )
		res.valid[ i ] = contains( trace_interval, res.x[ i ] );
	
	return res;
}
#endif
#ifdef METHOD_FORMULA
float epsilon = 0.0001;
struct roots
{
	float x[ DEGREE + 2 ];
	bool valid[ DEGREE + 2 ];
};
void correct( inout float x1, inout float x2 ) { if( x1 > x2 ) { float tmp = x1; x1 = x2; x2 = tmp; } }
roots init_roots( vec2 trace_interval )
{
	roots r;
	for( int i = 0; i < DEGREE + 2; i++ )
	{
		r.x[ i ] = trace_interval[ 0 ] - 1.0;
		r.valid[ i ] = false;
	}
	return r;
}
#if DEGREE == 1
roots solve( const in polynomial p, const in vec2 trace_interval )
{
	roots res = init_roots( trace_interval );
	res.x[ 1 ] = -p.a[ 0 ] / p.a[ 1 ];
	res.valid[ 1 ] = res.x[ 1 ] > trace_interval[ 0 ] && res.x[ 1 ] < trace_interval[ 1 ];
	return res;
}
#endif
#if DEGREE == 2
roots solve( const in polynomial p, const in vec2 trace_interval )
{
	roots res = init_roots( trace_interval );
	
	float diskriminante = p.a[ 1 ] * p.a[ 1 ] - 4.0 * p.a[ 2 ] * p.a[ 0 ];
	if( diskriminante >= 0.0 )
	{
		float q = -0.5 * ( p.a[ 1 ] + sign( p.a[ 1 ] ) * sqrt( diskriminante ) );
		res.x[ 1 ] = q / p.a[ 2 ];
		res.x[ 2 ] = p.a[ 0 ] / q;
		correct( res.x[ 1 ], res.x[ 2 ] );
	}
	res.valid[ 1 ] = res.x[ 1 ] > trace_interval[ 0 ] && res.x[ 1 ] < trace_interval[ 1 ];
	res.valid[ 2 ] = res.x[ 2 ] > trace_interval[ 0 ] && res.x[ 2 ] < trace_interval[ 1 ];
	return res;
}
#endif
#if DEGREE == 3
roots solve( const in polynomial p, const in vec2 trace_interval )
{
	roots res = init_roots( trace_interval );
	
//	if( abs( p.a[ 3 ] / p.a[ 2 ] ) < 10e-4 )
//		discard;
	
	#define SIN60 0.86602540378443864675
	#define HYPOT(x,y) (sqrt(x*x+y*y))
		
		float A, B, C;
		A = p.a[ 2 ] / p.a[ 3 ]; // normalisieren
		B = p.a[ 1 ] / p.a[ 3 ]; // normalisieren
		C = p.a[ 0 ] / p.a[ 3 ]; // normalisieren
	
		float Q = ( A * A - 3.0 * B ) / 9.0;
		float R = ( 2.0 * A * A * A - 9.0 * A * B + 27.0 * C ) / 54.0;
	
		// diskriminantenregel!!! (lässt sich aus R und Q bilden)
		if( R * R < Q * Q * Q )
		{
			// three real roots
			/*
			#define PI 3.1415926535897932384626433832795
			float theta = acos( R / sqrt( Q * Q * Q ) );
			roots[ 0 ] = -2.0 * sqrt( Q ) * cos( theta / 3.0 ) - A / 3.0;
			roots[ 1 ] = -2.0 * sqrt( Q ) * cos( ( theta + 2.0 * PI ) / 3.0 ) - A / 3.0;
			roots[ 2 ] = -2.0 * sqrt( Q ) * cos( ( theta - 2.0 * PI ) / 3.0 ) - A / 3.0;
			#undef PI
			*/
			
			float a0 = C;
			float a1 = B;
			float a2 = A / 3.0;
		
			float q = a1 / 3.0 - a2*a2;
			float r = ( a1 * a2 - a0 ) * 0.5  - a2 * a2 * a2 ;
	
			float d = q*q*q + r*r;
	
			d = sqrt( -d );
			q = atan( d, r ) / 3.0;
			r = pow( HYPOT( d, r ), 1.0 / 3.0 );
		
			{
				float s = ( 2.0 * r ) * cos( q );
				float t = abs( 2.0 * r * sin( q ) ) * SIN60;
				
				res.x[1] = s - A / 3.0;
				res.x[2] = -( 0.5 * s ) - A / 3.0 - t;
				res.x[3] = -( 0.5 * s ) - A / 3.0 + t;
			}
		}
		else
		{
			float X = -sign( R ) * pow( abs( R ) + sqrt( R * R - Q * Q * Q ), 1.0 / 3.0 );
			float Y = ( abs( X ) < 1e-10 ) ? 0.0 : Q / X;
			
			// first root is ...
			res.x[ 1 ] = ( X + Y ) - A / 3.0;
					
			if( abs( X - Y ) < 1e-10 )
			{
				// ... second and third root are also real
				res.x[ 2 ] = -0.5 * ( X + Y ) - A / 3.0;
				res.x[ 3 ] = -0.5 * ( X + Y ) - A / 3.0;
			}
		}
	#undef SIN60
	#undef HYPOT
	
	// sort
	correct( res.x[ 1 ], res.x[ 2 ] );
	correct( res.x[ 2 ], res.x[ 3 ] );
	correct( res.x[ 1 ], res.x[ 2 ] );
	
	// validate
	res.valid[ 1 ] = res.x[ 1 ] > trace_interval[ 0 ] && res.x[ 1 ] < trace_interval[ 1 ];
	res.valid[ 2 ] = res.x[ 2 ] > trace_interval[ 0 ] && res.x[ 2 ] < trace_interval[ 1 ];
	res.valid[ 3 ] = res.x[ 3 ] > trace_interval[ 0 ] && res.x[ 3 ] < trace_interval[ 1 ];
	
	return res;
}
#endif
#if DEGREE == 4
int solve_quadratic( float illegal, float a[ 3 ], out float roots[ 2 ] )
{
	roots[ 0 ] = roots[ 1 ] = illegal;
	int num = 0;

	float diskriminante = a[ 1 ] * a[ 1 ] - 4.0 * a[ 2 ] * a[ 0 ];
	if( diskriminante >= 0.0 )
	{
		float q = -0.5 * ( a[ 1 ] + sign( a[ 1 ] ) * sqrt( diskriminante ) );
		roots[ 0 ] = q / a[ 2 ];
		roots[ 1 ] = a[ 0 ] / q;
		num = 2;
	}
	
	return num;
}

// Lösungsmöglichkeiten siehe http://en.wikipedia.org/wiki/Cubic_equation
// Cardano's method
// Lagrange resolvents
// Chebyshev radicals
int solve_cubic( float illegal, float a[ 4 ], out float roots[ 3 ] )
{
	int num;
#define SIN60 0.86602540378443864675
#define HYPOT(x,y) (sqrt(x*x+y*y))

	roots[ 0 ] = roots[ 1 ] = roots[ 2 ] = illegal;

	float A, B, C;
	A = a[ 2 ] / a[ 3 ]; // normalisieren
	B = a[ 1 ] / a[ 3 ]; // normalisieren
	C = a[ 0 ] / a[ 3 ]; // normalisieren

	float Q = ( A * A - 3.0 * B ) / 9.0;
	float R = ( 2.0 * A * A * A - 9.0 * A * B + 27.0 * C ) / 54.0;

	// diskriminantenregel!!! (lässt sich aus R und Q bilden)
	if( R * R < Q * Q * Q )
	{
		float a0 = C;
		float a1 = B;
		float a2 = A / 3.0;
	
		float q = a1 / 3.0 - a2*a2;
		float r = ( a1 * a2 - a0 ) * 0.5  - a2 * a2 * a2 ;

		float d = q*q*q + r*r;

		d = sqrt( -d );
		q = atan( d, r ) / 3.0;
		r = pow( HYPOT( d, r ), 1.0 / 3.0 );
	
		{
			float s = ( 2.0 * r ) * cos( q );
			float t = abs( 2.0 * r * sin( q ) ) * SIN60;
			
			roots[0] = s - A / 3.0;
			roots[1] = -( 0.5 * s ) - A / 3.0 - t;
			roots[2] = -( 0.5 * s ) - A / 3.0 + t;
		}
		num = 3;
	}
	else
	{
		float X = -sign( R ) * pow( abs( R ) + sqrt( R * R - Q * Q * Q ), 1.0 / 3.0 );
		float Y = ( abs( X ) < 1e-10 ) ? 0.0 : Q / X;
		
		// first root is ...
		roots[ 0 ] = ( X + Y ) - A / 3.0;
		
		num = 1;
		
		if( abs( X - Y ) < 1e-10 )
		{
			// ... second and third root are also real
			roots[ 1 ] = -0.5 * ( X + Y ) - A / 3.0;
			roots[ 2 ] = -0.5 * ( X + Y ) - A / 3.0;
			num = 3;
		}
	}
#undef SIN60
#undef HYPOT

	return num;
}

roots solve( const in polynomial p, const in vec2 trace_interval )
{
	roots res = init_roots( trace_interval );

	float illegal = trace_interval[ 0 ] - 1.0;
	float a3,a2,a1,a0;
	float resolvant[ 4 ];
	float resolvant_roots[ 3 ];
	int resolvant_num_roots;
	int num = 0;

	a0 = p.a[0] / p.a[4];
	a1 = p.a[1] / p.a[4];
	a2 = p.a[2] / p.a[4];
	a3 = p.a[3] / p.a[4];

	resolvant[0] = -( a1*a1 + a0 * a3 * a3  - 4.0 * a0 * a2 );
	resolvant[1] = a1*a3 - 4.0 * a0;
	resolvant[2] = -a2;
	resolvant[3] = 1.0;

	resolvant_num_roots = solve_cubic( illegal, resolvant, resolvant_roots );

	bool found = false;
	for( int i = 0 ; i < 3 ; i++ )
	{
		if( !found && i < resolvant_num_roots )
		{
			float u,s,t;
			u = resolvant_roots[i];
	
			s = a3*a3 / 4.0 + u - a2;
			t = u * u / 4.0 - a0;
	
			if( s >= 0.0 && t >= 0.0 )
			{
				float quad[3];
				
				s = sqrt(s);
				t = sqrt(t);
				
				if( sign(s*t) != sign(a3 * u / 2.0 - a1) )
					t = -t;
	
				quad[0] = u / 2.0 + t;
				quad[1] = a3 / 2.0 + s;
				quad[2] = 1.0;
	
				float quad_roots[ 2 ];
				num += solve_quadratic( illegal,quad,quad_roots);
	
				quad[0] = u / 2.0 - t;
				quad[1] = a3 / 2.0 - s;
	
				if( num == 0 )
				{
					num += solve_quadratic( illegal,quad,quad_roots);
					res.x[ 1 ] = quad_roots[ 0 ];
					res.x[ 2 ] = quad_roots[ 1 ];
				}
				else
				{
					res.x[ 1 ] = quad_roots[ 0 ];
					res.x[ 2 ] = quad_roots[ 1 ];
					num += solve_quadratic( illegal,quad,quad_roots);
					res.x[ 3 ] = quad_roots[ 0 ];
					res.x[ 4 ] = quad_roots[ 1 ];
				}
				found = true;
			}
		}
	}

	// sort
	correct( res.x[ 1 ], res.x[ 2 ] );
	correct( res.x[ 2 ], res.x[ 3 ] );
	correct( res.x[ 3 ], res.x[ 4 ] );
	correct( res.x[ 1 ], res.x[ 2 ] );
	correct( res.x[ 2 ], res.x[ 3 ] );
	correct( res.x[ 1 ], res.x[ 2 ] );
	
	// validate
	res.valid[ 1 ] = res.x[ 1 ] > trace_interval[ 0 ] && res.x[ 1 ] < trace_interval[ 1 ];
	res.valid[ 2 ] = res.x[ 2 ] > trace_interval[ 0 ] && res.x[ 2 ] < trace_interval[ 1 ];
	res.valid[ 3 ] = res.x[ 3 ] > trace_interval[ 0 ] && res.x[ 3 ] < trace_interval[ 1 ];
	res.valid[ 4 ] = res.x[ 4 ] > trace_interval[ 0 ] && res.x[ 4 ] < trace_interval[ 1 ];
	
	return res;
}
#endif
#if DEGREE > 4
#error Threre are no formulas for degree above 4!
#endif
#endif
// all coordinates have to be in eye space!
// vectors have to be normalized
vec4 illuminate_one( vec3 eye_pos, vec3 hit_pos, vec3 n, gl_MaterialParameters material, gl_LightSourceParameters light, gl_LightProducts product )
{
	vec3 vp = normalize( light.position.xyz - hit_pos );
	float vp_length = length( light.position.xyz - hit_pos );
	vec3 h = normalize( vp + normalize( eye_pos - hit_pos ) );
	
	float pv_sdli = dot( -vp, normalize( light.spotDirection ) );
	vec2 mix_cond = vec2(	float( light.position.w == 0.0 ),		// attenuation
							float( light.spotCutoff == 180.0 ) );	// spot
	vec2 mix_vec_a = vec2(	1.0 / ( light.constantAttenuation + ( light.linearAttenuation + light.quadraticAttenuation * vp_length ) * vp_length ),	// attenuation
							pow( pv_sdli, light.spotExponent ) );																						// spot
	vec2 mix_vec_b = vec2(	1.0,	// attenuation
							1.0 );	// spot
	vec2 mixed_vec = mix( mix_vec_a, mix_vec_b, mix_cond );
	
	float att = mixed_vec[ 0 ];
	float spot = mixed_vec[ 1 ] * float( pv_sdli >= light.spotCosCutoff );
	float f = float( dot( n, vp ) != 0.0 );
	
	return att * spot * (	product.ambient + // ambient
							dot( n, vp ) * product.diffuse + // diffuse
							+ f * pow( max( 0.0, dot( n, h ) ), material.shininess ) * product.specular ); // specular
}

vec4 illuminate( vec3 hit_pos, vec3 n )
{
	vec3 eye_pos = vec3( 0.0 );
	bool use_front_material = dot( n, eye_pos - hit_pos ) <= 0.0;
	gl_MaterialParameters material;
	vec4 color;
	if( use_front_material )
	{
		material = gl_FrontMaterial;
		color = gl_FrontLightModelProduct.sceneColor;
		n = -n;
	}
	else
	{
		material = gl_BackMaterial;
		color = gl_BackLightModelProduct.sceneColor;
	}
	
	gl_LightSourceParameters light;
	gl_LightProducts product;
	for( int i = 0; i < MAX_LIGHTS; i++ )
	{
		light = gl_LightSource[ i ];
		if( use_front_material ) product = gl_FrontLightProduct[ i ]; else product = gl_BackLightProduct[ i ];
		color += mix( vec4( 0.0 ), illuminate_one( eye_pos, hit_pos, n, material, light, product ), float( light.ambient.a != 0.0 ) );
	}

	color.a = material.diffuse.a;

	return color;
}
/*
vec4 illuminate( vec3 hit_pos, vec3 n )
{
	vec3 eye_pos = vec3( 0.0 );
	vec3 vp = normalize( gl_LightSource[ 0 ].position.xyz - hit_pos );
	float vp_length = length( gl_LightSource[ 0 ].position.xyz - hit_pos );
	vec3 h = normalize( vp + normalize( eye_pos - hit_pos ) );
	float f = ( dot( n, vp ) != 0.0 ) ? 1.0 : 0.0;
	float att, spot;

	// calculate attenuation
	if( gl_LightSource[ 0 ].position.w != 0.0 )
		att = 1.0 / ( gl_LightSource[ 0 ].constantAttenuation + ( gl_LightSource[ 0 ].linearAttenuation + gl_LightSource[ 0 ].quadraticAttenuation * vp_length ) * vp_length );
	else
		att = 1.0;

	float pv_sdli = dot( -vp, normalize( gl_LightSource[ 0 ].spotDirection ) );
	if( gl_LightSource[ 0 ].spotCutoff == 180.0 )
		spot = 1.0;
	else if( pv_sdli < gl_LightSource[ 0 ].spotCosCutoff )
		spot = 0.0;
	else
		spot = pow( pv_sdli, gl_LightSource[ 0 ].spotExponent );
	
	
	gl_MaterialParameters material;
	gl_LightProducts product;
	if( dot( n, eye_pos - hit_pos ) <= 0.0 )
	{
		material = gl_FrontMaterial;
		product = gl_FrontLightProduct[ 0 ];
		n = -n;
	}
	else
	{
		material = gl_BackMaterial;
		product = gl_BackLightProduct[ 0 ];
	}

	vec4 ambient  = product.ambient;
	vec4 diffuse  = dot( n, vp ) * product.diffuse;
	vec4 specular = clamp( f * pow( max( 0.0, dot( n, h ) ), material.shininess ) * product.specular, 0.0, 1.0 );
	
	vec4 color = gl_FrontLightModelProduct.sceneColor + att * spot * ( ambient + diffuse + specular );
	color.a = material.diffuse.a;

	return color;
}
*/
float z_buffer_depth( vec3 p )
{
	vec4 proj_p = gl_ProjectionMatrix * vec4( p, 1.0 );
	float a = gl_DepthRange.far / gl_DepthRange.diff;
	float b = gl_DepthRange.far * gl_DepthRange.near / ( - gl_DepthRange.diff );
	return ( proj_p.z / proj_p.w ) * 0.5 + 0.5 ;
}

void process( roots r, vec3 eye, vec3 dir )
{
	bool any_valid = false;
	float first_root = 0.0;
#ifdef USE_TRANSPARENCY	
	if( gl_FrontMaterial.diffuse.a == 1.0 && gl_BackMaterial.diffuse.a == 1.0 )
	{
#endif
		// just process the first hit (the other hits can't be seen anyway)
		for( int i = 1; i < DEGREE + 2 - 1; i++ )
		{
			// assign to first_root if, and only if a valid root comes along and there was no valid root before (=first valid root)
			first_root = mix( first_root, r.x[ i ], float( r.valid[ i ] && !any_valid ) );
			any_valid = any_valid || r.valid[ i ];
		}
		
		if( any_valid )
		{
			// illuminate point of first root
			vec3 first_hit = eye + first_root * dir;
			vec3 eye_space_normal = normalize( ( gl_ModelViewMatrix * surface_transform * vec4( gradient( sse_opt + first_root * ssd_opt ), 0.0 ) ).xyz );
			gl_FragColor = illuminate( ese_opt + first_root * esd_opt, eye_space_normal );
#ifndef USE_TRANSPARENCY
			gl_FragColor.a = 1.0;
#endif			
			gl_FragDepth = z_buffer_depth( first_hit );			
		}
		else
			// surface not hit - discard the pixel
			discard;
#ifdef USE_TRANSPARENCY	
	}
	else
	{
		// process all hits with this surface
		gl_FragColor = vec4( 0.0 );
		vec3 first_hit = vec3( 0.0 );
		for( int i = DEGREE + 2 - 2; i >= 1; i-- )
		{
			if( r.valid[ i ] )
			{
				// this is a valid hit
				any_valid = true;
				first_root = r.x[ i ];
				first_hit = eye + first_root * dir;
				
				vec3 eye_space_normal = normalize( ( gl_ModelViewMatrix * surface_transform * vec4( gradient( sse_opt + first_root * ssd_opt ), 0.0 ) ).xyz );
				vec4 new_color = illuminate( ese_opt + first_root * esd_opt, eye_space_normal );
				gl_FragColor.rgb = min( vec3( 1.0 ), ( 1.0 - new_color.a ) * gl_FragColor.rgb + new_color.a * new_color.rgb );
				gl_FragColor.a = ( 1.0 - new_color.a ) * gl_FragColor.a + new_color.a;

			}
		}
		
		if( any_valid )
		{
			// calculate pseudo depth value of first root
			gl_FragDepth = z_buffer_depth( first_hit );
		}
		else
			// surface not hit - discard the pixel
			discard;
	}
#endif
}

vec2 clip( vec3 o, vec3 d )
{
	float l = length( d );
	d = d / l;

	vec2 res;
	if( clip_to == CLIP_TO_SPHERE )
	{
		// solve algebraic
		float B = dot( o, d );
		float C = dot( o, o ) - 1.0;
		float D = B * B - C;
		
		if( D < 0.0 )
			discard;
		
		res = vec2( -B - sqrt( D ), -B + sqrt( D ) );
	}
	else if( clip_to == CLIP_TO_CUBE )
	{
		// liang-barsky
		vec3 plane[ 6 ]; // use value as normal AND point on plane
		plane[ 0 ] = vec3( 1.0, 0.0, 0.0 );
		plane[ 1 ] = vec3( 0.0, 1.0, 0.0 );
		plane[ 2 ] = vec3( 0.0, 0.0, 1.0 );
		plane[ 3 ] = vec3( -1.0, 0.0, 0.0 );
		plane[ 4 ] = vec3( 0.0, -1.0, 0.0 );
		plane[ 5 ] = vec3( 0.0, 0.0, -1.0 );
		
		res = vec2( -2.0, 2.0 ); // max distance is diagonal (=2.0*sqrt(3.0)~=3.4642)
		vec3 p0 = o, p1 = p0 + d;
		
		for( int i = 0; i < 6; i++ )
		{
			float angle_ray_plane_normal = dot( plane[ i ], p1 - p0 );
			if( angle_ray_plane_normal > 0.0 )
				// ray is leaving through this plane
				res[ 1 ] = min( res[ 1 ], dot( plane[ i ], plane[ i ] - p0 ) / angle_ray_plane_normal );
			else if( angle_ray_plane_normal < 0.0 )
				// ray is entering through this plane
				res[ 0 ] = max( res[ 0 ], dot( plane[ i ], plane[ i ] - p0 ) / angle_ray_plane_normal );
		}
	}
	else
	{
		res = vec2( 0.0 );
		discard;
	}
	return res / l;
}

void optimize_rays()
{
	float best_start = -dot( clipping_space_eye, clipping_space_dir ) / dot( clipping_space_dir, clipping_space_dir );
	float scale = 2.0 * length( surface_space_dir );
	
	ese_opt = eye_space_eye + best_start * eye_space_dir;
	esd_opt = eye_space_dir / scale;
	
	cse_opt = clipping_space_eye + best_start * clipping_space_dir;
	csd_opt = clipping_space_dir / scale;
	
	sse_opt = surface_space_eye + best_start * surface_space_dir;
	ssd_opt = surface_space_dir / scale;
	
	epsilon = epsilon / length( esd_opt );
}

#ifdef METHOD_DESCARTES
/**
 * transform rays in a way that afterwards the interval (-1,1)
 * can be used for descartes method
 */
void normalize_interval( vec2 trace_interval )
{
	float ti_length = trace_interval[ 1 ] - trace_interval[ 0 ];
	float shift = 0.5 * ( trace_interval[ 1 ] + trace_interval[ 0 ] );
	float scale = 0.5 * ti_length;
	
	ese_opt = ese_opt + shift * esd_opt;
	esd_opt = esd_opt * scale;
	
	cse_opt = cse_opt + shift * csd_opt;
	csd_opt = csd_opt * scale;
	
	sse_opt = sse_opt + shift * ssd_opt;
	ssd_opt = ssd_opt * scale;
}
#endif

/**
 * main method, that guides the overall process
 */
void main( void )
{
	optimize_rays();
	
	// we will only look for roots in this interval
	vec2 trace_interval = clip( cse_opt, csd_opt );

#ifdef METHOD_DESCARTES
	normalize_interval( trace_interval );
	trace_interval = vec2( -1.0, 1.0 );
#endif
	
	// holds coefficients of univariate polynomial f( eye + t * dir )
	polynomial p = calc_coefficients( sse_opt, ssd_opt, trace_interval );
	
	// solve equation sum_{i=0}^DEGREE a[ i ] * x^i = 0
	roots r = solve( p, trace_interval );
	gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
	gl_FragDepth = 0.5;
	
	// now we can further process the pixel, i.e. illuminate it, calculate it's depth value ...
	process( r, ese_opt,esd_opt );
}