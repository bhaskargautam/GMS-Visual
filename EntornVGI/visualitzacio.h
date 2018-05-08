//******** GRAPHICS & MULTIMEDIA SYSTEMS (Engineering School - UAB)
//******** Basic Environment VS2013 MULTIWINDOW with MFC interface and Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Febrer 2018)
// visualitzacio.h: Visualitzacio interface
//

#ifndef GLVIS_H
#define GLVIS_H

/* ------------------------------------------------------------------------- */
/*                                Functions                                  */
/* ------------------------------------------------------------------------- */

// GMS Environment: Iluminació (Lighting)
void Iluminacio(char ilumin,bool ifix,bool ll_amb,LLUM lumin,bool textur,char obj,bool bc_lin,int step);

// GMS Environment: orthographic Projection functions;
void Projeccio_Orto(int minx, int minyy, GLfloat w, GLfloat h);
void Vista_Ortografica(int prj, CEsfe3D Raux,CColor col_fons,CColor col_object,char objecte,GLfloat mida,int step,
				bool oculta,bool testv,bool bck_ln,char iluminacio,bool llum_amb,LLUM lumi,bool textur,
				bool textur_map,bool ifix,bool eix);

// GMS Environment: Perspective Projection functions
void Projeccio_Perspectiva(int,int,GLsizei,GLsizei,float);
void Vista_Esferica(CEsfe3D opv,char VPol,bool pant,CPunt3D tr,CPunt3D trF,
				 CColor col_fons,CColor col_object,char objecte,double mida,int step,bool oculta,bool testv,
				 bool bck_ln,char iluminacio,bool llum_amb,LLUM lumi,bool textur,bool textur_map,bool ifix,bool eix);

void Vista_Navega(CPunt3D pv,bool pvb,GLfloat n[3],GLfloat v[3], bool pant,CPunt3D tr,CPunt3D trF,
				  CColor col_fons,CColor col_object,char objecte,bool color,
				 int step,bool oculta,bool testv,
				 bool bck_ln,char iluminacio,bool llum_amb,LLUM lumi,bool textur,bool textur_map,bool ifix,bool eix);

// GMS Environment. instancia: Geom. Transf. Loader
void instancia(bool TR, INSTANCIA tg, INSTANCIA tgF);

// GMS Environment. deixos: World Coordinates drawing
void deixos(void);

// GMS Environment: Screen Clears of different color given by int_fons variable (Fons), Black (Fons) or White (FonsW)
void Fons(CColor int_fons);
void FonsN(void);
void FonsB(void);

// GMS Environment. loadIma: image file loader of graphic formats admitted by DevIL/OpenIL library as a texture
bool loadIMA(char *filename, int texID);

// GMS Environment: Textures of the Truck object
void Init_Textures();

//----------------- SOURCE CODE OF GLU LIBRARY FUNCTIONS 
#define __glPi 3.14159265358979323846

void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
	GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
	GLdouble upz);

void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

static void __gluMakeIdentityf(GLfloat m[16]);
static void __gluMakeIdentityd(GLdouble m[16]);
static void normalize(float v[3]);
static void cross(float v1[3], float v2[3], float result[3]);
#endif