#ifndef __LIGHTPARAMETER_H__
#define __LIGHTPARAMETER_H__

#include "ui_lightParameters.h"
#include "LightProperties.h"

class LightParameters : public QWidget, protected Ui_lightParameters
{
	Q_OBJECT
	
	private:
		QDoubleSpinBox *posX, *posY, *posZ;
		QDoubleSpinBox *spotDirX, *spotDirY, *spotDirZ;
		QDoubleSpinBox *constantAttenuation, *linearAttenuation, *quadraticAttenuation;
		QDoubleSpinBox *spotCutoff;
		QDoubleSpinBox *spotCutoffExponent;
		ColorPicker *ambient;
		ColorPicker *diffuse;
		ColorPicker *specular;
		QGroupBox *groupBox;
		LightProperties lp;

		void setupPrivates();
		void setupConnections();

	private slots:
		void someValueChanged();

	public:
		LightParameters( QWidget *parent = 0 );
		LightParameters( const LightProperties &lp, QWidget *parent = 0 );

		const LightProperties &getValue();
		bool enabled();

	public slots:
		void setValue( const LightProperties &lp );
		void setEnabled( bool on );
	
	signals:
		void valueChanged( const LightProperties &lp );
		void toggled( bool on, const LightProperties &lp );
};

#endif
