//******** PRACTICAL WORK GRAPHICAL & MULTIMEDIA SYSTEMS (Engineering School - UAB)
//******** Basic Environment VS2013 MULTIWINDOW with MFC interface and Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (February 2018)
// material.h : interface de material.cpp
//

#ifndef GLMATERIAL_H
#define GLMATERIAL_H

#include "stdafx.h"

//-------------- GC2: Màxim nombre de Materials
#define MAX_MATERIALS  11

//Tipus de materials
#define MAT_CAP 		 -1
#define	MAT_DEFAULT       0
#define MAT_BASE          1
#define MAT_METALL_OBSCUR 2 
#define	MAT_VIDRE		  3
#define MAT_GOMA_NEGRA    4  
#define MAT_GESPA         5
#define MAT_METALL_GROC   6
#define MAT_LLUM_VERMELLA 7
#define MAT_LLUM_GROGA    8
#define MAT_LLUM_VERDA    9
#define MAT_FITXER	     10


/* ------------------------------------------------------------------------- */
/*                            Material Functions                             */
/* ------------------------------------------------------------------------- */

// Select Materials
void SeleccionaMaterial(int quin,bool sw_mater[4]);
void SeleccionaColor(bool col, bool sw_mater[4], CColor &c_obj);
void SeleccionaMaterialiColor(int quin,bool col,bool sw_mater[4],CColor &c_obj);

bool llegir_Material(CString filename,int index);
#endif