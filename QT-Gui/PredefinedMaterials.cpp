#include "PredefinedMaterials.h"	

#define NUM_MATERIALS 31
namespace
{
	static QString MaterialNames[NUM_MATERIALS] = {	"emerald", "jade", "obsidian", "pearl", "ruby", "turquoise", "brass",
											"bronze", "chrome", "copper", "gold", "silver", "black plastic", "cyan plastic",
											"green plastic", "red plastic", "white plastic", "yellow plastic", "black rubber",
											"cyan rubber", "green rubber", "red rubber", "white rubber", "yellow rubber",
											"bright white", "less bright white", "warmish white", "coolish white", "black", "benchmark 1", "benchmark 2" };
	static qreal Material[NUM_MATERIALS][10] = {
	{
		//EMERALD

		0.0215f, 0.1745f, 0.0215f,

		0.07568f, 0.61424f, 0.07568f, 

		0.633f, 0.727811f, 0.633f, 

		0.6f,
			},{
		//JADE

		0.135f, 0.2225f, 0.1575f,

		0.54f, 0.89f, 0.63f, 

		0.316228f, 0.316228f, 0.316228f, 

		0.1f,
			},{
		//OBSIDIAN

		0.05375f, 0.05f, 0.06625f,

		0.18275f, 0.17f, 0.22525f, 

		0.332741f, 0.328634f, 0.346435f, 

		0.3f,
			},{
		//PEARL

		0.25f, 0.20725f, 0.20725f,

		1.0f, 0.829f, 0.829f, 

		0.296648f, 0.296648f, 0.296648f, 

		0.088f,
			},{
		//RUBY

		0.1745f, 0.01175f, 0.01175f,

		0.61424f, 0.04136f, 0.04136f, 

		0.727811f, 0.626959f, 0.626959f, 

		0.6f,
			},{
		//TURQUOISE

		0.1f, 0.18725f, 0.1745f,

		0.396f, 0.74151f, 0.69102f, 

		0.297254f, 0.30829f, 0.306678f, 

		0.1f,
			},{
		//BRASS

		0.329412f, 0.223529f, 0.027451f,

		0.780392f, 0.568627f, 0.113725f, 

		0.992157f, 0.941176f, 0.807843f,

		0.21794872f,
			},{
		//BRONZE

		0.2125f, 0.1275f, 0.054f,

		0.714f, 0.4284f, 0.18144f, 

		0.393548f, 0.271906f, 0.166721f, 

		0.2f,
			},{
		//CHROME

		0.25f, 0.25f, 0.25f,

		0.4f, 0.4f, 0.4f, 

		0.774597f, 0.774597f, 0.774597f, 

		0.6f,
			},{
		//COPPER

		0.19125f, 0.0735f, 0.0225f,

		0.7038f, 0.27048f, 0.0828f, 

		0.256777f, 0.137622f, 0.086014f, 

		0.1f,
			},{
		//GOLD

		0.24725f, 0.1995f, 0.0745f,

		0.75164f, 0.60648f, 0.22648f, 

		0.628281f, 0.555802f, 0.366065f, 

		0.4f,
			},{
		//SILVER

		0.19225f, 0.19225f, 0.19225f,

		0.50754f, 0.50754f, 0.50754f, 

		0.508273f, 0.508273f, 0.508273f, 

		0.4f,
			},{
		//BLACK PLASTIC

		0.0f, 0.0f, 0.0f, 

		0.01f, 0.01f, 0.01f,

		0.50f, 0.50f, 0.50f, 

		.25f,
			},{
		//CYAN PLASTIC

		0.0f, 0.1f, 0.06f, 

		0.0f, 0.50980392f, 0.50980392f,

		0.50196078f, 0.50196078f, 0.50196078f, 

		.25f,
			},{
		//GREEN PLASTIC

		0.0f, 0.0f, 0.0f,

		0.1f, 0.35f, 0.1f, 

		0.45f, 0.55f, 0.45f, 

		.25f,
			},{
		//RED PLASTIC

		0.0f, 0.0f, 0.0f, 

		0.5f, 0.0f, 0.0f,

		0.7f, 0.6f, 0.6f, 

		.25f,
			},{
		//WHITE PLASTIC

		0.0f, 0.0f, 0.0f, 

		0.55f, 0.55f, 0.55f,

		0.70f, 0.70f, 0.70f, 

		.25f,
			},{
		//YELLOW PLASTIC

		0.0f, 0.0f, 0.0f, 

		0.5f, 0.5f, 0.0f,

		0.60f, 0.60f, 0.50f, 

		.25f,
			},{
		//BLACK RUBBER

		0.02f, 0.02f, 0.02f, 

		0.01f, 0.01f, 0.01f,

		0.4f, 0.4f, 0.4f, 

		.078125f,
			},{
		//CYAN RUBBER

		0.0f, 0.05f, 0.05f, 

		0.4f, 0.5f, 0.5f,

		0.04f, 0.7f, 0.7f, 

		.078125f,
			},{
		//GREEN RUBBER

		0.0f, 0.05f, 0.0f, 

		0.4f, 0.5f, 0.4f,

		0.04f, 0.7f, 0.04f, 

		.078125f,
			},{
		//RED RUBBER

		0.05f, 0.0f, 0.0f, 

		0.5f, 0.4f, 0.4f,

		0.7f, 0.04f, 0.04f, 

		.078125f,
			},{
		//WHITE RUBBER

		0.05f, 0.05f, 0.05f, 

		0.5f, 0.5f, 0.5f,

		0.7f, 0.7f, 0.7f, 

		.078125f,
			},{
		//YELLOW RUBBER

		0.05f, 0.05f, 0.0f, 

		0.5f, 0.5f, 0.4f,

		0.7f, 0.7f, 0.04f, 

		.078125f,
			},{
		// bright white

		0.2f, 0.2f, 0.2f,

		1.0f, 1.0f, 1.0f, 

		0.8f, 0.8f, 0.8f, 
			
		0.4f,
			},{
		// less bright white

		0.2f, 0.2f, 0.2f,

		0.8f, 0.8f, 0.8f, 

		0.5f, 0.5f, 0.5f, 

		0.35f,
			},{
		// warmish white

		0.3f, 0.2f, 0.2f,

		1.0f, 0.9f, 0.8f, 

		0.4f, 0.2f, 0.2f, 

		0.35f,
			},{
		// coolish white

		0.2f, 0.2f, 0.3f,

		0.8f, 0.9f, 1.0f, 

		0.2f, 0.2f, 0.4f, 

		0.35f

		},{
		// black hole

		0.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 

		0.0f, 0.0f, 0.0f, 

		0.0f

		},{
		// benchmark 1

		0.75f * 24.0f / 255.0f, 0.75f * 109.0f / 255.0f, 0.75f * 178.0f / 255.0f,

		0.0f, 0.0f, 0.0f, 

		0.0f, 0.0f, 0.0f, 

		0.0f

		},{
		// benchmark 2

		0.75f * 255.0f / 255.0f, 0.75f * 189.0f / 255.0f, 0.75f * 35.0f / 255.0f,

		0.0f, 0.0f, 0.0f, 

		0.0f, 0.0f, 0.0f, 

		0.0f

		}
	};
};

QMap< QString, MaterialProperties > *PredefinedMaterials::mat_map = PredefinedMaterials::initMaterials();

PredefinedMaterials::PredefinedMaterials( QWidget *parent ) : QComboBox( parent )
{
	initMaterials();
	this->addItem( "" );
	for( int i = 0; i < NUM_MATERIALS; i++ )
		this->addItem( MaterialNames[ i ] );
	QObject::connect( this, SIGNAL( activated( const QString & ) ), this, SLOT( materialSelected( const QString & ) ) );
}

void PredefinedMaterials::materialSelected( const QString &mat_name )
{
	if( mat_name != "" )
	{
		int index = findText( mat_name );
		if( index != -1 )
		{
			setCurrentIndex( index );
			materialSelected( (*mat_map)[ this->currentText() ] );
		}
	}
}

QMap< QString, MaterialProperties > *PredefinedMaterials::initMaterials()
{
	// copy material information to map
	QMap< QString, MaterialProperties > *mat_map = new QMap< QString, MaterialProperties >();
	for( int i = 0; i < NUM_MATERIALS; i++ )
	{
		MaterialProperties mp;
		int c = 0;

		mp.emission = Qt::black;

		mp.ambient = Qt::black;
		mp.ambient.setRed( 255 * Material[ i ][ c++ ] );
		mp.ambient.setGreen( 255 * Material[ i ][ c++ ] );
		mp.ambient.setBlue( 255 * Material[ i ][ c++ ] );
		
		mp.diffuse = Qt::black;
		mp.diffuse.setRed( 255 * Material[ i ][ c++ ] );
		mp.diffuse.setGreen( 255 * Material[ i ][ c++ ] );
		mp.diffuse.setBlue( 255 * Material[ i ][ c++ ] );

		mp.specular = Qt::black;
		mp.specular.setRed( 255 * Material[ i ][ c++ ] );
		mp.specular.setGreen( 255 * Material[ i ][ c++ ] );
		mp.specular.setBlue( 255 * Material[ i ][ c++ ] );

		mp.shininess = Material[ i ][ c++ ] * 128.0;
		mp.transparency = 0.0;

		(*mat_map)[ MaterialNames[ i ] ] = mp;
	}

	return mat_map;
}