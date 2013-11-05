#include "SurfaceRenderWidget.h"
#include <QMessageBox>
#include <QThread>
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <string>
#include <set>
#include <cmath>
#include <GL/glu.h>
#include <iostream>
#include <fstream>

using namespace glsl;

SurfaceRenderWidget::SurfaceRenderWidget( QWidget *parent ) : QGLWidget( QGLFormat( QGL::AlphaChannel ), parent )
{
	//this->setToolTip( "Left mouse button: rotate surface\nRight mouse button: translate surface\nMouse wheel or +/-: scale surface" );
	sm = NULL;
	show_bounding_object = false;
	show_fps = false;
	process_input_events = true;
	feedback_enabled = true;
	framecount = 0;
	time = QTime();
	time.start();
	float fps = 0.0f;
	fps_timer = new QTimer( this );
	connect( fps_timer, SIGNAL( timeout() ), this, SLOT( calculateFramesPerSecond() ) );
	fps_timer->setInterval( 1000 );
	fps_timer->setSingleShot( false );
	fps_timer->start();
}

SurfaceRenderWidget::~SurfaceRenderWidget()
{
	if( sm != NULL )
	{
		delete sm;
	}
}

void SurfaceRenderWidget::processInputEvents( bool process )
{
	this->process_input_events = process;
}

void SurfaceRenderWidget::setFeedbackEnabled( bool enabled )
{
	this->feedback_enabled = enabled;
	if( !enabled )
		this->setToolTip( "" );
}

QSize SurfaceRenderWidget::minimumSizeHint () const
{
	return QSize( 512, 512 );
}

void SurfaceRenderWidget::setBackgroundColor( const QColor &bg_color )
{
	makeCurrent();
    glClearColor( GLfloat( bg_color.redF() ), GLfloat( bg_color.greenF() ), GLfloat( bg_color.blueF() ), 0.0 );
	update();
}

void SurfaceRenderWidget::setFrontMaterial( const MaterialProperties &mp ) { this->front_material = mp; update(); }
void SurfaceRenderWidget::setBackMaterial( const MaterialProperties &mp ) { this->back_material = mp; update(); }

void SurfaceRenderWidget::setLight0( const LightProperties &lp ) { setLight( GL_LIGHT0, lp ); }
void SurfaceRenderWidget::setLight1( const LightProperties &lp ) { setLight( GL_LIGHT1, lp ); }
void SurfaceRenderWidget::setLight2( const LightProperties &lp ) { setLight( GL_LIGHT2, lp ); }
void SurfaceRenderWidget::setLight3( const LightProperties &lp ) { setLight( GL_LIGHT3, lp ); }
void SurfaceRenderWidget::setLight4( const LightProperties &lp ) { setLight( GL_LIGHT4, lp ); }
void SurfaceRenderWidget::setLight5( const LightProperties &lp ) { setLight( GL_LIGHT5, lp ); }
void SurfaceRenderWidget::setLight6( const LightProperties &lp ) { setLight( GL_LIGHT6, lp ); }
void SurfaceRenderWidget::setLight7( const LightProperties &lp ) { setLight( GL_LIGHT7, lp ); }
void SurfaceRenderWidget::toggleLight0( bool on, const LightProperties &lp ) { toggleLight( GL_LIGHT0, on, lp ); }
void SurfaceRenderWidget::toggleLight1( bool on, const LightProperties &lp ) { toggleLight( GL_LIGHT1, on, lp ); }
void SurfaceRenderWidget::toggleLight2( bool on, const LightProperties &lp ) { toggleLight( GL_LIGHT2, on, lp ); }
void SurfaceRenderWidget::toggleLight3( bool on, const LightProperties &lp ) { toggleLight( GL_LIGHT3, on, lp ); }
void SurfaceRenderWidget::toggleLight4( bool on, const LightProperties &lp ) { toggleLight( GL_LIGHT4, on, lp ); }
void SurfaceRenderWidget::toggleLight5( bool on, const LightProperties &lp ) { toggleLight( GL_LIGHT5, on, lp ); }
void SurfaceRenderWidget::toggleLight6( bool on, const LightProperties &lp ) { toggleLight( GL_LIGHT6, on, lp ); }
void SurfaceRenderWidget::toggleLight7( bool on, const LightProperties &lp ) { toggleLight( GL_LIGHT7, on, lp ); }

namespace
{
	std::string QStringToSTDString( const QString &s )
	{
		return std::string( s.toUtf8().data() );
	}

	QString STDStringToQString( const std::string &s )
	{
		QString res;
		for( unsigned int i = 0; i < s.size(); i++ )
			res[ i ] = s[ i ];
		return res;
	}
}

void SurfaceRenderWidget::setFormula( const QString &formula )
{
	makeCurrent();
	std::string std_formula = QStringToSTDString( formula );

	QThread *currentThread = QThread::currentThread();
	QThread::Priority current_priority = currentThread->priority();
	currentThread->setPriority( QThread::LowPriority );

	ShaderManager::ErrorCode error_code = sm->create_shader( std_formula );

	currentThread->setPriority( current_priority );

	callParsingComplete( error_code );
	update();
}

void SurfaceRenderWidget::callParsingComplete( ShaderManager::ErrorCode error_code )
{
	if( error_code != ShaderManager::SUCCESS )
	{
		QString q_error_msg = STDStringToQString( sm->get_error_message() );
		QString summary, text;
		switch( error_code )
		{
			case ShaderManager::EQUATION_PARSE_ERROR:
				summary = "Parse error in surface equation";
				text = q_error_msg;
				break;
			case ShaderManager::GLSL_COMPILATION_ERROR:
				summary = "GLSL-Shader compilation error";
				text = "";
				break;
			case ShaderManager::GLSL_LINKING_ERROR:
				summary = "GLSL-Shader linking error";
				text = "";
				break;
			case ShaderManager::GLSL_VALIDATION_ERROR:
				summary = "GLSL-Shader validation error";
				text = "";
				break;
			case ShaderManager::FILE_IO_ERROR:
				summary = "File I/O-error";
				text = q_error_msg;
				break;
		}
		if( error_code != ShaderManager::EQUATION_PARSE_ERROR && error_code != ShaderManager::FILE_IO_ERROR )
		{
			if( !( QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_2_0 ) )
			{
				if( this->feedback_enabled )
					QMessageBox::critical( 0, "OpenGL error", "Your graphics card does not support OpenGL 2.0.\nRendering will fail.\n\n Make sure, that you are using a graphics card and graphics driver with full support for OpenGL 2.0 and Shader Model 3." );
			}
			else
			{
				QFile glsl_error_file( "glsl_error.txt" );
				glsl_error_file.open( QIODevice::WriteOnly | QIODevice::Text );
				glsl_error_file.write( sm->get_error_message().c_str() );
				glsl_error_file.close();
				if( this->feedback_enabled )
					QMessageBox::critical( 0, summary, "RealSurf was unable to render the surface.\nDebug log was written to glsl_error.txt.\n\nMake sure, that you are using a graphics card and graphics driver with full support for OpenGL 2.0 and Shader Model 3. Complex surface equations may also cause such errors. Please check the error log for messages similar to \"too many instructions\" or \"temporary register limit exceeded\"." );
			}
		}
		if( this->feedback_enabled )
			parsingComplete( false, summary, text );
		else
			parsingComplete( false, "", "" );
		qDebug( sm->get_error_message().c_str() );
	}
	else
	{
		const std::set<std::string> std_param_list = sm->get_parameters();
		QSet< QString > param_list;
		for( std::set<std::string>::const_iterator it = std_param_list.begin(); it != std_param_list.end(); it++ )
			param_list << STDStringToQString( *it );
		parsingComplete( true, "", "" );
		newParameters( param_list );
	}
}

void SurfaceRenderWidget::setFloatParameter( const QString &name, double value )
{
	makeCurrent();
	std::string std_name = QStringToSTDString( name );
	sm->set_surface_parameter( std_name, GLfloat( value ) );
	update();
}

void SurfaceRenderWidget::setTransparencyEnabled( bool on )
{
	sm->set_transparency_enabled( on );
}

void SurfaceRenderWidget::reload()
{
	makeCurrent();
	ShaderManager::ErrorCode error_code = sm->reload();
	callParsingComplete( error_code );
	update();
}

void SurfaceRenderWidget::setLight( GLenum light, const LightProperties &lp )
{
	makeCurrent();
	#pragma message ( __FILE__ ": TODO: SET MV MATRIX FOR LIGHT" )

	GLfloat gl_light_position[ 4 ] = { GLfloat( lp.posX ), GLfloat( lp.posY ), GLfloat( lp.posZ ), 1.0f };

	// retrieve current ambient alpha ( 0.0 = light off; otherwise = light on)
	GLfloat gl_light_ambient[ 4 ];
	glGetLightfv( light, GL_AMBIENT, gl_light_ambient );
	gl_light_ambient[ 0 ] = GLfloat( lp.ambient.redF() );
	gl_light_ambient[ 1 ] = GLfloat( lp.ambient.greenF() );
	gl_light_ambient[ 2 ] = GLfloat( lp.ambient.blueF() );

	GLfloat gl_light_diffuse[ 4 ]  = { GLfloat( lp.diffuse.redF() ), GLfloat( lp.diffuse.greenF() ), GLfloat( lp.diffuse.blueF() ), 1.0f };
	GLfloat gl_light_specular[ 4 ] = { GLfloat( lp.specular.redF() ), GLfloat( lp.specular.greenF() ), GLfloat( lp.specular.blueF() ), 1.0f };

	GLfloat gl_light_spot_direction[ 3 ] = { GLfloat( lp.spotDirX ), GLfloat( lp.spotDirY ), GLfloat( lp.spotDirZ ) };

	glLightfv( light, GL_POSITION, gl_light_position );

	glLightfv( light, GL_AMBIENT, gl_light_ambient );
	glLightfv( light, GL_DIFFUSE, gl_light_diffuse );
	glLightfv( light, GL_SPECULAR, gl_light_specular );

	glLightf( light, GL_CONSTANT_ATTENUATION, GLfloat( lp.constantAttenuation ) );
	glLightf( light, GL_LINEAR_ATTENUATION, GLfloat( lp.linearAttenuation ) );
	glLightf( light, GL_QUADRATIC_ATTENUATION, GLfloat( lp.quadraticAttenuation ) );

    glLightfv( light, GL_SPOT_DIRECTION, gl_light_spot_direction );
	glLightf( light, GL_SPOT_CUTOFF, GLfloat( lp.spotCutoff ) );
	glLightf( light, GL_SPOT_EXPONENT, GLfloat( lp.spotCutoffExponent ) );
	update();
}

void SurfaceRenderWidget::setMaterial( GLenum face, const MaterialProperties &mp )
{
	makeCurrent();
	GLfloat emission_color[ 4 ]  = { GLfloat( mp.emission.redF() ), GLfloat( mp.emission.greenF() ), GLfloat( mp.emission.blueF() ), 1.0f };
	GLfloat ambient_color[ 4 ]  = { GLfloat( mp.ambient.redF() ), GLfloat( mp.ambient.greenF() ), GLfloat( mp.ambient.blueF() ), 1.0f };
	GLfloat diffuse_color[ 4 ]  = { GLfloat( mp.diffuse.redF() ), GLfloat( mp.diffuse.greenF() ), GLfloat( mp.diffuse.blueF() ), 1.0f - GLfloat( mp.transparency ) };
	GLfloat specular_color[ 4 ] = { GLfloat( mp.specular.redF() ), GLfloat( mp.specular.greenF() ), GLfloat( mp.specular.blueF() ), 1.0f };
	GLfloat shininess = GLfloat( mp.shininess );

	glMaterialfv( face, GL_EMISSION, emission_color );
	glMaterialfv( face, GL_AMBIENT, ambient_color );
	glMaterialfv( face, GL_DIFFUSE, diffuse_color );
	glMaterialfv( face, GL_SPECULAR, specular_color );
	glMaterialf( face, GL_SHININESS, shininess );
}

void SurfaceRenderWidget::toggleLight( GLenum light, bool on, const LightProperties &lp )
{
	makeCurrent();
	// set ambient alpha ( 0.0 = light off; otherwise = light on)
	GLfloat gl_light_ambient[ 4 ];
	glGetLightfv( light, GL_AMBIENT, gl_light_ambient );
	gl_light_ambient[ 3 ] = on ? 1.0f : 0.0f;
	glLightfv( light, GL_AMBIENT, gl_light_ambient );
	setLight( light, lp );
	update();
}

void SurfaceRenderWidget::resetTransformation()
{
	bbox_transform = GLMatrix<GLfloat>::identity();
	surface_transform.loadScale( 0.25f, 0.25f, 0.25f );
	//surface_transform.loadIdentity();
	update();
}

void SurfaceRenderWidget::clipToSphere()
{
	setClippingSurface( SPHERE );
}

void SurfaceRenderWidget::clipToCube()
{
	setClippingSurface( CUBE );
}

void SurfaceRenderWidget::setClippingSurface( ClippingSurface cs )
{
	makeCurrent();
	if( cs == CUBE )
		this->sm->set_clipping_surface( ShaderManager::CUBE );
	else
		this->sm->set_clipping_surface( ShaderManager::SPHERE );
	update();
}

void SurfaceRenderWidget::showClippingObject( bool show )
{
	show_bounding_object = show;
	update();
}

void SurfaceRenderWidget::showFramesPerSecond( bool show )
{
	show_fps = show;
	update();
}

void SurfaceRenderWidget::calculateFramesPerSecond()
{
	fps = ( framecount * 1000.0f ) / time.elapsed();
	framecount = 0;
	time.restart();
}

void SurfaceRenderWidget::initializeGL()
{
	// start blowupviz hack
	glEnable(GL_TEXTURE_2D);
	glGenTextures(3,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);       
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);          
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL ); 
	glDisable(GL_TEXTURE_2D);
	// end blowupviz hack
	
	glEnable( GL_LINE_SMOOTH );
	if( sm == NULL )
		sm = new ShaderManager();
	resetTransformation();
	update();
}

void SurfaceRenderWidget::resizeGL( int w, int h )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
	if( w > h )
	{
		glViewport( 0, 0, (GLsizei) w, (GLsizei) h );
		gluPerspective( 60.0, ( float ) w / ( float ) h, 1.0, 12.0 );
	}
	else
	{
		glViewport( 0, h / 2 - w / 2, (GLsizei) w, (GLsizei) w );
		gluPerspective( 60.0, 1.0, 1.0, 12.0 );
	}

	gluLookAt(	0.0, 0.0, 0.0,
				0.0, 0.0, -1.0,
				0.0, 1.0, 0.0 );
	update();
}

namespace
{
	void wireCube( GLdouble dSize );
	void solidCube( GLdouble dSize );
	void solidSphere( GLdouble radius, GLint slices, GLint stacks );
	void wireSphere( GLdouble radius, GLint slices, GLint stacks );
};

void SurfaceRenderWidget::paintGL()
{
	// reset modelview transformation
    glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// ... disable lighting for fixed function pipeline
    glDisable( GL_LIGHTING );

	// set materials
	setMaterial( GL_FRONT, this->front_material );
	setMaterial( GL_BACK, this->back_material );

	// enable alpha blending
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable( GL_BLEND );

	// enable backface culling
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	// clear depth buffer
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );

//********************************
// begin blowupviz hack	

float r=sm->get_surface_parameter("r");
float a=sm->get_surface_parameter("a");
float b=sm->get_surface_parameter("b");

std::ofstream myfile;
myfile.open ("curve.pic");
myfile << "width=256;height=256;color_file_format = ppm;scale_x=0.1;scale_y=0.1;" << std::endl;
myfile << "double r="<<r<<";" << std::endl;
myfile << "double a="<<a<<";" << std::endl;
myfile << "double b="<<b<<";" << std::endl;
myfile << "poly u=x*r;poly v=y*r;" << std::endl;
myfile << "curve_red=255;curve_green=255;curve_blue=255;" << std::endl;
//myfile << "curve=v^2-u*(u-a)*(u-b);" << std::endl;
//myfile << "curve=u^2-a^2;" << std::endl;
myfile << "curve=(8*u^4-8*u^2+1)+(8*v^4-8*v^2+1);" << std::endl;

myfile << "draw_curve;" << std::endl;
myfile << "curve_red=255;curve_green=255;curve_blue=0;" << std::endl;
//myfile << "curve=v^2;" << std::endl;
//myfile << "curve=v^2-a^2;" << std::endl;
myfile << "curve=u^2+v^2-a^2;" << std::endl;

myfile << "draw_curve;" << std::endl;
myfile << "curve_red=255;curve_green=255;curve_blue=255;" << std::endl;
myfile << "curve=x^2+y^2-1;" << std::endl;
myfile << "draw_curve;filename = \"curve\";save_color_image;" << std::endl;
myfile.close();

system("./surf curve.pic");

QImage image( "curve.ppm" );
for( int x = 0; x < 0.2*image.width(); ++x )
	image.setPixel( x, 0, 0 );
image.invertPixels();

glViewport( 0, 0, (GLsizei) width(), (GLsizei) height() );
glDisable(GL_DEPTH_TEST);
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();
gluOrtho2D(0,this->width(),0,this->height());
glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glLoadIdentity();        
glEnable(GL_TEXTURE_2D);

glBindTexture(GL_TEXTURE_2D,texture); 
glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0 , image.width(), image.height(),  GL_BGRA, GL_UNSIGNED_BYTE, image.bits() ); 
int wh = height() < width() ? height() : width();
double img_scale = 0.2;
glBegin(GL_QUADS);   // in theory triangles are better
	glTexCoord2i(1,0); glVertex2i(0.05*img_scale*wh,img_scale*wh);
	glTexCoord2i(0,0); glVertex2i(0.05*img_scale*wh,0.05*img_scale*wh);
	glTexCoord2i(0,1); glVertex2i(img_scale*wh,0.05*img_scale*wh);
	glTexCoord2i(1,1); glVertex2i(img_scale*wh,img_scale*wh);
	
/*	
	glTexCoord2i(0,0); glVertex2i(0.05*img_scale*wh,img_scale*wh);
	glTexCoord2i(0,1); glVertex2i(0.05*img_scale*wh,0.05*img_scale*wh);
	glTexCoord2i(1,1); glVertex2i(img_scale*wh,0.05*img_scale*wh);
	glTexCoord2i(1,0); glVertex2i(img_scale*wh,img_scale*wh);*/
glEnd();   
glMatrixMode(GL_PROJECTION);
glPopMatrix();
glMatrixMode(GL_MODELVIEW);
glPopMatrix();
if( width() > height() )
	glViewport( 0, 0, (GLsizei) width(), (GLsizei) height() );
else
	glViewport( 0, height() / 2 - width() / 2, (GLsizei) width(), (GLsizei) width() );
// end blowupviz hack	
//*********************************

	// move clipping object to desired position
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glTranslated( 0.0, 0.0, -7.5 );

	// apply local transformation (i.e. rotation)
	bbox_transform.glMultMatrix();

	// apply transformation to surface
	if( sm->get_clipping_surface() == ShaderManager::CUBE )
		sm->set_surface_transformation( surface_transform * GLMatrix<GLfloat>().loadScale( 1.73205f, 1.73205f, 1.73205f ) );
	else
		sm->set_surface_transformation( surface_transform );

	// scale surrounding box
	glScaled( 3.55, 3.55, 3.55 );
	if( sm->get_clipping_surface() == ShaderManager::CUBE )		
		glScaled( 1.0 / 1.73205, 1.0 / 1.73205, 1.0 / 1.73205 );

	// start raytracing
	sm->enable_glsl();

	// raytrace in surrounding box ( size: 2x2x2 )
	glDepthFunc( GL_LEQUAL );
	solidCube( 2.0 );

	// draw object background
	sm->enable_fixed_function_pipeline();

	// draw wireframe box
	if( show_bounding_object )
	{
		glPushAttrib( GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT );
		glDepthFunc( GL_LESS );
		glPolygonMode( GL_BACK, GL_LINE );
		glCullFace( GL_FRONT );
		glColor4d( 0.75, 0.75, 0.75, 1.0 );
		if( sm->get_clipping_surface() == ShaderManager::CUBE )
			solidCube( 2.0 );
		else
			solidSphere( 1.0, 30, 30 );
		glPopAttrib();
	}

	// reset transformation
	glPopMatrix();

	framecount++;

	if( show_fps )
	{
		float c[ 4 ];
		glGetFloatv( GL_COLOR_CLEAR_VALUE, c ); 
		glColor3f( 1.0f - c[ 0 ], 1.0f - c[ 1 ], 1.0f - c[ 2 ] );
		QString fpsString;
		QTextStream fpsText( &fpsString );
		fpsText.setRealNumberPrecision( 2 );
		fpsText.setRealNumberNotation( QTextStream::FixedNotation );
		fpsText << fps << " fps";
		fpsText.flush();
		glViewport( 0, 0, (GLsizei) width(), (GLsizei) height() );
		renderText( 5, 5 + QFontInfo( QApplication::font() ).pixelSize(), fpsString );
		this->resizeGL( width(), height() );
		this->update();
	}
}

void SurfaceRenderWidget::mousePressEvent( QMouseEvent *event )
{
	if( !this->process_input_events )
		return;

	last_point = event->pos();
}

void SurfaceRenderWidget::mouseMoveEvent( QMouseEvent *event )
{
	if( !this->process_input_events )
		return;

	if( event->buttons() & Qt::LeftButton )
	{
		int x = event->x();
		int y = event->y();

		// rotate clipping object
		GLdouble angle = sqrt( ( double ) ( last_point.y() - y ) * ( last_point.y() - y ) + ( last_point.x() - x ) * ( last_point.x() - x ) );
		GLdouble axis_x = -( last_point.y() - y ) / angle;
		GLdouble axis_y = -( last_point.x() - x ) / angle;

		if( ( axis_x != 0 || axis_y != 0 ) && angle != 0 )
			bbox_transform = GLMatrix<GLfloat>::identity().applyRotate( angle, axis_x, axis_y, 0.0 ) * bbox_transform;
	}
	else
	{
		QPoint diff = last_point - event->pos();
		surface_transform = bbox_transform.inverse() * GLMatrix<GLfloat>().loadTranslate( -diff.x() / GLfloat( this->width() ), diff.y() / GLfloat( this->height() ), 0.0f ) * bbox_transform * surface_transform;
	}
	last_point = event->pos();
	update();
}

#define SCALE_FACTOR ( 1.0f + 0.015625f )

void SurfaceRenderWidget::wheelEvent( QWheelEvent *event )
{
	if( !this->process_input_events )
		return;

	if( event->delta() < 0 )
	{
		/* zoom in */ 
		//for( int i = abs( event->delta() ); i > 0; i -= 8 )
			this->scaleUp();
	}
	else
	{
		/* zoom out */
		//for( int i = abs( event->delta() ); i > 0; i -= 8 )
			this->scaleDown();
	}
}

void SurfaceRenderWidget::scaleUp()
{
	GLMatrix<GLfloat> scale_mat;
	/* zoom in */ scale_mat.loadScale( SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR );
	surface_transform = scale_mat * surface_transform;
	update();
}

void SurfaceRenderWidget::scaleDown()
{
	GLMatrix<GLfloat> scale_mat;
	/* zoom out */ scale_mat.loadScale( 1.0f / SCALE_FACTOR, 1.0f / SCALE_FACTOR, 1.0f / SCALE_FACTOR );
	surface_transform = scale_mat * surface_transform;
	update();
}

#undef SCALE_FACTOR

void SurfaceRenderWidget::setBBoxTransform( const float m[ 16 ] )
{
	for( int i = 0; i < 16; i++ )
		this->bbox_transform[ i ] = m[ i ];
	update();
}

void SurfaceRenderWidget::setSurfaceTransform( const float m[ 16 ] )
{
	for( int i = 0; i < 16; i++ )
		this->surface_transform[ i ] = m[ i ];
	update();
}

void SurfaceRenderWidget::getBBoxTransform( float m[ 16 ] )
{
	for( int i = 0; i < 16; i++ )
		m[ i ] = this->bbox_transform[ i ];
}

void SurfaceRenderWidget::getSurfaceTransform( float m[ 16 ] )
{
	for( int i = 0; i < 16; i++ )
		m[ i ] = this->surface_transform[ i ];
}

QString SurfaceRenderWidget::getFormula()
{
	return STDStringToQString( sm->get_formula() );
}

QMap<QString, float> SurfaceRenderWidget::getFloatParameters()
{
	std::set<std::string> std_param_list = sm->get_parameters();
	QMap<QString, float> pm;
	for( std::set<std::string>::const_iterator it = std_param_list.begin(); it != std_param_list.end(); it++ )
		pm[ STDStringToQString( *it ) ] = sm->get_surface_parameter( *it );
	return pm;
}

SurfaceRenderWidget::ClippingSurface SurfaceRenderWidget::getClippingSurface()
{
	return ( SurfaceRenderWidget::ClippingSurface ) this->sm->get_clipping_surface();
}

bool SurfaceRenderWidget::getShowClippingObject()
{
	return this->show_bounding_object;
}

bool SurfaceRenderWidget::getShowFramesPerSecond()
{
	return this->show_fps;
}


const QMap< ShaderManager::PolyCoeffMethod, QString > SurfaceRenderWidget::getPolyCoeffMethods()
{
	const std::map< glsl::ShaderManager::PolyCoeffMethod, std::string >* polyCoeffMethodsStd = glsl::ShaderManager::get_poly_coeff_methods();
	QMap< glsl::ShaderManager::PolyCoeffMethod, QString > polyCoeffMethods = QMap< glsl::ShaderManager::PolyCoeffMethod, QString >();
	for( std::map< glsl::ShaderManager::PolyCoeffMethod, std::string >::const_iterator it = polyCoeffMethodsStd->begin(); it != polyCoeffMethodsStd->end(); it++ )
		polyCoeffMethods[ it->first ] = STDStringToQString( it->second );
	return polyCoeffMethods;
}

const QMap< ShaderManager::RootFinderMethod, QString > SurfaceRenderWidget::getRootFinderMethods()
{
	const std::map< glsl::ShaderManager::RootFinderMethod, std::string >* rootFinderMethodsStd = glsl::ShaderManager::get_root_finder_methods();
	QMap< glsl::ShaderManager::RootFinderMethod, QString > rootFinderMethods = QMap< glsl::ShaderManager::RootFinderMethod, QString >();
	for( std::map< glsl::ShaderManager::RootFinderMethod, std::string >::const_iterator it = rootFinderMethodsStd->begin(); it != rootFinderMethodsStd->end(); it++ )
		rootFinderMethods[ it->first ] = STDStringToQString( it->second );
	return rootFinderMethods;
}

void SurfaceRenderWidget::setPolyCoeffMethod( glsl::ShaderManager::PolyCoeffMethod pcm )
{
	this->sm->set_poly_coeff_method( pcm );
}

void SurfaceRenderWidget::setRootFinderMethod( glsl::ShaderManager::RootFinderMethod rfm )
{
	this->sm->set_root_finder_method( rfm );
}

namespace
{
	// Copied from Freeglut library
	/*
	 * Draws a wireframed cube. Code contributed by Andreas Umbach <marvin@dataway.ch>
	 */
	void wireCube( GLdouble dSize )
	{
		double size = dSize * 0.5;

	#   define V(a,b,c) glVertex3d( a size, b size, c size );
	#   define N(a,b,c) glNormal3d( a, b, c );

		/* PWO: I dared to convert the code to use macros... */
		glBegin( GL_LINE_LOOP ); N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+); glEnd();
		glBegin( GL_LINE_LOOP ); N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+); glEnd();
		glBegin( GL_LINE_LOOP ); N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+); glEnd();
		glBegin( GL_LINE_LOOP ); N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-); glEnd();
		glBegin( GL_LINE_LOOP ); N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+); glEnd();
		glBegin( GL_LINE_LOOP ); N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-); glEnd();

	#   undef V
	#   undef N
	}

	/*
	 * Draws a solid cube. Code contributed by Andreas Umbach <marvin@dataway.ch>
	 */
	void solidCube( GLdouble dSize )
	{
		double size = dSize * 0.5;

	#   define V(a,b,c) glVertex3d( a size, b size, c size );
	#   define N(a,b,c) glNormal3d( a, b, c );

		/* PWO: Again, I dared to convert the code to use macros... */
		glBegin( GL_QUADS );
			N( 1.0, 0.0, 0.0); V(+,-,+); V(+,-,-); V(+,+,-); V(+,+,+);
			N( 0.0, 1.0, 0.0); V(+,+,+); V(+,+,-); V(-,+,-); V(-,+,+);
			N( 0.0, 0.0, 1.0); V(+,+,+); V(-,+,+); V(-,-,+); V(+,-,+);
			N(-1.0, 0.0, 0.0); V(-,-,+); V(-,+,+); V(-,+,-); V(-,-,-);
			N( 0.0,-1.0, 0.0); V(-,-,+); V(-,-,-); V(+,-,-); V(+,-,+);
			N( 0.0, 0.0,-1.0); V(-,-,-); V(-,+,-); V(+,+,-); V(+,-,-);
		glEnd();

	#   undef V
	#   undef N
	}

	/*
	 * Compute lookup table of cos and sin values forming a cirle
	 *
	 * Notes:
	 *    It is the responsibility of the caller to free these tables
	 *    The size of the table is (n+1) to form a connected loop
	 *    The last entry is exactly the same as the first
	 *    The sign of n can be flipped to get the reverse loop
	 */
	static void circleTable(double **sint,double **cost,const int n)
	{
		int i;

		/* Table size, the sign of n flips the circle direction */

		const int size = abs(n);

		/* Determine the angle between samples */

		const double angle = 2*M_PI/(double)( ( n == 0 ) ? 1 : n );

		/* Allocate memory for n samples, plus duplicate of first entry at the end */

		*sint = (double *) calloc(sizeof(double), size+1);
		*cost = (double *) calloc(sizeof(double), size+1);

		/* Bail out if memory allocation fails, fgError never returns */

		if (!(*sint) || !(*cost))
		{
			free(*sint);
			free(*cost);
		}

		/* Compute cos and sin around the circle */

		(*sint)[0] = 0.0;
		(*cost)[0] = 1.0;

		for (i=1; i<size; i++)
		{
			(*sint)[i] = sin(angle*i);
			(*cost)[i] = cos(angle*i);
		}

		/* Last sample is duplicate of the first */

		(*sint)[size] = (*sint)[0];
		(*cost)[size] = (*cost)[0];
	}

	/*
	 * Draws a solid sphere
	 */
	void solidSphere(GLdouble radius, GLint slices, GLint stacks)
	{
		int i,j;

		/* Adjust z and radius as stacks are drawn. */

		double z0,z1;
		double r0,r1;

		/* Pre-computed circle */

		double *sint1,*cost1;
		double *sint2,*cost2;

		circleTable(&sint1,&cost1,-slices);
		circleTable(&sint2,&cost2,stacks*2);

		/* The top stack is covered with a triangle fan */

		z0 = 1.0;
		z1 = cost2[(stacks>0)?1:0];
		r0 = 0.0;
		r1 = sint2[(stacks>0)?1:0];

		glBegin(GL_TRIANGLE_FAN);

			glNormal3d(0,0,1);
			glVertex3d(0,0,radius);

			for (j=slices; j>=0; j--)
			{
				glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
				glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
			}

		glEnd();

		/* Cover each stack with a quad strip, except the top and bottom stacks */

		for( i=1; i<stacks-1; i++ )
		{
			z0 = z1; z1 = cost2[i+1];
			r0 = r1; r1 = sint2[i+1];

			glBegin(GL_QUAD_STRIP);

				for(j=0; j<=slices; j++)
				{
					glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
					glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
					glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
					glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
				}

			glEnd();
		}

		/* The bottom stack is covered with a triangle fan */

		z0 = z1;
		r0 = r1;

		glBegin(GL_TRIANGLE_FAN);

			glNormal3d(0,0,-1);
			glVertex3d(0,0,-radius);

			for (j=0; j<=slices; j++)
			{
				glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
				glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
			}

		glEnd();

		/* Release sin and cos tables */

		free(sint1);
		free(cost1);
		free(sint2);
		free(cost2);
	}

	/*
	 * Draws a wire sphere
	 */
	void wireSphere(GLdouble radius, GLint slices, GLint stacks)
	{
		int i,j;

		/* Adjust z and radius as stacks and slices are drawn. */

		double r;
		double x,y,z;

		/* Pre-computed circle */

		double *sint1,*cost1;
		double *sint2,*cost2;

		circleTable(&sint1,&cost1,-slices  );
		circleTable(&sint2,&cost2, stacks*2);

		/* Draw a line loop for each stack */

		for (i=1; i<stacks; i++)
		{
			z = cost2[i];
			r = sint2[i];

			glBegin(GL_LINE_LOOP);

				for(j=0; j<=slices; j++)
				{
					x = cost1[j];
					y = sint1[j];

					glNormal3d(x,y,z);
					glVertex3d(x*r*radius,y*r*radius,z*radius);
				}

			glEnd();
		}

		/* Draw a line loop for each slice */

		for (i=0; i<slices; i++)
		{
			glBegin(GL_LINE_STRIP);

				for(j=0; j<=stacks; j++)
				{
					x = cost1[i]*sint2[j];
					y = sint1[i]*sint2[j];
					z = cost2[j];

					glNormal3d(x,y,z);
					glVertex3d(x*radius,y*radius,z*radius);
				}

			glEnd();
		}

		/* Release sin and cos tables */

		free(sint1);
		free(cost1);
		free(sint2);
		free(cost2);
	}
};