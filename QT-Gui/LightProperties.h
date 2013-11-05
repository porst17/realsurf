#ifndef __LIGHTPROPERTIES_H__
#define __LIGHTPROPERTIES_H__

#include <QColor>

struct LightProperties
{
	double posX, posY, posZ;
	double spotDirX, spotDirY, spotDirZ;
	double constantAttenuation, linearAttenuation, quadraticAttenuation;
	double spotCutoff;
	double spotCutoffExponent;
	QColor ambient;
	QColor diffuse;
	QColor specular;
};

bool operator==( const LightProperties& l1, const LightProperties& l2 );
bool operator!=( const LightProperties& l1, const LightProperties& l2 );

QDataStream & operator<< ( QDataStream & stream, const LightProperties & lp );
QDataStream & operator>> ( QDataStream & stream, LightProperties & lp );

#endif