//******** PRACTICAL WORK GRAPHICAL & MULTIMEDIA SYSTEMS (Engineering School - UAB)
//******** Basic Environment VS2013 MULTIWINDOW with MFC interface and Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (February 2018)
// EntornVGIView.cpp: implementation of class CEntornVGIView
// FUNTIONS:		- Main loop control (OnPaint)
//					- Keyboard c (OnKeyDown)
//					- Interactive mouse and mouse buttons 
//							(OnLButtomDown, OnRButtomDown, OnMouseMove)
//					- Menu options (On*, OnUpdate*)
//					- Background color control by means of keyboard (FONS)
//					- Interactive geometrical transforms with mouse
//
//    Version 2.0:	- To change object color by means of keyboard (OBJ)
//					- To fix geometrical transforms non-activing Transforma options (INSERT inside Transform options)
//					- To delete fixed Geometrical Transforms (DELETE inside Transform options)
//					- To fix screen traslation outside Vista->Pan? option (INSERT inside opció View->Pan? option)
//					- To delete fixed screen traslation (DELETE inside opció View->Pan? option)
//
//

#include "stdafx.h"

// Se pueden definir SHARED_HANDLERS en un proyecto ATL implementando controladores de vista previa, miniatura
// y filtro de búsqueda, y permiten compartir código de documentos con ese proyecto.
#ifndef SHARED_HANDLERS
#include "EntornVGI.h"
#endif

#include "EntornVGIDoc.h"
#include "EntornVGIView.h"
#include "visualitzacio.h"	// Include funcions de projeció i il.luminació
#include "escena.h"			// Include funcions d'objectes OpenGL

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntornVGIView

IMPLEMENT_DYNCREATE(CEntornVGIView, CView)

BEGIN_MESSAGE_MAP(CEntornVGIView, CView)
	// Comandos de impresión estándar
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEntornVGIView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_ARXIU_OBRIR_FRACTAL, &CEntornVGIView::OnArxiuObrirFractal)
	ON_COMMAND(ID_ARXIU_OBRIR_FITXER_OBJ, &CEntornVGIView::OnArxiuObrirFitxerObj)
	ON_COMMAND(ID_ARXIU_OBRIR_FITXER_3DS, &CEntornVGIView::OnArxiuObrirFitxer3ds)
	ON_COMMAND(ID_VISTA_Mobil, &CEntornVGIView::OnVistaMobil)
	ON_UPDATE_COMMAND_UI(ID_VISTA_Mobil, &CEntornVGIView::OnUpdateVistaMobil)
	ON_COMMAND(ID_VISTA_ZOOM, &CEntornVGIView::OnVistaZoom)
	ON_UPDATE_COMMAND_UI(ID_VISTA_ZOOM, &CEntornVGIView::OnUpdateVistaZoom)
	ON_COMMAND(ID_VISTA_POLARS_X, &CEntornVGIView::OnVistaPolarsX)
	ON_UPDATE_COMMAND_UI(ID_VISTA_POLARS_X, &CEntornVGIView::OnUpdateVistaPolarsX)
	ON_COMMAND(ID_VISTA_POLARS_Y, &CEntornVGIView::OnVistaPolarsY)
	ON_UPDATE_COMMAND_UI(ID_VISTA_POLARS_Y, &CEntornVGIView::OnUpdateVistaPolarsY)
	ON_COMMAND(ID_VISTA_POLARS_Z, &CEntornVGIView::OnVistaPolarsZ)
	ON_UPDATE_COMMAND_UI(ID_VISTA_POLARS_Z, &CEntornVGIView::OnUpdateVistaPolarsZ)
	ON_COMMAND(ID_VISTA_PAN, &CEntornVGIView::OnVistaPan)
	ON_UPDATE_COMMAND_UI(ID_VISTA_PAN, &CEntornVGIView::OnUpdateVistaPan)
	ON_COMMAND(ID_VISTA_ORIGENPAN, &CEntornVGIView::OnVistaOrigenpan)
	ON_COMMAND(ID_VISTA_NAVEGA, &CEntornVGIView::OnVistaNavega)
	ON_UPDATE_COMMAND_UI(ID_VISTA_NAVEGA, &CEntornVGIView::OnUpdateVistaNavega)
	ON_COMMAND(ID_VISTA_ORIGENNAVEGA, &CEntornVGIView::OnVistaOrigennavega)
	ON_COMMAND(ID_VISTA_EIXOS, &CEntornVGIView::OnVistaEixos)
	ON_UPDATE_COMMAND_UI(ID_VISTA_EIXOS, &CEntornVGIView::OnUpdateVistaEixos)
	ON_COMMAND(ID_PROJECCIO_PERSPECTIVA, &CEntornVGIView::OnProjeccioPerspectiva)
	ON_UPDATE_COMMAND_UI(ID_PROJECCIO_PERSPECTIVA, &CEntornVGIView::OnUpdateProjeccioPerspectiva)
	ON_COMMAND(ID_PROJECCIO_ORTHO, &CEntornVGIView::OnProjeccioOrthogonal)
	ON_UPDATE_COMMAND_UI(ID_PROJECCIO_ORTHO, &CEntornVGIView::OnUpdateProjeccioOrthogonal)
	ON_COMMAND(ID_PROJECCIO_AXONOMETRIC, &CEntornVGIView::OnProjeccioAxonometric)
	ON_UPDATE_COMMAND_UI(ID_PROJECCIO_AXONOMETRIC, &CEntornVGIView::OnUpdateProjeccioAxonometric)
	ON_COMMAND(ID_OBJECTE_CUB, &CEntornVGIView::OnObjecteCub)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_CUB, &CEntornVGIView::OnUpdateObjecteCub)
	ON_COMMAND(ID_OBJECTE_ESFERA, &CEntornVGIView::OnObjecteEsfera)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_ESFERA, &CEntornVGIView::OnUpdateObjecteEsfera)
	ON_COMMAND(ID_OBJECTE_TETERA, &CEntornVGIView::OnObjecteTetera)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_TETERA, &CEntornVGIView::OnUpdateObjecteTetera)
	ON_COMMAND(ID_OBJECTE_TRUCK, &CEntornVGIView::OnObjecteTruck)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_TRUCK, &CEntornVGIView::OnUpdateObjecteTruck)
	ON_COMMAND(ID_OBJECTE_ROBOT, &CEntornVGIView::OnObjecteRobot)
	ON_UPDATE_COMMAND_UI(ID_OBJECTE_ROBOT, &CEntornVGIView::OnUpdateObjecteRobot)
	ON_COMMAND(ID_TRANSFORMA_TRASLACIO, &CEntornVGIView::OnTransformaTraslacio)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_TRASLACIO, &CEntornVGIView::OnUpdateTransformaTraslacio)
	ON_COMMAND(ID_TRANSFORMA_ORIGENTRASLACIO, &CEntornVGIView::OnTransformaOrigentraslacio)
	ON_COMMAND(ID_TRANSFORMA_ROTACIO, &CEntornVGIView::OnTransformaRotacio)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_ROTACIO, &CEntornVGIView::OnUpdateTransformaRotacio)
	ON_COMMAND(ID_TRANSFORMA_ORIGENROTACIO, &CEntornVGIView::OnTransformaOrigenrotacio)
	ON_COMMAND(ID_TRANSFORMA_ESCALAT, &CEntornVGIView::OnTransformaEscalat)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_ESCALAT, &CEntornVGIView::OnUpdateTransformaEscalat)
	ON_COMMAND(ID_TRANSFORMA_ORIGENESCALAT, &CEntornVGIView::OnTransformaOrigenescalat)
	ON_COMMAND(ID_TRANSFORMA_MOBILX, &CEntornVGIView::OnTransformaMobilx)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_MOBILX, &CEntornVGIView::OnUpdateTransformaMobilx)
	ON_COMMAND(ID_TRANSFORMA_MOBILY, &CEntornVGIView::OnTransformaMobily)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_MOBILY, &CEntornVGIView::OnUpdateTransformaMobily)
	ON_COMMAND(ID_TRANSFORMA_MOBILZ, &CEntornVGIView::OnTransformaMobilz)
	ON_UPDATE_COMMAND_UI(ID_TRANSFORMA_MOBILZ, &CEntornVGIView::OnUpdateTransformaMobilz)
	ON_COMMAND(ID_OCULTACIONS_TESTVIS, &CEntornVGIView::OnOcultacionsTestvis)
	ON_UPDATE_COMMAND_UI(ID_OCULTACIONS_TESTVIS, &CEntornVGIView::OnUpdateOcultacionsTestvis)
	ON_COMMAND(ID_OCULTACIONS_ZBUFFER, &CEntornVGIView::OnOcultacionsZbuffer)
	ON_UPDATE_COMMAND_UI(ID_OCULTACIONS_ZBUFFER, &CEntornVGIView::OnUpdateOcultacionsZbuffer)
	ON_COMMAND(ID_OCULTACIONS_BACKLINE, &CEntornVGIView::OnOcultacionsBackline)
	ON_UPDATE_COMMAND_UI(ID_OCULTACIONS_BACKLINE, &CEntornVGIView::OnUpdateOcultacionsBackline)
	ON_COMMAND(ID_ILUMINACIO_LLUMFIXE, &CEntornVGIView::OnIluminacioLlumfixe)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_LLUMFIXE, &CEntornVGIView::OnUpdateIluminacioLlumfixe)
	ON_COMMAND(ID_ILUMINACIO_FILFERROS, &CEntornVGIView::OnIluminacioFilferros)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_FILFERROS, &CEntornVGIView::OnUpdateIluminacioFilferros)
	ON_COMMAND(ID_ILUMINACIO_PLANA, &CEntornVGIView::OnIluminacioPlana)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_PLANA, &CEntornVGIView::OnUpdateIluminacioPlana)
	ON_COMMAND(ID_ILUMINACIO_GOURAUD, &CEntornVGIView::OnIluminacioGouraud)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_GOURAUD, &CEntornVGIView::OnUpdateIluminacioGouraud)
	ON_COMMAND(ID_MATERIAL_EMISSIO, &CEntornVGIView::OnMaterialEmissio)
	ON_UPDATE_COMMAND_UI(ID_MATERIAL_EMISSIO, &CEntornVGIView::OnUpdateMaterialEmissio)
	ON_COMMAND(ID_MATERIAL_AMBIENT, &CEntornVGIView::OnMaterialAmbient)
	ON_UPDATE_COMMAND_UI(ID_MATERIAL_AMBIENT, &CEntornVGIView::OnUpdateMaterialAmbient)
	ON_COMMAND(ID_MATERIAL_DIFUSA, &CEntornVGIView::OnMaterialDifusa)
	ON_UPDATE_COMMAND_UI(ID_MATERIAL_DIFUSA, &CEntornVGIView::OnUpdateMaterialDifusa)
	ON_COMMAND(ID_MATERIAL_ESPECULAR, &CEntornVGIView::OnMaterialEspecular)
	ON_UPDATE_COMMAND_UI(ID_MATERIAL_ESPECULAR, &CEntornVGIView::OnUpdateMaterialEspecular)
	ON_COMMAND(ID_ILUMINACIO_TEXTURES, &CEntornVGIView::OnIluminacioTextures)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_TEXTURES, &CEntornVGIView::OnUpdateIluminacioTextures)
	ON_COMMAND(ID_ILUMINACIO_TEXTURA_FITXERIMATGE, &CEntornVGIView::OnIluminacioTexturaFitxerimatge)
	ON_UPDATE_COMMAND_UI(ID_ILUMINACIO_TEXTURA_FITXERIMATGE, &CEntornVGIView::OnUpdateIluminacioTexturaFitxerimatge)
	ON_COMMAND(ID_LLUMS_LLUMAMBIENT, &CEntornVGIView::OnLlumsLlumambient)
	ON_UPDATE_COMMAND_UI(ID_LLUMS_LLUMAMBIENT, &CEntornVGIView::OnUpdateLlumsLlumambient)
	ON_COMMAND(ID_LLUMS_LLUM0, &CEntornVGIView::OnLlumsLlum0)
	ON_UPDATE_COMMAND_UI(ID_LLUMS_LLUM0, &CEntornVGIView::OnUpdateLlumsLlum0)
	ON_COMMAND(ID_SHADERS_SENSE, &CEntornVGIView::OnShadersSense)
	ON_UPDATE_COMMAND_UI(ID_SHADERS_SENSE, &CEntornVGIView::OnUpdateShadersSense)
	ON_COMMAND(ID_SHADERS_GOURAUD, &CEntornVGIView::OnShadersGouraud)
	ON_UPDATE_COMMAND_UI(ID_SHADERS_GOURAUD, &CEntornVGIView::OnUpdateShadersGouraud)
	ON_COMMAND(ID_SHADERS_PHONG, &CEntornVGIView::OnShadersPhong)
	ON_UPDATE_COMMAND_UI(ID_SHADERS_PHONG, &CEntornVGIView::OnUpdateShadersPhong)
	ON_COMMAND(ID_VISTA_SATELIT, &CEntornVGIView::OnVistaSatelit)
	ON_UPDATE_COMMAND_UI(ID_VISTA_SATELIT, &CEntornVGIView::OnUpdateVistaSatelit)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Construcción o destrucción de CEntornVGIView

CEntornVGIView::CEntornVGIView()
{
// TODO: agregar aquí el código de construcción
//	int i = 0;

//------GMS Environment: Inicialization of global variables of CEntornVGIView

// GMS Environment: Variables de control per Menú Veure->Pantalla Completa 
	fullscreen = false;

// GMS Environment: Control variables of pop up View: Interactive Point of View, Zoom, Pan, Walk, and to draw World Coordinate Axes 
	mobil = true;	zzoom = true;	satelit = false;	pan = false;	navega = false;		eixos = true;

// GMS Environment: Variables of the pop up option View->Pan
	fact_pan = 1;
	tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;		tr_cpvF.x = 0;	tr_cpvF.y = 0;	tr_cpvF.z = 0;

// GMS Environment: Variables de control de l'opció View->Walk?
	n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
	opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
	angleZ = 0.0;

// GMS Environment: Control variables de control to pop ups Projection, Object
	projeccio = CAP;			objecte = CAP;

// GMS Environment:  Control variables of pop up Transform
	transf = false;		trasl = false;		rota = false;		escal = false;
	fact_Tras = 1;		fact_Rota = 90;
	TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0;	TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0;
	TG.VRota.x = 0;		TG.VRota.y = 0;		TG.VRota.z = 0;	TGF.VRota.x = 0;	TGF.VRota.y = 0;	TGF.VRota.z = 0;
	TG.VScal.x = 1;		TG.VScal.y = 1;		TG.VScal.z = 1;	TGF.VScal.x = 1;	TGF.VScal.y = 1;	TGF.VScal.z = 1;

	transX = false;	transY = false;	transZ = false;

// GMS Environment:  Control variables of pop up Hidden
	oculta = false;			test_vis = false;			back_line = false;

// GMS Environment: Variables de control del menú Iluminació		
	ilumina = FILFERROS;			ifixe = false;
// Reflexions switched on: Ambient [1], Diffuse [2] and Specular [3]. Switched off: Emission [0]. 
	sw_material[0] = false;			sw_material[1] = true;			sw_material[2] = true;			sw_material[3] = true;
	sw_material_old[0] = false;		sw_material_old[1] = true;		sw_material_old[2] = true;		sw_material_old[3] = true;
	textura = false;				t_textura = CAP;				textura_map = true;

// GMS Environment:  Control variables of pop up Lights
	llum_ambient = true;
	llumGL.encesa = true;
	llumGL.difusa[0] = 1.0f;		llumGL.difusa[1] = 1.0f;		llumGL.difusa[2] = 1.0f;		llumGL.difusa[3] = 1.0f;
	llumGL.especular[0] = 1.0f;		llumGL.especular[1] = 1.0f;		llumGL.especular[2] = 1.0f;		llumGL.especular[3] = 1.0f;

	llumGL.posicio.R = 75.0;		llumGL.posicio.alfa = 90.0;		llumGL.posicio.beta = 0.0;		// Posició llum (x,y,z)=(0,0,75)
	llumGL.atenuacio.a = 0.0;		llumGL.atenuacio.b = 0.0;		llumGL.atenuacio.c = 1.0;		// Llum sense atenuació per distància (a,b,c)=(0,0,1)
	llumGL.restringida = false;
	llumGL.spotdirection[0] = 0.0;	llumGL.spotdirection[1] = 0.0;	llumGL.spotdirection[2] = 0.0;
	llumGL.cutoff = 0.0;			llumGL.exponent = 0.0;

// GMS Environment:  Control variables of pop up Shaders
	sw_shader = false;				shader_menu = CAP;				shader_program = 0;

// GMS Environment:  VControl variables of the mouse buttons
	m_PosEAvall = (0, 0);		m_PosDAvall = (0, 0);
	m_ButoEAvall = false;		m_ButoDAvall = false;
	m_EsfeEAvall.R = 0.0;		m_EsfeEAvall.alfa = 0.0;	m_EsfeEAvall.beta = 0.0;
	m_EsfeIncEAvall.R = 0.0;	m_EsfeIncEAvall.alfa = 0.0;	m_EsfeIncEAvall.beta = 0.0;

// GMS Environment:  Variables controlling visualization parameters: Size of windows and Point of View
	w = 0;			h = 0;									// Windows size
	OPV.R = 15.0;		OPV.alfa = 0.0;		OPV.beta = 0.0;	// Spherical Point of View Coordinates
	Vis_Polar = POLARZ;

// GMS Environment:  Background and object color
	fonsR = true;		fonsG = true;		fonsB = true;
	c_fons.r = 0.0;		c_fons.g = 0.0;		c_fons.b = 0.0;
	sw_color = false;
	col_obj.r = 1.0;	col_obj.g = 1.0;	col_obj.b = 1.0;		col_obj.a = 1.0;

// GMS Environment:  3DS object
	Ob3DS = NULL;

// GMS Environment:  OBJ object
	ObOBJ = NULL;

// GMS Environment:  Timer variables
	t = 0;			anima = false;

// GMS Environment:  Variables of the FRACTAL object
	t_fractal = CAP;	soroll = 'C';
	pas = 64;			pas_ini = 64;
	sw_il = true;		palcolFractal = false;

// GMS Environment:  Other variabless
	mida = 1.0;			nom = "";		buffer = "";

// GMS Environment:  Inicialization of DevIL/OpenIL libraries to load image textures
	ilInit();					// Inicialize IL library
	iluInit();					// Inicialize ILU library
	ilutRenderer(ILUT_OPENGL);	// Inicialize ILUT library for OpenGL

}

CEntornVGIView::~CEntornVGIView()
{
}

BOOL CEntornVGIView::PreCreateWindow(CREATESTRUCT& cs)
{
// TODO: modificar aquí la clase Window o los estilos cambiando
//  CREATESTRUCT cs

// An OpenGL window must be created with the following flags and must not
// include CS_PARENTDC for the class style.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// Impresión de CEntornVGIView

void CEntornVGIView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEntornVGIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Preparación predeterminada
	return DoPreparePrinting(pInfo);
}

void CEntornVGIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
// TODO: agregar inicialización adicional antes de imprimir
}

void CEntornVGIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
// TODO: agregar limpieza después de imprimir
}

void CEntornVGIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// Diagnósticos de CEntornVGIView

#ifdef _DEBUG
void CEntornVGIView::AssertValid() const
{
	CView::AssertValid();
}

void CEntornVGIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEntornVGIDoc* CEntornVGIView::GetDocument() const // La versión de no depuración está alineada
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEntornVGIDoc)));
	return (CEntornVGIDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// Controladores de mensaje de CEntornVGIView

int CEntornVGIView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

// TODO:  Agregue aquí su código de creación especializado

	//CDC* pDC = GetDC();
	//m_glRenderer.CreateGLContext(pDC);
	m_pDC = new CClientDC(this);

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

// GMS Environment: Pixel format that better adjusts to pfd description
	//int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	int nPixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	if (!nPixelFormat)
	{	::MessageBoxW(NULL, L"Error en SetPixelFormat", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);		// This sends a message telling the program to quit
		return false;
	}

// To enable the pixel format to device context
	//BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);
	BOOL bResult = SetPixelFormat(m_pDC->GetSafeHdc(), nPixelFormat, &pfd);

	if (!bResult) 
	{	::MessageBoxW(NULL, L"Error en SetPixelFormat", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);		// This sends a message telling the program to quit
		return false;
	}

// GMS Environment:  OpenGL context creation
	// m_hRC=wglCreateContext(m_hDC);
	m_hRC = wglCreateContext(m_pDC->GetSafeHdc());
	if (!m_hRC)
	{	::MessageBoxW(NULL, L"Error en GL Rendering Context", L"Error", MB_OK | MB_ICONERROR);
		PostQuitMessage(0);		// This sends a message telling the program to quit
		return false;
	}

// OPENGL 2.0 - OPENGL 3.0 **********************************************************
	// --- OpenGL 3.x ---
	HGLRC tempContext = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), tempContext);

//Get access to modern OpenGL functionality from this old style context.
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(_T("GLEW could not be initialized!"));
		return FALSE;
	}

  //  InitAPI();

//	if (glewIsExtensionSupported("GLEW_ARB_vertex_shader"))
//	AfxMessageBox(_T("INFO: GLEW_ARB_vertex_shader supported, proceeding\n"));
//	else AfxMessageBox(_T("INFO: GLEW_ARB_vertex_shader NOT supported, proceeding\n"));
//
//	if (glewIsExtensionSupported("GLEW_ARB_fragment_shader"))
//	AfxMessageBox(_T("INFO: GLEW_ARB_fragment_shader supported, proceeding\n"));
//	else AfxMessageBox(_T("INFO: GLEW_ARB_fragment_shader NOT supported, proceeding\n"));
//
	int major, minor;
	GetGLVersion(&major, &minor);
/* Check for Opengl support.
	if (glewIsSupported("GL_VERSION_2_0")) //(GLEW_VERSION_2_0)
	{
		AfxMessageBox(_T("INFO: OpenGL 2.0 supported!. Proceed\n"));
	}
	else
	{
		AfxMessageBox(_T("INFO: OpenGL 2.0 not supported!. Exit\n"));
		return EXIT_FAILURE;
	}

	if (major < 3 || (major == 3 && minor < 2))
	{
		major = major + major - major;
		AfxMessageBox(_T("OpenGL 3.2 is not supported!. Exit\n"));
	}
	else
	{
		AfxMessageBox(_T("OpenGL 3.2 is supported!. Proceed"));
	}*/
	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,// | WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, //0x9126 , 0x00000001,
		0
	};

	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	if (wglCreateContextAttribsARB != NULL)
	{
		m_hrc = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, attribs);
	}

	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hrc);

// GMS Environment: Inicialització de funcions de shaders per a OpenGL 2.0
	InitAPI();

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempContext);

	if (!m_hrc)
	{
		AfxMessageBox(_T("OpenGL 3.x RC was not created!"));
		return false;
	}

// Permet la coexistencia d'altres contextes de generació
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

	return true;
}

void CEntornVGIView::InitAPI()
{
/*	Vendor, Renderer, Version, Shading Laguage Version i Extensions suportades per la placa gràfica gravades en fitxer extensions.txt
	CString nomf;
	nomf = "extensions.txt";
	char *nomfitxer = CString2Char(nomf);
	char* str = (char*)glGetString(GL_VENDOR);
	FILE* f = fopen(nomfitxer, "w");
	if(f)	{	fprintf(f,"VENDOR: %s\n",str);
				str = (char*)glGetString(GL_RENDERER);
				fprintf(f, "RENDERER: %s\n", str);
				str = (char*)glGetString(GL_VERSION);
				fprintf(f, "VERSION: %s\n", str);
				str = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
				fprintf(f, "SHADING_LANGUAGE_VERSION: %s\n", str);
				str = (char*)glGetString(GL_EXTENSIONS);
				fprintf(f, "EXTENSIONS: %s\n", str);
				fclose(f);
			}
	*/

// Program
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
	glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
	glUniform3iv = (PFNGLUNIFORM3IVPROC)wglGetProcAddress("glUniform3iv");
	glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");
	glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
	glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)wglGetProcAddress("glVertexAttrib1fv");
	glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)wglGetProcAddress("glVertexAttrib2fv");
	glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress("glVertexAttrib4fv");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");

// Shader
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");

// VBO
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
}

void CEntornVGIView::GetGLVersion(int* major, int* minor)
{
	// for all versions
	char* ver = (char*)glGetString(GL_VERSION); // ver = "3.2.0"

	*major = ver[0] - '0';
	if (*major >= 3)
	{
		// for GL 3.x
		glGetIntegerv(GL_MAJOR_VERSION, major);		// major = 3
		glGetIntegerv(GL_MINOR_VERSION, minor);		// minor = 2
	}
	else
	{
		*minor = ver[2] - '0';
	}

	// GLSL
	ver = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);	// 1.50 NVIDIA via Cg compiler
}


void CEntornVGIView::OnDestroy()
{
	CView::OnDestroy();

// TODO: Add your message handler code here and/or call default
	CDC* pDC = GetDC();
	ReleaseDC(pDC);
}


void CEntornVGIView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

// TODO: Add your message handler code here and/or call default

// A resize event occured; cx and cy are the window's new width and height.
// Find the OpenGL change size function given in the Lab 1 notes and call it here

// GMS Environment:  MODIFICATION -> We establish the size of current window
	w = cx;
	h = cy;

}


void CEntornVGIView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	//m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


/////////////////////////////////////////////////////////////////////////////
// Dibujo de CEntornVGIView

void CEntornVGIView::OnDraw(CDC* /*pDC*/)
{

// TODO: Add your draw code for native data

	CEntornVGIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


void CEntornVGIView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

// TODO: Add your message handler code here and/or call default
	GLfloat vpv[3] = { 0.0, 0.0, 1.0 };

// Switch on OpenGL context (from this point you accept OpenGL commands)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

// We call the functions to construct the views (projection, point of view and objects) according the
// type of projection, controlled by the projeccio variable
	switch (projeccio)
	{
	case AXONOM:
// AXONOMETRIC VIEW
// Switch on Scissor Test
		glEnable(GL_SCISSOR_TEST);

// Scissor
		glScissor(0, 0, w, h);
		glViewport(0, 0, w, h);

// Here we do the projective calls for Axonometric View: BEGIN -------------------------
		Projeccio_Orto(0, 0, w, h);
		Vista_Esferica(OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
			oculta, test_vis, back_line, ilumina, llum_ambient, llumGL, textura,
			textura_map, ifixe, eixos);
// Here we do the projective calls for Axonometric View: END -------------------------

// Draw the object or scene
		glPushMatrix();
			configura_Escena();     // To apply Geometrical Transforms according Transform pop up and to configure scene objects
			dibuixa_Escena();		// Draw scene geometry using OpenGL commands.
		glPopMatrix();

// Swap OpenGL buffer --> Screen Buffer
		SwapBuffers(m_pDC->GetSafeHdc());
		break;

	case ORTO:
// ORTOGRAPHIC VIEW
		
// Switch on Scissor Test
		glEnable(GL_SCISSOR_TEST);

// Scissor
		glScissor(0, 0, w, h);
		glViewport(0, 0, w, h);
		
		//glDisable(GL_SCISSOR_TEST);
// Background according to background color
		if ((c_fons.r < 0.5) || (c_fons.g < 0.5) || (c_fons.b<0.5))
			FonsB();
		else
			FonsN();

// Here we call the ProjeccioOrto and Ortografica calls to obtain the four orthographic views (top, front, isometric)
// ----------GMS Environment: YOU MUST COMPLETE WHEN YOU IMPLEMENT ORTHOGRAPHIC VIEW
// PLANTA (Upper Right)
		
// Definition of Viewport, Projection and Camera
		
		Projeccio_Orto(w/2, h/2, w/2, h/2);
		Vista_Ortografica(0, OPV, c_fons, col_obj, objecte, mida, pas, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, textura, textura_map, ifixe, eixos);
		
		// Draw the object or scene
		glPushMatrix();
		  configura_Escena();   // To apply Geometrical Transforms according Transform pop up and to configure scene objects
		  dibuixa_Escena();		// Draw scene geometry using OpenGL commands.
		glPopMatrix();
		
// ALÇAT (Upper Left)
// Definition of Viewport, Projection and Camera
		Projeccio_Orto(0, h/2, w/2, h/2);
		Vista_Ortografica(1, OPV, c_fons, col_obj, objecte, mida, pas, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, textura, textura_map, ifixe, eixos);
		
		// Draw the object or scene
		glPushMatrix();
		  configura_Escena();   // To apply Geometrical Transforms according Transform pop up and to configure scene objects
		  dibuixa_Escena();		// Draw scene geometry using OpenGL commands.
		glPopMatrix();
		
		
// PROFILE (Lower Left)
// Definition of Viewport, Projection and Camera
		Projeccio_Orto(0, 0 , w/2,h/2);
		Vista_Ortografica(2, OPV, c_fons, col_obj, objecte, mida, pas, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, textura, textura_map, ifixe, eixos);

// Draw the object or scene
		glPushMatrix();
		  configura_Escena();   // To apply Geometrical Transforms according Transform pop up and to configure scene objects
		  dibuixa_Escena();		// Draw scene geometry using OpenGL commands.
		glPopMatrix();
		
// ISOMETRIC (Lower Right)
// Definition of Viewport, Projection and Camera
		Projeccio_Orto(w/2, 0 , w/2, h/2);
		Vista_Ortografica(3, OPV, c_fons, col_obj, objecte, mida, pas, oculta,
			test_vis, back_line, ilumina, llum_ambient, llumGL, textura, textura_map, ifixe, eixos);

// Draw the object or scene
		glPushMatrix();
		  configura_Escena();   // To apply Geometrical Transforms according Transform pop up and to configure scene objects
		  dibuixa_Escena();		// Draw scene geometry using OpenGL commands.
		glPopMatrix();
		
// Swap OpenGL buffer --> Screen Buffer
		SwapBuffers(m_pDC->GetSafeHdc());
		break;

	case PERSPECT:
// PERSPECTIVE VIEW
// Here we call ProjeccioPerspectiva and Vista_Esferica functions
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Set Perspective Calculations To Most Accurate
		glDisable(GL_SCISSOR_TEST);							// Disable screen clipping.

// Definition of Viewport, Projection and Camera
		Projeccio_Perspectiva(0, 0, w, h, OPV.R);
		if (navega)	{	Vista_Navega(opvN, false, n, vpv, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, true, pas,
							oculta, test_vis, back_line, ilumina, llum_ambient, llumGL, textura, textura_map, ifixe, eixos);
					}
		else {	n[0] = 0;		n[1] = 0;		n[2] = 0;
				Vista_Esferica(OPV, Vis_Polar, pan, tr_cpv, tr_cpvF, c_fons, col_obj, objecte, mida, pas,
					oculta, test_vis, back_line, ilumina, llum_ambient, llumGL, textura,
					textura_map, ifixe, eixos);
			}

// Draw the object or scene
		glPushMatrix();
		  configura_Escena();   // To apply Geometrical Transforms according Transform pop up and to configure scene objects
		  dibuixa_Escena();		// Draw scene geometry using OpenGL commands.
		glPopMatrix();

// Swap OpenGL buffer --> Screen Buffer
		SwapBuffers(m_pDC->GetSafeHdc());
		break;

	default:
// GMS Environment:  Creation of the precompiled list to draw the World Coordinates Axes.	
		glNewList(EIXOS, GL_COMPILE);
// Draw the axes without lighting
		  glDisable(GL_LIGHTING);
// Draw the axes without textures
		  glDisable(GL_TEXTURE_2D);
// Switch off fog
		  glDisable(GL_FOG);
		  deixos();						// Function that draws the World Coordinates Axes
		  if (ilumina != FILFERROS) glEnable(GL_LIGHTING);
		  if (textura) glEnable(GL_TEXTURE_2D);
		glEndList();

// Clear background with white color.
		FonsB();

// Swap OpenGL buffer --> Screen Buffer
		SwapBuffers(m_pDC->GetSafeHdc());
		break;
}

// Switch off OpenGL context (from this point you don't accept OpenGL commands)
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

//  Update the Status Bar of application with the values R,A,B,PVx,PVy,PVz and others.
	Barra_Estat();
}


// configura_Escena: This function calls instancia function that defines the Geometrical Transforms defined in pop up Transforma.
void CEntornVGIView::configura_Escena() {

// To apply Geometrical Transforms according to Transform pop up.
	instancia(transf, TG, TGF);
}

// dibuixa_Escena: Function that calls the function that draws the different scene graphic elements
void CEntornVGIView::dibuixa_Escena() {

//	Drawing of scene geometry with GL commands.
	dibuixa_EscenaGL(objecte, col_obj, true, sw_material, textura);
}

// Barra_Estat: Updates the Status Bar of the applications with the values of R,A,B,PVx,PVy,PVz and others.
void CEntornVGIView::Barra_Estat()
{
	CString sss;
	CEsfe3D OPVAux;
	float PVx, PVy, PVz;

// Status Bar fractal file
	if (nom != "") GetStatusBar().SetPaneText(0, nom);

// Calculation of the values of menu option View->Walk
	if (projeccio != CAP && projeccio != ORTO) {
		if (navega)
		{
			OPVAux.R = sqrt(opvN.x*opvN.x + opvN.y*opvN.y + opvN.z*opvN.z);
			OPVAux.alfa = (asin(opvN.z / OPVAux.R) * 180) / pi;
			OPVAux.beta = (atan(opvN.y / opvN.x)) * 180 / pi;
		}
		else {
			OPVAux.R = OPV.R; OPVAux.alfa = OPV.alfa; OPVAux.beta = OPV.beta; //Raux=R; angv=anglev; angh=angleh; 
		}
	}
	else {
		OPVAux.R = OPV.R; OPVAux.alfa = OPV.alfa; OPVAux.beta = OPV.beta; //Raux=R; angv=anglev; angh=angleh; 
	}

// Status Bar R- Point of View Origin (camera position)
	if (projeccio == CAP) buffer = "       ";	
		else if (projeccio==ORTO) buffer=" ORTO   ";
			else if (navega) buffer = " NAV   ";
			else buffer.Format(_T("%.1f"), OPVAux.R);
	sss = _T("R=") + buffer;
// Update R position of Status Bar
	GetStatusBar().SetPaneText(1, sss);

// Status Bar for alfa angle of Point of View (camera position)
	if (projeccio == CAP) buffer = "       ";
		else if (projeccio==ORTO) buffer="ORTO   ";
			else if (navega) buffer = " NAV   ";
				else buffer.Format(_T("%.1f"), OPVAux.alfa);
	sss = _T("a=") + buffer;
// Update alfa position of Status Bar
	GetStatusBar().SetPaneText(2, sss);

// Status Bar for beta angle of Point of View (camera position)
	if (projeccio == CAP) buffer = "       ";
		else if (projeccio==ORTO) buffer="ORTO   ";
			else if (navega) buffer = " NAV   ";
				else buffer.Format(_T("%.1f"), OPVAux.beta);
	sss = _T("ß=") + buffer;
// Update beta position of Status Bar
	GetStatusBar().SetPaneText(3, sss);

// To transform Spherical Coordinates of Point of View (R,anglev,angleh) --> Cartesian Coord. (PVx,PVy,PVz)
	if (navega) { PVx = opvN.x; PVy = opvN.y; PVz = opvN.z; }
	else {
		if (Vis_Polar == POLARZ) {
			PVx = OPVAux.R*cos(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVy = OPVAux.R*sin(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVz = OPVAux.R*sin(OPVAux.alfa*pi / 180);
		}
		else if (Vis_Polar == POLARY) {
			PVx = OPVAux.R*sin(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVy = OPVAux.R*sin(OPVAux.alfa*pi / 180);
			PVz = OPVAux.R*cos(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
		}
		else {
			PVx = OPVAux.R*sin(OPVAux.alfa*pi / 180);
			PVy = OPVAux.R*cos(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
			PVz = OPVAux.R*sin(OPVAux.beta*pi / 180)*cos(OPVAux.alfa*pi / 180);
		}
	}

// Status Bar PVx
	if (projeccio == CAP) buffer = "       ";
		else if (pan) float2CString(tr_cpv.x);
			else float2CString(PVx);
	sss = _T("PVx=") + buffer;
// Update PVx position of Status Bar
	GetStatusBar().SetPaneText(4, sss);

// Status Bar PVy
	if (projeccio == CAP) buffer = "       ";
		else if (pan) float2CString(tr_cpv.y);
			else float2CString(PVy);
	sss = _T("PVy=") + buffer;
// Update PVy position of Status Bar
	GetStatusBar().SetPaneText(5, sss);

// Status Bar PVz
	if (projeccio == CAP) buffer = "       ";
		else if (pan) float2CString(tr_cpv.z);
			else float2CString(PVz);
	sss = _T("PVz=") + buffer;
// Update PVz position of Status Bar
	GetStatusBar().SetPaneText(6, sss);

// Status to give the change of color mode (BACKGROUND or OBJECT)
	sss = " ";
	if (pan) sss = " PAN";
		else if (sw_color) sss = " OBJ";
			else sss = "FONS";

// Update Color mode of Status Bar
	GetStatusBar().SetPaneText(7, sss);

// Status Bar to give the Geometrical Transform type (TRAS, ROT, ESC)
	sss = " ";
	if (transf) {
		if (rota) sss = "ROT";
		else if (trasl) sss = "TRA";
		else if (escal) sss = "ESC";
	}
	else {
		// Components d'intensitat de fons que varien per teclat
		if ((fonsR) && (fonsG) && (fonsB)) sss = " RGB";
		else if ((fonsR) && (fonsG)) sss = " RG ";
		else if ((fonsR) && (fonsB)) sss = " R   B";
		else if ((fonsG) && (fonsB)) sss = "   GB";
		else if (fonsR) sss = " R  ";
		else if (fonsG) sss = "   G ";
		else if (fonsB) sss = "      B";
	}
// Update Transformations mode of Status Bar
	GetStatusBar().SetPaneText(8, sss);

// Status Bar of the parameters of Transform, Color and positions of Robot and human leg.
	sss = " ";
	if (transf)
	{	if (rota)
		{	buffer.Format(_T("%.1f"), TG.VRota.x);
			sss = _T("  ") + buffer + _T("   ");

			buffer.Format(_T("%.1f"), TG.VRota.y);
			sss = sss + buffer + _T("   ");

			buffer.Format(_T("%.1f"), TG.VRota.z);
			sss = sss + buffer;
		}
		else if (trasl)
		{	buffer.Format(_T("%.1f"), TG.VTras.x);
			sss = _T("  ") + buffer + _T("   ");

			buffer.Format(_T("%.1f"), TG.VTras.y);
			sss = sss + buffer + _T("   ");

			buffer.Format(_T("%.1f"), TG.VTras.z);
			sss = sss + buffer;
		}
		else if (escal)
		{	buffer.Format(_T("%.2f"), TG.VScal.x);
			sss = _T(" ") + buffer + _T("  ");

			buffer.Format(_T("%.2f"), TG.VScal.y);
			sss = sss + buffer + _T("  ");

			buffer.Format(_T("%.2f"), TG.VScal.x);
			sss = sss + buffer;
		}
	}
	else {	// Background Color
		if (!sw_color)
		{	buffer.Format(_T("%.3f"), c_fons.r);
			sss = _T(" ") + buffer + _T("  ");

			buffer.Format(_T("%.3f"), c_fons.g);
			sss = sss + buffer + _T("  ");

			buffer.Format(_T("%.3f"), c_fons.b);
			sss = sss + buffer;
		}
		else
		{	// Object Color
			buffer.Format(_T("%.3f"), col_obj.r);
			sss = _T(" ") + buffer + _T("  ");

			buffer.Format(_T("%.3f"), col_obj.g);
			sss = sss + buffer + _T("  ");

			buffer.Format(_T("%.3f"), col_obj.b);
			sss = sss + buffer;
		}
	}

// Update Background or object Color of Status Bar
	GetStatusBar().SetPaneText(9, sss);

// Status Bar to indicate the fractal step
	if (objecte == O_FRACTAL)
	{	buffer.Format(_T("%.1d"), pas);
		sss = _T("Pas=") + buffer;
	}
	else { sss = "          "; }

// Update fractal step of Status Bar
	GetStatusBar().SetPaneText(10, sss);
}

void CEntornVGIView::float2CString(float varf)
{
	float vfloat;
	vfloat = varf;
	if (abs(varf)<1.0) buffer.Format(_T("%.5f"), varf);
	else if (abs(varf)<99.0) buffer.Format(_T("%.4f"), varf);
	else if (abs(varf)<999.0) buffer.Format(_T("%.3f"), varf);
	else if (abs(varf)<9999.0) buffer.Format(_T("%.2f"), varf);
	else if (abs(varf)<99999.0) buffer.Format(_T("%.1f"), varf);
	else buffer.Format(_T("%.0f"), varf);

}


/* ------------------------------------------------------------------------- */
/*                                  SHADERS                                  */
/* ------------------------------------------------------------------------- */

/*****************************************************************************
* initializeShaders()
*      Loads custom shader by specifying filename (expects frag/vert pair)
*****************************************************************************/
GLuint CEntornVGIView::initializeShaders(CString filename)
{
	GLuint v, f, p;
	char *vs = NULL, *fs = NULL;
	CString file_Vert, file_Frag;
	int info_log_length = 0, compile_result = 0;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

// To read Vertex Shader <filename>.vert
	file_Vert = filename + _T(".vert");
	char* fitxer_Vert = CString2Char(file_Vert);
	vs = textFileRead(fitxer_Vert);

// To read Fragment Shader <filename>.vert
	file_Frag = filename + _T(".frag");
	char* fitxer_Frag = CString2Char(file_Frag);
	fs = textFileRead(fitxer_Frag);

// If the files are dound, proceed
	if ((vs != NULL) && (fs != NULL))
	{
		const char * vv = vs;
		const char * ff = fs;

		glShaderSource(v, 1, &vv, NULL);
		glShaderSource(f, 1, &ff, NULL);

		free(vs);
		free(fs);

// Vertex Shader Compilation
		glCompileShader(v);
		glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);
// Error compilation list of Vertex Shader
		if (compile_result == GL_FALSE){
			GLint maxLength = 0;

			glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

			AfxMessageBox(_T("Error compilació Vertex Shader"));

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(v); // Don't leak the shader.
			return 0;
		}

// Fragment Shader Compilation
		glCompileShader(f);
		glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);

// Error compilation list of Fragment Shader
		if (compile_result == GL_FALSE){
			GLint maxLength = 0;

			glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

			AfxMessageBox(_T("Error compilació Fragment Shader"));

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(v); // Don't leak the shader.
			return 0;
		}

// To create the Program that includes Vertex and Fragment Shader
		p = glCreateProgram();
		glAttachShader(p, v);	// Relates Vertex Shader to the Program
		glAttachShader(p, f);	// Relates Fragment Shader to the Program

// Program Link
		glLinkProgram(p);

// Programa Set Up
		glUseProgram(p);

// It returns the reference of created Program
		return p;
	}
	else return 0;	// To return 0 if file not found
}


char *CEntornVGIView::textFileRead(char *fn) {
	FILE *fp;
	char *content = NULL;
	int count = 0;
	if (fn != NULL) {
		fp = fopen(fn, "rt");
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
		else {	AfxMessageBox(_T("Vertex or Fragment Program not found"));
				return NULL;
			}
	}
	return content;
}

/*****************************************************************************
* releaseAllShaders()
*      Unload shaders, return to OpenGL fixed function.
*****************************************************************************/
void CEntornVGIView::releaseAllShaders()
{
// GMS Environment:  Enable OpenGL context.
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	glUseProgram(0);

// GMS Environment: Disable OpenGL context.
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
}


/* ------------------------------------------------------------------------- */
/*                           KEYBOARD CONTROL                                */
/* ------------------------------------------------------------------------- */

// OnKeyDown: Control function to event keyboard (it is called when the user press a key)
//   PARAMETERS:
//    - nChar: ASCII code of the key pressed
//    - nRepCnt: Occurrences of key pressed (to give acceleration to de action because the user press frequently the same key)
//    - nFlags: Flags.
void CEntornVGIView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// TODO: Add your message handler code here and/or call default
	const float incr = 0.025f;
	float modul = 0;
	GLfloat vdir[3] = { 0, 0, 0 };

	if ((!pan) && (!transf) && (!navega))
	{
		if (!sw_color) Teclat_ColorFons(nChar, nRepCnt);
		else Teclat_ColorObjecte(nChar, nRepCnt);
	}
	else {	if (transf)
			{	if (rota) Teclat_TransRota(nChar, nRepCnt);
				  else if (trasl) Teclat_TransTraslada(nChar, nRepCnt);
					else if (escal) Teclat_TransEscala(nChar, nRepCnt);
			}
			if (pan) Teclat_Pan(nChar, nRepCnt);
			 else if (navega) Teclat_Navega(nChar, nRepCnt);
		}

// OnPaint() call to redraw the scene
	InvalidateRect(NULL, false);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CEntornVGIView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
// TODO: Add your message handler code here and/or call default

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


// Teclat_ColorObjecte: keys to change the object color by means of keyboard.
void CEntornVGIView::Teclat_ColorObjecte(UINT nChar, UINT nRepCnt)
{
	const float incr = 0.025f;

// FRACTAL: Resolution change with '+' and '-' keys
	if (objecte == O_FRACTAL)
	{
		if (nChar == 109) // '-' key
		{	pas = pas * 2;
			if (pas>64) pas = 64;
			sw_il = true;
		}
		 else if (nChar == 107) // '+' key
		 {	pas = pas / 2;
			if (pas<1) pas = 1;
			sw_il = true;
		 }
	}
	//	else 
	if (nChar == VK_DOWN) 
	{	if (fonsR) {	col_obj.r -= nRepCnt*incr;
						if (col_obj.r<0.0) col_obj.r = 0.0;
					}
		if (fonsG) {	col_obj.g -= nRepCnt*incr;
						if (col_obj.g<0.0) col_obj.g = 0.0;
					}
		if (fonsB) {	col_obj.b -= nRepCnt*incr;
						if (col_obj.b<0.0) col_obj.b = 0.0;
					}
	}
	else if (nChar == VK_UP) 
		{	if (fonsR) {	col_obj.r += nRepCnt*incr;
							if (col_obj.r>1.0) col_obj.r = 1.0;
						}
			if (fonsG) {	col_obj.g += nRepCnt*incr;
							if (col_obj.g>1.0) col_obj.g = 1.0;
						}
			if (fonsB) {	col_obj.b += nRepCnt*incr;
							if (col_obj.b>1.0) col_obj.b = 1.0;
						}
		}
		else if (nChar == VK_SPACE) 
			{	if ((fonsR) && (fonsG) && (fonsB)) {	fonsG = false;
														fonsB = false;
													}
				  else if ((fonsR) && (fonsG)) {	fonsG = false;
													fonsB = true;
												}
					else if ((fonsR) && (fonsB)) {	fonsR = false;
													fonsG = true;
												}
						else if ((fonsG) && (fonsB)) fonsR = true;
							else if (fonsR) {	fonsR = false;
												fonsG = true;
											}
								else if (fonsG) {	fonsG = false;
													fonsB = true;
												}
									else if (fonsB) {	fonsR = true;
														fonsG = true;
														fonsB = false;
													}
			}
				else if ((nChar == 'o') || (nChar == 'O')) sw_color = true;
					else if ((nChar == 'f') || (nChar == 'F')) sw_color = false;

}


// Teclat_ColorFons: Keys to change background color.
void CEntornVGIView::Teclat_ColorFons(UINT nChar, UINT nRepCnt)
{
	const float incr = 0.025f;

// FRACTAL: Resolution change with '+' and '-' keys
	if (objecte == O_FRACTAL)
	{	if (nChar == 109) // '-' key
		{	pas = pas * 2;
			if (pas>64) pas = 64;
			sw_il = true;
		}
		else if (nChar == 107) // '+' key
		{	pas = pas / 2;
			if (pas<1) pas = 1;
			sw_il = true;
		}
	}
	//	else 
	if (nChar == VK_DOWN) {
		if (fonsR) {	c_fons.r -= nRepCnt*incr;
						if (c_fons.r<0.0) c_fons.r = 0.0;
					}
		if (fonsG) {	c_fons.g -= nRepCnt*incr;
						if (c_fons.g<0.0) c_fons.g = 0.0;
					}
		if (fonsB) {	c_fons.b -= nRepCnt*incr;
						if (c_fons.b<0.0) c_fons.b = 0.0;
					}
		}
	else if (nChar == VK_UP) {
		if (fonsR) {	c_fons.r += nRepCnt*incr;
						if (c_fons.r>1.0) c_fons.r = 1.0;
					}
		if (fonsG) {	c_fons.g += nRepCnt*incr;
						if (c_fons.g>1.0) c_fons.g = 1.0;
					}
		if (fonsB) {	c_fons.b += nRepCnt*incr;
						if (c_fons.b>1.0) c_fons.b = 1.0;
					}
		}
	else if (nChar == VK_SPACE) {
		if ((fonsR) && (fonsG) && (fonsB)) {	fonsG = false;
												fonsB = false;
											}
			else if ((fonsR) && (fonsG)) {		fonsG = false;
												fonsB = true;
										}
				else if ((fonsR) && (fonsB)) {	fonsR = false;
												fonsG = true;
											}
					else if ((fonsG) && (fonsB)) fonsR = true;
						else if (fonsR) {	fonsR = false;
											fonsG = true;
										}
							else if (fonsG) {	fonsG = false;
												fonsB = true;
											}
								else if (fonsB) {	fonsR = true;
													fonsG = true;
													fonsB = false;
												}
		}
		else if ((nChar == 'o') || (nChar == 'O')) sw_color = true;
			else if ((nChar == 'f') || (nChar == 'F')) sw_color = false;
}

// Teclat_Navega: Keys to move in Walking mode
void CEntornVGIView::Teclat_Navega(UINT nChar, UINT nRepCnt)
{
	GLfloat vdir[3] = { 0, 0, 0 };
	float modul = 0;

// GMS Environment: Update variables for Walking movement
	vdir[0] = n[0] - opvN.x;
	vdir[1] = n[1] - opvN.y;
	vdir[2] = n[2] - opvN.z;
	modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
	vdir[0] = vdir[0] / modul;
	vdir[1] = vdir[1] / modul;
	vdir[2] = vdir[2] / modul;
	switch (nChar)
	{	
	// Key Up cursor
	case VK_UP:
		opvN.x += nRepCnt*fact_pan*vdir[0];
		opvN.y += nRepCnt*fact_pan*vdir[1];
		n[0] += nRepCnt*fact_pan*vdir[0];
		n[1] += nRepCnt*fact_pan*vdir[1];
		break;

	// Key Down cursor 
	case VK_DOWN:
		opvN.x -= nRepCnt*fact_pan*vdir[0];
		opvN.y -= nRepCnt*fact_pan*vdir[1];
		n[0] -= nRepCnt*fact_pan*vdir[0];
		n[1] -= nRepCnt*fact_pan*vdir[1];
		break;

	// Key Left cursor
	case VK_LEFT:
		angleZ = +nRepCnt*fact_pan;
		n[0] = n[0] - opvN.x;
		n[1] = n[1] - opvN.y;
		n[0] = n[0] * cos(angleZ*pi / 180) - n[1] * sin(angleZ*pi / 180);
		n[1] = n[0] * sin(angleZ*pi / 180) + n[1] * cos(angleZ*pi / 180);
		n[0] = n[0] + opvN.x;
		n[1] = n[1] + opvN.y;
		break;

	// Key Right cursor
	case VK_RIGHT:
		angleZ = 360 - nRepCnt*fact_pan;
		n[0] = n[0] - opvN.x;
		n[1] = n[1] - opvN.y;
		n[0] = n[0] * cos(angleZ*pi / 180) - n[1] * sin(angleZ*pi / 180);
		n[1] = n[0] * sin(angleZ*pi / 180) + n[1] * cos(angleZ*pi / 180);
		n[0] = n[0] + opvN.x;
		n[1] = n[1] + opvN.y;
		break;

	// Key Home
	case VK_HOME:
		opvN.z += nRepCnt*fact_pan;
		n[2] += nRepCnt*fact_pan;
		break;

	// Key End
	case VK_END:
		opvN.z -= nRepCnt*fact_pan;
		n[2] -= nRepCnt*fact_pan;
		break;

	// Key PgUp
	case VK_PRIOR:
		fact_pan /= 2;
		if (fact_pan<1) fact_pan = 1;
		break;

	// Key PgDown
	case VK_NEXT:
		fact_pan *= 2;
		if (fact_pan>2048) fact_pan = 2048;
		break;

	default:
		break;
	}
}


// Teclat_Pan: Teclat pels moviments de Pan.
void CEntornVGIView::Teclat_Pan(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
	// Key Up cursor
	case VK_UP:
		tr_cpv.y -= nRepCnt*fact_pan;
		if (tr_cpv.y<-100000) tr_cpv.y = 100000;
		break;

	// Key Down cursor
	case VK_DOWN:
		tr_cpv.y += nRepCnt*fact_pan;
		if (tr_cpv.y>100000) tr_cpv.y = 100000;
		break;

	// Key Left cursor
	case VK_LEFT:
		tr_cpv.x += nRepCnt*fact_pan;
		if (tr_cpv.x>100000) tr_cpv.x = 100000;
		break;

	// Key Right cursor
	case VK_RIGHT:
		tr_cpv.x -= nRepCnt*fact_pan;
		if (tr_cpv.x<-100000) tr_cpv.x = 100000;
		break;

	// key PgUp
	case VK_PRIOR:
		fact_pan /= 2;
		if (fact_pan<0.125) fact_pan = 0.125;
		break;

	// Key PgDown
	case VK_NEXT:
		fact_pan *= 2;
		if (fact_pan>2048) fact_pan = 2048;
		break;

	// Key Insert: To Fix screen displacement (pan)
	case VK_INSERT:
		// To pass Geometrical Transforms (variable TG) and pan variables to fixed variables (TGF variable)
		tr_cpvF.x += tr_cpv.x;		tr_cpv.x = 0.0;
		if (tr_cpvF.x>100000) tr_cpvF.y = 100000;
		tr_cpvF.y += tr_cpv.y;		tr_cpv.y = 0.0;
		if (tr_cpvF.y>100000) tr_cpvF.y = 100000;
		tr_cpvF.z += tr_cpv.z;		tr_cpv.z = 0.0;
		if (tr_cpvF.z>100000) tr_cpvF.z = 100000;
		break;

	// Key Delete: To initialize screen displacement (pan)
	case VK_DELETE:
		// To initialize pan values of variables tr_cpv and tr_cpvF.
		tr_cpv.x = 0.0;			tr_cpv.y = 0.0;			tr_cpv.z = 0.0;
		tr_cpvF.x = 0.0;		tr_cpvF.y = 0.0;		tr_cpvF.z = 0.0;
		break;

	default:
		break;
	}
}

// Teclat_TransEscala: Keys to change scale values for X,Y,Z axes.
void CEntornVGIView::Teclat_TransEscala(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
// To modify Scale vector by means of keyboard (activated only with Scale mode)
	// Key '+' (To increase the scale factor)
	case 107:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Key '-' (to decrease the scale factor)
	case 109:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Key Up cursor ('8')
	case 104:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		break;

	// Key Down cursor ('2')
	case 98:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		break;

	// Key Left cursor ('4')
	case 100:
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		break;

	// Key Right cursor ('6')
	case 102:
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		break;

	// TKey HOME ('7')
	case 103:
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Key END ('1')
	case 97:
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Key INSERT
	case VK_INSERT:
		// To pass Geometrical Transforms (variable TG) and pan variables to fixed variables (TGF variable)
		TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
		if (TGF.VScal.x>8192)		TGF.VScal.x = 8192;
		if (TGF.VScal.y>8192)		TGF.VScal.y = 8192;
		if (TGF.VScal.z>8192)		TGF.VScal.z = 8192;
		TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
		TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
		if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x<0) TGF.VRota.x += 360;
		if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y<0) TGF.VRota.y += 360;
		if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z<0) TGF.VRota.z += 360;
		TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
		TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
		if (TGF.VTras.x<-100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x>10000) TGF.VTras.x = 100000;
		if (TGF.VTras.y<-100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y>10000) TGF.VTras.y = 100000;
		if (TGF.VTras.z<-100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z>10000) TGF.VTras.z = 100000;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Key Delete: To delete Geometrical Transforms calculated
	case VK_DELETE:
		// To initialize Geometrical Transforms variables and pan variables in fixed variables
		TGF.VScal.x = 1.0;		TGF.VScal.y = 1.0;;		TGF.VScal.z = 1.0;
		TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
		TGF.VRota.x = 0.0;		TGF.VRota.y = 0.0;		TGF.VRota.z = 0.0;
		TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
		TGF.VTras.x = 0.0;		TGF.VTras.y = 0.0;		TGF.VTras.z = 0.0;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	default:
		break;
	}
}

// Teclat_TransRota: keys to change the values of the vectors that contains rotation angles X,Y,Z.
void CEntornVGIView::Teclat_TransRota(UINT nChar, UINT nRepCnt)
{
	switch (nChar)
	{
	// Key Up cursor ('8')
	case 104:
		TG.VRota.x += nRepCnt*fact_Rota;
		if (TG.VRota.x >= 360) TG.VRota.x -= 360;
		break;

	// Key Down cursor ('2')
	case 98:
		TG.VRota.x -= nRepCnt*fact_Rota;
		if (TG.VRota.x<1) TG.VRota.x += 360;
		break;

	// Key Left cursor ('4')
	case 100:
		TG.VRota.y -= nRepCnt*fact_Rota;
		if (TG.VRota.y<1) TG.VRota.y += 360;
		break;

	// Key Right cursor ('6')
	case 102:
		TG.VRota.y += nRepCnt*fact_Rota;
		if (TG.VRota.y >= 360) TG.VRota.y -= 360;
		break;

	// Key HOME ('7')
	case 103:
		TG.VRota.z += nRepCnt*fact_Rota;
		if (TG.VRota.z >= 360) TG.VRota.z -= 360;
		break;

	// Key END ('1')
	case 97:
		TG.VRota.z -= nRepCnt*fact_Rota;
		if (TG.VRota.z<1) TG.VRota.z += 360;
		break;

	// Key PgUp ('9')
	case 105:
		fact_Rota /= 2;
		if (fact_Rota<1) fact_Rota = 1;
		break;

	// Key PgDown ('3')
	case 99:
		fact_Rota *= 2;
		if (fact_Rota>90) fact_Rota = 90;
		break;

// To modify Scale vector by means of keyboard (activated only with Scale mode)
	// Key '+' (To increase the scale factor)
	case 107:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Key '-' (To decrease the scale factor)
	case 109:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Key Insert: // To pass Geometrical Transforms (variable TG) and pan variables to fixed variables (TGF variable)
	case VK_INSERT:
		TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
		if (TGF.VScal.x>8192)		TGF.VScal.x = 8192;
		if (TGF.VScal.y>8192)		TGF.VScal.y = 8192;
		if (TGF.VScal.z>8192)		TGF.VScal.z = 8192;
		TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
		TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
		if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x<0) TGF.VRota.x += 360;
		if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y<0) TGF.VRota.y += 360;
		if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z<0) TGF.VRota.z += 360;
		TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
		TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
		if (TGF.VTras.x<-100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x>10000) TGF.VTras.x = 100000;
		if (TGF.VTras.y<-100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y>10000) TGF.VTras.y = 100000;
		if (TGF.VTras.z<-100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z>10000) TGF.VTras.z = 100000;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Key Delete: To delete Geometrical Transforms calculated
	case VK_DELETE:
		// To initialize Geometrical Transforms variables and pan variables in fixed variables.
		TGF.VScal.x = 1.0;	TGF.VScal.y = 1.0;;	TGF.VScal.z = 1.0;
		TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
		TGF.VRota.x = 0.0;	TGF.VRota.y = 0.0;	TGF.VRota.z = 0.0;
		TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
		TGF.VTras.x = 0.0;	TGF.VTras.y = 0.0;	TGF.VTras.z = 0.0;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Key Space
	case VK_SPACE:
		rota = !rota;
		trasl = !trasl;
		break;

	default:
		break;
	}
}


// Teclat_TransTraslada: Keys to change the translation values for X,Y,Z.
void CEntornVGIView::Teclat_TransTraslada(UINT nChar, UINT nRepCnt)
{
	GLfloat vdir[3] = { 0, 0, 0 };
	float modul = 0;

	switch (nChar)
	{
	// Key Up cursor ('8')
	case 104:
		TG.VTras.x -= nRepCnt*fact_Tras;
		if (TG.VTras.x<-100000) TG.VTras.x = 100000;
		break;

	// Key Down cursor ('2')
	case 98:
		TG.VTras.x += nRepCnt*fact_Tras;
		if (TG.VTras.x>10000) TG.VTras.x = 100000;
		break;

	// Key Left cursor ('4')
	case 100:
		TG.VTras.y -= nRepCnt*fact_Tras;
		if (TG.VTras.y<-100000) TG.VTras.y = -100000;
		break;

	// Key Right cursor ('6')
	case 102:
		TG.VTras.y += nRepCnt*fact_Tras;
		if (TG.VTras.y>100000) TG.VTras.y = 100000;
		break;

	// Key HOME ('7')
	case 103:
		TG.VTras.z += nRepCnt*fact_Tras;
		if (TG.VTras.z>100000) TG.VTras.z = 100000;
		break;

	// Key END ('1')
	case 97:
		TG.VTras.z -= nRepCnt*fact_Tras;
		if (TG.VTras.z<-100000) TG.VTras.z = -100000;
		break;

	// Key PgUp ('9')
	case 105:
		fact_Tras /= 2;
		if (fact_Tras<1) fact_Tras = 1;
		break;

	// Key PgDown ('3')
	case 99:
		fact_Tras *= 2;
		if (fact_Tras>100000) fact_Tras = 100000;
		break;

// To modify Scale vector by means of keyboard (activated only with Scale mode)
	// Key '+' (To increase the scale factor)
	case 107:
		TG.VScal.x = TG.VScal.x * 2;
		if (TG.VScal.x>8192) TG.VScal.x = 8192;
		TG.VScal.y = TG.VScal.y * 2;
		if (TG.VScal.y>8192) TG.VScal.y = 8192;
		TG.VScal.z = TG.VScal.z * 2;
		if (TG.VScal.z>8192) TG.VScal.z = 8192;
		break;

	// Key '-' (to decrease the scale factor)
	case 109:
		TG.VScal.x = TG.VScal.x / 2;
		if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
		TG.VScal.y = TG.VScal.y / 2;
		if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
		TG.VScal.z = TG.VScal.z / 2;
		if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
		break;

	// Key INSERT
	case VK_INSERT:
		// To pass Geometrical Transforms (variable TG) and pan variables to fixed variables (TGF variable)
		TGF.VScal.x *= TG.VScal.x;	TGF.VScal.y *= TG.VScal.y; TGF.VScal.z *= TG.VScal.z;
		if (TGF.VScal.x>8192)		TGF.VScal.x = 8192;
		if (TGF.VScal.y>8192)		TGF.VScal.y = 8192;
		if (TGF.VScal.z>8192)		TGF.VScal.z = 8192;
		TG.VScal.x = 1.0;				TG.VScal.y = 1.0;			TG.VScal.z = 1.0;
		TGF.VRota.x += TG.VRota.x;	TGF.VRota.y += TG.VRota.y; TGF.VRota.z += TG.VRota.z;
		if (TGF.VRota.x >= 360)		TGF.VRota.x -= 360; 		if (TGF.VRota.x<0) TGF.VRota.x += 360;
		if (TGF.VRota.y >= 360)		TGF.VRota.y -= 360;		if (TGF.VRota.y<0) TGF.VRota.y += 360;
		if (TGF.VRota.z >= 360)		TGF.VRota.z -= 360;		if (TGF.VRota.z<0) TGF.VRota.z += 360;
		TG.VRota.x = 0.0;				TG.VRota.y = 0.0;					TG.VRota.z = 0.0;
		TGF.VTras.x += TG.VTras.x;	TGF.VTras.y += TG.VTras.y; TGF.VTras.z += TG.VTras.z;
		if (TGF.VTras.x<-100000)		TGF.VTras.x = 100000;		if (TGF.VTras.x>10000) TGF.VTras.x = 100000;
		if (TGF.VTras.y<-100000)		TGF.VTras.y = 100000;		if (TGF.VTras.y>10000) TGF.VTras.y = 100000;
		if (TGF.VTras.z<-100000)		TGF.VTras.z = 100000;		if (TGF.VTras.z>10000) TGF.VTras.z = 100000;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Key Delete: To delete Geometrical Transforms calculated
	case VK_DELETE:
		// To initialize Geometrical Transforms variables and pan variables in fixed variables
		TGF.VScal.x = 1.0;		TGF.VScal.y = 1.0;;		TGF.VScal.z = 1.0;
		TG.VScal.x = 1.0;		TG.VScal.y = 1.0;		TG.VScal.z = 1.0;
		TGF.VRota.x = 0.0;		TGF.VRota.y = 0.0;		TGF.VRota.z = 0.0;
		TG.VRota.x = 0.0;		TG.VRota.y = 0.0;		TG.VRota.z = 0.0;
		TGF.VTras.x = 0.0;		TGF.VTras.y = 0.0;		TGF.VTras.z = 0.0;
		TG.VTras.x = 0.0;		TG.VTras.y = 0.0;		TG.VTras.z = 0.0;
		break;

	// Key Space
	case VK_SPACE:
		rota = !rota;
		trasl = !trasl;
		break;

	default:
		break;
	}
}

/* ------------------------------------------------------------------------- */
/*                           MOUSE CONTROL                                   */
/* ------------------------------------------------------------------------- */

// OnLButtonDown: Callback function called when the mouse left button is pressed.
void CEntornVGIView::OnLButtonDown(UINT nFlags, CPoint point)
{
// TODO: Add your message handler code here and/or call default

// GMS Environment:  We detect the cursor position (point) when the mouse left button has been pressed
//				and we store it in m_PosEAvall variable activating flag m_ButoEAvall.
	m_ButoEAvall = true;
	m_PosEAvall = point;
	m_EsfeEAvall = OPV;

	CView::OnLButtonDown(nFlags, point);
}


// OnLButtonUp: Callback function called when the mouse left button is up pressed.
void CEntornVGIView::OnLButtonUp(UINT nFlags, CPoint point)
{
// TODO: Add your message handler code here and/or call default
// GMS Environment:  We disable the flag m_ButoEAvall when the mouse left button is up.
	m_ButoEAvall = false;

// OPCIÓ VIEW-->SATELLITE: Calculation of the displacement of the camera position 
	if ((satelit) && (projeccio != ORTO))
	{	//m_EsfeIncEAvall.R = m_EsfeEAvall.R - OPV.R;
		m_EsfeIncEAvall.alfa = 0.01f * (OPV.alfa - m_EsfeEAvall.alfa); //if (abs(m_EsfeIncEAvall.alfa)<0.01) { if ((m_EsfeIncEAvall.alfa)>0.0) m_EsfeIncEAvall.alfa = 0.01 else m_EsfeIncEAvall.alfa=0.01}
		m_EsfeIncEAvall.beta = 0.01f * (OPV.beta - m_EsfeEAvall.beta);
		if (abs(m_EsfeIncEAvall.beta) < 0.01) {
			if ((m_EsfeIncEAvall.beta) > 0.0) m_EsfeIncEAvall.beta = 0.01;
			else m_EsfeIncEAvall.beta = 0.01;
		}
		if ((m_EsfeIncEAvall.R == 0.0) && (m_EsfeIncEAvall.alfa == 0.0) && (m_EsfeIncEAvall.beta == 0.0)) KillTimer(WM_TIMER);
		else SetTimer(WM_TIMER, 10, NULL);
	}

	CView::OnLButtonUp(nFlags, point);
}


// OnRButtonUp: Callback function called when the mouse right button is up pressed.
void CEntornVGIView::OnRButtonDown(UINT nFlags, CPoint point)
{
// TODO: Agregue aquí su código de controlador de mensajes o llame al valor predeterminado
	// GMS Environment:  We detect the cursor position (point) when the mouse right button has been pressed
	//				and we store it in m_PosDAvall variable activating flag m_ButoDAvall.
	m_ButoDAvall = true;
	m_PosDAvall = point;

	CView::OnRButtonDown(nFlags, point);
}


// OnLButtonDown: Funció que es crida quan deixem d'apretar el botó dret del mouse.
void CEntornVGIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
// GMS Environment:  We disable the flag m_ButoDAvall when the mouse right button is up.
	m_ButoDAvall = false;

	ClientToScreen(&point);
	OnContextMenu(this, point);
}


// OnMouseMove: Callback function called when the mouse is moved. We use the function to 
//				  the Interactive Visualization with mouse keys pressed to modify 
//				  the Point of View parameters (R,angleh,anglev) according to the 
//				  mouse movements (horizontal or vertical).
//      PARAMETERS: - nFlags: Flags that control if the button is pressed or not.
//					- point: struct (x,y) giving the mouse position 
//							 (screen coordinates) when the button is pressed.
void CEntornVGIView::OnMouseMove(UINT nFlags, CPoint point)
{
// TODO: Add your message handler code here and/or call default
	float modul = 0;
	GLfloat vdir[3] = { 0, 0, 0 };

// TODO: Add your message handler code here and/or call default
	if (m_ButoEAvall && mobil && projeccio != CAP)
	{	
// GMS Environment: Determinació dels angles (en graus) segons l'increment
//				horitzontal i vertical de la posició del mouse.
		CSize gir = m_PosEAvall - point;
		m_PosEAvall = point;
		OPV.beta = OPV.beta - gir.cx / 2;
		OPV.alfa = OPV.alfa + gir.cy / 2;

// GMS Environment: Control per evitar el creixement desmesurat dels angles.
		if (OPV.alfa >= 360)	OPV.alfa = OPV.alfa - 360;
		if (OPV.alfa<0)		OPV.alfa = OPV.alfa + 360;
		if (OPV.beta >= 360)	OPV.beta = OPV.beta - 360;
		if (OPV.beta<0)		OPV.beta = OPV.beta + 360;
		InvalidateRect(NULL, false);
	}
	else if (m_ButoEAvall && navega && (projeccio != CAP && projeccio != ORTO)) // Opció Navegació
	{
// GMS Environment: Canviar orientació en opció de Navegació
		CSize girn = m_PosEAvall - point;
		angleZ = girn.cx / 2.0;
// GMS Environment: Control per evitar el creixement desmesurat dels angles.
		if (angleZ >= 360) angleZ = angleZ - 360;
		if (angleZ<0)	angleZ = angleZ + 360;

		n[0] = n[0] - opvN.x;
		n[1] = n[1] - opvN.y;
		n[0] = n[0] * cos(angleZ*pi / 180) - n[1] * sin(angleZ*pi / 180);
		n[1] = n[0] * sin(angleZ*pi / 180) + n[1] * cos(angleZ*pi / 180);
		n[0] = n[0] + opvN.x;
		n[1] = n[1] + opvN.y;

		m_PosEAvall = point;
		InvalidateRect(NULL, false);
	}

// GMS Environment: Transformació Geomètrica interactiva pels eixos X,Y boto esquerra del mouse.
	else {
		bool transE = transX || transY;
		if (m_ButoEAvall && transE && transf)
		{
			// Calcular increment
			CSize girT = m_PosEAvall - point;
			if (transX)
			{	long int incrT = girT.cx;
				if (trasl)
				{	TG.VTras.x += incrT*fact_Tras;
					if (TG.VTras.x<-100000) TG.VTras.x = 100000;
					if (TG.VTras.x>100000) TG.VTras.x = 100000;
				}
				else if (rota)
				{	TG.VRota.x += incrT*fact_Rota;
					while (TG.VRota.x >= 360) TG.VRota.x -= 360;
					while (TG.VRota.x<0) TG.VRota.x += 360;
				}
				else if (escal)
				{	if (incrT<0) incrT = -1 / incrT;
					TG.VScal.x = TG.VScal.x*incrT;
					if (TG.VScal.x<0.25) TG.VScal.x = 0.25;
					if (TG.VScal.x>8192) TG.VScal.x = 8192;
				}
			}
			if (transY)
			{	long int incrT = girT.cy;
				if (trasl)
				{	TG.VTras.y += incrT*fact_Tras;
					if (TG.VTras.y<-100000) TG.VTras.y = 100000;
					if (TG.VTras.y>100000) TG.VTras.y = 100000;
				}
				else if (rota)
				{	TG.VRota.y += incrT*fact_Rota;
					while (TG.VRota.y >= 360) TG.VRota.y -= 360;
					while (TG.VRota.y<0) TG.VRota.y += 360;
				}
				else if (escal)
				{	if (incrT <= 0) {	if (incrT >= -2) incrT = -2;
										incrT = 1 / Log2(-incrT);
									}
					  else incrT = Log2(incrT);
					TG.VScal.y = TG.VScal.y*incrT;
					if (TG.VScal.y<0.25) TG.VScal.y = 0.25;
					if (TG.VScal.y>8192) TG.VScal.y = 8192;
				}
			}
			m_PosEAvall = point;
			InvalidateRect(NULL, false);
		}
	}

// GMS Environment: Determinació del desplaçament del pan segons l'increment
//				vertical de la posició del mouse (tecla dreta apretada).
	if (m_ButoDAvall && pan && (projeccio != CAP && projeccio != ORTO))
	{
		CSize zoomincr = m_PosDAvall - point;
		long int incrx = zoomincr.cx;
		long int incry = zoomincr.cy;

// Desplaçament pan vertical
		tr_cpv.y -= incry*fact_pan;
		if (tr_cpv.y>100000) tr_cpv.y = 100000;
		  else if (tr_cpv.y<-100000) tr_cpv.y = -100000;

// Desplaçament pan horitzontal
		tr_cpv.x += incrx*fact_pan;
		if (tr_cpv.x>100000) tr_cpv.x = 100000;
		  else if (tr_cpv.x<-100000) tr_cpv.x = -100000;

		m_PosDAvall = point;
		InvalidateRect(NULL, false);
	}
// Determinació del paràmetre R segons l'increment
//   vertical de la posició del mouse (tecla dreta apretada)
	//else if (m_ButoDAvall && zzoom && (projeccio!=CAP && projeccio!=ORTO))
	else if (m_ButoDAvall && zzoom && (projeccio != CAP))
	{	CSize zoomincr = m_PosDAvall - point;
		long int incr = zoomincr.cy / 1.0;

//		zoom=zoom+incr;
		OPV.R = OPV.R + incr;
		if (OPV.R<1) OPV.R = 1;
		m_PosDAvall = point;
		InvalidateRect(NULL, false);
	}
	else if (m_ButoDAvall && navega && (projeccio != CAP && projeccio != ORTO))
	{	// Avançar en opció de Navegació
		if (m_PosDAvall != point)
		{	CSize zoomincr = m_PosDAvall - point;

			float incr = zoomincr.cy / 2;
//			long int incr=zoomincr.cy/2.0;  // Causa assertion en "afx.inl" lin 169
			vdir[0] = n[0] - opvN.x;
			vdir[1] = n[1] - opvN.y;
			vdir[2] = n[2] - opvN.z;
			modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
			vdir[0] = vdir[0] / modul;
			vdir[1] = vdir[1] / modul;
			vdir[2] = vdir[2] / modul;
			opvN.x += incr*vdir[0];
			opvN.y += incr*vdir[1];
			n[0] += incr*vdir[0];
			n[1] += incr*vdir[1];
			m_PosDAvall = point;
			InvalidateRect(NULL, false);
		}
	}

// GMS Environment: Transformació Geomètrica interactiva per l'eix Z amb boto dret del mouse.
	else if (m_ButoDAvall && transZ && transf)
	{
		// Calcular increment
		CSize girT = m_PosDAvall - point;
		long int incrT = girT.cy;
		if (trasl)
		{	TG.VTras.z += incrT*fact_Tras;
			if (TG.VTras.z<-100000) TG.VTras.z = 100000;
			if (TG.VTras.z>100000) TG.VTras.z = 100000;
		}
		else if (rota)
		{	incrT = girT.cx;
			TG.VRota.z += incrT*fact_Rota;
			while (TG.VRota.z >= 360) TG.VRota.z -= 360;
			while (TG.VRota.z<0) TG.VRota.z += 360;
		}
		else if (escal)
		{	if (incrT <= 0) {	if (incrT >= -2) incrT = -2;
								incrT = 1 / Log2(-incrT);
							}
			  else incrT = Log2(incrT);
			TG.VScal.z = TG.VScal.z*incrT;
			if (TG.VScal.z<0.25) TG.VScal.z = 0.25;
			if (TG.VScal.z>8192) TG.VScal.z = 8192;
		}

		m_PosDAvall = point;
		InvalidateRect(NULL, false);
	}

// Do not call CView::OnPaint() for painting messages
	CView::OnMouseMove(nFlags, point);
}

// OnMouseWheel: Callback function called when the mouse wheel is moved. We use it to  
//				  Interactive Visualization to modify the R parameterof the Point of View (R,angleh,anglev) 
//				  according to the mouse wheel movement.
//      PARAMETERS: - nFlags: Flags that control if the button is pressed or not.
//					- zDelta: Units of displacement of the mouse wheel.
//					- point: struct (x,y) giving the mouse position 
//							 (screen coordinates) when the button is pressed.
BOOL CEntornVGIView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
// TODO: Add your message handler code here and/or call default
	float modul = 0;
	GLfloat vdir[3] = { 0, 0, 0 };

// Zoom function when the function pan is switch on or Gemetrical Transforms
	if ((zzoom) || (transX) || (transY) || (transZ))
	{
		OPV.R = OPV.R + zDelta / 4;
		if (OPV.R<1) OPV.R = 1;
		InvalidateRect(NULL, false);
	}
	else if (navega)
	{	vdir[0] = n[0] - opvN.x;
		vdir[1] = n[1] - opvN.y;
		vdir[2] = n[2] - opvN.z;
		modul = sqrt(vdir[0] * vdir[0] + vdir[1] * vdir[1] + vdir[2] * vdir[2]);
		vdir[0] = vdir[0] / modul;
		vdir[1] = vdir[1] / modul;
		vdir[2] = vdir[2] / modul;
		opvN.x += (zDelta / 4)*vdir[0];
		opvN.y += (zDelta / 4)*vdir[1];
		n[0] += (zDelta / 4)*vdir[0];
		n[1] += (zDelta / 4)*vdir[1];
		InvalidateRect(NULL, false);
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


/* ------------------------------------------------------------------------- */
/*					     TIMER (COMPUTER ANIMATION)							 */
/* ------------------------------------------------------------------------- */
void CEntornVGIView::OnTimer(UINT_PTR nIDEvent)
{
// TODO: Add your message handler code here and/or call default
	if (anima)	{
		// Message handler of animation when n ms. have occurred

		// OnPaint() call to redraw the scene
		InvalidateRect(NULL, false);
		}
	else if (satelit)	{	// SATELLITE OPTION: OPV increment according mpouse movements
		//OPV.R = OPV.R + m_EsfeIncEAvall.R;
		OPV.alfa = OPV.alfa + m_EsfeIncEAvall.alfa;
		while (OPV.alfa > 360) OPV.alfa = OPV.alfa - 360;	while (OPV.alfa < 0) OPV.alfa = OPV.alfa + 360;
		OPV.beta = OPV.beta + m_EsfeIncEAvall.beta;
		while (OPV.beta > 360) OPV.beta = OPV.beta - 360;	while (OPV.beta < 0) OPV.beta = OPV.beta + 360;

		// OnPaint() call to redraw the scene
		InvalidateRect(NULL, false);
		}

	CView::OnTimer(nIDEvent);
}


/* ------------------------------------------------------------------------- */
/*   MENU RESOURCES (pop ups) OF THE APPLICATION:                            */
/*					1. FILES												 */
/*					2. TOOLS: ToolBar, StatusBar, FullScreen				 */
/*					3. VIEW: Point of View, Zoom, Pan, Walk And Axes	     */
/*					4. PROJECTION                                            */
/*					5. OBJECT					                             */
/*					6. TRANSFORM											 */
/*					7. HIDDEN												 */
/*					8. LIGHTING												 */
/*					9. LIGHTS												 */
/*				   10. SHADERS												 */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/*					1. FILES 												 */
/* ------------------------------------------------------------------------- */

// To open Fractal file
void CEntornVGIView::OnArxiuObrirFractal()
{
// TODO: Add your command controller here
// GMS Environment:  To open file dialog to read a file name (*.MNT files )
	CFileDialog openMunt(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("MNT Files(*.mnt)|*.mnt|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (openMunt.DoModal() != IDOK)
		return;							// stay with old data file
	else nom = openMunt.GetPathName();

// GMS Environment:  We have here a CString variable 'nom' that contains the path and name of the file selected.

// GMS Environment:  To convert the variable CString 'nom' to the variable char '*nomfitx', 
//		that is compatible with the functions to load fractal files.
	char *nomfitx = CString2Char(nom);

// GMS Environment:  Variable char *nomfitx contains the file name selected.

// OnPaint() call to redraw the scene
	InvalidateRect(NULL, false);
}


// OnArchivoObrirFitxerObj: To open graphic file in OBJ format
void CEntornVGIView::OnArxiuObrirFitxerObj()
{
// TODO: Add your command controller here
//if (ObOBJ != NULL) delete ObOBJ;

	objecte = OBJOBJ;

	// GMS Environment:  To open file dialog to read a file name (*.3DS files )
	CFileDialog openOBJ(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("OBJ Files(*.obj)|*.obj|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (openOBJ.DoModal() != IDOK)	return;  // stay with old data file
	else nom = openOBJ.GetPathName();

// GMS Environment:  We have here a CString variable 'nom' that contains the path and name of the file selected.

// GMS Environment:  To convert the variable CString 'nom' to the variable char '*nomfitx', 
//		that is compatible with the functions to load fractal files.
	char *nomfitx = CString2Char(nom);

// i we load
	// Switch on OpenGL context (from this point OpenGL commands are accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	

	if (ObOBJ == NULL) ObOBJ = new COBJModel;
	ObOBJ->LoadModel(nomfitx, OBJECTEOBJ);
	
	// Switch off OpenGL context (from this point OpenGL commands aren't accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	

// OnPaint() call to redraw the scene
	InvalidateRect(NULL, false);

}

// OnArxiuObrirFitxer3ds: To open graphic file in 3DS format
void CEntornVGIView::OnArxiuObrirFitxer3ds()
{
// TODO: Add your command controller here
	//if(Ob3DS!=NULL) delete Ob3DS;

	objecte = OBJ3DS;	textura = true;

// GMS Environment: Obrir diàleg de lectura de fitxer
	CFileDialog open3DS(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("3DS Files(*.3ds)|*.3ds|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (open3DS.DoModal() != IDOK)	return;  // stay with old data file
	else nom = open3DS.GetPathName();

// GMS Environment:  We have here a CString variable 'nom' that contains the path and name of the file selected.

// GMS Environment:  To convert the variable CString 'nom' to the variable char '*nomfitx', 
//		that is compatible with the functions to load fractal files.
	char *nomfitx = CString2Char(nom);

// ad we load
	// Switch on OpenGL context (from this point OpenGL commands are accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	if (Ob3DS == NULL) Ob3DS = new Obj_3DS;
	Ob3DS->EliminarMemoria();
	Ob3DS->Inicialitzar();
	Ob3DS->Carregar3DS(nomfitx);

// New objects precompilation: OBJECTE3DS: 3DS Object without textures and  OBJECTE3DST amb textures,
	Ob3DS->Dibuixa3DS(false, OBJECTE3DS, false);
	Ob3DS->Dibuixa3DS(false, OBJECTE3DST, true);
    
	// Switch off OpenGL context(from this point OpenGL commands aren't accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL); 

//  Entorn GMS: Modify R to center View and scale of the object (for ortographic projection).
	mida = sqrtf(3.0) * 10;
	OPV.R = 0.5*mida / sin(30 * pi / 180) + p_near;

// Return to main loop OnPaint() to redraw the scene.
	InvalidateRect(NULL, false);

}


/* ------------------------------------------------------------------------- */
/*					2. TOOLS: ToolBar, StatusBar, FullScreen                 */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/*					3. VIEW: Point of View, Zoom, Pan, Walk and Axes		 */
/* ------------------------------------------------------------------------- */

// VISTA: Mobile. Interactive Visualization and change of Point of View by mouse (boolean option)
void CEntornVGIView::OnVistaMobil()
{
// TODO: Add your command controller here
	if ((projeccio != ORTO) || (projeccio != CAP)) mobil = !mobil;
// Disable Interactive Geometrical Transforms by mouse if this option is enabled.
//		si Visualització Interactiva activada.	
	if (mobil)	{	transX = false;	transY = false; transZ = false;
				}

// Return to main loop OnPaint() to redraw the scene.
	InvalidateRect(NULL, false);
}


void CEntornVGIView::OnUpdateVistaMobil(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (mobil) 	pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

// VISTA: Zoom. Interactive Zoom (boolean option)
void CEntornVGIView::OnVistaZoom()
{
// TODO: Add your command controller here
	if ((projeccio != ORTO) || (projeccio != CAP)) zzoom = !zzoom;
// Disable Interactive Geometrical Transforms by mouse if Zoom option is enabled.
	if (zzoom)	{	transX = false;	transY = false;	transZ = false;
				}

// Return to main loop OnPaint() to redraw the scene.
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateVistaZoom(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (zzoom) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// VISTA: Satèlit. Interactive View and animated adding the movement defined by mouse (boolean option)
void CEntornVGIView::OnVistaSatelit()
{
// TODO: Add your command controller here
	if (projeccio != CAP || projeccio != ORTO) satelit = !satelit;
	if (satelit) mobil = true;
	bool testA = anima;									// Test if there are any animation active despite Satellite
	if ((!satelit) && (!testA)) KillTimer(WM_TIMER);	// If Satellite option is switched off and there aren't any animation the timer is switched off

// Return to main loop OnPaint() to redraw the scene.
	InvalidateRect(NULL, false);
}


void CEntornVGIView::OnUpdateVistaSatelit(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (satelit) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// VIEW: Shperical Coord. with X Axis Up.
void CEntornVGIView::OnVistaPolarsX()
{
// TODO: Add your command controller here
	if ((projeccio != CAP) && (!navega)) Vis_Polar = POLARX;

// Return to main loop OnPaint() to redraw the scene.
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateVistaPolarsX(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (Vis_Polar == POLARX) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// VIEW: Shperical Coord. with Y Axis Up.
void CEntornVGIView::OnVistaPolarsY()
{
// TODO: Add your command controller here
	if ((projeccio != CAP) && (!navega)) Vis_Polar = POLARY;

// Return to main loop OnPaint() to redraw the scene.
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateVistaPolarsY(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (Vis_Polar == POLARY) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// VIEW: Shperical Coord. with Z Axis Up.
void CEntornVGIView::OnVistaPolarsZ()
{
// TODO: Add your command controller here
	if ((projeccio != CAP) && (!navega)) Vis_Polar = POLARZ;

// Return to main loop OnPaint() to redraw the scene.
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateVistaPolarsZ(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (Vis_Polar == POLARZ) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// VISTA: Enable / Disable pan (horizontal and vertical displacement of screen) (boolean option)
void CEntornVGIView::OnVistaPan()
{
// TODO: Add your command controller here
	if ((projeccio != ORTO) || (projeccio != CAP)) pan = !pan;
// Disable Interactive Geometrical Transforms by mouse and walk (navega) if pan option is enabled.
	if (pan)	{	mobil = true;		zzoom = true;
					transX = false;	transY = false;	transZ = false;
					navega = false;
				}

// Return to main loop OnPaint() to redraw the scene.
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateVistaPan(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (pan) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}

// To go back to origin
void CEntornVGIView::OnVistaOrigenpan()
{
// TODO: Add your command controller here
	if (pan) {	fact_pan = 1;
				tr_cpv.x = 0;	tr_cpv.y = 0;	tr_cpv.z = 0;
			}

// Return to main loop OnPaint() to redraw the scene.
	InvalidateRect(NULL, false);
}

// VISTA: AWalking mode over Z=0 plane with key buttons or mouse (nav) (boolean option)
void CEntornVGIView::OnVistaNavega()
{
// TODO: Add your command controller here
	if ((projeccio != ORTO) || (projeccio != CAP)) navega = !navega;
// Disable Interactive Geometrical Transforms by mouse and pan if walk (navega) option is enabled.
	if (navega)
	{	mobil = false;	zzoom = false;
		transX = false;	transY = false;	transZ = false;
		pan = false;
		tr_cpv.x = 0.0;		tr_cpv.y = 0.0;		tr_cpv.z = 0.0;	// Inicialitzar a 0 desplaçament de pantalla
		tr_cpvF.x = 0.0;	tr_cpvF.y = 0.0;	tr_cpvF.x = 0.0; // Inicialitzar a 0 desplaçament de pantalla
	}
	else {	mobil = true;
			zzoom = true;
		}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateVistaNavega(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (navega) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}

// To go back to origin
void CEntornVGIView::OnVistaOrigennavega()
{
// TODO: Add your command controller here
	if (navega) {	n[0] = 0.0;		n[1] = 0.0;		n[2] = 0.0;
					opvN.x = 10.0;	opvN.y = 0.0;		opvN.z = 0.0;
					angleZ = 0.0;
				}

// OnPaint() call to redraw the scene
	InvalidateRect(NULL, false);
}

// VISTA: To show World Coordinates axes (boolean option)
void CEntornVGIView::OnVistaEixos()
{
// TODO: Add your command controller here
	eixos = !eixos;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}


void CEntornVGIView::OnUpdateVistaEixos(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (eixos) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}



/* ------------------------------------------------------------------------- */
/*					4. PROJECTION                                            */
/* ------------------------------------------------------------------------- */

// PROJECTION: Perspective
void CEntornVGIView::OnProjeccioPerspectiva()
{
// TODO: Add your command controller here
	projeccio = PERSPECT;
	mobil = true;			zzoom = true;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);

}

void CEntornVGIView::OnUpdateProjeccioPerspectiva(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (projeccio == PERSPECT) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}

// PROJECTION: Orthogonal
void CEntornVGIView::OnProjeccioOrthogonal()
{
	// TODO: Add your command controller here
	projeccio = ORTO;
	mobil = true;			zzoom = true;

	// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);

}

void CEntornVGIView::OnUpdateProjeccioOrthogonal(CCmdUI *pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (projeccio == ORTO) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}

// PROJECTION: Axonometric
void CEntornVGIView::OnProjeccioAxonometric()
{
	// TODO: Add your command controller here
	projeccio = AXONOM;
	mobil = true;			zzoom = true;

	// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);

}

void CEntornVGIView::OnUpdateProjeccioAxonometric(CCmdUI *pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (projeccio == AXONOM) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					5. OBJECT					                             */
/* ------------------------------------------------------------------------- */

// OBJECT: Cube
void CEntornVGIView::OnObjecteCub()
{
// TODO: Add your command controller here
	objecte = CUBE;

//	---- Entorn GMS: PAY ATTENTION!!. To change the scale of the object to fit it in the Volume of Visualization (-1,1,-1,1,-1,1) (Orthographic Views)

//  ---- Entorn GMS: PAY ATTENTION!!. Modify R parameter of Point of View to fit the object in screen (Perspective, Axonometric projections)

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateObjecteCub(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == CUBE) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OBJECT Sphere
void CEntornVGIView::OnObjecteEsfera()
{
// TODO: Add your command controller here
	objecte = SPHERE;

//	---- Entorn GMS: PAY ATTENTION!!. To change the scale of the object to fit it in the Volume of Visualization (-1,1,-1,1,-1,1) (Orthographic Views)

//  ---- Entorn GMS: PAY ATTENTION!!. Modify R parameter of Point of View to fit the object in screen (Perspective, Axonometric projections)

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateObjecteEsfera(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == SPHERE) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}

// OBJECT Teapot
void CEntornVGIView::OnObjecteTetera()
{
// TODO: Add your command controller here
	objecte = TEAPOT;

//	---- Entorn GMS: PAY ATTENTION!!. To change the scale of the object to fit it in the Volume of Visualization (-1,1,-1,1,-1,1) (Orthographic Views)

//  ---- Entorn GMS: PAY ATTENTION!!. Modify R parameter of Point of View to fit the object in screen (Perspective, Axonometric projections)

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateObjecteTetera(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == TEAPOT) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OBJECT Truck
void CEntornVGIView::OnObjecteTruck()
{
// TODO: Add your command controller here
	objecte = TRUCK;		textura = true;
   
// Switch on OpenGL context (from this point OpenGL commands are accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	
// Inicialize texture image vector o the Truck.
	Init_Textures();
	
// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);	

//	---- Entorn GMS: PAY ATTENTION!!. To change the scale of the object to fit it in the Volume of Visualization (-1,1,-1,1,-1,1) (Orthographic Views)

//  ---- Entorn GMS: PAY ATTENTION!!. Modify R parameter of Point of View to fit the object in screen (Perspective, Axonometric projections)

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateObjecteTruck(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == TRUCK) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// OBJECT ROBOT
void CEntornVGIView::OnObjecteRobot()
{
	// TODO: Add your command controller here
	objecte = ROBOT;		textura = true;

	// Switch on OpenGL context (from this point OpenGL commands are accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	// Inicialize texture image vector o the Truck.
	Init_Textures();

	// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	//	---- Entorn GMS: PAY ATTENTION!!. To change the scale of the object to fit it in the Volume of Visualization (-1,1,-1,1,-1,1) (Orthographic Views)

	//  ---- Entorn GMS: PAY ATTENTION!!. Modify R parameter of Point of View to fit the object in screen (Perspective, Axonometric projections)

	// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateObjecteRobot(CCmdUI *pCmdUI)
{
	// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (objecte == ROBOT) pCmdUI->SetCheck(1);
	else pCmdUI->SetCheck(0);
}



/* ------------------------------------------------------------------------- */
/*					6. TRANSFORM											 */
/* ------------------------------------------------------------------------- */

// TRANSFORM: TRASLATION
void CEntornVGIView::OnTransformaTraslacio()
{
// TODO: Add your command controller here
	trasl = !trasl;
	rota = false;
	if (trasl) escal = true;
	transf = trasl || rota || escal;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}


void CEntornVGIView::OnUpdateTransformaTraslacio(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (trasl) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// TRANSFORM: To define the TRASLATION Origin
void CEntornVGIView::OnTransformaOrigentraslacio()
{
// TODO: Add your command controller here
	if (trasl)
	{	fact_Tras = 1;
		TG.VTras.x = 0.0;	TG.VTras.y = 0.0;	TG.VTras.z = 0;
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}


// TRANSFORM: ROTATION
void CEntornVGIView::OnTransformaRotacio()
{
// TODO: Add your command controller here
	rota = !rota;
	trasl = false;
	if (rota) escal = true;
	transf = trasl || rota || escal;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateTransformaRotacio(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (rota) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// TRANSFORM: To define the ROTATION Origin
void CEntornVGIView::OnTransformaOrigenrotacio()
{
// TODO: Add your command controller here
	if (rota)
	{	fact_Rota = 90;
		TG.VRota.x = 0;		TG.VRota.y = 0;		TG.VRota.z = 0;
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}


// TRANSFORM: SCALE
void CEntornVGIView::OnTransformaEscalat()
{
// TODO: Add your command controller here
	if ((!rota) && (!trasl)) escal = !escal;
	transf = trasl || rota || escal;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateTransformaEscalat(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (escal) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// TRANSFORM: To define the SCALE Origin
void CEntornVGIView::OnTransformaOrigenescalat()
{
// TODO: Add your command controller heres
	if (escal) { TG.VScal.x = 1;	TG.VScal.y = 1;	TG.VScal.z = 1; }

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}


// TRANSFOM: Mobil X Axis? (boolean option).
void CEntornVGIView::OnTransformaMobilx()
{
// TODO: Add your command controller here
	if (transf)
	{	transX = !transX;
		if (transX) {	mobil = false;	zzoom = false;
						pan = false;	navega = false;
					}
			else if ((!transY) && (!transZ)){	mobil = true;
												zzoom = true;
											}
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateTransformaMobilx(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (transX) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// TRANSFOM: Mobil Y Axis? (boolean option).
void CEntornVGIView::OnTransformaMobily()
{
// TODO: Add your command controller here
	if (transf)
	{	transY = !transY;
		if (transY) {	mobil = false;	zzoom = false;
						pan = false;	navega = false;
					}
			else if ((!transX) && (!transZ)){	mobil = true;
												zzoom = true;
											}
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateTransformaMobily(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (transY) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// TRANSFOM: Mobil Z Axis? (boolean option).
void CEntornVGIView::OnTransformaMobilz()
{
// TODO: Add your command controller here
	if (transf)
	{	transZ = !transZ;
		if (transZ) {	mobil = false;	zzoom = false;
						pan = false;	navega = false;
					}
		else if ((!transX) && (!transY)) {	mobil = true;
											zzoom = true;
										}
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateTransformaMobilz(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (transZ) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					7. HIDDEN												 */
/* ------------------------------------------------------------------------- */

// HIDDEN: Back Face Culling (boolean option).
void CEntornVGIView::OnOcultacionsTestvis()
{
// TODO: Add your command controller here
	test_vis = !test_vis;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateOcultacionsTestvis(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (test_vis) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// HIDDEN: Z-Buffer? (boolean option).
void CEntornVGIView::OnOcultacionsZbuffer()
{
// TODO: Add your command controller here
	oculta = !oculta;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateOcultacionsZbuffer(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (oculta) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// HIDDEN: Back Line? (boolean option).
void CEntornVGIView::OnOcultacionsBackline()
{
// TODO: Add your command controller here
	back_line = !back_line;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateOcultacionsBackline(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (back_line) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					8. LIGHTING												 */
/* ------------------------------------------------------------------------- */

// LIGHTING: Fixed source light? (boolean option).
void CEntornVGIView::OnIluminacioLlumfixe()
{
// TODO: Add your command controller here
	ifixe = !ifixe;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateIluminacioLlumfixe(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (ifixe) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LIGHTING: WIREFRAMES
void CEntornVGIView::OnIluminacioFilferros()
{
// TODO: Add your command controller here
	ilumina = FILFERROS;
	test_vis = false;		oculta = false;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateIluminacioFilferros(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (ilumina == FILFERROS) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LIGHTING: FLAT
void CEntornVGIView::OnIluminacioPlana()
{
// TODO: Add your command controller here
	ilumina = PLANA;
	test_vis = false;		oculta = true;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);

}

void CEntornVGIView::OnUpdateIluminacioPlana(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (ilumina == PLANA) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LIGHTING: GOURAUD
void CEntornVGIView::OnIluminacioGouraud()
{
// TODO: Add your command controller here
	ilumina = GOURAUD;
	test_vis = false;		oculta = true;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);

}

void CEntornVGIView::OnUpdateIluminacioGouraud(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (ilumina == GOURAUD) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LIGHTING->EMISSION MATERIAL REFLECTIVITY: Enable or disable of material emission reflectivity.
void CEntornVGIView::OnMaterialEmissio()
{
// TODO: Add your command controller here
	sw_material[0] = !sw_material[0];

	if (shader_menu != CAP_SHADER) {
		// Switch on OpenGL context (from this point accept OpenGL commands are accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[0]"), sw_material[0]);
		// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateMaterialEmissio(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (sw_material[0])	pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// ILUMINACIÓ->AMBIENT MATERIAL REFLECTIVITY: Enable or disable of material ambient reflectivity.
void CEntornVGIView::OnMaterialAmbient()
{
// TODO: Add your command controller here
	sw_material[1] = !sw_material[1];

	if (shader_menu != CAP_SHADER) {
		// Switch on OpenGL context (from this point accept OpenGL commands are accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[1]"), sw_material[1]);
		// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateMaterialAmbient(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (sw_material[1])	pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LIGHTING->DIFFUSE MATERIAL REFLECTIVITY: Enable or disable of material diffuse reflectivity.
void CEntornVGIView::OnMaterialDifusa()
{
// TODO: Add your command controller here
	sw_material[2] = !sw_material[2];

	if (shader_menu != CAP_SHADER) {
		// Switch on OpenGL context (from this point accept OpenGL commands are accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[2]"), sw_material[2]);
		// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateMaterialDifusa(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (sw_material[2])	pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LIGHTING->SPECULAR MATERIAL REFLECTIVITY: Enable or disable the specular material reflectivity.
void CEntornVGIView::OnMaterialEspecular()
{
// TODO: Add your command controller here
	sw_material[3] = !sw_material[3];

	if (shader_menu != CAP_SHADER) {
		// Switch on OpenGL context (from this point accept OpenGL commands are accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
		// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[3]"), sw_material[3]);
		// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateMaterialEspecular(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (sw_material[3])	pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LIGHTING: Textures?: Enable (TRUE) or disable (FALSE) of texturing effect.
void CEntornVGIView::OnIluminacioTextures()
{
// TODO: Add your command controller here
	textura = !textura;

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateIluminacioTextures(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (textura) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LIGHTING->PREDEFINED TEXTURES
// WOOD Texture
// MARBLE Texture
// METAL Texture


void CEntornVGIView::OnIluminacioTexturaFitxerimatge()
{
// TODO: Add your command controller here
	CString nomf;
	t_textura = FITXERIMA;

	// GMS Environment:  To open file dialog to read a file name (*.JPG files )
	CFileDialog openTextur(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("JPG Files(*.jpg)|*.jpg|Error Files (*.err)|*err|All Files (*.*)|*.*||"));;

	if (openTextur.DoModal() != IDOK)	return;                 // stay with old data file
	else nomf = openTextur.GetPathName();

// GMS Environment:  We have here a CString variable 'nom' that contains the path and name of the file selected.

// GMS Environment:  To convert the variable CString 'nom' to the variable char '*nomfitx', 
//		that is compatible with the functions to load fractal files.
	char *nomfitx = CString2Char(nomf);

// Switch on OpenGL context (from this point accept OpenGL commands are accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	loadIMA(nomfitx, 0);

// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateIluminacioTexturaFitxerimatge(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (t_textura == FITXERIMA) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					9. LIGHTS												 */
/* ------------------------------------------------------------------------- */

// LIGHTS: Enable / Disable Ambient Light
void CEntornVGIView::OnLlumsLlumambient()
{
// TODO: Add your command controller here
	llum_ambient = !llum_ambient;

	if (shader_menu != CAP_SHADER) {
		// Switch on OpenGL context (from this point accept OpenGL commands are accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[1]"), (llum_ambient && sw_material[1]));
		// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateLlumsLlumambient(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (llum_ambient) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// LIGHTS: Enable / Disable of light #0 (GL_LIGHT0)
void CEntornVGIView::OnLlumsLlum0()
{
// TODO: Add your command controller here
	llumGL.encesa = !llumGL.encesa;

	if (shader_menu != CAP_SHADER) {
		// Switch on OpenGL context (from this point accept OpenGL commands are accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
// Pas màscara llums
		glUniform1i(glGetUniformLocation(shader_program, "sw_lights[0]"), llumGL.encesa);
		// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
		wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);
	}

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}


void CEntornVGIView::OnUpdateLlumsLlum0(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (llumGL.encesa) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


/* ------------------------------------------------------------------------- */
/*					10. SHADERS												 */
/* ------------------------------------------------------------------------- */

// SHADERS: Disable all shaders
void CEntornVGIView::OnShadersSense()
{
// TODO: Add your command controller here
	shader_menu = CAP_SHADER;
	releaseAllShaders();

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}

void CEntornVGIView::OnUpdateShadersSense(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (shader_menu == CAP) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// SHADERS: Enable a GOURAUD shader (gouraud.vert, gouraud.frag)
void CEntornVGIView::OnShadersGouraud()
{
// TODO: Add your command controller here
	bool apagat = false;
	if (sw_shader) releaseAllShaders();
	else sw_shader = true;
	shader_menu = GOURAUD_SHADER;

// Switch on OpenGL context (from this point accept OpenGL commands are accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	shader_program = initializeShaders(_T("./shaders/gouraud_shdrML"));

	glUniform1i(glGetUniformLocation(shader_program, "texture"), textura);
	glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));
// Pas màscara reflectivitat
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[0]"), sw_material[0]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[1]"), sw_material[1]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[2]"), sw_material[2]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[3]"), sw_material[3]);
//Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[0]"), llumGL.encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[1]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[2]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[3]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[4]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[5]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[6]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[7]"), apagat);

// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}


void CEntornVGIView::OnUpdateShadersGouraud(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (shader_menu == GOURAUD_SHADER) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}


// SHADERS: Enable a PHONG shader (phong.vert, phong.frag)
void CEntornVGIView::OnShadersPhong()
{
// TODO: Add your command controller here
	bool apagat=false;
	if (sw_shader) releaseAllShaders();		// Eliminar Programs previs
		else sw_shader = true;
	shader_menu = PHONG_SHADER;

// Switch on OpenGL context (from this point accept OpenGL commands are accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

// Carregar shaders phong_shdrML.vert i phong_shdrML.frag en Program shader_program
	shader_program = initializeShaders(_T("./shaders/phong_shdrML"));	

// Linkar uniform variables amb variables C++
	glUniform1i(glGetUniformLocation(shader_program, "texture"), textura);
	glUniform1i(glGetUniformLocation(shader_program, "texture0"), GLint(0));
// Pas màscara reflectivitat
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[0]"), sw_material[0]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[1]"), sw_material[1]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[2]"), sw_material[2]);
	glUniform1i(glGetUniformLocation(shader_program, "sw_intensity[3]"), sw_material[3]);
//Pas màscara llums
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[0]"), llumGL.encesa);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[1]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[2]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[3]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[4]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[5]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[6]"), apagat);
	glUniform1i(glGetUniformLocation(shader_program, "sw_lights[7]"), apagat);

// Switch off OpenGL context (from this point accept OpenGL commands are'nt accepted)
	wglMakeCurrent(m_pDC->GetSafeHdc(), NULL);

// Return to main loop OnPaint() to redraw the scene
	InvalidateRect(NULL, false);
}


void CEntornVGIView::OnUpdateShadersPhong(CCmdUI *pCmdUI)
{
// TODO: Agregue aquí su código de controlador de IU para actualización de comandos
	if (shader_menu == PHONG_SHADER) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
}

/* ------------------------------------------------------------------------- */
/*   BUTTONS RESOURCES (ToolBar) OF THE APPLICATION                          */
/* ------------------------------------------------------------------------- */
// You must include here the callbacks of toolabr buttons


// ---------------- Entorn GMS: Local functions of EntornVGIView.cpp

// Log2: To calculate the base 2 log function.
int CEntornVGIView::Log2(int num)
{
	int tlog;

	if (num >= 8192) tlog = 13;
	else if (num >= 4096) tlog = 12;
	else if (num >= 2048) tlog = 11;
	else if (num >= 1024) tlog = 10;
	else if (num >= 512) tlog = 9;
	else if (num >= 256) tlog = 8;
	else if (num >= 128) tlog = 7;
	else if (num >= 64) tlog = 6;
	else if (num >= 32) tlog = 5;
	else if (num >= 16) tlog = 4;
	else if (num >= 8) tlog = 3;
	else if (num >= 4) tlog = 2;
	else if (num >= 2) tlog = 1;
	else tlog = 0;

	return tlog;
}


// CString2char: Function to convert the CString variable to a char * variable for filenames 
char * CEntornVGIView::CString2Char(CString entrada)
{
//char * par_sortida=" ";
// Variable de tipus CString 'entrada' conté un string tipus CString
//-------------------------------------------------------------------------------------
// Conversió de la variable CString entrada a la variable char *sortida, 
//	compatible amb la funció Carregar3DS, de càrrega de fitxers 3DS
//	char * nomfitx = (char *)(LPCTSTR)nom;

// Conversió variable w_char --> char *
//	char *nomf="  ";
//	wcstombs(nomf,strFileName.GetBuffer(3),90);
//	char *nomf = reinterpret_cast<char *> (nom.GetBuffer(3));

	size_t origsize = wcslen(entrada.GetBuffer(3)) + 1;
	size_t convertedChars = 0;

// Use a multibyte string to append the type of string
// to the new string before displaying the result.
	char strConcat[] = " (char *)";
	size_t strConcatsize = (strlen(strConcat) + 1) * 2;

// Allocate two bytes in the multibyte output string for every wide
// character in the input string (including a wide character
// null). Because a multibyte character can be one or two bytes,
// you should allot two bytes for each character. Having extra
// space for the new string is not an error, but having
// insufficient space is a potential security problem.
	const size_t newsize = origsize * 2;
// The new string will contain a converted copy of the original
// string plus the type of string appended to it.
//	char *nomfitx = new char[newsize + strConcatsize];
	char *par_sortida = new char[newsize + strConcatsize];

// Put a copy of the converted string into nstring
	wcstombs_s(&convertedChars, par_sortida, newsize, entrada.GetBuffer(3), _TRUNCATE);
// append the type of string to the new string.
//----------------------------------------------------------------------------------

// Variable de tipus char *nomfitx conté el nom del fitxer seleccionat
	return par_sortida;
}


// Refl_MaterialOff: Enable Material Reflexion
void CEntornVGIView::Refl_MaterialOff()
{
	sw_material_old[0] = sw_material[0];	sw_material[0] = false;
	sw_material_old[1] = sw_material[1];	sw_material[1] = false;
	sw_material_old[2] = sw_material[2];	sw_material[2] = false;
	sw_material_old[3] = sw_material[3];	sw_material[3] = false;
}

// Refl_MaterialOn: Disable Material Reflexion
void CEntornVGIView::Refl_MaterialOn()
{
	sw_material[0] = sw_material_old[0];
	sw_material[1] = sw_material_old[1];
	sw_material[2] = sw_material_old[2];
	sw_material[3] = sw_material_old[3];
}

