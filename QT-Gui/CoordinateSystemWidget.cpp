#include "CoordinateSystemWidget.h"
#include <QPalette>
#include <QColor>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolTip>
#include <cmath>

CoordinateSystemWidget::CoordinateSystemWidget( QWidget *parent ) : QWidget( parent )
{
	this->setPropertiesX( "x", FloatParameterProperties() );
	this->setPropertiesY( "y", FloatParameterProperties() );
	this->fixedDir = CoordinateSystemWidget::BOTH;
	this->setFocusPolicy( Qt::ClickFocus );

	QSizePolicy sp( this->sizePolicy().horizontalPolicy(), this->sizePolicy().verticalPolicy() );
	sp.setHeightForWidth( true );
	this->setSizePolicy( sp );
}

CoordinateSystemWidget::CoordinateSystemWidget( const QString &nameX, const FloatParameterProperties &pX, const QString &nameY, const FloatParameterProperties &pY, QWidget *parent ) : QWidget( parent )
{
	this->setPropertiesX( nameX, pX );
	this->setPropertiesY( nameY, pY );
	this->fixedDir = CoordinateSystemWidget::BOTH;
	this->setFocusPolicy( Qt::ClickFocus );

	QSizePolicy sp( this->sizePolicy().horizontalPolicy(), this->sizePolicy().verticalPolicy() );
	sp.setHeightForWidth( true );
	this->setSizePolicy( sp );
}


QRect CoordinateSystemWidget::innerCanvasRect()
{
	return QRect( 63, 0, this->width() - 64, this->height() - 22 );
}

void CoordinateSystemWidget::paintEvent( QPaintEvent * event )
{
	QPointF pos;
	pos.setX( ( this->x.value() - this->x.minimum() ) / ( this->x.maximum() - this->x.minimum() ) );
	pos.setY( ( this->y.value() - this->y.minimum() ) / ( this->y.maximum() - this->y.minimum() ) );

	QWidget::paintEvent( event );
	QPainter p( this );
	p.setBrush( QColor( 255, 255, 255 ) );
	QRect innerCanvasRect = this->innerCanvasRect();
	p.drawRect( innerCanvasRect );
	p.setBrush( QColor( 0, 0, 0 ) );

	QPointF topLeft( innerCanvasRect.x(), innerCanvasRect.y() );
	QPointF topRight( innerCanvasRect.x() + innerCanvasRect.width(), innerCanvasRect.y() );
	QPointF bottomLeft( innerCanvasRect.x(), innerCanvasRect.y() + innerCanvasRect.height() );
	QPointF bottomRight( innerCanvasRect.x() + innerCanvasRect.width(), innerCanvasRect.y() + innerCanvasRect.height() );

	// ticks
	int numMajorTicks = 4;
	int numMinorTicks = 2;
	int majorTickSize = 5;
	int minorTickSize = 2;
	for( int i = 0; i < numMinorTicks * numMajorTicks + 1; i++ )
	{
		int ticksize = i % numMinorTicks == 0 ? majorTickSize : minorTickSize;
		float t = i / float( numMinorTicks * numMajorTicks );
		QPointF lineStart;

		// bottom
		lineStart = bottomLeft * t + bottomRight * ( 1.0f - t );
		p.drawLine( lineStart, lineStart + QPointF( 0.0f, ticksize ) );

		// left
		lineStart = topLeft * t + bottomLeft * ( 1.0f - t );
		p.drawLine( lineStart, lineStart + QPointF( -ticksize, 0.0f ) );
	}

	// text on first and last horizontal and vertical tick
	QString text;
	QPointF start;

	// bottom left (y-axis)
	text.setNum( this->y.minimum(), 'g', 4 );
	start = bottomLeft + QPointF( -1, 0 );
	p.drawText( QRectF( start + QPointF( -100, -100 ), start ), Qt::AlignBottom | Qt::AlignRight, text );

	// top left (y-axis)
	text.setNum( this->y.maximum(), 'g', 4 );
	start = topLeft + QPointF( -1, 0 );
	p.drawText( QRectF( start + QPointF( -100, 0 ), start + QPointF( 0, 100 ) ), Qt::AlignTop | Qt::AlignRight, text );

	// bottom left (x-axis)
	text.setNum( this->x.minimum(), 'g', 4 );
	start = bottomLeft + QPointF( 2, 0 );
	p.drawText( QRectF( start, start + QPointF( 100, 100 ) ), Qt::AlignTop | Qt::AlignLeft, text );

	// bottom right (x-axis)
	text.setNum( this->x.maximum(), 'g', 4 );
	start = bottomRight + QPointF( -2, 0 );
	p.drawText( QRectF( start + QPointF( -100, 0 ), start + QPointF( 0, 100 ) ), Qt::AlignTop | Qt::AlignRight, text );

	// y-label
	p.drawText( QRectF( topLeft + QPointF( -100, 0 ), bottomLeft + QPointF( -30, 0 ) ), Qt::AlignVCenter | Qt::AlignRight, yName );

	// x-label
	p.drawText( QRectF( bottomLeft + QPointF( 0, 1 * majorTickSize ), bottomRight + QPointF( 0, 1 * majorTickSize + 100 ) ), Qt::AlignHCenter | Qt::AlignTop, xName );

	// draw cross
	QPoint crossCenter( topLeft.x() * ( 1.0 - pos.x() ) + topRight.x() * pos.x(), topLeft.y() * pos.y() + bottomLeft.y() * ( 1.0 - pos.y() ) );

	p.setPen( this->fixedDir & CoordinateSystemWidget::HORIZONTAL ? QColor( 0, 0, 0 ) : QColor( 200, 200, 200 ) );
	p.drawLine( crossCenter.x(), topRight.y() + 1, crossCenter.x(), bottomRight.y() - 1 );
	p.setPen( this->fixedDir & CoordinateSystemWidget::VERTICAL ? QColor( 0, 0, 0 ) : QColor( 200, 200, 200 ) );
	p.drawLine( topLeft.x() + 1, crossCenter.y(), topRight.x() - 1, crossCenter.y() );
	p.setPen( QColor( 0, 0, 0 ) );

	p.setClipping( true );
	p.setClipRect( innerCanvasRect );
	p.drawEllipse( ( int ) crossCenter.x() - 2, ( int ) crossCenter.y() - 2, 4, 4 );
	p.setClipping( false );

	// draw tooltip with actual parameter values
	if( this->underMouse() )
		QToolTip::showText( this->mapToGlobal( crossCenter ), xName + ":" + QString::number( this->x.value() ) + ", " + yName + ":" + QString::number( this->y.value() ), this );
}

namespace
{
	double clamp01( double v )
	{
		return v > 1.0 ? 1.0 : ( v < 0.0 ? 0.0 : v );
	}
};

void CoordinateSystemWidget::keyReleaseEvent( QKeyEvent *event )
{
	if( event->key() == Qt::Key_Shift )
	{
		this->fixedDir = CoordinateSystemWidget::BOTH;
		update();

		QMouseEvent *event = new QMouseEvent( QEvent::MouseMove, this->mapFromGlobal( QCursor::pos() ), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier );
		this->mouseMoveEvent( event );
		delete event;
	}
}

void CoordinateSystemWidget::mousePressEvent( QMouseEvent * event )
{
	if( event->buttons() & Qt::LeftButton )
		mouseMoveEvent( event );
}

void CoordinateSystemWidget::mouseReleaseEvent( QMouseEvent * event )
{
	this->fixedDir = CoordinateSystemWidget::BOTH;
	update();
}

void CoordinateSystemWidget::mouseMoveEvent( QMouseEvent * event )
{
	QRect innerCanvasRect = this->innerCanvasRect();
	if( event->buttons() & Qt::LeftButton )
	{
		double x_t = double( event->pos().x() - innerCanvasRect.left() ) / ( innerCanvasRect.width() - 1 );
		double x_value = this->x.minimum() * ( 1.0 - x_t ) + this->x.maximum() * x_t;

		double y_t = 1.0 - ( double( event->pos().y() - innerCanvasRect.top() ) / ( innerCanvasRect.height() - 1 ) );
		double y_value = this->y.minimum() * ( 1.0 - y_t ) + this->y.maximum() * y_t;

		if( event->modifiers() & Qt::ShiftModifier ) 
		{
			 if( this->fixedDir == CoordinateSystemWidget::BOTH )
				 if( std::fabs( this->x.value() - x_value ) > std::fabs( this->y.value() - y_value ) )
					 this->fixedDir = CoordinateSystemWidget::HORIZONTAL;
				 else
					 this->fixedDir = CoordinateSystemWidget::VERTICAL;
		}
		else
		{
			this->fixedDir = CoordinateSystemWidget::BOTH;
		}

		if( this->fixedDir & CoordinateSystemWidget::HORIZONTAL && this->x.value() != x_value )
		{
			this->x.setValue( x_value );
			propertiesXChanged( this->xName, this->x );
			propertiesChanged( this->xName, this->x );
			update();
		}

		if( this->fixedDir & CoordinateSystemWidget::VERTICAL && this->y.value() != y_value )
		{
			this->y.setValue( y_value );
			propertiesYChanged( this->yName, this->y );
			propertiesChanged( this->yName, this->y );
			update();
		}
	}
}

FloatParameterProperties CoordinateSystemWidget::getPropertiesX() const
{
	return this->x;
}

FloatParameterProperties CoordinateSystemWidget::getPropertiesY() const
{
	return this->y;
}

void CoordinateSystemWidget::getPropertiesX( QString &name, FloatParameterProperties &p ) const
{
	name.clear();
	name.append( xName );
	p.setProperties( this->x );
}

void CoordinateSystemWidget::getPropertiesY( QString &name, FloatParameterProperties &p ) const
{
	name.clear();
	name.append( yName );
	p.setProperties( this->y );
}

void CoordinateSystemWidget::setPropertiesX( const QString &name, const FloatParameterProperties &p )
{
	if( this->xName != name || this->x != p )
	{
		this->xName = name;
		this->x = p;
		update();
		propertiesXChanged( this->xName, this->x );
		propertiesChanged( this->xName, this->x );
	}
}

void CoordinateSystemWidget::setPropertiesY( const QString &name, const FloatParameterProperties &p )
{
	if( this->yName != name || this->y != p )
	{
		this->yName = name;
		this->y = p;
		update();
		propertiesYChanged( this->yName, this->y );
		propertiesChanged( this->yName, this->y );
	}
}
int CoordinateSystemWidget::heightForWidth( int w ) const
{
	return ( w - 63 ) + 21;
}