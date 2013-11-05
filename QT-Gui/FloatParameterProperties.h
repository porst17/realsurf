#ifndef __FLOATPARAMETERPROPERTIES_H__
#define __FLOATPARAMETERPROPERTIES_H__

#include <QDataStream.h>

struct FloatParameterProperties
{
	FloatParameterProperties( double min = 0.0, double max = 1.0, double value = 0.5 );
	FloatParameterProperties( const FloatParameterProperties &p );

	inline double minimum() const { return _min; }
	inline double maximum() const { return _max; }
	inline double value() const { return _value; }

	inline void setMin( double min ) { setProperties( min, this->_max, this->_value ); }
	inline void setMax( double max ) { setProperties( this->_min, max, this->_value ); }
	inline void setValue( double value ) { setProperties( this->_min, this->_max, value ); }

	void setProperties( double min, double max, double value );
	void setProperties( const FloatParameterProperties &p );

	private:
		double _min;
		double _max;
		double _value;
};

bool operator==( const FloatParameterProperties& fpp1, const FloatParameterProperties& fpp2 );
bool operator!=( const FloatParameterProperties& fpp1, const FloatParameterProperties& fpp2 );

QDataStream & operator<< ( QDataStream & stream, const FloatParameterProperties & fpp );
QDataStream & operator>> ( QDataStream & stream, FloatParameterProperties & fpp );

#endif
