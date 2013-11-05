#include "ParameterWidget.h"
#include <QVBoxLayout>
#include <QScrollArea>

ParameterWidget::ParameterWidget( QWidget *parent )
{
	this->setLayout( new QVBoxLayout() );
	this->oneD = new QList< FloatParameter * >();
	this->twoD = new QList< CoordinateSystemWidget * >();
	this->tabs = new QTabWidget( this );

	QScrollArea *sa1D = new QScrollArea( this->tabs );
	sa1D->setWidgetResizable( true );
	sa1D->setBackgroundRole(QPalette::AlternateBase);
	sa1D->setFrameStyle( QFrame::NoFrame );
	sa1D->setFrameShadow( QFrame::Plain );

	QScrollArea *sa2D = new QScrollArea( this->tabs );
	sa2D->setWidgetResizable( true );
	sa2D->setBackgroundRole(QPalette::AlternateBase);
	sa2D->setFrameStyle( QFrame::NoFrame );
	sa2D->setFrameShadow( QFrame::Plain );

	this->widget1D = new QWidget( sa1D );
	this->widget1D->setLayout( new QVBoxLayout() );
	( ( QVBoxLayout * ) this->widget1D->layout() )->addStretch();
	this->widget2D = new QWidget( sa2D );
	this->widget2D->setLayout( new QVBoxLayout() );
	( ( QVBoxLayout * ) this->widget2D->layout() )->addStretch();

	sa1D->setWidget( this->widget1D );
	sa2D->setWidget( this->widget2D );

	this->tabs->addTab( sa1D, "1D" );
	this->tabs->addTab( sa2D, "2D" );

	this->layout()->addWidget( this->tabs );

	this->oneD = new QList< FloatParameter * >();
	this->twoD = new QList< CoordinateSystemWidget * >();
}


ParameterWidget::~ParameterWidget()
{
	clear();
	delete this->oneD;
	delete this->twoD;
}

void ParameterWidget::setProperties( const QString &name, const FloatParameterProperties &p )
{
	FloatParameter *fp = NULL;
	for( QList< FloatParameter * >::iterator i = this->oneD->begin(); i != this->oneD->end(); i++ )
		if( ( *i )->getName() == name )
			fp = *i;

	if( fp == NULL )
	{
		// insert new parameter
		QList< CoordinateSystemWidget * > newCSWs = QList< CoordinateSystemWidget * >();
		QVBoxLayout *l1 = ( QVBoxLayout * ) this->widget2D->layout();
		l1->takeAt( l1->count() - 1 );
		for( QList< FloatParameter * >::iterator i = this->oneD->begin(); i != this->oneD->end(); i++ )
		{
			CoordinateSystemWidget *csw = new CoordinateSystemWidget( name, p, ( *i )->getName(), ( *i )->getProperties(), this->widget2D );
			l1->addWidget( csw );
			this->twoD->push_back( csw );
			QObject::connect( csw, SIGNAL( propertiesChanged( QString, FloatParameterProperties ) ), this, SLOT( setProperties( QString, FloatParameterProperties ) ) );
		}
		l1->addStretch();
		( *( this->twoD ) ) << newCSWs;
		fp = new FloatParameter( name, FloatParameterProperties( p.minimum(), p.maximum(), p.value() ), this->widget1D );
		QVBoxLayout *l0 = ( QVBoxLayout * ) this->widget1D->layout();
		l0->takeAt( l0->count() - 1 );
		l0->addWidget( fp );
		l0->addStretch();
		this->oneD->append( fp );
		QObject::connect( fp, SIGNAL( propertiesChanged( QString, FloatParameterProperties ) ), this, SLOT( setProperties( QString, FloatParameterProperties ) ) );
		propertiesChanged( name, p );
		valueChanged( name, p.value() );
	}
	else
	{
		// update existing parameter
		bool p_changed = false;
		bool v_changed = false;
		QObject::disconnect( fp, SIGNAL( propertiesChanged( QString, FloatParameterProperties ) ), this, SLOT( setProperties( QString, FloatParameterProperties ) ) );
		p_changed = p_changed || fp->getProperties() != p;
		v_changed = v_changed || fp->getProperties().value() != p.value();
		fp->setProperties( p );
		QObject::connect( fp, SIGNAL( propertiesChanged( QString, FloatParameterProperties ) ), this, SLOT( setProperties( QString, FloatParameterProperties ) ) );
		for( QList< CoordinateSystemWidget * >::iterator i = this->twoD->begin(); i != this->twoD->end(); i++ )
		{
			QString tmpName;
			FloatParameterProperties tmpFpp;

			QObject::disconnect( *i, SIGNAL( propertiesChanged( QString, FloatParameterProperties ) ), this, SLOT( setProperties( QString, FloatParameterProperties ) ) );
			( *i )->getPropertiesX( tmpName, tmpFpp );
			if( tmpName == name )
			{
				p_changed = p_changed || tmpFpp != p;
				v_changed = v_changed || tmpFpp.value() != p.value();
				( *i )->setPropertiesX( tmpName, p );
			}
			( *i )->getPropertiesY( tmpName, tmpFpp );
			if( tmpName == name )
			{
				p_changed = p_changed || tmpFpp != p;
				v_changed = v_changed || tmpFpp.value() != p.value();
				( *i )->setPropertiesY( tmpName, p );
			}
			QObject::connect( *i, SIGNAL( propertiesChanged( QString, FloatParameterProperties ) ), this, SLOT( setProperties( QString, FloatParameterProperties ) ) );
		}
		if( p_changed || this->twoD->isEmpty() )
			propertiesChanged( name, p );
		if( v_changed || this->twoD->isEmpty() )
			valueChanged( name, p.value() );
	}

	update();
}

void ParameterWidget::clear()
{
	for( QList< FloatParameter * >::iterator i = this->oneD->begin(); i != this->oneD->end(); i++ )
		QObject::disconnect( *i, 0, 0, 0 );
	this->oneD->clear();
	for( QList< CoordinateSystemWidget * >::iterator i = this->twoD->begin(); i != this->twoD->end(); i++ )
		QObject::disconnect( *i, 0, 0, 0 );
	this->twoD->clear();

	QLayoutItem *child;
	while( ( child = this->widget1D->layout()->takeAt( 0 ) ) != 0 )
	{
		if( child->widget() != 0 )
			child->widget()->close();
		delete child->widget();
		delete child;
	}
	( ( QVBoxLayout * ) this->widget1D->layout() )->addStretch();
	while( ( child = this->widget2D->layout()->takeAt( 0 ) ) != 0 )
	{
		if( child->widget() != 0 )
			child->widget()->close();
		delete child->widget();
		delete child;
	}
}

QMap< QString, FloatParameterProperties > ParameterWidget::getProperties() const
{
	QMap< QString, FloatParameterProperties > m;
	for( QList< FloatParameter * >::const_iterator i = this->oneD->begin(); i != this->oneD->end(); i++ )
		m.insert( ( *i )->getName(), ( *i )->getProperties() );
	return m;
}

