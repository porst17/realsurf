#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtGui>
#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QToolButton>
#include <QListWidget>
#include "LightParameters.h"
#include "MaterialParameters.h"
#include "SurfaceRenderWidget.h"
#include "ColorPicker.h"
#include "ParameterWidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	protected:
		QGridLayout *top_layout;
		ParameterWidget *surface_parameter_widget;
		SurfaceRenderWidget *srw;
		QComboBox *surfaceEquation;
		QPushButton *surfaceConfirmButton;
		LightParameters *light_1, *light_2, *light_3;
		MaterialParameters *front_mat, *back_mat;
		LightProperties default_lp;
		QDialog *aboutDialog, *helpDialog;
		QListWidget *savedSurfaceList;
		QWidget* surfaceEquationEditor;
		QWidget* tabWidget;
		QComboBox *polyCoeffMethods;
		QComboBox *rootFinderMethods;

		ColorPicker *backgroundColorButton;
		QToolButton *screenshotButton;
		QToolButton *resetTransformButton;
		QToolButton *resetLightButton;
		QToolButton *helpButton;
		QToolButton *showBoundingObjectButton;
		QToolButton *clipToSphereButton;
		QToolButton *clipToCubeButton;
		QPushButton *swapMaterialsButton;
		QPushButton *enableTransparencyButton;

	public:
		MainWindow( bool loadInitialSurface = true );
		virtual ~MainWindow();

	public slots:
		void newParameters( const QSet< QString > &param_list );
		void parsingComplete( bool success, const QString &summary, const QString &text );
		void makeScreenshot();
		void resetLightConfiguration();
		void swapMaterials();
		void setTransparencyEnabled( bool on );
		void help();
		void about();
		void newSurfaceEquation();
		void saveCurrentSurface();
		void addFilesToSurfaceList();
		void removeFromSurfaceList();
		void loadSurface( QListWidgetItem *item );
		void polyCoeffMethodChanged( int index );
		void rootFinderMethodChanged( int index );
		void minimalGUI( bool minimal );

	protected slots:
		void setApplicationBusy();

	protected:
		virtual QWidget *setupTabWidget();
		virtual QWidget *setupSurfaceEquationEditor();
		virtual void keyPressEvent( QKeyEvent *event );
		virtual void closeEvent( QCloseEvent *event );
		void load( const QImage &surface );
		void addToSurfaceList( const QImage &surface );
		void loadSurface( const QImage &surface );
		bool isStoredSurface( const QImage &image );
};

#endif