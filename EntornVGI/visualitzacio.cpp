//******** PRACTICAL WORK GRAPHICAL & MULTIMEDIA SYSTEMS (Engineering School - UAB)
//******** Basic Environment VS2013 MULTIWINDOW with MFC interface and Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (February 2018)
// visualitzacio.cpp : Functions to define visualization transforms to screen
// FUNCIONS:	- Lighting (Iluminacio)
//				- Ortographic View (ProjeccioOrto and Ortografica)
//				- Perspective View in spherical coordinates (ProjeccioPerspectiva,Vista_Esferica)
//              - Perspective View with Walk by means of cursor keys (ProjeccioPerspectiva,Vista_Navega)
//				- To draw the world coordinate axes (deixos)
//				- To clear the background: white (FonsW) and black (Fons)
//				- To load image files compatibles with DevIL/OpenIL libraries to defne as OpenGL texture (loadIMA)
//				- To initialize texture images to Truck object (Init_textures)
//

#include "stdafx.h"
#include "visualitzacio.h"
#include "escena.h"

// TEXTURES: Vector of texture names
GLuint textures[NUM_MAX_TEXTURES]={0,1,2,3,4,5,6,7,8,9};

// Iluminacio: To configure scene lighting
void Iluminacio(char ilumin,bool ifix,bool ll_amb,LLUM lumin,bool textur,bool textur_map,char obj,bool bc_lin,int step)
{   
	//bool ll_amb=true;
	GLfloat angv,angh;

// To configure source light GL_LIGHT0
	GLfloat position[]={0.0,0.0,200.0,1.0};
	GLfloat especular[]={0.0,0.0,0.0,1.0};
    GLfloat ambientg[]={.5,.5,.5,1.0};

// Definition of ambient light according to boolean variable ll_amb
	if (ll_amb) glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientg);
		else glLightModelfv(GL_LIGHT_MODEL_AMBIENT,especular);

	if (lumin.encesa) {	if (!ifix) glLightfv(GL_LIGHT0,GL_POSITION,position);
							else {	// To convert angles from degrees -> radiants
									angv=lumin.posicio.alfa*pi/180;
									angh=lumin.posicio.beta*pi/180;
		
									// To convert Spherical Coordinates->Cartesian Coordinates
									position[0]=lumin.posicio.R*cos(angh)*cos(angv);	
									position[1]=lumin.posicio.R*sin(angh)*cos(angv);	
									position[2]=lumin.posicio.R*sin(angv);
									position[3]=1.0;
									glLightfv(GL_LIGHT0,GL_POSITION,position);			
								}
						glLightfv(GL_LIGHT0,GL_DIFFUSE,lumin.difusa);
						glLightfv(GL_LIGHT0,GL_SPECULAR,lumin.especular);

						// Attenuation factor coefficients f_att=1/(ad2+bd+c)
						glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,lumin.atenuacio.c);
						glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,lumin.atenuacio.b);
						glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,lumin.atenuacio.a);
						if (lumin.restringida) {
							glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,lumin.spotdirection);
							glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,lumin.cutoff);
							glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,lumin.exponent);
							}
						glEnable(GL_LIGHT0);
					}
		else glDisable(GL_LIGHT0);

	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

// To select lighting model according to ilumin variable
	switch(ilumin)
	{
// Wireframes lighting model
	case FILFERROS:
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glDisable(GL_COLOR_MATERIAL);

// Switch off Z-buffer hidden surface algorithm
//		glDisable(GL_DEPTH_TEST);

// Switch off lightng model (set to wireframes)
		glDisable(GL_LIGHTING);
		break;

// Flat lighting model
    case PLANA:
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

// To draw back faces as lines according bc_lin variable
		if (bc_lin) glPolygonMode(GL_BACK,GL_LINE);
		
		glEnable(GL_COLOR_MATERIAL);

// Switch off Z-buffer hidden surface algorithm
//		glEnable(GL_DEPTH_TEST);

// Enable vector normalization
		glEnable(GL_NORMALIZE); 

// Flat lighting model
		glShadeModel(GL_FLAT); 

// Enable lighting
		glEnable(GL_LIGHTING);
		break;

// GOURAUD lighting model
	case GOURAUD:
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

// To draw back faces as lines according bc_lin variable
		if (bc_lin) glPolygonMode(GL_BACK,GL_LINE);
		
		glEnable(GL_COLOR_MATERIAL);

// Switch on Z-buffer hidden surface algorithm
//    	glEnable(GL_DEPTH_TEST);

// Enable vector normalization
		glEnable(GL_NORMALIZE); 
		
// Gouraud lighting model
        glShadeModel(GL_SMOOTH); 

// Activació de la llum
    	glEnable(GL_LIGHTING);
		break;
	}

// Texture configuration.
	if (textur)
	{
// Switch on texture #0.
	  glBindTexture(GL_TEXTURE_2D,textures[0]) ;

	  if ((obj != OBJ3DS) && (obj != OBJOBJ))
	  {
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	
	 
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
  		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	   }

// Pregunta 9 enunciat
	  if (textur_map) glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;
	  else glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

// Generació coordenades textura automàtiques per als objectes Fractals.
/*	  if (obj == O_FRACTAL)
	  {	  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
		  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

// Coeficicients coordenades textura per a fractals
//			      sfPlane =     p1,p2,p3,p4
		  GLfloat sfPlane[4] = {  ,  ,  ,  }; // s=p1*x0+p2*y0+p3*z0+p4*w0 on p=(x0,y0,z0,w0)
		  //	  tfPlane =     p1,p2,p3,p4
		  GLfloat tfPlane[4] = {  ,  ,  ,  }; // t=p1*x0+p2*y0+p3*z0+p4*w0 on p=(x0,y0,z0,w0)

		  glTexGenfv(GL_S, GL_OBJECT_PLANE, sfPlane);
		  glTexGenfv(GL_T, GL_OBJECT_PLANE, tfPlane);

		  glEnable(GL_TEXTURE_GEN_S);
		  glEnable(GL_TEXTURE_GEN_T);
	  }
*/
// Coeficicients generació coordenades textura automàtiques per a resta d'objectes
 /*	  else
	  {	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
	  glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);

// Generation of texture coordinates.
	  GLfloat sPlane[4] = { -1.0f, 1.0f, 0.0f, 1.0f };
	  GLfloat tPlane[4] = { -1.0f, 0.0f, 1.0f, 1.0f };

	  glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
	  glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);

	  glEnable(GL_TEXTURE_GEN_S);
	  glEnable(GL_TEXTURE_GEN_T);
	  }
*/
	  glEnable(GL_TEXTURE_2D) ;
	}
	else { glDisable(GL_TEXTURE_2D);
	       glDisable(GL_TEXTURE_GEN_S);
	       glDisable(GL_TEXTURE_GEN_T);
		}

// GMS Environment:  Creation of the precompiled list to draw the World Coordinates Axes.	
	glNewList(EIXOS,GL_COMPILE);
// Draw the axes without lighting
//	if (ilumin!=FILFERROS) 
		glDisable(GL_LIGHTING);           	
// Draw the axes without textures
//	if (textura) 
		glDisable(GL_TEXTURE_2D);
	// Switch off fog
	glDisable(GL_FOG);
	deixos();
	if (ilumin!=FILFERROS) glEnable(GL_LIGHTING);   
	if (textur) glEnable(GL_TEXTURE_2D);
	glEndList();
}

// -------- Entorn GMS: ORTOGRAPHIC VIEW (Projeccio_Orto and Vista_Ortografica functions)

// Projeccio_Orto: Definition of Viewport and glOrtho 
// ---- GMS Environment: PAY ATTENTION!!. YOU MUST DEFINE FUNCTION PARAMETERS HERE
void Projeccio_Orto(int minx, int miny, GLfloat w, GLfloat h)
{   
// ---- GMS Environment: PAY ATTENTION!!. YOU MUST DEFINE HERE THE PARAMETERS OF ORTOGRAPHIC PROJECTION
//			        LOADED IN PROJECTION MATRIX GL_PROJECTION
	// Viewport definitio

	// Scissor
	glScissor(minx, miny, w, h);	
	glViewport(minx, miny, w, h);
	if (h == 0) h = 1;
	
	// Switch on the GL_PROJECTION matrix structure
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	
	glOrtho((float) -w/16, (float)w/16, (float)-h/16, (float)h/16, p_near, p_far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Vista_Ortogràfica: To define lighting conditions (calling Iluminacio function) and load parameters in GL_MODELVIEW MATRIX (Point of View). 
//                    It calls gluLookAt function according to prj variable (top, front, isometric views).
void Vista_Ortografica(int prj,CEsfe3D opv,CColor col_fons,CColor col_object,char objecte,GLfloat mida,int step, 
				bool oculta,bool testv,bool bck_ln,char iluminacio,bool llum_amb,LLUM lumi,
				bool textur,bool textur_map,bool ifix,bool eix)
{

// Lighting conditions with light source attached to camera (before gluLookAt)
	if (!ifix) Iluminacio(iluminacio,ifix,llum_amb,lumi,textur,textur_map,objecte,bck_ln,step);

// Top, Front, isometric views implamentation 
// ---- GMS Environment: PAY ATTENTION!!. TO SPECIFY THE POINT OF VIEW HERE
//								You must define here the point of View (gluLookAt) according
//								the ortographic (top, front, isometric) view given by prj variable 
//								(see the prj values in case ORTOGRAFICA in OnPaint() function)
	
		//gluLookAt(cam[0], cam[1], cam[2], 0., 0., 0., up[0], up[1], up[2]);
		

	GLfloat Raux = opv.R;
	switch (prj)
	{
	case 0: 
		gluLookAt(0, opv.R, 0, 0, 0, 0, 0, 0, opv.R);
		break;
	case 1:
		gluLookAt(opv.R, 0, 0, 0, 0, 0, 0, 0, opv.R);
		break;
	case 2:
		gluLookAt(0, 0, opv.R, 0, 0, 0, -opv.R, 0, 0);
		break;
	case 3:
		gluLookAt(opv.R, opv.R, opv.R, 0, 0, 0, 0, 0, opv.R);
		break;
	}
		
// Clear the color and depth buffers.
	Fons(col_fons);

// Fixed lighting conditions, not attached to camera (after gluLookAt).
	if (ifix) Iluminacio(iluminacio,ifix,llum_amb,lumi,textur,textur_map,objecte,bck_ln,step);

// Back face culling according to testv variable.
	if (testv) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

// Z-buffer hidden surface algorithm according the oculta variable.
	if (oculta) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

// To draw back faces as lines according bc_lin variable.
	if (bck_ln) glPolygonMode(GL_BACK, GL_LINE);

// To draw the World Coordinates Axes according eix variable. 
	if (eix) glCallList(EIXOS);
}

// --------GMS Environment: PERSPECTIVE VIEW (Projeccio_Perspectiva and Vista_Esferica functions)

// Projeccio_Perspectiva: Viewport and gluPerspective definition.
void Projeccio_Perspectiva(int minx,int miny,GLsizei w,GLsizei h,float zoom)
{

// Viewport definition
	glViewport(minx,miny,w,h);
	if (h==0) h=1;

// Switch on the GL_PROJECTION matrix structure
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

// PERSPECTIVE PROJECTION: We define the Volume of Visualization according to the size of the windows	
// With gluPerspective
	if (w>=h) 
	gluPerspective(60.0,1.0*w/h,p_near,p_far+zoom);
	else gluPerspective(60.0*h/w,1.0*w/h,p_near,p_far+zoom);

// Switch on the GL_MODELVIEW matrix structure
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Vista_Esferica: Here we define gluLookAt with the tool of moving 
//					the Point of View interactively with the mouse, 
//					alse we define lighting conditions.
void Vista_Esferica(CEsfe3D opv,char VPol,bool pant,CPunt3D tr,CPunt3D trF,
				 CColor col_fons,CColor col_object,char objecte,double mida,int step, 
				 bool oculta,bool testv,bool bck_ln,char iluminacio, bool llum_amb,LLUM lumi,bool textur,
				 bool textur_map,bool ifix,bool eix)
{    
	GLfloat cam[3],up[3];

// To convert angles from radiants -> degrees
   	opv.alfa=opv.alfa*pi/180;
    opv.beta=opv.beta*pi/180;

	if(opv.R<1.0) opv.R=1.0;

// To clear color and depth
	Fons(col_fons);

// Camera position and up vector definition
	if (VPol==POLARZ) { cam[0]=opv.R*cos(opv.beta)*cos(opv.alfa);
						cam[1]=opv.R*sin(opv.beta)*cos(opv.alfa);
						cam[2]=opv.R*sin(opv.alfa);		
						up[0]=-cos(opv.beta)*sin(opv.alfa);
						up[1]=-sin(opv.beta)*sin(opv.alfa);
						up[2]=cos(opv.alfa);	}
		else if (VPol==POLARY) {	cam[0]=opv.R*sin(opv.beta)*cos(opv.alfa);
									cam[1]=opv.R*sin(opv.alfa);
									cam[2]=opv.R*cos(opv.beta)*cos(opv.alfa);		
									up[0]=-sin(opv.beta)*sin(opv.alfa);
									up[1]=cos(opv.alfa);
									up[2]=-cos(opv.beta)*sin(opv.alfa);		}
			else {	cam[0]=opv.R*sin(opv.alfa);
					cam[1]=opv.R*cos(opv.beta)*cos(opv.alfa);
					cam[2]=opv.R*sin(opv.beta)*cos(opv.alfa);		
					up[0]=cos(opv.alfa);
					up[1]=-cos(opv.beta)*sin(opv.alfa);
					up[2]=-sin(opv.beta)*sin(opv.alfa);		}

// Fixed lighting conditions, not attached to camera (after gluLookAt)
	if (!ifix) Iluminacio(iluminacio,ifix,llum_amb,lumi,textur,textur_map,objecte,bck_ln,step);

// Pan option: Displacement of the center of screen outside world coordinates origin (pant=true)
	if (pant) glTranslatef(tr.x,tr.y,tr.z);
	glTranslatef(trF.x,trF.y,trF.z);	// Traslació fixada amb la INSERT dins l'opció pan

// Point of View (camera) definition.
   gluLookAt(cam[0],cam[1],cam[2],0.,0.,0.,up[0],up[1],up[2]);

  // Fixed lighting conditions, not attached to camera (after gluLookAt).
	if (ifix) Iluminacio(iluminacio,ifix,llum_amb,lumi,textur,textur_map,objecte,bck_ln,step);

// Back face culling according to testv variable.
	if (testv) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

// Z-buffer hidden surface algorithm according the oculta variable.
	if (oculta) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

// To draw back faces as lines according bc_lin variable.
	if (bck_ln) glPolygonMode(GL_BACK, GL_LINE);

// To draw the World Coordinates Axes according eix variable. 
	if (eix) glCallList(EIXOS);
}


//Vista_Navega: Definition of the parameters gluLookAt explicity (without spherical coordinates).
//                with the tool of moving the Point of View interactively with keyboard to
//               give the sensation to walk in a 3D scene.
void Vista_Navega(CPunt3D pv,bool pvb,GLfloat n[3],GLfloat v[3],bool pant,CPunt3D tr,CPunt3D trF,
				  CColor col_fons,CColor col_object,char objecte,bool color,
				 int step,bool oculta,bool testv,bool bck_ln, char iluminacio,bool llum_amb,LLUM lumi, 
				 bool textur,bool textur_map,bool ifix,bool eix)
{    
//    int i,j;
	double altfar=0;
//	GLfloat pvZ=0.0;

// Clear color and depth buffers.
	Fons(col_fons);

// Fixed lighting conditions, not attached to camera (after gluLookAt)
	if (!ifix) Iluminacio(iluminacio,ifix,llum_amb,lumi,textur,textur_map,objecte,bck_ln,step);

// Pan option: Displacement of the center of screen outside world coordinates origin (pant=true)
	if (pant) glTranslatef(tr.x,tr.y,tr.z);
	glTranslatef(trF.x,trF.y,trF.z);	// Translation fixed with INSERT key inside pan option

// Point of View (camera) definition.
	gluLookAt(pv.x,pv.y,pv.z,n[0],n[1],n[2],v[0],v[1],v[2]);

// Fixed lighting conditions, not attached to camera (after gluLookAt)
	if (ifix) Iluminacio(iluminacio,ifix,llum_amb,lumi,textur,textur_map,objecte,bck_ln,step);

// Back face culling according to testv variable.
	if (testv) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

// Z-buffer hidden surface algorithm according the oculta variable.
	if (oculta) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

// To draw back faces as lines according bc_lin variable.
	if (bck_ln) glPolygonMode(GL_BACK, GL_LINE);

// To draw the World Coordinates Axes according eix variable. 
	if (eix) glCallList(EIXOS);
}


// instancia: To apply Gemetrical Transforms defined in Transforma pop up.
void instancia(bool TR, INSTANCIA tg, INSTANCIA tgF)
{

	// Transformacions geometriques sobre objecte (Traslacio, Rotacions i Escalatge)
	if (TR) glTranslatef(tg.VTras.x, tg.VTras.y, tg.VTras.z);
	glTranslatef(tgF.VTras.x, tgF.VTras.y, tgF.VTras.z);	// Translation fixed with INSERT key in Transform pop up
	if (TR) glRotatef(tg.VRota.x, 1, 0, 0);
	glRotatef(tgF.VRota.x, 1, 0, 0);					// X Rotation fixed with INSERT key in Transform pop up
	if (TR) glRotatef(tg.VRota.y, 0, 1, 0);
	glRotatef(tgF.VRota.y, 0, 1, 0);					// Y Rotation fixed with INSERT key in Transform pop up
	if (TR) glRotatef(tg.VRota.z, 0, 0, 1);
	glRotatef(tgF.VRota.z, 0, 0, 1);					// Z Rotation fixed with INSERT key in Transform pop up
	if (TR) glScalef(tg.VScal.x, tg.VScal.y, tg.VScal.z);
	glScalef(tgF.VScal.x, tgF.VScal.y, tgF.VScal.z);	// Scaling fixed with INSERT key in Transforma pop up
}

// deixos: Drawing of World Coordinates Axes
void deixos()
{
// X Axe (Red)
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
	 glVertex3f(0.0,0.0,0.0);
	 glVertex3f(300.0,0.0,0.0);
	glEnd();

// Y Axe (Green)
    glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
	 glVertex3f(0.0,0.0,0.0);
	 glVertex3f(0.0,300.0,0.0);
	glEnd();

// Z Axe (Blue)
	glColor3f(0.0,1.0,1.0);
	glBegin(GL_LINES);
	 glVertex3f(0.0,0.0,0.0);
	 glVertex3f(0.0,0.0,300.0);
	glEnd();
}

// Fons: Clear the background according the int_fons color
void Fons(CColor int_fons)
{
	glClearColor(int_fons.r,int_fons.g,int_fons.b,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_DEPTH_BUFFER_BIT);
   glFlush();
}

// FonsN: Clear the backgroun in black color
void FonsN()
{
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glFlush();
}

// FonsB: Clear the backgroun in white color
void FonsB()
{
	glClearColor(1.f,1.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_DEPTH_BUFFER_BIT);
   glFlush();
}


// TEXTURES------------------------------------------------------
// loadIMA: This function receives as input the image filename and an 
// integer identifier (0 for the first texture) and creates an OpenGL 
// texture which is stored in the global array 'textures'
// Paràmetres:
//		- filename: Image file. Formats accepted:
//					BMP, JPG, TIFF, TGA, GIF, and others supported by OpenIL/DevIL
//		- texID: Identifier in texture table
bool loadIMA(char * filename, int texID)
{
	FILE *file = NULL;
	int errno = 0;
	char szFilePath[255];

// Open the image file for reading
//  file=fopen(nomf,"r");					// Visual Studio 6.0 function
	errno = fopen_s(&file, filename, "rb");	// Visual 2010 function

// If the file is empty (or non existent) print an error and return false
// if (file == NULL)
	if (errno != 0)
	{
	//	printf("Could not open file '%s'.\n",filename) ;
		return false;
	}

// Close the image file
	fclose(file);

// strcpy_s: To convert variable char* --> char[255]
	strcpy_s(szFilePath, filename); // Copiar path sencer amb nom de fitxer.

// reinterpret_cast: Funció de converió d'una variable char * -> wchar_t *
	wchar_t *nomf = reinterpret_cast <wchar_t *> (szFilePath);

// ilutGLLoadImage: This functions reads image file in nomf varible
//				if it is compatible with DevIL/OpenIL formats (BMP,JPG,GIF,TIF,TGA,etc.)
//				and it defines this image as OpenGL texture and returs the texture identifier in textures[texID].
	textures[texID] = ilutGLLoadImage(nomf);

// If execution arrives here it means that all went well. Return true
	return true;
}


// To initialize image textures for Truck object
void Init_Textures()
{	
	loadIMA("./textures/vent.bmp",1);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	
 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

	loadIMA("./textures/plat.bmp",2);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	
 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

	loadIMA("./textures/reixeta.bmp",3);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	
 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

	loadIMA("./textures/fars.bmp",4);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

	loadIMA("./textures/txapa.bmp",5);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	
 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;

	loadIMA("./textures/metall.bmp",6); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);	
	 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE) ;
}

//----------------- SOURCES GLU LIBRARY

// gluLookAt: Funtion that defines the PV matrix in load it to a OpenGL matrix structure switched on (GL_MODELVIEW or GL_PROJECTION) 
void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
	GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
	GLdouble upz)
{
	float forward[3], side[3], up[3];
	GLfloat m[4][4];

	forward[0] = centerx - eyex;
	forward[1] = centery - eyey;
	forward[2] = centerz - eyez;

	up[0] = upx;
	up[1] = upy;
	up[2] = upz;

	normalize(forward);

	/* Side = forward x up */
	cross(forward, up, side);
	normalize(side);

	/* Recompute up as: up = side x forward */
	cross(side, forward, up);

	__gluMakeIdentityf(&m[0][0]);
	m[0][0] = side[0];
	m[1][0] = side[1];
	m[2][0] = side[2];

	m[0][1] = up[0];
	m[1][1] = up[1];
	m[2][1] = up[2];

	m[0][2] = -forward[0];
	m[1][2] = -forward[1];
	m[2][2] = -forward[2];

	glMultMatrixf(&m[0][0]);
	glTranslated(-eyex, -eyey, -eyez);
}

// gluPerspective: Function that defines the Perspective Projection matrix and load it to a OpenGL matrix 
//					structure switched on (GL_MODELVIEW or GL_PROJECTION)  
void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble m[4][4];
	double sine, cotangent, deltaZ;
	double radians = fovy / 2 * __glPi / 180;

	deltaZ = zFar - zNear;
	sine = sin(radians);
	if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
		return;
	}
	cotangent = cos(radians) / sine;

	__gluMakeIdentityd(&m[0][0]);

	m[0][0] = cotangent / aspect;
	m[1][1] = cotangent;
	m[2][2] = -(zFar + zNear) / deltaZ;
	m[2][3] = -1;
	m[3][2] = -2 * zNear * zFar / deltaZ;
	m[3][3] = 0;
	glMultMatrixd(&m[0][0]);
}


// gluMakeIdentityf: Defines Identity matrix 
static void __gluMakeIdentityf(GLfloat m[16])
{
	m[0 + 4 * 0] = 1; m[0 + 4 * 1] = 0; m[0 + 4 * 2] = 0; m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0; m[1 + 4 * 1] = 1; m[1 + 4 * 2] = 0; m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0; m[2 + 4 * 1] = 0; m[2 + 4 * 2] = 1; m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0; m[3 + 4 * 1] = 0; m[3 + 4 * 2] = 0; m[3 + 4 * 3] = 1;
}

static void __gluMakeIdentityd(GLdouble m[16])
{
	m[0 + 4 * 0] = 1; m[0 + 4 * 1] = 0; m[0 + 4 * 2] = 0; m[0 + 4 * 3] = 0;
	m[1 + 4 * 0] = 0; m[1 + 4 * 1] = 1; m[1 + 4 * 2] = 0; m[1 + 4 * 3] = 0;
	m[2 + 4 * 0] = 0; m[2 + 4 * 1] = 0; m[2 + 4 * 2] = 1; m[2 + 4 * 3] = 0;
	m[3 + 4 * 0] = 0; m[3 + 4 * 1] = 0; m[3 + 4 * 2] = 0; m[3 + 4 * 3] = 1;
}

static void normalize(float v[3])
{
	float r;

	r = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (r == 0.0) return;

	v[0] /= r;
	v[1] /= r;
	v[2] /= r;
}

static void cross(float v1[3], float v2[3], float result[3])
{
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

