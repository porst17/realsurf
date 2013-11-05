#define MAX_LIGHTS 3

uniform mat4 surface_transform_inverse;
uniform mat4 surface_transform;

uniform int clip_to;

/*uniform */float epsilon = 0.000001;

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

polynomial power( polynomial p, int exp, int degree )
{
	polynomial res = create_poly_0( 1.0 );
	int res_degree = degree;
	for( ; exp > 0; exp-- )
	{
		res = mult( res, p, res_degree );
		res_degree += degree;
	}
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

float power( float base, int exp )
{
	float res = 1.0;
	for( int i = 0; i < exp; i++ )
		res *= base;
	return res;
}

/*
// Versuche mit gcd - geht noch nicht
polynomial div( polynomial zaehler, int z_degree, polynomial nenner, int n_degree )
{
	polynomial quotient;
	for( int i = z_degree - n_degree; i >= 0; i-- )
	{
		// schrittweise Reduktion von zaehler bis nicht mehr teilbar -> zaehler enthält am Ende den Rest
		quotient.a[ i ] = zaehler.a[ i + n_degree ] / nenner.a[ n_degree ];
		for( int j = n_degree; j >= 0; j-- )
		{
			zaehler.a[ i + j ] = zaehler.a[ i + j ] - nenner.a[ j ] * quotient.a[ i ];
		}
	}
	return quotient;
}

polynomial mod( polynomial zaehler, int z_degree, polynomial nenner, int n_degree )
{
	polynomial quotient;
	for( int i = z_degree - n_degree; i >= 0; i-- )
	{
		// schrittweise Reduktion von zaehler bis nicht mehr teilbar -> zaehler enthält am Ende den Rest
		quotient.a[ i ] = zaehler.a[ i + n_degree ] / nenner.a[ n_degree ];
		for( int j = n_degree; j >= 0; j-- )
		{
			zaehler.a[ i + j ] = zaehler.a[ i + j ] - nenner.a[ j ] * quotient.a[ i ];
		}
	}
	return zaehler;
}

int real_degree( polynomial p )
{
	int degree = DEGREE;
	bool degree_found = false;
	for( int i = DEGREE; i >= 0; i-- )
	{
		if( !degree_found && abs( p.a[ i ] ) < 0.0000000001 )
		{
			degree = i;
			degree_found = true;
		}
	}
	return degree;
}

polynomial simplify_roots( polynomial p, int degree )
{
	// calculate derivate of p
	polynomial p_diff;
	for( int i = 0; i < degree - 1; i++ )
		p_diff.a[ i ] = float( i + 1 ) * p.a[ i + 1 ];
	
	// calculate gcd of p and p_diff
	polynomial a = p;
	polynomial b = p_diff;
	int real_degree_a = degree;
	int real_degree_b = degree - 1;
	for( int degree_a = degree; degree_a > 0; degree_a-- )
	{
		if( degree_a == real_degree_a )
		{
			for( int degree_b = degree_a; degree_b > 0; degree_b++ )
			{
				if( degree_b == real_degree_b )
				{
					polynomial h;
					h = mod( a, degree_a, b, degree_b );
					a = b;
					real_degree_a = real_degree_b;
					b = h;
					real_degree_b = real_degree( b );
				}
			}
		}
	}
	// a is now gcd
	
	// p / gcd has only simple roots
	polynomial result;
	for( int i = 0; i < DEGREE; i++ )
		if( i == real_degree_a )
			result = div( p, degree, a, i );
	
	return result;
}
*/

/**
 * methods, that operate with that algebraic function
 */
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

float bisection( int f_index, float x0, float f0, float x1, float f1 )
{
	float x2 = x0;
	while( abs( x0 - x1 ) > epsilon )
	{
		x2 = 0.5 * ( x0 + x1 );
		float f2 = eval_f( f_index, x2 );
		if( f2 * f0 < 0 )
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

struct roots
{
	float x[ DEGREE + 2 ];
	bool valid[ DEGREE + 2 ];
};

bool contains( const in vec2 interval, const in float value )
{
	return ( interval[ 0 ] < value && value < interval[ 1 ] ) || ( interval[ 0 ] > value && value > interval[ 1 ] );
}

float newton_special( int f_index, float x )
{
	float last_x = x + 2.0 * epsilon;
	while( abs( x - last_x ) > epsilon )
	{
		last_x = x;
		float f = eval_f( f_index, x );
		float df = eval_f( f_index + 1, x );
		x = x - f / df;
	}
	return x;
}

void calc_roots( const in int derivate_num, const in roots read_from, inout roots write_to )
{
	// for each root pair of the current derivate: look for roots of previous derivate
	float tmp[ DEGREE + 2 ] = write_to.x;
	for( int interval_num = 0; interval_num < DEGREE + 2 - 1 - derivate_num; interval_num++ )
	{
		write_to.x[ interval_num + 1 ] = write_to.x[ interval_num ];
		write_to.valid[ interval_num + 1 ] = false;
		
		if( read_from.valid[ interval_num + 1 ] )
		{
			float f0 = eval_f( derivate_num, read_from.x[ interval_num ] );
			float f1 = eval_f( derivate_num, read_from.x[ interval_num + 1 ] );
			if( write_to.valid[ interval_num + 1 ] = ( f0 * f1 < 0.0 ) )
			{
				
				if( DEGREE - derivate_num > 2 && read_from.x[ interval_num ] != read_from.x[ 0 ] && read_from.x[ interval_num + 1 ] != read_from.x[ DEGREE - derivate_num + 1 ] )
					// newton
					write_to.x[ interval_num + 1 ] = newton_special( derivate_num, tmp[ interval_num ] );
				else
					// bisection
					write_to.x[ interval_num + 1 ] = bisection( derivate_num, read_from.x[ interval_num ], f0, read_from.x[ interval_num + 1 ], f1 );
			}
				// there is one root in current interval and root finder will converge
				//write_to.x[ interval_num + 1 ] = bisection( derivate_num, read_from.x[ interval_num ], f0, read_from.x[ interval_num + 1 ], f1 );
				//write_to.x[ interval_num + 1 ] = newton_iteration( derivate_num, read_from.x[ interval_num ], f0, read_from.x[ interval_num + 1 ], f1 );
				//write_to.x[ interval_num + 1 ] = newton( derivate_num, read_from.x[ interval_num ], f0, read_from.x[ interval_num + 1 ], f1 );
				//write_to.x[ interval_num + 1 ] = zbrent( derivate_num, read_from.x[ interval_num ], f0, read_from.x[ interval_num + 1 ], f1 );
				//write_to.x[ interval_num + 1 ] = regula_falsi( derivate_num, read_from.x[ interval_num ], f0, read_from.x[ interval_num + 1 ], f1 );
				//write_to.x[ interval_num + 1 ] = sekant( derivate_num, read_from.x[ interval_num ], f0, read_from.x[ interval_num + 1 ], f1 );
			
		}
	}
	
	write_to.x[ DEGREE - derivate_num + 1 ] = read_from.x[ DEGREE - derivate_num + 1 ];
	write_to.valid[ DEGREE - derivate_num + 1 ] = true;
}

roots solve( const in polynomial p, const in vec2 trace_interval )
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
		roots tmp = read_from;
		read_from = write_to;
		write_to = tmp;
	}
#endif
	return read_from;
}

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
	float scale = length( surface_space_dir );
	
	ese_opt = eye_space_eye + best_start * eye_space_dir;
	esd_opt = eye_space_dir / scale;
	
	cse_opt = clipping_space_eye + best_start * clipping_space_dir;
	csd_opt = clipping_space_dir / scale;
	
	sse_opt = surface_space_eye + best_start * surface_space_dir;
	ssd_opt = surface_space_dir / scale;
	
	epsilon = epsilon / length( esd_opt );
}

/**
 * main method, that guides the overall process
 */
void main( void )
{
	optimize_rays();
	
	// we will only look for roots in this interval
	vec2 trace_interval = clip( cse_opt, csd_opt );
	
	// holds coefficients of univariate polynomial f( eye + t * dir )
	polynomial p = calc_coefficients( sse_opt, ssd_opt );
	
	// solve equation sum_{i=0}^DEGREE a[ i ] * x^i = 0 and return true, if there is any root within the trace interval
	roots r = solve( p, trace_interval );
	
	// now we can further process the pixel, i.e. illuminate it, calculate it's depth value ...
	process( r, ese_opt,esd_opt );
}