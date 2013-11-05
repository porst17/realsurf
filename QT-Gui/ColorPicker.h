#ifndef __COLORPICKER_H__
#define __COLORPICKER_H__

#include <QPushButton>
#include <QColorDialog>

class ColorPicker : public QPushButton
{
	Q_OBJECT
	Q_PROPERTY( QColor color READ getColor WRITE setColor )

	private:
		QColor color;
		void init();

	public:
		ColorPicker( QWidget *parent = 0 );
		ColorPicker( const QString &text, QWidget *parent = 0 );
		ColorPicker( const QColor &color, const QString &text, QWidget *parent = 0 );

		QColor getColor() const;

	public slots:
		void setColor( const QColor &color );
		void chooseColor();

	signals:
		void valueChanged( const QColor &color );
};

#endif