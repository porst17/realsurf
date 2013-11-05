#ifndef __BENCHMARKMAINWINDOW_H__
#define __BENCHMARKMAINWINDOW_H__

#include "MainWindow.h"

class BenchmarkMainWindow : public MainWindow
{
	Q_OBJECT

	public:
		BenchmarkMainWindow();

	protected:
		virtual void dragEnterEvent( QDragEnterEvent *event );
		virtual void dragLeaveEvent( QDragLeaveEvent *event );
		virtual void dragMoveEvent( QDragMoveEvent *event );
		virtual void dropEvent( QDropEvent *event );
		virtual void keyPressEvent( QKeyEvent *event );
		virtual void keyReleaseEvent( QKeyEvent *event );
		virtual void mouseDoubleClickEvent( QMouseEvent *event );
		virtual void mouseMoveEvent( QMouseEvent *event );
		virtual void mousePressEvent( QMouseEvent *event );
		virtual void mouseReleaseEvent( QMouseEvent *event );
		virtual void moveEvent( QMoveEvent *event );
		virtual void tabletEvent( QTabletEvent *event );
		virtual void wheelEvent( QWheelEvent *event );

	signals:
		void startBenchmark();

	protected slots:
		void benchmark();
		void fitSize( int w, int h );
		void saveCurrentView( const QString &path_without_separator, const QString &filename_without_file_extension );
};


#endif