#include "FloatParameterProperties.h"

FloatParameterProperties::FloatParameterProperties( double min, double max, double value )
{
	this->setProperties( min, max, value );
}

FloatParameterProperties::FloatParameterProperties( const FloatParameterProperties &p ) : _min( p._min ), _max( p._max ), _value( p._value ) {}

void FloatParameterProperties::setProperties( double min, double max, double value )
{
	if( max < min )
		max = min;
	if( value < min )
		value = min;
	if( value > max )
		value = max;
	this->_min = min;
	this->_max = max;
	this->_value = value;
}

void FloatParameterProperties::setProperties( const FloatParameterProperties &p )
{
	this->_min = p._min;
	this->_max = p._max;
	this->_value = p._value;
}

bool operator==( const FloatParameterProperties& fpp1, const FloatParameterProperties& fpp2 )
{
	return fpp1.minimum() == fpp2.minimum()
		&& fpp1.maximum() == fpp2.maximum()
		&& fpp1.value() == fpp2.value();
}

bool operator!=( const FloatParameterProperties& fpp1, const FloatParameterProperties& fpp2 )
{
	return !( fpp1 == fpp2 );
}


QDataStream & operator<< ( QDataStream & stream, const FloatParameterProperties & fpp )
{
	stream << fpp.minimum() << fpp.maximum() << fpp.value();
	return stream;
}

QDataStream & operator>> ( QDataStream & stream, FloatParameterProperties & fpp )
{
	double min;
	double max;
	double value;
	stream >> min >> max >> value;
	fpp.setProperties( min, max, value );
	return stream;
}
