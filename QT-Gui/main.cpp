#include "MainWindow.h"
#include "BenchmarkMainWindow.h"
#include <QApplication>
#include <QCleanlooksStyle>

int main( int argc, char *argv[] )
{
	QApplication::setDesktopSettingsAware( false );
	QApplication::setStyle( new QCleanlooksStyle() );
	QApplication::setEffectEnabled( Qt::UI_AnimateCombo );
	QApplication::setEffectEnabled( Qt::UI_AnimateTooltip );
	QApplication::setEffectEnabled( Qt::UI_FadeTooltip );
	QApplication app( argc, argv );

	MainWindow *mw;
	if( app.arguments().contains( "--benchmark" ) )
		mw = new BenchmarkMainWindow();
	else
		mw = new MainWindow();
	mw->show();
	app.exec();
	delete mw;
}