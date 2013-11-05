#include "MaterialProperties.h"

bool operator==( const MaterialProperties& m1, const MaterialProperties& m2 )
{
	return m1.ambient == m2.ambient
		&& m1.diffuse == m2.diffuse
		&& m1.emission == m2.emission
		&& m1.shininess == m2.shininess
		&& m1.specular == m2.specular
		&& m1.transparency == m2.transparency;
}

bool operator!=( const MaterialProperties& m1, const MaterialProperties& m2 )
{
	return !( m1 == m2 );
}

QDataStream & operator<< ( QDataStream & stream, const MaterialProperties & mp )
{
	stream << mp.emission << mp.ambient << mp.diffuse << mp.specular;
	stream << mp.shininess << mp.transparency;
	return stream;
}

QDataStream & operator>> ( QDataStream & stream, MaterialProperties & mp )
{
	stream >> mp.emission >> mp.ambient >> mp.diffuse >> mp.specular;
	stream >> mp.shininess >> mp.transparency;	
	return stream;
}
