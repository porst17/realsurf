//#include <QDebug>

#include <QTabWidget>
#include <QWhatsThis>
#include <QFrame>
#include <QScrollArea>
#include <QToolBox>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QSplitter>
#include <QImage>
#include <QMimeData>
#include <QMessageBox>
#include <QClipboard>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QLineEdit>
#include <QBuffer>
#include <QStringList>
#include "ui_surfaceEquationEditor.h"
#include "ui_miscellaneous.h"
#include "ui_about.h"
#include "ui_help.h"
#include "ui_savedSurfaces.h"
#include "ui_enterSurfaceDescrption.h"
#include "ui_help.h"
#include "FloatParameter.h"
#include "MainWindow.h"
#include<cmath>

MainWindow::MainWindow( bool loadInitialSurface ) : QMainWindow()
{
	this->setWindowTitle( "RealSurf - Realtime Visualization of Algebraic Surfaces" );
	this->setWindowIconText( "RealSurf" );
	this->setFocusPolicy( Qt::ClickFocus );

	QWidget *central_widget = new QWidget( this );
	top_layout = new QGridLayout( this );
	central_widget->setLayout( top_layout );
	this->setCentralWidget( central_widget );

	// the OpenGL window must be initialized first and immediately to allow GL state changes
	srw = new SurfaceRenderWidget( this );
	srw->updateGL();
	srw->setUpdatesEnabled( false );

	QSizePolicy srw_policy( QSizePolicy::Preferred, QSizePolicy::Expanding );
	srw_policy.setHorizontalStretch( 10 );
	srw_policy.setVerticalStretch( 10 );
	srw->setSizePolicy( srw_policy );
	srw->setMinimumSize( 32, 32 );

	QVBoxLayout *srw_frame_layout = new QVBoxLayout( this );
	srw_frame_layout->addWidget( srw );
	srw_frame_layout->setMargin( 0 );

	QFrame *srw_frame = new QFrame( this );
	srw_frame->setFrameShape( QFrame::StyledPanel );
	srw_frame->setFrameShadow( QFrame::Raised );
	srw_frame->setLayout( srw_frame_layout );

	this->tabWidget = setupTabWidget();
	QSplitter *splitter = new QSplitter( this );
	splitter->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
	splitter->addWidget( srw_frame );
	splitter->addWidget( this->tabWidget );
	splitter->setStretchFactor( 0, 1 );
	splitter->setStretchFactor( 1, 0 );

	this->surfaceEquationEditor = setupSurfaceEquationEditor();
	top_layout->addWidget( splitter, 0, 0 );
	top_layout->addWidget( this->surfaceEquationEditor, 1, 0 );
	QObject::connect( srw, SIGNAL( newParameters( const QSet< QString > & ) ), this, SLOT( newParameters( const QSet< QString > & ) ) );
	QObject::connect( srw, SIGNAL( parsingComplete( bool , const QString &, const QString & ) ), this, SLOT( parsingComplete( bool , const QString &, const QString & ) ) );

	// select a surface on startup
	if( loadInitialSurface )
		loadSurface( this->savedSurfaceList->currentItem() );

	srw->setUpdatesEnabled( true );

	this->aboutDialog = NULL;
	this->helpDialog = NULL;
}

MainWindow::~MainWindow() {}

QWidget *MainWindow::setupSurfaceEquationEditor()
{
	QWidget *see = new QWidget( this );
	Ui::surfaceEquationEditor().setupUi( see );

	// setup help button
	this->helpButton = see->findChild< QToolButton * >( "helpButton" );
	QObject::connect( this->helpButton, SIGNAL( clicked() ), this, SLOT( help() ) );

	this->surfaceEquation = see->findChild< QComboBox * >( "surfaceEquation" );
	this->surfaceConfirmButton = see->findChild< QPushButton * >( "surfaceEquationOkButton" );
	QObject::connect( this->surfaceConfirmButton, SIGNAL( clicked() ), this, SLOT( newSurfaceEquation() ), Qt::QueuedConnection );

	return see;
}
#include <iostream>
QWidget *MainWindow::setupTabWidget()
{
	// create tab widget
	QTabWidget *result = new QTabWidget( this );
	result->setTabPosition( QTabWidget::West );

	// create tab for storing/loading surfaces
	QWidget *store_load_widget = new QWidget( this );
	Ui::savedSurfaces().setupUi( store_load_widget );
	this->savedSurfaceList = store_load_widget->findChild< QListWidget * >( "savedSurfacesList" );
	QObject::connect( this->savedSurfaceList, SIGNAL( itemActivated( QListWidgetItem * ) ), this, SLOT( loadSurface( QListWidgetItem * ) ) );
	QObject::connect( store_load_widget->findChild< QPushButton * >( "saveButton" ), SIGNAL( clicked() ), this, SLOT( saveCurrentSurface() ) );
	QObject::connect( store_load_widget->findChild< QPushButton * >( "addToListButton" ), SIGNAL( clicked() ), this, SLOT( addFilesToSurfaceList() ) );
	QObject::connect( store_load_widget->findChild< QPushButton * >( "removeFromListButton" ), SIGNAL( clicked() ), this, SLOT( removeFromSurfaceList() ) );

	// add predefined surfaces to surface list
	QDir predefined_surfaces_res_dir( ":/resources/surfaces/" );
	QStringList predefined_surfaces = predefined_surfaces_res_dir.entryList( QDir::Files, QDir::Name | QDir::Reversed );
	for( QStringList::iterator it = predefined_surfaces.begin(); it != predefined_surfaces.end(); it++ )
	{
		QImage surface( predefined_surfaces_res_dir.filePath( *it ), "PNG" );
		if( isStoredSurface( surface ) )
			addToSurfaceList( surface );
	}
	this->savedSurfaceList->setCurrentRow( 2 );

	// add surfaces in 'surfaces'-folder to surface list
	QDir predefined_surfaces_dir( QCoreApplication::applicationDirPath () + QDir::separator() + "surfaces" );
	predefined_surfaces = predefined_surfaces_dir.entryList( QDir::Files );
	for( QStringList::iterator it = predefined_surfaces.begin(); it != predefined_surfaces.end(); it++ )
	{
		QImage surface( predefined_surfaces_dir.filePath( *it ), "PNG" );
		if( isStoredSurface( surface ) )
			addToSurfaceList( surface );
	}

	// create tab for float surface parameters
	this->surface_parameter_widget = new ParameterWidget( this );
	QObject::connect( this->surface_parameter_widget, SIGNAL( valueChanged( QString, double ) ), this->srw, SLOT( setFloatParameter( QString, double ) ) );

	// create tab for material parameters
	QWidget *material_widget = new QWidget( this );
	front_mat = new MaterialParameters( "Front Material" );
	back_mat = new MaterialParameters( "Back Material" );

	QObject::connect( front_mat, SIGNAL( valueChanged( const MaterialProperties & ) ), this->srw, SLOT( setFrontMaterial( const MaterialProperties & ) ) );
	front_mat->usePredefinedMaterial( "brass" );
	front_mat->setTransparencyEnabled( false );
	QObject::connect( back_mat, SIGNAL( valueChanged( const MaterialProperties & ) ), this->srw, SLOT( setBackMaterial( const MaterialProperties & ) ) );
	back_mat->usePredefinedMaterial( "ruby" );
	back_mat->setTransparencyEnabled( false );

	swapMaterialsButton = new QPushButton( "Swap Materials", this );
	swapMaterialsButton->setToolTip( "Swap front and back material.\nThis has the same effect as negating the surface equation and thus inverting the surface normals." );
	QObject::connect( swapMaterialsButton, SIGNAL( clicked() ), this, SLOT( swapMaterials() ) );

	enableTransparencyButton = new QPushButton( "Enable/Disable Transparency", this );
	enableTransparencyButton->setCheckable( true );
	enableTransparencyButton->setToolTip( "Enable or disable the use of transparencies.\nTo change this option the current shader must be recompiled.\nEnabling this may decrease performance." );
	QObject::connect( enableTransparencyButton, SIGNAL( clicked( bool ) ), this, SLOT( setTransparencyEnabled( bool ) ) );
	QObject::connect( enableTransparencyButton, SIGNAL( clicked( bool ) ), this->front_mat, SLOT( setTransparencyEnabled( bool ) ) );
	QObject::connect( enableTransparencyButton, SIGNAL( clicked( bool ) ), this->back_mat, SLOT( setTransparencyEnabled( bool ) ) );

	QVBoxLayout *material_widget_layout = new QVBoxLayout( material_widget );
	material_widget_layout->addWidget( front_mat );
	material_widget_layout->addWidget( back_mat );
	material_widget_layout->addWidget( swapMaterialsButton );
	material_widget_layout->addWidget( enableTransparencyButton );
	material_widget_layout->addStretch( 1 );

	// create tab for light parameters
	QToolBox *light_widget = new QToolBox( this );
	light_widget->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Preferred ) );
	this->light_1 = new LightParameters( this );
	this->light_2 = new LightParameters( this );
	this->light_3 = new LightParameters( this );
	this->default_lp = light_1->getValue();
	QObject::connect( this->light_1, SIGNAL( valueChanged( const LightProperties & ) ), this->srw, SLOT( setLight0( const LightProperties & ) ) );
	QObject::connect( this->light_1, SIGNAL( toggled( bool , const LightProperties & ) ), this->srw, SLOT( toggleLight0( bool , const LightProperties & ) ) );
	this->light_1->setEnabled( true );
	QObject::connect( this->light_2, SIGNAL( valueChanged( const LightProperties & ) ), this->srw, SLOT( setLight1( const LightProperties & ) ) );
	QObject::connect( this->light_2, SIGNAL( toggled( bool , const LightProperties & ) ), this->srw, SLOT( toggleLight1( bool , const LightProperties & ) ) );
	this->light_2->setEnabled( false );
	QObject::connect( this->light_3, SIGNAL( valueChanged( const LightProperties & ) ), this->srw, SLOT( setLight2( const LightProperties & ) ) );
	QObject::connect( this->light_3, SIGNAL( toggled( bool , const LightProperties & ) ), this->srw, SLOT( toggleLight2( bool , const LightProperties & ) ) );
	this->light_3->setEnabled( false );

	light_widget->addItem( light_1, "Light 1" );
	light_widget->addItem( light_2, "Light 2" );
	light_widget->addItem( light_3, "Light 3" );

	// create tab for misc stuff
	QWidget *misc = new QWidget( this );
	Ui::miscellaneous().setupUi( misc );
	backgroundColorButton = misc->findChild< ColorPicker * >( "bgColor" );
	screenshotButton = misc->findChild< QToolButton * >( "screenshot" );
	clipToSphereButton = misc->findChild< QToolButton * >( "clipToSphere" );
	clipToCubeButton = misc->findChild< QToolButton * >( "clipToCube" );
	showBoundingObjectButton = misc->findChild< QToolButton * >( "showHideBoundingObject" );
	resetTransformButton = misc->findChild< QToolButton * >( "resetSurfaceTransformation" );
	resetLightButton = misc->findChild< QToolButton * >( "resetLightConfig" );
	polyCoeffMethods = misc->findChild< QComboBox * >( "polyCoeffMethod" );
	rootFinderMethods = misc->findChild< QComboBox * >( "rootFinderMethod" );

	// add different algorithms to combo boxes
	QMap< glsl::ShaderManager::PolyCoeffMethod, QString > pcmData = SurfaceRenderWidget::getPolyCoeffMethods();
	for( QMap< glsl::ShaderManager::PolyCoeffMethod, QString >::const_iterator pcm_it = pcmData.constBegin(); pcm_it != pcmData.constEnd(); pcm_it++ )
		polyCoeffMethods->addItem( pcm_it.value(), pcm_it.key() );
	QMap< glsl::ShaderManager::RootFinderMethod, QString > rfmData = SurfaceRenderWidget::getRootFinderMethods();
	for( QMap< glsl::ShaderManager::RootFinderMethod, QString >::const_iterator rfm_it = rfmData.constBegin(); rfm_it != rfmData.constEnd(); rfm_it++ )
		rootFinderMethods->addItem( rfm_it.value(), rfm_it.key() );
 
	QObject::connect( screenshotButton, SIGNAL( clicked( bool ) ), this, SLOT( makeScreenshot() ) );
	QObject::connect( backgroundColorButton, SIGNAL( valueChanged( const QColor & ) ), this->srw, SLOT( setBackgroundColor( const QColor & ) ) );
	this->srw->setBackgroundColor( backgroundColorButton->getColor() );
	QObject::connect( clipToSphereButton, SIGNAL( clicked() ), this->srw, SLOT( clipToSphere() ) );
	QObject::connect( clipToCubeButton, SIGNAL( clicked() ), this->srw, SLOT( clipToCube() ) );
	clipToSphereButton->click();
	QObject::connect( showBoundingObjectButton, SIGNAL( clicked( bool ) ), this->srw, SLOT( showClippingObject( bool ) ) );
	QObject::connect( resetLightButton, SIGNAL( clicked( bool ) ), this, SLOT( resetLightConfiguration() ) );
	QObject::connect( resetTransformButton, SIGNAL( clicked( bool ) ), this->srw, SLOT( resetTransformation() ) );
	QObject::connect( polyCoeffMethods, SIGNAL( currentIndexChanged( int ) ), this, SLOT( polyCoeffMethodChanged( int ) ) );
	QObject::connect( rootFinderMethods, SIGNAL( currentIndexChanged( int ) ), this, SLOT( rootFinderMethodChanged( int ) ) );
	QObject::connect( misc->findChild< QToolButton * >( "about" ), SIGNAL( clicked() ), this, SLOT( about() ) );

	// add tabs to tab widget
	result->addTab( store_load_widget, "Saved Surfaces" );
	result->addTab( material_widget, "Materials" );
	result->addTab( light_widget, "Lights" );
	result->addTab( this->surface_parameter_widget, "Parameters" );
	result->addTab( misc, "Miscellaneous" );

	return result;
}

void MainWindow::newParameters( const QSet< QString > &param_list )
{
	this->setUpdatesEnabled( false );
	this->surface_parameter_widget->clear();

	// add new parameter widgets
	for( QSet< QString >::const_iterator it = param_list.constBegin(); it != param_list.constEnd(); it++ )
		this->surface_parameter_widget->setProperties( *it, FloatParameterProperties() );
	this->setUpdatesEnabled( true );
}

void MainWindow::parsingComplete( bool success, const QString &summary, const QString &text )
{
	QApplication::restoreOverrideCursor();
	QPalette p = surfaceEquation->palette();
	p.setColor( QPalette::Base, success ? Qt::white : QColor( 255, 90, 90 ) );
	surfaceEquation->setPalette( p );
	QString tooltip_text = success ? "" : summary + ( text != "" ? "\n" + text : "" );
	surfaceEquation->setToolTip( tooltip_text );
	if( !success && tooltip_text != "" )
		QWhatsThis::showText( this->pos() + QPoint( this->width() / 2, this->height() / 2 ), "<html><body><pre>"+tooltip_text+"</pre></body></html>", this );
}

void MainWindow::newSurfaceEquation()
{
	this->setApplicationBusy();
	QString formula = this->surfaceEquation->currentText();
	this->srw->setFormula( formula );
}

void MainWindow::setApplicationBusy()
{
	bool updatesEnabled = this->updatesEnabled();
	this->setUpdatesEnabled( true );
	QPalette p = this->surfaceEquation->palette();
	p.setColor( QPalette::Base, QColor( 255, 255, 127 ) );
	this->surfaceEquation->setPalette( p );
	this->surfaceEquation->repaint();
	QApplication::setOverrideCursor( Qt::BusyCursor );
	this->setUpdatesEnabled( updatesEnabled );
}

void MainWindow::makeScreenshot()
{
	QString filename = QFileDialog::getSaveFileName( 0, "Save screenshot", "screenshot.png", "Portable Network Graphics (*.png)", 0 );
	if( filename != "" )
	{
		bool withAlpha = QMessageBox::question( this, "Save option", "Save image with transparent background?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes;
		QImage pic = this->srw->grabFrameBuffer( withAlpha );
		if( pic.height() > pic.width() )
			pic.copy( 0, ( pic.height() - pic.width() + 1 ) / 2, pic.width(), pic.width() ).save( filename, "PNG" );
		else
			pic.copy( ( pic.width() - pic.height() + 1 ) / 2, 0, pic.height(), pic.height() ).save( filename, "PNG" );
	}
}

void MainWindow::resetLightConfiguration()
{
	this->light_1->setValue( this->default_lp );
	this->light_2->setValue( this->default_lp );
	this->light_3->setValue( this->default_lp );
	this->light_1->setEnabled( true );
	this->light_2->setEnabled( false );
	this->light_3->setEnabled( false );
}

void MainWindow::swapMaterials()
{
	MaterialProperties front_mp = this->front_mat->getValue();
	MaterialProperties back_mp = this->back_mat->getValue();
	this->front_mat->setValue( back_mp );
	this->back_mat->setValue( front_mp );
}

void MainWindow::setTransparencyEnabled( bool on )
{
	QMessageBox::information( 0, "Information", "The current expression will be recompiled in order to change this feature." );
	this->srw->setTransparencyEnabled( on );
	this->newSurfaceEquation();
}

#include <iostream>

void MainWindow::help()
{

	if( this->helpDialog == NULL )
	{
		this->helpDialog = new QDialog();
		Ui::helpDialog().setupUi( this->helpDialog );
		this->helpDialog->setWindowFlags( Qt::Window );
		this->helpDialog->show();
		this->helpDialog->findChild< QTextBrowser * >( "helpBrowser" )->setSource( QUrl( "qrc:/resources/help.html" ) );
	}
	this->helpDialog->show();

/*
	// to test the help when it is not compiled into resource
	if( this->helpDialog == NULL )
	{
		this->helpDialog = new QDialog();
		Ui::helpDialog().setupUi( this->helpDialog );
	}
	QFile helpText( "resources/help.html" );
	helpText.open( QIODevice::ReadOnly | QIODevice::Text );
	QString helpString = QString( helpText.readAll() );
	helpText.close();
	this->helpDialog->show();
	this->helpDialog->findChild< QTextBrowser * >( "helpBrowser" )->setHtml( helpString );
*/
}

void MainWindow::polyCoeffMethodChanged( int index )
{
	QMessageBox::information( 0, "Information", "The current expression will be recompiled in order to change apply new algorithm." );
	glsl::ShaderManager::PolyCoeffMethod pcm = ( glsl::ShaderManager::PolyCoeffMethod ) this->polyCoeffMethods->itemData( index ).toInt();
	this->srw->setPolyCoeffMethod( pcm );
	this->newSurfaceEquation();
}

void MainWindow::rootFinderMethodChanged( int index )
{
	QMessageBox::information( 0, "Information", "The current expression will be recompiled in order to change apply new algorithm." );
	glsl::ShaderManager::RootFinderMethod rfm = ( glsl::ShaderManager::RootFinderMethod ) this->rootFinderMethods->itemData( index ).toInt();
	this->srw->setRootFinderMethod( rfm );
	this->newSurfaceEquation();
}

void MainWindow::about()
{
	if( this->aboutDialog == NULL )
	{
		this->aboutDialog = new QDialog();
		Ui::aboutDialog().setupUi( this->aboutDialog );
		this->aboutDialog->setWindowFlags( Qt::Tool );
	}
	this->aboutDialog->show();
}

void MainWindow::keyPressEvent( QKeyEvent *event )
{
	event->accept();
	switch( event->key() )
	{
		case Qt::Key_Escape:
			// reset window to "normal mode"
			
			// show equation editor and tab widget
			this->surfaceEquationEditor->show();
			this->tabWidget->show();

			// bring window back to normal mode
			this->showNormal();
			break;
		case Qt::Key_F1:
			this->helpButton->animateClick();
			break;
		case Qt::Key_F5:
			this->setApplicationBusy();
			this->srw->reload();
			break;
		case Qt::Key_F6:
			this->screenshotButton->animateClick();
			event->accept();
			break;
		case Qt::Key_F11:
			if( this->isFullScreen() )
				// bring window back to normal mode
				this->showNormal();
			else
				// bring window into fullscreen mode
				this->showFullScreen();
			break;
		case Qt::Key_F12:
			if( !this->surfaceEquationEditor->isVisible() && !this->tabWidget->isVisible() )
			{
				// show equation editor and tab widget
				this->surfaceEquationEditor->show();
				this->tabWidget->show();
			}
			else
			{
				// hide equation editor and tab widget
				this->surfaceEquationEditor->hide();
				this->tabWidget->hide();
			}
			break;		
		case Qt::Key_F:
			this->srw->showFramesPerSecond( !( this->srw->getShowFramesPerSecond() ) );
			break;
		case Qt::Key_R:
			this->resetTransformButton->animateClick();
			break;
		/* disabled; risc to high to reset light setting by mistake
		case Qt::Key_L:
			this->resetLightButton->animateClick();
			break;
		*/
		case Qt::Key_B:
			this->showBoundingObjectButton->animateClick();
			break;
		case Qt::Key_C:
			if( event->modifiers() & Qt::ControlModifier )
			{
				// copy framebuffer to clipboard
				QImage pic = this->srw->grabFrameBuffer( true );
				if( pic.height() > pic.width() )
					pic = pic.copy( 0, ( pic.height() - pic.width() + 1 ) / 2, pic.width(), pic.width() );
				else
					pic = pic.copy( ( pic.width() - pic.height() + 1 ) / 2, 0, pic.height(), pic.height() );
				QClipboard* clipboard = QApplication::clipboard();
				QMimeData* md = new QMimeData;
				md->setImageData( pic );
				clipboard->setImage( pic );
				clipboard->setMimeData( md );
			}
			else
			{
				// switch betweend cube and sphere clipping
				if( this->clipToCubeButton->isChecked() )
					this->clipToSphereButton->animateClick();
				else
					this->clipToCubeButton->animateClick();
			}
			break;
		case Qt::Key_S:
			this->swapMaterialsButton->animateClick();
			break;
		case Qt::Key_Plus:
			this->srw->scaleUp();
			break;
		case Qt::Key_Minus:
			this->srw->scaleDown();
			break;
		default:
			// don't want this key event
			event->ignore();
			break;
	}
}

void MainWindow::minimalGUI( bool minimal )
{
	if( minimal )
	{
		// hide equation editor and tab widget
		this->surfaceEquationEditor->hide();
		this->tabWidget->hide();
	}
	else
	{
		// show equation editor and tab widget
		this->surfaceEquationEditor->show();
		this->tabWidget->show();
	}
}


void MainWindow::closeEvent( QCloseEvent *event )
{
	QCoreApplication::quit();
}

void MainWindow::saveCurrentSurface()
{
	// let the user enter a surface name and surface description
	QString surface_name, surface_description;
	QDialog *d = new QDialog( this );
	Ui::surfaceDescriptionDialog().setupUi( d );
	d->setWindowFlags( Qt::Tool );
	if( d->exec() == QDialog::Accepted )
	{
		// dialog accepted, retrieve name and description
		surface_name = d->findChild< QLineEdit * >( "surfaceName" )->text();
		surface_description = d->findChild< QLineEdit * >( "surfaceDescription" )->text();

		// let the user enter a filename
		QString filename = QFileDialog::getSaveFileName( this, "Save surface", QCoreApplication::applicationDirPath () + QDir::separator() + "surfaces" + QDir::separator() + ( surface_name == "" ? "surface" : surface_name ) + ".png", "Portable Network Graphics (*.png)", 0 );
		if( filename != "" )
		{
			// get thumbnail image
			QImage pic = this->srw->grabFrameBuffer( this->backgroundColorButton->getColor() == Qt::white );
			if( pic.height() > pic.width() )
				pic = pic.copy( 0, ( pic.height() - pic.width() + 1 ) / 2, pic.width(), pic.width() );
			else
				pic = pic.copy( ( pic.width() - pic.height() + 1 ) / 2, 0, pic.height(), pic.height() );
			pic = pic.scaled( QSize( 64, 64 ), Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

			// store surface data in image

			// all other data written into one DataStream
			// prepare buffers and streams
			QByteArray ba;
			QDataStream ds( &ba, QIODevice::WriteOnly | QIODevice::Text );
			ds.setVersion( QDataStream::Qt_4_2 );

			// surface parameters
			QMap< QString, FloatParameterProperties > pm = this->surface_parameter_widget->getProperties();
			ds << pm;

			// bbox_transform
			float bbox_transform[ 16 ];
			this->srw->getBBoxTransform( bbox_transform );
			for( int i = 0; i < 16; i++ )
				ds << bbox_transform[ i ];

			// surface_transform
			float surface_transform[ 16 ];
			this->srw->getSurfaceTransform( surface_transform );
			for( int i = 0; i < 16; i++ )
				ds << surface_transform[ i ];

			// clipping_object
			ds << (qint32) this->srw->getClippingSurface();

			// show_clipping_object
			ds << (qint8) this->srw->getShowClippingObject();

			// background_color
			ds << this->backgroundColorButton->getColor();

			// front_material
			ds << front_mat->getValue();

			// back_material
			ds << back_mat->getValue();

			// transparency_enabled
			ds << ( qint8 ) enableTransparencyButton->isChecked();

			// light_1
			ds << ( qint8 ) light_1->enabled();
			ds << light_1->getValue();

			// light_2
			ds << ( qint8 ) light_2->enabled();
			ds << light_2->getValue();

			// light_3
			ds << ( qint8 ) light_3->enabled();
			ds << light_3->getValue();

			// save data in base64 encoding
			pic.setText( "realsurf_data", QString::fromAscii( ba.toBase64() ) );

			// name
			pic.setText( "realsurf_name", surface_name );

			// description
			pic.setText( "realsurf_description", surface_description );
			
			// formula
			pic.setText( "realsurf_formula", this->srw->getFormula() );

			// save image and data
			if( pic.save( filename, "PNG" ) )
			{
				// add surface to saved surface list

				// hack: save the image to a buffer an load it again from the buffer,
				// because the text()-function does not work otherwise
				QByteArray ba;
				QBuffer buffer( &ba );
				buffer.open( QBuffer::ReadWrite );
				pic.save( &buffer, "PNG" );
				buffer.seek( 0 );
				QImage hack_image;
				hack_image.load( &buffer, "PNG" );
				addToSurfaceList( hack_image );
			}
			else
			{
				QMessageBox::critical( this, "Error", "Could not save file '" + filename + "'." );
			}
		}
	}
}

void MainWindow::addFilesToSurfaceList()
{
	QStringList files = QFileDialog::getOpenFileNames ( this, "Open Surface Files", "", "Portable Network Graphics (*.png)" );
	for( QStringList::iterator it = files.begin(); it != files.end(); it++ )
	{
		QImage surface( *it, "PNG" );
		if( isStoredSurface( surface ) )
			addToSurfaceList( surface );
		else
			QMessageBox::warning( this, "Error opening file", "Could not load '" + (*it) + "'.\n\nInvalid file format." );
	}
}

void MainWindow::addToSurfaceList( const QImage &surface )
{
	QListWidgetItem *item = new QListWidgetItem( QIcon( QPixmap::fromImage( surface ) ), surface.text( "realsurf_name" ) );
	QString item_tooltip = surface.text( "realsurf_name" );
	if( surface.text( "realsurf_description" ) != "" )
		item_tooltip += "\n\n" + surface.text( "realsurf_description" );
	item_tooltip += "\n\n" + surface.text( "realsurf_formula" );
	item->setToolTip( item_tooltip );
	item->setData( Qt::UserRole, qVariantFromValue( surface ) );
	this->savedSurfaceList->insertItem( 0, item );
}

void MainWindow::removeFromSurfaceList()
{
	this->savedSurfaceList->takeItem( this->savedSurfaceList->currentRow() );
}

void MainWindow::loadSurface( QListWidgetItem *item )
{
	loadSurface( item->data( Qt::UserRole ).value< QImage >() );
}

void MainWindow::loadSurface( const QImage &surface )
{
	// disable updates
	this->setUpdatesEnabled( false );

	// restore surface data from image

	// prepare buffers and streams
	QByteArray ba = QByteArray::fromBase64( surface.text( "realsurf_data" ).toAscii() );
	QDataStream ds( &ba, QIODevice::ReadOnly | QIODevice::Text );
	ds.setVersion( QDataStream::Qt_4_2 );

	// name
	surface.text( "realsurf_name" );

	// description
	surface.text( "realsurf_description" );

	// surface parameters (read now, process later)
	QMap< QString, FloatParameterProperties > pm;
	ds >> pm;

	// bbox_transform
	float bbox_transform[ 16 ];
	for( int i = 0; i < 16; i++ )
		ds >> bbox_transform[ i ];
	this->srw->setBBoxTransform( bbox_transform );

	// surface_transform
	float surface_transform[ 16 ];
	for( int i = 0; i < 16; i++ )
		ds >> surface_transform[ i ];
	this->srw->setSurfaceTransform( surface_transform );

	// clipping_object
	qint32 clipping_surface;
	ds >> clipping_surface;
	this->srw->setClippingSurface( ( SurfaceRenderWidget::ClippingSurface ) clipping_surface );

	// show_clipping_object
	qint8 show_clipping_object;
	ds >> show_clipping_object;
	this->srw->showClippingObject( ( bool ) show_clipping_object );

	// background_color
	QColor bg;
	ds >> bg;
	this->backgroundColorButton->setColor( bg );

	// front_material
	MaterialProperties front_mp;
	ds >> front_mp;
	this->front_mat->setValue( front_mp );

	// back_material
	MaterialProperties back_mp;
	ds >> back_mp;
	this->back_mat->setValue( back_mp );

	// transparency_enabled
	qint8 transparencyEnabled;
	ds >> transparencyEnabled;
	enableTransparencyButton->setChecked( ( bool ) transparencyEnabled );
	this->front_mat->setTransparencyEnabled( ( bool ) transparencyEnabled );
	this->back_mat->setTransparencyEnabled( ( bool ) transparencyEnabled );
	this->srw->setTransparencyEnabled( ( bool ) transparencyEnabled );

	// light_1
	LightProperties lp_1;
	qint8 light_1_enabled;
	ds >> light_1_enabled;
	ds >> lp_1;
	light_1->setEnabled( ( bool ) light_1_enabled );
	light_1->setValue( lp_1 );

	// light_2
	LightProperties lp_2;
	qint8 light_2_enabled;
	ds >> light_2_enabled;
	ds >> lp_2;
	light_2->setEnabled( ( bool ) light_2_enabled );
	light_2->setValue( lp_2 );

	// light_3
	LightProperties lp_3;
	qint8 light_3_enabled;
	ds >> light_3_enabled;
	ds >> lp_3;
	light_3->setEnabled( ( bool ) light_3_enabled );
	light_3->setValue( lp_3 );

	// formula
	this->surfaceEquation->addItem( surface.text( "realsurf_formula" ) );
	this->surfaceEquation->setCurrentIndex( this->surfaceEquation->findText( surface.text( "realsurf_formula" ) ) );
	this->surfaceConfirmButton->click();

	// be sure, that parameter widgets are already initialized
	QCoreApplication::sendPostedEvents();

	// set parameter values now
	for( QMap< QString, FloatParameterProperties >::const_iterator i = pm.constBegin(); i != pm.constEnd(); i++ )
		this->surface_parameter_widget->setProperties( i.key(), i.value() );

	// enable updates
	this->setUpdatesEnabled( true );
	update();
}

bool MainWindow::isStoredSurface( const QImage &image )
{
	QStringList text_keys = image.textKeys();
	if( text_keys.contains( "realsurf_name" ) && text_keys.contains( "realsurf_formula" ) && text_keys.contains( "realsurf_description" ) && text_keys.contains( "realsurf_data" ) )
		return true;
	else
		return false;
}