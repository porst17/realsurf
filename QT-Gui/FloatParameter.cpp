#include "FloatParameter.h"
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QMessageBox>

FloatParameter::FloatParameter( const QString &name, const FloatParameterProperties &fpp, QWidget *parent ) : QWidget( parent )
{
	// setup user interface
	setupUi( this );

	// save pointers to subcomponents
	this->valueSpinBox = this->findChild< QDoubleSpinBox * >( "floatParameterValue" );
	this->minSpinBox = this->findChild< QDoubleSpinBox * >( "floatParameterMin" );
	this->maxSpinBox = this->findChild< QDoubleSpinBox * >( "floatParameterMax" );
	this->valueSlider = this->findChild< QSlider * >( "floatParameterSlider" );
	this->groupBox = this->findChild< QGroupBox * >( "floatParameterGroupBox" );

	// setup connections
	connectSubComponents();

	// setup group box label
	this->groupBox->setTitle( name );

	// set values
	setProperties( fpp );
}

void FloatParameter::connectSubComponents()
{
	QObject::connect( this->minSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( spinBoxChanged() ) );
	QObject::connect( this->maxSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( spinBoxChanged() ) );
	QObject::connect( this->valueSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( spinBoxChanged() ) );
	QObject::connect( this->valueSlider, SIGNAL( valueChanged( int ) ), this, SLOT( setValueFromSlider() ) );
}

void FloatParameter::disconnectSubComponents()
{
	QObject::disconnect( this->minSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( spinBoxChanged() ) );
	QObject::disconnect( this->maxSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( spinBoxChanged() ) );
	QObject::disconnect( this->valueSpinBox, SIGNAL( valueChanged( double ) ), this, SLOT( spinBoxChanged() ) );
	QObject::disconnect( this->valueSlider, SIGNAL( valueChanged( int ) ), this, SLOT( setValueFromSlider() ) );
}

QString FloatParameter::getName() { return this->groupBox->title(); }
FloatParameterProperties FloatParameter::getProperties() const
{
	return FloatParameterProperties( minSpinBox->value(), maxSpinBox->value(), valueSpinBox->value() );
}

void FloatParameter::setProperties( const FloatParameterProperties &fpp )
{
	this->minSpinBox->setMaximum( fpp.maximum() );
	this->minSpinBox->setValue( fpp.minimum() );

	this->maxSpinBox->setValue( fpp.maximum() );
	this->maxSpinBox->setMinimum( fpp.minimum() );

	this->valueSpinBox->setValue( fpp.value() );
	this->valueSpinBox->setMinimum( fpp.minimum() );
	this->valueSpinBox->setMaximum( fpp.maximum() );
	this->valueSpinBox->setSingleStep( ( fpp.maximum() - fpp.minimum() ) / 100.0 );

	this->setValueToSlider();
	update();

	this->propertiesChanged( this->getName(), this->getProperties() );
}

void FloatParameter::spinBoxChanged()
{
	this->setProperties( FloatParameterProperties( minSpinBox->value(), maxSpinBox->value(), valueSpinBox->value() ) );
	update();
}

void FloatParameter::setValueFromSlider()
{
	double percent = double( valueSlider->value() - valueSlider->minimum() ) / double( valueSlider->maximum() - valueSlider->minimum() );
	this->disconnectSubComponents();
	valueSpinBox->setValue( this->minSpinBox->value() + ( this->maxSpinBox->value() - this->minSpinBox->value() ) * percent );
	update();
	this->connectSubComponents();
	this->propertiesChanged( this->getName(), this->getProperties() );
}

void FloatParameter::setValueToSlider()
{
	double percent = ( this->valueSpinBox->value() - this->minSpinBox->value() ) / ( this->maxSpinBox->value() - this->minSpinBox->value() );
	this->disconnectSubComponents();
	valueSlider->setValue( int( valueSlider->minimum() + ( valueSlider->maximum() - valueSlider->minimum() ) * percent ) );
	update();
	this->connectSubComponents();
}
