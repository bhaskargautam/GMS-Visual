
// ChildFrm.h: CChildFrame class interface
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Atributos
public:

// Operaciones
public:

// Reemplazos
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementaci�n
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Funciones de asignaci�n de mensajes generadas
protected:
	DECLARE_MESSAGE_MAP()
};