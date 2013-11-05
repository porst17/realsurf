#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QTabWidget>

#include "FloatParameter.h"
#include "ParameterWidget.h"

#include <iostream>



int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
//#define ABC
#ifdef ABC
	ParameterWidget *pw = new ParameterWidget();
	pw->setProperties( "a", FloatParameterProperties() );
	pw->setProperties( "b", FloatParameterProperties() );
	pw->setProperties( "c", FloatParameterProperties() );
	pw->setProperties( "d", FloatParameterProperties() );
	pw->show();
#else
	QTabWidget *tw = new QTabWidget();

	QScrollArea *sa = new QScrollArea( );
	sa->setWidgetResizable( true );

	QWidget *w = new QWidget(  );
	w->setLayout( new QVBoxLayout() );
	w->layout()->addWidget( new CoordinateSystemWidget( "a", FloatParameterProperties(), "b", FloatParameterProperties(), w ) );
	( ( QVBoxLayout * ) w->layout() )->addStretch();

	sa->setWidget( w );

	tw->addTab( sa, "bla" );
	tw->show();

	app.processEvents();
	w->layout()->addWidget( new CoordinateSystemWidget() );
#endif

	return app.exec();
}