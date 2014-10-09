#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "cWNDManager.h"
#include "cTriangle.h"


int WINAPI WinMain( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR cmdLine,
	int cmdShow )
{

	//Set our window settings
	const int windowWidth = 1024;
	const int windowHeight = 768;
	const int windowBPP = 16;



	//This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance( );

	//The example OpenGL code
	windowOGL theOGLWnd;

	//Attach our example to our window
	pgmWNDMgr->attachOGLWnd( &theOGLWnd );

	// Triangle
	cTriangle myTriangle;

	//Attempt to create the window
	if ( !pgmWNDMgr->createWND( windowWidth, windowHeight, windowBPP ) )
	{
		MessageBox( NULL, L"Unable to create the OpenGL Window", L"An error occurred", MB_ICONERROR | MB_OK );
		pgmWNDMgr->destroyWND( );
		return 1;
	}


	//This is the mainloop, we render frames until isRunning returns false
	while ( pgmWNDMgr->isWNDRunning( ) )
	{
		pgmWNDMgr->processWNDEvents( );
		float elapsedTime = pgmWNDMgr->getElapsedSeconds( );
		myTriangle.prepareTri( elapsedTime );
		theOGLWnd.renderOGL( myTriangle.getRotAngle( ) );
		pgmWNDMgr->swapBuffers( );
	}

	theOGLWnd.shutdown( );//Free any resources
	pgmWNDMgr->destroyWND( );//Destroy the program window

	return 0; //Return success
}
