#ifndef __FLOATPARAMETER_H__
#define __FLOATPARAMETER_H__

#include "ui_floatParameter.h"
#include "FloatParameterProperties.h"

class FloatParameter : public QWidget, protected Ui_floatParameter
{
	Q_OBJECT
	
	private:
		QDoubleSpinBox *valueSpinBox;
		QDoubleSpinBox *minSpinBox;
		QDoubleSpinBox *maxSpinBox;
		QSlider *valueSlider;
		QGroupBox *groupBox;

	public:
		FloatParameter( const QString &name, const FloatParameterProperties &fpp, QWidget *parent = 0 );

		FloatParameterProperties getProperties() const;
		QString getName();

	public slots:
		void setProperties( const FloatParameterProperties &fpp );
	
	protected:
		void connectSubComponents();
		void disconnectSubComponents();

	protected slots:
		void spinBoxChanged();
		void setValueFromSlider();
		void setValueToSlider();

	signals:
		void propertiesChanged( const QString & name, const FloatParameterProperties &fpp );
};

bool operator==( const FloatParameterProperties& fpp1, const FloatParameterProperties& fpp2 );
bool operator!=( const FloatParameterProperties& fpp1, const FloatParameterProperties& fpp2 );

QDataStream & operator<< ( QDataStream & stream, const FloatParameterProperties & fpp );
QDataStream & operator>> ( QDataStream & stream, FloatParameterProperties & fpp );

#endif
