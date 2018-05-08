//******** PRACTICAL WORK GRAPHICAL & MULTIMEDIA SYSTEMS (Engineering School - UAB)
//******** Basic Environment VS2013 MULTIWINDOW with MFC interface and Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (February 2018)
// material.cpp : Functions to define the object reflectivity properties.

#include "stdafx.h"
#include "material.h"

// Types of materials that can be selected in SeleccionaMaterial function
MATERIAL  materials[MAX_MATERIALS] =
{
	{//Default
		{0.2f, 0.2f, 0.2f, 1.0f},	// Ambient
		{0.8f, 0.8f, 0.8f, 1.0f},	// Difusse
		{0.0f, 0.0f, 0.0f, 1.0f},	// Specular
		{0.0f, 0.0f, 0.0f, 1.0f},	// Emission
		0							// Shinniness
	},
	{//Material base
		{0, 0, 0.75f, 1},
		{0, 0, 0.75f, 1},
		{0.0f, 0.5f, 1, 1},
		{0, 0, 0, 1},
		20
	},

	{//Metall Obscur
		{0, 0, 0, 1},
		{0.5f, 0.5f, 0.5f, 1},
		{0.7f, 0.7f, 0.7f, 1},
		{0, 0, 0, 1},
		100
	},

	{//Vidre
		{0.2f, 0.65f, 0.9f, 0.5},
		{0.3f, 0.75f, 1, 0.5},
		{1, 1, 1, 1},
		{0, 0, 0, 1},
		50
	},

	{//Goma Negra
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0.3f, 0.3f, 0.3f, 1},
		{0, 0, 0, 1},
		20
	},

	{//Gespa
		{0, 0.2f, 0, 1},
		{0.1f, 0.5f, 0.2f, 1},
		{0.1f, 0.5f, 0.2f, 1},
		{0, 0, 0, 1},
		0
	},

	{//Metall Groc
		{0.3f, 0.3f, 0.1f, 1},
		{1, 1, 0.25f, 1},
		{1, 1, 0.5f, 1},
		{0, 0, 0, 1},
		128
	},

	{//Llum vermella
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0.5f, 0.5f, 1},
		{1, 0, 0, 1},
		128
	},

	{//Llum Groga
		{1.0f, 1.0f, 0, 1},
		{1, 1.0f, 0, 1},
		{1, 1.0f, 0, 1},
		{1, 0.8f, 0, 1},
		128
	},

	{//Llum verda
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0.5f, 1, 0.5f, 1},
		{0, 1, 0, 1},
		128
	},

	{// Fitxer
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		0
	}

};


/******************************************************************************\
|* SeleccionaMaterialiColor                                                   *|
|* Select un of materials   	                                              *|
\******************************************************************************/
void SeleccionaMaterialiColor(int quin,bool col,bool sw_mater[4],CColor &c_obj)
{
	const float zeros[4]={0.0f,0.0f,0.0f,1.0f};
	const float uns[4]={1.0f,1.0f,1.0f,1.0f};
//	float color[4]={0.0f,0.0f,0.0f,1.0f};

	if (!col) {
		float color[4] = { c_obj.r, c_obj.g, c_obj.b, c_obj.a };
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glColor4fv(color);
		glMaterialfv(GL_FRONT, GL_SPECULAR, color);
		glMaterialf(GL_FRONT, GL_SHININESS, 128);
		glColor4fv(color);
	}
	else if ((quin>=0) && (quin<MAX_MATERIALS))	
		{	
// Light Emission component of the material
			glColorMaterial(GL_FRONT,GL_EMISSION);
			if (sw_mater[0]) glColor4fv(materials[quin].emission);
				else glColor4fv(zeros);
			//glMaterialfv(GL_FRONT, GL_EMISSION, materials[quin].emission);

// Ambient reflectivity coefficient of the material
			glColorMaterial(GL_FRONT,GL_AMBIENT);
			if (sw_mater[1]) glColor4fv(materials[quin].ambient);
				else glColor4fv(zeros);
			//glMaterialfv(GL_FRONT, GL_AMBIENT, materials[quin].ambient);

// Diffuse reflectivity coefficient of the material
			glColorMaterial(GL_FRONT,GL_DIFFUSE);
			if (sw_mater[2]) glColor4fv(materials[quin].diffuse);
					// glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[quin].diffuse);	// Difuse Reflexion Coeff.
				else glColor4fv(zeros);
					// glMaterialfv(GL_FRONT, GL_DIFFUSE, zeros);

// Coeficient de reflectivitat especular del material
			glColorMaterial(GL_FRONT,GL_SPECULAR);
			if (sw_mater[3]){	glColor4fv(materials[quin].specular);
						//glMaterialfv(GL_FRONT, GL_SPECULAR, materials[quin].specular);	// Specular Reflexion Coeff.
						glMaterialf(GL_FRONT, GL_SHININESS, materials[quin].shininess);
					}
				else {	glColor4fv(zeros);
						//glMaterialfv(GL_FRONT, GL_SPECULAR, zeros);
						glMaterialf(GL_FRONT, GL_SHININESS, 200);
					}
		}
	else {
		// Light Emission component of the material
		glColorMaterial(GL_FRONT, GL_EMISSION);
		if (sw_mater[0]) {
			float color[4] = { c_obj.r, c_obj.g, c_obj.b, c_obj.a };
			glMaterialfv(GL_FRONT, GL_EMISSION, color);
			//glColor4fv(color);
		}
		else {	glMaterialfv(GL_FRONT, GL_EMISSION, zeros);
				glColor4fv(zeros);
			}

		// Ambient reflectivity coefficient of the material
		glColorMaterial(GL_FRONT, GL_AMBIENT);
		if (sw_mater[1]) {
			float color[4] = { c_obj.r, c_obj.g, c_obj.b, c_obj.a };
			glMaterialfv(GL_FRONT, GL_AMBIENT, color);
			glColor4fv(color);

		}
		else {	glMaterialfv(GL_FRONT, GL_AMBIENT, zeros);
				glColor4fv(zeros);
			}
		// Diffuse reflectivity coefficient of the material
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		if (sw_mater[2]) {
			float color[4] = { c_obj.r, c_obj.g, c_obj.b, c_obj.a };
			glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
			glColor4fv(color);
		}
		else {	glMaterialfv(GL_FRONT, GL_DIFFUSE, zeros);
				glColor4fv(zeros);
			}

		// Specular Reflectivity coefficient of the material
		glColorMaterial(GL_FRONT, GL_SPECULAR);
		if (sw_mater[3]) {
			float color[4] = { c_obj.r, c_obj.g, c_obj.b, c_obj.a };
			glMaterialfv(GL_FRONT, GL_SPECULAR, color);
			glMaterialf(GL_FRONT, GL_SHININESS, 128);
			glColor4fv(color);
		}
		else {	glMaterialfv(GL_FRONT, GL_SPECULAR, zeros);
				glMaterialf(GL_FRONT, GL_SHININESS, 0);
				glColor4fv(zeros);
			}
		/*
		glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
		if (sw_mater[2]) {	float color[4]={c_obj.r,c_obj.g,c_obj.b,c_obj.a};
		glColor4fv(color);
		}
		else glColor4fv(zeros);
		*/
	}

}


/******************************************************************************\
|* SeleccionaMaterial                                                         *|
|* Select one of the materials	                                              *|
\******************************************************************************/
void SeleccionaMaterial(int quin,bool sw_mater[4])
{
	const float zeros[4]={0.0f,0.0f,0.0f,1.0f};
	const float uns[4]={1.0f,1.0f,1.0f,1.0f};
//	float color[4]={0.0f,0.0f,0.0f,1.0f};

	if ((quin>=0) && (quin<MAX_MATERIALS))	
		{	
// Light Emission component of the material
			glColorMaterial(GL_FRONT,GL_EMISSION);
			if (sw_mater[0]) glColor4fv(materials[quin].emission);
				else glColor4fv(zeros);
			//glMaterialfv(GL_FRONT, GL_EMISSION, materials[quin].emission);

// Ambient Emission component of the material
			glColorMaterial(GL_FRONT,GL_AMBIENT);
			if (sw_mater[1]) glColor4fv(materials[quin].ambient);
				else glColor4fv(zeros);
			//glMaterialfv(GL_FRONT, GL_AMBIENT, materials[quin].ambient);

// Diffuse Emission component of the material
			glColorMaterial(GL_FRONT,GL_DIFFUSE);
			if (sw_mater[2]) glColor4fv(materials[quin].diffuse);
					// glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[quin].diffuse);	// Coef. Reflexió Difusa
				else glColor4fv(zeros);
					// glMaterialfv(GL_FRONT, GL_DIFFUSE, zeros);

// Specular Emission component of the material
			glColorMaterial(GL_FRONT,GL_SPECULAR);
			if (sw_mater[3]){	glColor4fv(materials[quin].specular);
						//glMaterialfv(GL_FRONT, GL_SPECULAR, materials[quin].specular);	// Coef. Reflexió Especular
						glMaterialf(GL_FRONT, GL_SHININESS, materials[quin].shininess);
					}
				else {	glColor4fv(zeros);
						//glMaterialfv(GL_FRONT, GL_SPECULAR, zeros);
						glMaterialf(GL_FRONT, GL_SHININESS, 0);
					}
			glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
		}

}


/******************************************************************************\
|* SeleccionaColor                                                            *|
|* Select one color           	                                              *|
\******************************************************************************/
void SeleccionaColor(bool col, bool sw_mater[4], CColor &c_obj)
{
	const float zeros[4]={0.0f,0.0f,0.0f,1.0f};
//	const float uns[4]={1.0f,1.0f,1.0f,1.0f};
	float color[4] = { c_obj.r, c_obj.g, c_obj.b, c_obj.a };

	if (!col) {
		float color[4] = { c_obj.r, c_obj.g, c_obj.b, c_obj.a };
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glColor4fv(color);
		glMaterialfv(GL_FRONT, GL_SPECULAR, color);
		glMaterialf(GL_FRONT, GL_SHININESS, 128);
		glColor4fv(color);
		}
		else {
				// Light Emission component of the material
				glColorMaterial(GL_FRONT, GL_EMISSION);
				if (sw_mater[0]) {	glMaterialfv(GL_FRONT, GL_EMISSION, color);
									glColor4fv(color);
								}
					else {	glMaterialfv(GL_FRONT, GL_EMISSION, zeros);
							glColor4fv(zeros);
						}

				// Ambient Emission component of the material
				glColorMaterial(GL_FRONT, GL_AMBIENT);
				if (sw_mater[1]) {	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
									glColor4fv(color);
								}
					else {	glMaterialfv(GL_FRONT, GL_AMBIENT, zeros);
							glColor4fv(zeros);
						}
				// Diffuse Emission component of the material
				glColorMaterial(GL_FRONT, GL_DIFFUSE);
				if (sw_mater[2]) {	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
									glColor4fv(color);
								}
					else {	glMaterialfv(GL_FRONT, GL_DIFFUSE, zeros);
							glColor4fv(zeros);
						}

				// Specular Emission component of the material
				glColorMaterial(GL_FRONT, GL_SPECULAR);
				if (sw_mater[3]) {	glMaterialfv(GL_FRONT, GL_SPECULAR, color);
									glMaterialf(GL_FRONT, GL_SHININESS, 128);
									glColor4fv(color);
								}
					else {	glMaterialfv(GL_FRONT, GL_SPECULAR, zeros);
							glMaterialf(GL_FRONT, GL_SHININESS, 0);
							glColor4fv(zeros);
						}
			}
}


// llegir_Material: To read material properties from a file according to MATERIAL struct
//			It returns MATERIAL struct read and a boolean variable if the file has been found [TRUE] or not [FALSE].
bool llegir_Material(CString filename,int index)
{ 
	FILE *fd;
	char strMessage[255] = { 0 };
	GLfloat auxemis[4],auxamb[4],auxdiff[4],auxspec[4];
	GLfloat auxshin;

// Variables Inicialization
	auxamb[0]=0.0f;			auxamb[1]=0.0f;			auxamb[2]=0.0f;			auxamb[3]=1.0f;
	auxemis[0]=0.0f;		auxemis[1]=0.0f;		auxemis[2]=0.0f;		auxemis[3]=1.0f;
	auxdiff[0]=0.0f,		auxdiff[1]=0.0f;		auxdiff[2]=0.0f;		auxdiff[3]=1.0f;
	auxspec[0]=0.0f;		auxspec[1]=0.0f;		auxspec[2]=0.0f;		auxspec[3]=1.0f;
	auxshin=0.0f;

// To convert w_char --> char * variables
//	wcstombs(nomf,filename.GetBuffer(3),90);
	char *nomf = reinterpret_cast<char *> (filename.GetBuffer(3));

//if ( (fd=fopen(nomf,"rt"))==NULL)			// VS6.0
if ( (fopen_s(&fd,nomf,"rt"))!=0)			// VS2005
	{	
		sprintf_s(strMessage, "Unable to find the file: %s!", nomf);
//		MessageBox(NULL, strMessage, "Error", MB_OK);
		LPCWSTR texte1 = reinterpret_cast<LPCWSTR> (strMessage);
		LPCWSTR texte2 = reinterpret_cast<LPCWSTR> ("Error");
		MessageBox(NULL, texte1, texte2, MB_OK);
		return false;
	}

fscanf(fd,"%f %f %f %f \n",&auxemis[0],&auxemis[1],&auxemis[2],&auxemis[3]);
fscanf(fd,"%f %f %f %f \n",&auxamb[0],&auxamb[1],&auxamb[2],&auxamb[3]);
fscanf(fd,"%f %f %f %f \n",&auxdiff[0],&auxdiff[1],&auxdiff[2],&auxdiff[3]);
fscanf(fd,"%f %f %f %f \n",&auxspec[0],&auxspec[1],&auxspec[2],&auxspec[3]);
fscanf(fd,"%f \n",&auxshin);

fclose(fd);

materials[index].emission[0]=auxemis[0];	materials[index].emission[1]=auxemis[1];
materials[index].emission[2]=auxemis[2];	materials[index].emission[3]=auxemis[3];

materials[index].ambient[0]=auxamb[0];		materials[index].ambient[1]=auxamb[1];
materials[index].ambient[2]=auxamb[2];		materials[index].ambient[3]=auxamb[3];

materials[index].diffuse[0]=auxdiff[0];		materials[index].diffuse[1]=auxdiff[1];
materials[index].diffuse[2]=auxdiff[2];		materials[index].diffuse[3]=auxdiff[3];

materials[index].specular[0]=auxspec[0];	materials[index].specular[1]=auxspec[1];
materials[index].specular[2]=auxspec[2];	materials[index].specular[3]=auxspec[3];

materials[index].shininess=auxshin;
return true;

}
