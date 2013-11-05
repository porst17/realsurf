#include "ColorPicker.h"
#include <QColorDialog>
#include <QPainter>
#include <QPixmap>
#include <QIcon>

ColorPicker::ColorPicker( QWidget *parent ) : QPushButton( parent ), color( Qt::white ) { this->init(); }
ColorPicker::ColorPicker( const QString &text, QWidget *parent ) : QPushButton( text, parent ), color( Qt::white ) { this->init(); }
ColorPicker::ColorPicker( const QColor &color, const QString &text, QWidget *parent ) : QPushButton( text, parent ), color( color ) { this->init(); }

void ColorPicker::init()
{
	QColor tmp = this->color;
	this->color = QColor();
	this->setColor( tmp );
	QObject::connect( this, SIGNAL( clicked( bool ) ), this, SLOT( chooseColor() ) );
}

void ColorPicker::setColor( const QColor &color )
{
	if( this->color != color )
	{
		this->color = color;

		QPixmap color_pixmap( 16, 16 );
		color_pixmap.fill( this->color );
		QPainter color_painter( &color_pixmap );
		color_painter.drawRect( 0, 0, color_pixmap.width() - 1, color_pixmap.height() - 1 );

		this->setIcon( QIcon( color_pixmap ) );
		valueChanged( this->color );
	}
}

QColor ColorPicker::getColor() const
{
	return this->color;
}

void ColorPicker::chooseColor()
{
	QColor dlgColor = QColorDialog::getColor( this->color );
	if( dlgColor.isValid() )
		this->setColor( dlgColor );
}