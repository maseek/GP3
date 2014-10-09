#include "cWNDManager.h"
#include <ctime>
#include <iostream>
#include <Windows.h>
#include <GL/glut.h>
#include "wglext.h"
#include "windowOGL.h"

cWNDManager* cWNDManager::pInstance = NULL;

typedef HGLRC( APIENTRYP PFNWGLCREATECONTEXTATTRIBSARBPROC )( HDC, HGLRC, const int* );
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

cWNDManager::cWNDManager( )
{
	m_isRunning = false;
	m_winOGL = NULL;
	m_hinstance = NULL;
	m_lastTime = 0;
}

cWNDManager* cWNDManager::getInstance( )
{
	if ( pInstance == NULL )
	{
		pInstance = new cWNDManager( );
	}
	return cWNDManager::pInstance;
}

bool cWNDManager::createWND( int width, int height, int bpp )
{
	DWORD dwExStyle;
	DWORD dwStyle;

	m_windowRect.left = ( long ) 0;
	m_windowRect.right = ( long ) width;
	m_windowRect.top = ( long ) 0;
	m_windowRect.bottom = ( long ) height;

	m_windowClass.cbSize = sizeof( WNDCLASSEX );
	m_windowClass.style = CS_HREDRAW | CS_VREDRAW;
	m_windowClass.lpfnWndProc = ( WNDPROC ) cWNDManager::WndProc;
	m_windowClass.cbClsExtra = 0;
	m_windowClass.cbWndExtra = 0;
	m_windowClass.hInstance = m_hinstance;
	m_windowClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	m_windowClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	m_windowClass.hbrBackground = NULL;
	m_windowClass.lpszMenuName = NULL;
	m_windowClass.lpszClassName = L"winOGL";
	m_windowClass.hIconSm = LoadIcon( NULL, IDI_WINLOGO );

	if ( !RegisterClassEx( &m_windowClass ) )
	{
		return false;
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx( &m_windowRect, dwStyle, false, dwExStyle );

	m_hwnd = CreateWindowEx( NULL, L"winOGL",
		L"OpenGL Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_windowRect.right - m_windowRect.left,
		m_windowRect.bottom - m_windowRect.top,
		NULL,
		NULL,
		m_hinstance,
		this );

	if ( !m_hwnd )
	{
		return 0;
	}

	m_hdc = GetDC( m_hwnd );

	ShowWindow( m_hwnd, SW_SHOW );
	UpdateWindow( m_hwnd );

	m_lastTime = GetTickCount( ) * 0.001f;
	return true;
}

void cWNDManager::destroyWND( )
{
	ShowCursor( true );
}

void cWNDManager::attachOGLWnd( windowOGL* OGLWindow )
{
	m_winOGL = OGLWindow;
}

bool cWNDManager::isWNDRunning( )
{
	return m_isRunning;
}

void cWNDManager::processWNDEvents( )
{
	MSG msg;
	while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}

void cWNDManager::setupPixelFormat( )
{
	int pixelFormat;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof( PIXELFORMATDESCRIPTOR ),
		1,
		PFD_SUPPORT_OPENGL |
		PFD_DRAW_TO_WINDOW |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	pixelFormat = ChoosePixelFormat( m_hdc, &pfd );
	SetPixelFormat( m_hdc, pixelFormat, &pfd );
}

LRESULT CALLBACK cWNDManager::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	cWNDManager* theWindow = NULL;
	switch ( uMsg )
	{
	case WM_CREATE:
	{
		pInstance->m_hdc = GetDC( hWnd );
		pInstance->setupPixelFormat( );

		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 0,
			0 };

		HGLRC tmpContext = wglCreateContext( pInstance->m_hdc );
		wglMakeCurrent( pInstance->m_hdc, tmpContext );

		wglCreateContextAttribsARB = ( PFNWGLCREATECONTEXTATTRIBSARBPROC ) wglGetProcAddress( "wglCreateCotextAttrbsARB" );
		if ( !wglCreateContextAttribsARB )
		{
			std::cerr << "OpenGL 3.0 is not supported, falling back to GL 2.1" << std::endl;
			pInstance->m_hglrc = tmpContext;
		}
		else
		{
			pInstance->m_hglrc = wglCreateContextAttribsARB( pInstance->m_hdc, 0, attribs );
			wglDeleteContext( tmpContext );
		}

		wglMakeCurrent( pInstance->m_hdc, pInstance->m_hglrc );

		pInstance->m_isRunning = true;
	}
		break;
	case WM_DESTROY:
	case WM_CLOSE:
		wglMakeCurrent( pInstance->m_hdc, NULL );
		wglDeleteContext( pInstance->m_hglrc );
		pInstance->m_isRunning = false;
		PostQuitMessage( 0 );
		return 0;
		break;
	case WM_SIZE:
	{
		int height = HIWORD( lParam );
		int width = LOWORD( lParam );
		pInstance->getAttachedWND( )->onResize( width, height );
	}
		break;
	case WM_KEYDOWN:
		if ( wParam == VK_ESCAPE )
		{
			DestroyWindow( pInstance->m_hwnd );
		}
		break;
	default:
		break;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

windowOGL* cWNDManager::getAttachedWND( )
{
	return m_winOGL;
}

float cWNDManager::getElapsedSeconds( )
{
	float currentTime = float( GetTickCount( ) )*0.001f;
	float seconds = float( currentTime - m_lastTime );
	m_lastTime = currentTime;
	return seconds;
}