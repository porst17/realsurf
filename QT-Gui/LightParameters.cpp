#include "LightParameters.h"

LightParameters::LightParameters( QWidget *parent ) : QWidget( parent )
{
	// setup user interface, pointers to gui elements, connections and internal light parameter state
	setupUi( this );
	setupPrivates();
	setupConnections();
	getValue();
}

LightParameters::LightParameters( const LightProperties &lp, QWidget *parent ) : QWidget( parent )
{
	// setup user interface, pointers to gui elements, initial value, connections and internal light parameter state
	setupUi( this );
	setupPrivates();
	setValue( lp );
	setupConnections();
	getValue();
}

void LightParameters::setupPrivates()
{
	posX = findChild< QDoubleSpinBox * >( "posX" );
	posY = findChild< QDoubleSpinBox * >( "posY" );
	posZ = findChild< QDoubleSpinBox * >( "posZ" );
	spotDirX = findChild< QDoubleSpinBox * >( "spotDirX" );
	spotDirY = findChild< QDoubleSpinBox * >( "spotDirY" );
	spotDirZ = findChild< QDoubleSpinBox * >( "spotDirZ" );
	constantAttenuation = findChild< QDoubleSpinBox * >( "constantAttenuation" );
	linearAttenuation = findChild< QDoubleSpinBox * >( "linearAttenuation" );
	quadraticAttenuation = findChild< QDoubleSpinBox * >( "quadraticAttenuation" );
	spotCutoff = findChild< QDoubleSpinBox * >( "spotCutoff" );
	spotCutoffExponent = findChild< QDoubleSpinBox * >( "spotCutoffExponent" );
	ambient = findChild< ColorPicker * >( "ambient" );
	diffuse = findChild< ColorPicker * >( "diffuse" );
	specular = findChild< ColorPicker * >( "specular" );
	groupBox = findChild< QGroupBox * >( "groupBox" );
}

void LightParameters::setupConnections()
{
	QObject::connect( posX, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( posY, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( posZ, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );

	QObject::connect( spotDirX, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( spotDirY, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( spotDirZ, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );

	QObject::connect( constantAttenuation, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( linearAttenuation, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( quadraticAttenuation, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );

	QObject::connect( spotCutoff, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( spotCutoffExponent, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );

	QObject::connect( ambient, SIGNAL( valueChanged( const QColor & ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( diffuse, SIGNAL( valueChanged( const QColor & ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( specular, SIGNAL( valueChanged( const QColor & ) ), this, SLOT( someValueChanged() ) );

	QObject::connect( groupBox, SIGNAL( toggled( bool ) ), this, SLOT( setEnabled( bool ) ) );
}

void LightParameters::someValueChanged()
{
	valueChanged( getValue() );
}

void LightParameters::setEnabled( bool on )
{
	groupBox->setChecked( on );
	toggled( on, getValue() );
}

const LightProperties &LightParameters::getValue() 
{
	lp.posX = posX->value();
	lp.posY = posY->value();
	lp.posZ = posZ->value();
	lp.spotDirX = spotDirX->value();
	lp.spotDirY = spotDirY->value();
	lp.spotDirZ = spotDirZ->value();
	lp.constantAttenuation = constantAttenuation->value();
	lp.linearAttenuation = linearAttenuation->value();
	lp.quadraticAttenuation = quadraticAttenuation->value();
	if( spotCutoff->value() > 90.0 )
			spotCutoff->setValue( 180.0 );
	lp.spotCutoff = spotCutoff->value();
	lp.spotCutoffExponent = spotCutoffExponent->value();
	lp.ambient = ambient->getColor();
	lp.diffuse = diffuse->getColor();
	lp.specular = specular->getColor();

	return lp;
}

bool LightParameters::enabled()
{
	return groupBox->isChecked();
}


void LightParameters::setValue( const LightProperties &lp )
{
	if( lp != this->lp )
	{
		posX->setValue( lp.posX );
		posY->setValue( lp.posX );
		posZ->setValue( lp.posX );
		spotDirX->setValue( lp.spotDirX );
		spotDirY->setValue( lp.spotDirY );
		spotDirZ->setValue( lp.spotDirZ );
		constantAttenuation->setValue( lp.constantAttenuation );
		linearAttenuation->setValue( lp.linearAttenuation );
		quadraticAttenuation->setValue( lp.quadraticAttenuation );
		if( lp.spotCutoff > 90.0 )
			spotCutoff->setValue( 180.0 );
		else
			spotCutoff->setValue( lp.spotCutoff );
		spotCutoffExponent->setValue( lp.spotCutoffExponent );
		ambient->setColor( lp.ambient );
		diffuse->setColor( lp.diffuse );
		specular->setColor( lp.specular );

		valueChanged( lp );
	}
}