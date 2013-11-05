#include "LightProperties.h"
#include <QByteArray>
#include <QDataStream>

bool operator==( const LightProperties& l1, const LightProperties& l2 )
{
	return l1.posX == l2.posX
		&& l1.posY == l2.posY
		&& l1.posZ == l2.posZ
		&& l1.spotDirX == l2.spotDirX
		&& l1.spotDirY == l2.spotDirY
		&& l1.spotDirZ == l2.spotDirZ
		&& l1.constantAttenuation == l2.constantAttenuation
		&& l1.linearAttenuation == l2.linearAttenuation
		&& l1.quadraticAttenuation == l2.quadraticAttenuation
		&& l1.spotCutoff == l2.spotCutoff
		&& l1.spotCutoffExponent == l2.spotCutoffExponent
		&& l1.ambient == l2.ambient
		&& l1.diffuse == l2.diffuse
		&& l1.specular == l2.specular;
}

bool operator!=( const LightProperties& l1, const LightProperties& l2 )
{
	return !( l1 == l2 );
}

QDataStream & operator<< ( QDataStream & stream, const LightProperties & lp )
{
	stream << lp.posX << lp.posY << lp.posZ;
	stream << lp.spotDirX << lp.spotDirY << lp.spotDirZ;
	stream << lp.constantAttenuation << lp.linearAttenuation << lp.quadraticAttenuation;
	stream << lp.spotCutoff << lp.spotCutoffExponent;
	stream << lp.ambient << lp.diffuse << lp.specular;
	return stream;
}

QDataStream & operator>> ( QDataStream & stream, LightProperties & lp )
{
	stream >> lp.posX >> lp.posY >> lp.posZ;
	stream >> lp.spotDirX >> lp.spotDirY >> lp.spotDirZ;
	stream >> lp.constantAttenuation >> lp.linearAttenuation >> lp.quadraticAttenuation;
	stream >> lp.spotCutoff >> lp.spotCutoffExponent;
	stream >> lp.ambient >> lp.diffuse >> lp.specular;
	return stream;
}
