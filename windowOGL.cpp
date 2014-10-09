#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>
#include "windowOGL.h"

windowOGL::windowOGL( )
{
}

bool windowOGL::initOGL( )
{
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	return true;
}

void windowOGL::renderOGL( float rotAngle )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity( );
	glRotatef( rotAngle, 0, 0, 1 );
	glBegin( GL_TRIANGLES );
	glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
	glVertex3f( -1.0f, -0.5f, -4.0f );
	glColor4f( 0.0f, 1.0f, 0.0f, 1.0f );
	glVertex3f( 1.0f, -0.5f, -4.0f );
	glColor4f( 0.0f, 0.0f, 1.0f, 1.0f );
	glVertex3f( 0.0f, 0.5f, -4.0f );
	glEnd( );
}
void windowOGL::shutdown( )
{
}
void windowOGL::onResize( int width, int height )
{
	glViewport( 0, 0, width, height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluPerspective( 45.0f, float( width ) / float( height ), 1.f, 100.f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
}