#ifndef __PARAMETER_WIDGET_H__
#define __PARAMETER_WIDGET_H__

#include <QWidget>
#include <QTabWidget>
#include <QMap>
#include <QList>
#include "FloatParameter.h"
#include "CoordinateSystemWidget.h"
#include "FloatParameterProperties.h"

class ParameterWidget : public QWidget
{
	Q_OBJECT

	public slots:
		void setProperties( const QString &name, const FloatParameterProperties &p );

	public:
		ParameterWidget( QWidget *parent = 0 );
		virtual ~ParameterWidget();

		QMap< QString, FloatParameterProperties > getProperties() const;
		void clear();

	protected:
		QList< FloatParameter * > *oneD;
		QList< CoordinateSystemWidget * > *twoD;
		QTabWidget *tabs;
		QWidget *widget1D;
		QWidget *widget2D;

		void init();

	signals:
		void propertiesChanged( const QString &name, const FloatParameterProperties &p );
		void valueChanged( const QString &name, double value );
};

#endif