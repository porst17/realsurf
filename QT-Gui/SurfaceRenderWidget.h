#ifndef __SURFACERENDERWIDGET_H__
#define __SURFACERENDERWIDGET_H__

#include <QtGui>
#include <GL/GLee.h>
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QSet>
#include <QTime>
#include <QTimer>
#include <MathGL/GLMatrix.h>
#include "LightProperties.h"
#include "MaterialProperties.h"
#include "../RealSurf/ShaderManager.h"

class SurfaceRenderWidget : public QGLWidget
{
	Q_OBJECT

	public:
		enum ClippingSurface { SPHERE = glsl::ShaderManager::SPHERE, CUBE = glsl::ShaderManager::CUBE };

	private:
		glsl::ShaderManager *sm;
		GLMatrix<GLfloat> bbox_transform;
		GLMatrix<GLfloat> surface_transform;
		QPoint last_point;
		MaterialProperties front_material;
		MaterialProperties back_material;
		bool show_bounding_object;
		bool show_fps;
		bool process_input_events;
		bool feedback_enabled;
		int framecount;
		QTime time;
		QTimer *fps_timer;
		float fps;
		GLuint texture;

	public:
		SurfaceRenderWidget( QWidget *parent = 0 );
		virtual ~SurfaceRenderWidget();
		virtual QSize minimumSizeHint () const;
		void getBBoxTransform( float m[ 16 ] );
		void getSurfaceTransform( float m[ 16 ] );
		QString getFormula();
		QMap<QString, float> getFloatParameters();
		ClippingSurface getClippingSurface();
		bool getShowClippingObject();
		bool getShowFramesPerSecond();
		static const QMap< glsl::ShaderManager::PolyCoeffMethod, QString > getPolyCoeffMethods();
		static const QMap< glsl::ShaderManager::RootFinderMethod, QString > getRootFinderMethods();

	public slots:
		void setBackgroundColor( const QColor &bg_color );
		void setFrontMaterial( const MaterialProperties &mp );
		void setBackMaterial( const MaterialProperties &mp );
		void setLight0( const LightProperties &lp );
		void setLight1( const LightProperties &lp );
		void setLight2( const LightProperties &lp );
		void setLight3( const LightProperties &lp );
		void setLight4( const LightProperties &lp );
		void setLight5( const LightProperties &lp );
		void setLight6( const LightProperties &lp );
		void setLight7( const LightProperties &lp );
		void toggleLight0( bool on, const LightProperties &lp );
		void toggleLight1( bool on, const LightProperties &lp );
		void toggleLight2( bool on, const LightProperties &lp );
		void toggleLight3( bool on, const LightProperties &lp );
		void toggleLight4( bool on, const LightProperties &lp );
		void toggleLight5( bool on, const LightProperties &lp );
		void toggleLight6( bool on, const LightProperties &lp );
		void toggleLight7( bool on, const LightProperties &lp );
		void setFormula( const QString &formula );
		void setFloatParameter( const QString &name, double value );
		void clipToSphere();
		void clipToCube();
		void setClippingSurface( ClippingSurface cs );
		void showClippingObject( bool show );
		void showFramesPerSecond( bool show );
		void reload();
		void resetTransformation();
		void setTransparencyEnabled( bool on );
		void scaleUp();
		void scaleDown();
		void setBBoxTransform( const float m[ 16 ] );
		void setSurfaceTransform( const float m[ 16 ] );
		void setPolyCoeffMethod( glsl::ShaderManager::PolyCoeffMethod pcm );
		void setRootFinderMethod( glsl::ShaderManager::RootFinderMethod rfm );
		void processInputEvents( bool process );
		void setFeedbackEnabled( bool enabled );

	private slots:
		void calculateFramesPerSecond();

	private:
		void setLight( GLenum light, const LightProperties &lp );
		void setMaterial( GLenum face, const MaterialProperties &mp );
		void toggleLight( GLenum light, bool on, const LightProperties &lp );
		void callParsingComplete( glsl::ShaderManager::ErrorCode error_code );

	signals:
		void parsingComplete( bool success, const QString &summary, const QString &text );
		void newParameters( const QSet< QString > &param_list );

	protected:
		virtual void initializeGL();
		virtual void resizeGL( int w, int h );
		virtual void paintGL();

		virtual void mousePressEvent( QMouseEvent *event );
		virtual void mouseMoveEvent( QMouseEvent *event );
		virtual void wheelEvent( QWheelEvent *event );
};

#endif