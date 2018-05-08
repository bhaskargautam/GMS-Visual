// stdafx.h: file to add the system standard include files,
// or specific files  of the project used frequently,
// but rarely changed

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // To exclude material rarely used of Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // Some CString constructors will, be explicits

// Desactiva la ocultación de MFC para algunos mensajes de advertencia comunes y, muchas veces, omitidos de forma consciente
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // Componentes principales y estándar de MFC
#include <afxext.h>         // Extensiones de MFC

#include <afxdisp.h>        // Clases de automatización de MFC

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Compatibilidad MFC para controles comunes de Internet Explorer 4
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Compatibilidad MFC para controles comunes de Windows
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // Compatibilidad de MFC con cintas y barras de control

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

/* ------------------------------------------------------------------------- */
/*                                INCLUDES                                   */
/* ------------------------------------------------------------------------- */

// GMS: Shaders GLSL includes
#define GLEW_STATIC
#define GLEW_NO_GLU	// Sense usar llibreria GLU
#include <gl/glew.h>
#include <gl/wglew.h>
#pragma comment(lib, "../lib/glew32s.lib")	// Link against GLEW library 

// GMS: OpenGL libreries
//#include <gl/gl.h>
#pragma comment(lib, "../lib/opengl32.lib")	// Link against GL library  
//#include <gl/glaux.h>
//#include <gl/glu.h>
//#pragma comment(lib, "../lib/glu32.lib")	// Link against GLU library 
//#include <gl/glut.h>

#include "glut_geometry.h"			// Fonts dels Objectes de la llibreria freeglut

// GMS: DevIL/OpenIL libraries to read image file formats 
//     (BMP,JPG,TIF,GIF,etc.) in loadIMA function (visualitzacio.cpp)
//#include <il\il.h>
//#include <il\ilu.h>
#include <il\ilut.h>

// GMS: Includes to read files, trigonometric functions and random numbers.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// GMS: Constants values of the application EntornVGI
#include "constants.h"

// Unable in compilation listing the warning C4244: 'argumento': converrsion from 'double' to 'GLfloat'; perhaps data losing
#  pragma warning (disable:4244)  /* Disable bogus VC++ 4.2 conversion warnings. */
#  pragma warning (disable:4305)  /* VC++ 5.0 version of above warning. */

// GMS: GLSL libraries
//#pragma comment( lib, "../lib/glew32.lib" )		// Search For glew32.lib While Linking
//#pragma comment( lib, "../lib/glew32mx.lib" )		// Search For glew32mx.lib While Linking
//#pragma comment( lib, "../lib/glew32mxs.lib" )	// Search For glew32mxs.lib While Linking
//#pragma comment( lib, "../lib/glew32s.lib" )		// Search For glew32s.lib While Linking

