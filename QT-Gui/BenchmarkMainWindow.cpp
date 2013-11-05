#include <QtGui>
#include <QMessageBox>
#include <QImage>
#include <QDir>
#include <QStringList>
#include <QTime>
#include <QFileDialog>
#include <QFile>
#include <QStatusBar>
#include "BenchmarkMainWindow.h"
#include <GL/GLee.h>

BenchmarkMainWindow::BenchmarkMainWindow() : MainWindow( false )
{
	this->setStatusBar( new QStatusBar() );
	this->statusBar()->setSizeGripEnabled( false );

	QMessageBox::information( this, "Information", "RealSurf started in benchmark mode." );

	if( !( QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_2_0 ) )
	{
		QMessageBox::critical( 0, "OpenGL error", "Your graphics card does not support OpenGL 2.0 or Shader Model 3.\nRealSurf will be closed now." );
		QCoreApplication::quit();
	}

	this->minimalGUI( true );
	this->srw->processInputEvents( false );
	this->srw->setFeedbackEnabled( false );
	this->ensurePolished();

	QObject::connect( this, SIGNAL( startBenchmark() ), this, SLOT( benchmark() ), Qt::QueuedConnection );
	startBenchmark();
}

void BenchmarkMainWindow::benchmark()
{
	QString dirname = QString();
	while( dirname.isNull() )
		dirname = QFileDialog::getExistingDirectory( this, "Select directory for benchmark results", QCoreApplication::applicationDirPath() + QDir::separator() + "benchmark results" );

	// size of renderings
	QSize s = QSize( 512, 512 );
	this->fitSize( s.width(), s.height() );

	// retrieve all available methods
	const QMap< glsl::ShaderManager::PolyCoeffMethod, QString > pcms = SurfaceRenderWidget::getPolyCoeffMethods();
	const QMap< glsl::ShaderManager::RootFinderMethod, QString > rfms = SurfaceRenderWidget::getRootFinderMethods();

	// load all surfaces from 'benchmark' folder
	this->statusBar()->showMessage( "loading surfaces" );
	QList< QImage* > bms_list;
	QDir benchmark_surfaces_dir( QCoreApplication::applicationDirPath () + QDir::separator() + "benchmark" );
	QStringList benchmark_surfaces = benchmark_surfaces_dir.entryList( QDir::Files );
	QStringList surface_equations;
	surface_equations << "===================================================================================================\n";
	surface_equations << "equations of surfaces\n";
	surface_equations << "===================================================================================================\n";
	surface_equations << "\n";
	for( QStringList::iterator it = benchmark_surfaces.begin(); it != benchmark_surfaces.end(); it++ )
	{
		QImage *surface = new QImage( benchmark_surfaces_dir.filePath( *it ), "PNG" );
		if( isStoredSurface( *surface ) )
		{
			bms_list.append( surface );
			surface_equations << surface->text( "realsurf_name" ) << ": " << surface->text( "realsurf_formula" ) << "=0\n";
		}
	}

	// how much tests must be performed?
	int number_of_tests = ( pcms.size() + rfms.size() - 1 /* computer algebra & d-chain bisection counted twice, but used once */ ) * bms_list.size();
	int current_test = 0;
	int max_time = 5 * 1000 /* 5s */;
	QTime t = QTime();

	// benchmark methods for finding polynomial coefficients (d-chain with bisection is used as root finder)
	QStringList pcm_result;
	{
		this->srw->setRootFinderMethod( glsl::ShaderManager::D_CHAIN_BISECTION );
		pcm_result << "===================================================================================================\n";
		pcm_result << "benchmark of calculation of polynomial coefficients (d-chain with bisection is used as root finder)\n";
		pcm_result << "(table cell format: compile time ms|render time ms|render fps)\n";
		pcm_result << "===================================================================================================\n";
		pcm_result << "\n";
		pcm_result << "surface";
		for( QMap< glsl::ShaderManager::PolyCoeffMethod, QString >::const_iterator pcm_it = pcms.constBegin(); pcm_it != pcms.constEnd(); pcm_it++ )
			pcm_result << " & " << pcm_it.value();
		pcm_result << "\n";
		for( QList< QImage* >::iterator it = bms_list.begin(); it != bms_list.end(); it++ )
		{
			QString surface_name = (*it)->text( "realsurf_name" );
			pcm_result << surface_name;
			for( QMap< glsl::ShaderManager::PolyCoeffMethod, QString >::const_iterator pcm_it = pcms.constBegin(); pcm_it != pcms.constEnd(); pcm_it++ )
			{
				QCoreApplication::processEvents();

				pcm_result << " & ";

				this->srw->setPolyCoeffMethod( pcm_it.key() );
				current_test++;
				this->statusBar()->showMessage( "benchmarking " + pcm_it.value() + " on " + surface_name + " (test " + QString::number( current_test ) + " of " + QString::number( number_of_tests ) + ")" );

				t.restart();
				loadSurface( **it );
				pcm_result << QString::number( t.elapsed() ) << "ms";

				// process pending events and draw surface several times (because first few renderings seem to have some overhead)
				QCoreApplication::processEvents();
				this->srw->repaint();

				// save original view
				this->saveCurrentView( dirname, surface_name + " (" + pcm_it.value() + ")" );

				// set special benchmark materials
				this->front_mat->usePredefinedMaterial( "benchmark 1" );
				this->back_mat->usePredefinedMaterial( "benchmark 2" );
				this->srw->repaint();
				this->saveCurrentView( dirname, surface_name + " - bm (" + pcm_it.value() + ")" );

				QCoreApplication::processEvents();
				t.restart();
				int number_of_frames = 0;
				int my_time;
				while( ( my_time = t.elapsed() ) < 5000 )
				{
					this->srw->repaint();
					number_of_frames++;
				}
				pcm_result << ";" << QString::number( my_time / float( number_of_frames ), 'f', 2 ) << "ms;" << QString::number( ( float( number_of_frames ) / ( my_time / 1000.0f ) ), 'f', 2 ) << "fps";
			}
			pcm_result << "\n";
		}
	}

	// benchmark methods for root finding (computer algebra is used for coefficient calculation)
	QStringList rfm_result;
	{
		this->srw->setPolyCoeffMethod( glsl::ShaderManager::COMPUTER_ALGEBRA );
		rfm_result << "===================================================================================================\n";
		rfm_result << "benchmark methods for root finding (computer algebra is used for coefficient calculation)\n";
		rfm_result << "(table cell format: compile time ms|render time ms|render fps)\n";
		rfm_result << "===================================================================================================\n";
		rfm_result << "\n";
		rfm_result << "surface";
		for( QMap< glsl::ShaderManager::RootFinderMethod, QString >::const_iterator rfm_it = rfms.constBegin(); rfm_it != rfms.constEnd(); rfm_it++ )
			if( rfm_it.key() != glsl::ShaderManager::D_CHAIN_BISECTION )
				rfm_result << " & " << rfm_it.value();
		rfm_result << "\n";
		for( QList< QImage* >::iterator it = bms_list.begin(); it != bms_list.end(); it++ )
		{
			QString surface_name = (*it)->text( "realsurf_name" );
			rfm_result << surface_name;
			for( QMap< glsl::ShaderManager::RootFinderMethod, QString >::const_iterator rfm_it = rfms.constBegin(); rfm_it != rfms.constEnd(); rfm_it++ )
			{
				if( rfm_it.key() == glsl::ShaderManager::D_CHAIN_BISECTION )
					continue;

				QCoreApplication::processEvents();

				rfm_result << " & ";

				this->srw->setRootFinderMethod( rfm_it.key() );
				current_test++;
				this->statusBar()->showMessage( "benchmarking " + rfm_it.value() + " on " + surface_name + " (test " + QString::number( current_test ) + " of " + QString::number( number_of_tests ) + ")" );

				t.restart();
				loadSurface( **it );
				rfm_result << QString::number( t.elapsed() ) << "ms";

				// process pending events and draw surface several times (because first few renderings seem to have some overhead)
				QCoreApplication::processEvents();
				this->srw->repaint();

				// save original view
				this->saveCurrentView( dirname, surface_name + " (" + rfm_it.value() + ")" );

				// set special benchmark materials
				this->front_mat->usePredefinedMaterial( "benchmark 1" );
				this->back_mat->usePredefinedMaterial( "benchmark 2" );
				this->srw->repaint();
				this->saveCurrentView( dirname, surface_name + " - bm (" + rfm_it.value() + ")" );

				QCoreApplication::processEvents();
				t.restart();
				int number_of_frames = 0;
				int my_time;
				while( ( my_time = t.elapsed() ) < 5000 )
				{
					this->srw->repaint();
					number_of_frames++;
				}
				rfm_result << ";" << QString::number( my_time / float( number_of_frames ), 'f', 2 ) << "ms;" << QString::number( ( float( number_of_frames ) / ( my_time / 1000.0f ) ), 'f', 2 ) << "fps";
			}
			rfm_result << "\n";
		}
	}

	// delete surfaces
	for( QList< QImage * >::iterator s_it = bms_list.begin(); s_it != bms_list.end(); s_it++ )
		delete *s_it;

	// format the result tables
	QStringList result;
	result << "benchmark results for " << QString::fromAscii( ( char * ) glGetString( GL_RENDERER ) ) << " (" + QString::fromAscii( ( char * ) glGetString( GL_VENDOR ) ) << ")\n";
	result << "image resolution: " << QString::number( s.width() ) << "x" << QString::number( s.height() ) << "\n";
	result << "\n\n";
	result << pcm_result;
	result << "\n\n";
	result << rfm_result;
	result << "\n\n";
	result << surface_equations;

	// write result table in result directory
	QFile f( dirname + QDir::separator() + "results.txt" );
	f.open( QIODevice::WriteOnly | QIODevice::Text );
	f.write( result.join( "" ).toAscii() );
	f.close();

	// benchmark complete -> close application
	QMessageBox::information( this, "Information", "Benchmark complete. RealSurf will be closed now." );
	QCoreApplication::quit();
}

void BenchmarkMainWindow::fitSize( int w, int h )
{
	// allow all sizes for main window
	this->setMinimumSize( w, h );
	this->setMaximumSize( 16777215, 16777215 );

	// set size of surface render widget
	this->srw->setMinimumSize( w, h );
	this->srw->setMaximumSize( w, h );

	// make main window as small as possible (tight fit :-) )
	QCoreApplication::processEvents();
	this->adjustSize();
	QCoreApplication::processEvents();

	// restrict size of main window to this "ideal" size
	this->setMinimumSize( this->size() );
	this->setMaximumSize( this->size() );
}

void BenchmarkMainWindow::saveCurrentView( const QString &path_without_separator, const QString &filename_without_file_extension )
{
	QImage pic = this->srw->grabFrameBuffer( true );
	if( pic.height() > pic.width() )
		pic = pic.copy( 0, ( pic.height() - pic.width() + 1 ) / 2, pic.width(), pic.width() );
	else
		pic = pic.copy( ( pic.width() - pic.height() + 1 ) / 2, 0, pic.height(), pic.height() );
	pic.save( path_without_separator + QDir::separator() + filename_without_file_extension + ".png", "PNG", 83 );
}

void BenchmarkMainWindow::dragEnterEvent( QDragEnterEvent * event ) { event->accept(); }
void BenchmarkMainWindow::dragLeaveEvent( QDragLeaveEvent * event ) { event->accept(); }
void BenchmarkMainWindow::dragMoveEvent( QDragMoveEvent * event ) { event->accept(); }
void BenchmarkMainWindow::dropEvent( QDropEvent * event ) { event->accept(); }
void BenchmarkMainWindow::keyPressEvent( QKeyEvent * event ) { event->accept(); }
void BenchmarkMainWindow::keyReleaseEvent( QKeyEvent * event ) { event->accept(); }
void BenchmarkMainWindow::mouseDoubleClickEvent( QMouseEvent * event ) { event->accept(); }
void BenchmarkMainWindow::mouseMoveEvent( QMouseEvent * event ) { event->accept(); }
void BenchmarkMainWindow::mousePressEvent( QMouseEvent * event ) { event->accept(); }
void BenchmarkMainWindow::mouseReleaseEvent( QMouseEvent * event ) { event->accept(); }
void BenchmarkMainWindow::moveEvent( QMoveEvent * event ) { event->accept(); }
void BenchmarkMainWindow::tabletEvent( QTabletEvent * event ) { event->accept(); }
void BenchmarkMainWindow::wheelEvent( QWheelEvent * event ) { event->accept(); }