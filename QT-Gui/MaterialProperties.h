#ifndef __MATERIALPROPERTIES_H__
#define __MATERIALPROPERTIES_H__

#include <QColor>
#include <QDataStream>

struct MaterialProperties
{
	double shininess;
	double transparency;
	QColor ambient;
	QColor diffuse;
	QColor specular;
	QColor emission;
};

bool operator==( const MaterialProperties& m1, const MaterialProperties& m2 );
bool operator!=( const MaterialProperties& m1, const MaterialProperties& m2 );

QDataStream & operator<< ( QDataStream & stream, const MaterialProperties & mp );
QDataStream & operator>> ( QDataStream & stream, MaterialProperties & mp );

#endif