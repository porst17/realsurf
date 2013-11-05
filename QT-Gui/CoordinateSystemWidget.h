#ifndef __COORDINATE_SYSTEM_WIDGET_H__
#define __COORDINATE_SYSTEM_WIDGET_H__

#include <QWidget>
#include "FloatParameterProperties.h"

class CoordinateSystemWidget : public QWidget
{
	Q_OBJECT

	public:
		CoordinateSystemWidget( QWidget *parent = 0 );
		CoordinateSystemWidget( const QString &nameX, const FloatParameterProperties &pX, const QString &nameY, const FloatParameterProperties &pY, QWidget *parent = 0 );
		FloatParameterProperties getPropertiesX() const;
		FloatParameterProperties getPropertiesY() const;
		void getPropertiesX( QString &name, FloatParameterProperties &p ) const;
		void getPropertiesY( QString &name, FloatParameterProperties &p ) const;
		virtual int heightForWidth( int w ) const;

	public slots:
		void setPropertiesX( const QString &name, const FloatParameterProperties &p );
		void setPropertiesY( const QString &name, const FloatParameterProperties &p );

	signals:
		void propertiesXChanged( const QString &name, const FloatParameterProperties &p );
		void propertiesYChanged( const QString &name, const FloatParameterProperties &p );
		void propertiesChanged( const QString &name, const FloatParameterProperties &p );

	protected:
		virtual void paintEvent( QPaintEvent *event );
		virtual void mouseMoveEvent( QMouseEvent *event );
		virtual void mousePressEvent( QMouseEvent *event );
		virtual void mouseReleaseEvent( QMouseEvent *event );
		virtual void keyReleaseEvent( QKeyEvent *event );

		QRect innerCanvasRect();

	private:
		QString xName;
		QString yName;
		FloatParameterProperties x;
		FloatParameterProperties y;

		enum Direction { BOTH = 0x3, HORIZONTAL = 0x1, VERTICAL = 0x2 };
		Direction fixedDir;
};
#endif