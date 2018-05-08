
// EntornVGI.h: File of main header to EntornVGI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // Principal Symbols


// CEntornVGIApp:
// Consulte la sección EntornVGI.cpp para obtener información sobre la implementación de esta clase
//

class CEntornVGIApp : public CWinAppEx
{
public:
	CEntornVGIApp();


// Reemplazos
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementación
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEntornVGIApp theApp;
