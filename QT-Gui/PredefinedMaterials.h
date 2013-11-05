#ifndef __PREDEFINEDMATERIALS_H__
#define __PREDEFINEDMATERIALS_H__

#include <QComboBox>
#include <QMap>
#include "MaterialProperties.h"

class PredefinedMaterials : public QComboBox
{
	Q_OBJECT

	protected:
		static QMap< QString, MaterialProperties > *initMaterials();
		static QMap< QString, MaterialProperties > *mat_map;
		
	public:
		PredefinedMaterials( QWidget *parent = 0 );

	public slots:
		void materialSelected( const QString &mat_name );

	signals:
		void materialSelected( const MaterialProperties &mp );	
};

#endif