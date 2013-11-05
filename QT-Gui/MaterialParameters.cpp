#include "MaterialParameters.h"

MaterialParameters::MaterialParameters( const QString &name, QWidget *parent ) : QWidget( parent )
{
	// setup user interface, pointers to gui elements and connections
	setupUi( this );
	setupPrivates();
	setName( name );
	setupConnections();
}

MaterialParameters::MaterialParameters( const QString &name, const MaterialProperties &mp, QWidget *parent ) : QWidget( parent )
{
	// setup user interface, pointers to gui elements, initial value and connections
	setupUi( this );
	setupPrivates();
	setName( name );
	setValue( mp );
	setupConnections();
}

void MaterialParameters::setupPrivates()
{
	shininess = findChild< QDoubleSpinBox * >( "shininess" );
	transparency = findChild< QDoubleSpinBox * >( "transparency" );
	transparencyLabel = findChild< QLabel * >( "transparencyLabel" );
	ambient = findChild< ColorPicker * >( "ambient" );
	diffuse = findChild< ColorPicker * >( "diffuse" );
	specular = findChild< ColorPicker * >( "specular" );
	emission = findChild< ColorPicker * >( "emission" );
	groupBox = findChild< QGroupBox * >( "groupBox" );
	predefinedMaterials = findChild< PredefinedMaterials * >( "predefinedMaterials" );
}

void MaterialParameters::setupConnections()
{
	QObject::connect( shininess, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( transparency, SIGNAL( valueChanged( double ) ), this, SLOT( someValueChanged() ) );

	QObject::connect( ambient, SIGNAL( valueChanged( const QColor & ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( diffuse, SIGNAL( valueChanged( const QColor & ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( specular, SIGNAL( valueChanged( const QColor & ) ), this, SLOT( someValueChanged() ) );
	QObject::connect( emission, SIGNAL( valueChanged( const QColor & ) ), this, SLOT( someValueChanged() ) );

	QObject::connect( predefinedMaterials, SIGNAL( materialSelected( const MaterialProperties & ) ), this, SLOT( setValue( const MaterialProperties & ) ) );
}

void MaterialParameters::someValueChanged()
{
	valueChanged( getValue() );
}

void MaterialParameters::setTransparencyEnabled( bool on )
{
	transparency->setEnabled( on );
	transparencyLabel->setEnabled( on );
}

const MaterialProperties &MaterialParameters::getValue() 
{
	mp.ambient = ambient->getColor();
	mp.diffuse = diffuse->getColor();
	mp.specular = specular->getColor();
	mp.emission = emission->getColor();
	mp.shininess = shininess->value();
	mp.transparency = transparency->value();

	return mp;
}

QString MaterialParameters::getName()
{
	return this->groupBox->title();
}

void MaterialParameters::usePredefinedMaterial( const QString &name )
{
	predefinedMaterials->materialSelected( name );
}

void MaterialParameters::setValue( const MaterialProperties &mp )
{
	if( mp != this->mp )
	{
		shininess->setValue( mp.shininess );
		transparency->setValue( mp.transparency );
		ambient->setColor( mp.ambient );
		diffuse->setColor( mp.diffuse );
		specular->setColor( mp.specular );
		emission->setColor( mp.emission );

		valueChanged( mp );
	}
}

void MaterialParameters::setName( const QString &name )
{
	if( this->groupBox->title() != name )
	{
		this->groupBox->setTitle( name );
		nameChanged( name );
	}
}