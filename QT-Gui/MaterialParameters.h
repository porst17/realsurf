#ifndef __MATERIALPARAMETER_H__
#define __MATERIALPARAMETER_H__

#include "ui_materialParameters.h"
#include "MaterialProperties.h"
#include "PredefinedMaterials.h"

class MaterialParameters : public QWidget, protected Ui_materialParameters
{
	Q_OBJECT
	
	private:
		PredefinedMaterials *predefinedMaterials;
		QDoubleSpinBox *shininess;
		QDoubleSpinBox *transparency;
		ColorPicker *ambient;
		ColorPicker *diffuse;
		ColorPicker *specular;
		QGroupBox *groupBox;
		QLabel *transparencyLabel;
		MaterialProperties mp;

		void setupPrivates();
		void setupConnections();

	private slots:
		void someValueChanged();

	public:
		MaterialParameters( const QString &name, QWidget *parent = 0 );
		MaterialParameters( const QString &name, const MaterialProperties &mp, QWidget *parent = 0 );

		const MaterialProperties &getValue();
		QString getName();

	public slots:
		void usePredefinedMaterial( const QString &name );
		void setValue( const MaterialProperties &mp );
		void setName( const QString &name );
		void setTransparencyEnabled( bool on );
	
	signals:
		void valueChanged( const MaterialProperties &mp );
		void nameChanged( const QString &name );
};

#endif
