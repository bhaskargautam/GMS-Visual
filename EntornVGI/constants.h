//******** PRACTICAL WORK GRAPHICAL & MULTIMEDIA SYSTEMS (Engineering School - UAB)
//******** Basic Environment VS2013 MULTIWINDOW with MFC interface and Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (February 2018)
// constants.h : Definition of program constants shared to
//				 CEntornVGIView.cpp, visualitzacio.cpp and escena.cpp


#ifndef CONST_H
#define CONST_H

//--------------- GMS: Projection type
#define CAP ' '
#define AXONOM 'A'
#define ORTO 'O'
#define PERSPECT 'P'
#define IMA_PICK 3
#define PASSEIG_V 4


//--------------- GMS: Polar presentation types for world coordinates axes (to Interactive visualization)
#define POLARZ 'Z'
#define POLARY 'Y'
#define POLARX 'X'

//-------------- GMS: Object os scene types (Object pop up)
#define CAMIO 'm'
#define CILINDER 'y'
#define CUBE 'c'
#define SPHERE 'e'
#define O_FRACTAL 'f'
#define HIDROAVIO 'H'
#define ICOSAEDER 'i'
#define ROBOT 'u'
#define TEAPOT 't'
#define TORUS 'o'
#define TRUCK 'T'
#define BOAT 'v'
#define OBJ3DS '3'		// Object in 3DS format
#define OBJOBJ '4'		// Object in OBJ format

//-------------- GMS: Lighting models (Lighting pop up)
#define PUNTS 'P'
#define FILFERROS 'f'
#define PLANA 'p'
#define GOURAUD 'g'

//-------------- GMS: OpenGL precompiled objects with glNewList
#define EIXOS 1			// Axes
#define FRACTAL 2		// Fractal
#define OBJECTE3DS 3	// 3DS Object without textures
#define OBJECTE3DST 4	// 3DS Object with textures
#define OBJECTEOBJ 5	// OBJ Object without textures
#define OBJECTEOBJT 6   // OBJ Object with textures

// -------------- GMS: Definition of near and far values of the Volume of Visualization in Perspective View
const int p_near=1;
const int p_far=50000;

//-------------- GMS: Types of Picking Objects
#define PICKCAP 0
#define PICKFAR 1
#define PICKVAIXELL 2 
#define PICKHIDRO 3 

// -------------- GMS: TEXTURES CONSTANTS
// Maximum number of textures
#define NUM_MAX_TEXTURES 10

// Types of textures
#define CAP ' '
#define FUSTA 'F'
#define MARBRE 'M'
#define METALL 'E'
#define FITXER 'f'
#define FITXERBMP 'f'
#define FITXERIMA 'I'

// It allows to use the functions _s and not the others deprecated (strcpy -> strcpy_s). Defined yet.
//#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

// Delete warning missages from deprecated calls (strcpy -> strcpy_s).
//#define _CRT_SECURE_NO_WARNINGS -> Posat a "Project"->"Properties"->"Configuration properties"->"C/C++"->"Preprocessor"->"Preprocessor definitions".

// -------------- GMS: SHADERS --> Shaders types
#define CAP_SHADER ' '
#define GOURAUD_SHADER 'G'
#define PHONG_SHADER 'P'
#define GOURAUD_TOON 'g'
#define PHONG_TOON 'p'

//--------------- GMS: Pi constant value
const double pi=3.14159;
const double TWOPI = 2 * pi;
const double PID2 = pi / 2;
//const double pi=3.14159265358979323846264338327950288419716939937510f;

// --------------- GMS. TRUCK: Wheel resolution (Number of cylinder and disks divisions)
#define RESOLUCIO_RODA 16


/* --------------- GMS: 2D coordinate structure (Yet defined in Visual Studio 2013)
struct CPoint
{   GLint x;
    GLint y;
};
*/

// --------------- GMS: 3D coordinate structure
struct CPunt3D
{   GLfloat x;
    GLfloat y;
	GLfloat z;
};

// --------------- GMS: R,G,B,A color structure
struct CColor
{   GLfloat r;
    GLfloat g;
	GLfloat b;
	GLfloat a;
};

// --------------- GMS: 3D Spherical coordinate structure
struct CEsfe3D
{   GLfloat R;
    GLfloat alfa;
	GLfloat beta;
};

// --------------- GMS: INSTANCIA (Geometrical Transformations for instancin an object)
struct INSTANCIA
{	CPunt3D VTras;	// Translation Vector
	CPunt3D VScal;	// Scale Vector
	CPunt3D VRota;	// Rotation Vector
};

// --------------- GMS: Coefficients of the attenuation light equation fatt=1/(ad2+bd+c)
struct CAtenua
{   GLfloat a;
    GLfloat b;
	GLfloat c;
};

// --------------- GMS: Coefficients of material reflectivity
struct MATERIAL
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat emission[4];
	GLfloat shininess;
};

// --------------- GMS: Source of light structure
struct LLUM
{	bool encesa;				// Boolean variable that controls if the source light is on [TRUE] or off [FALSE]
	CEsfe3D posicio;			// Source light position in spherical coordinates (to be moved by mouse).
	GLfloat difusa[4];			// Diffuse intensity of light source (r,g,b,a)
	GLfloat especular[4];		// Specular intensity of light source (r,g,b,a)
	CAtenua atenuacio;			// Coefficients of the attenuation light equation fatt=1/(ad2+bd+c)
	bool restringida;			// Boolean variable that indicates if the light source is restricted [TRUE] and according to that the following parameters are valid or not [FALSE].
	GLfloat spotdirection[3];	// Direction of restricted light source (x,y,z).
	GLfloat cutoff;				// Scope angle ofe the restricted light source.
	GLfloat exponent;			// Exponent giving the light attenuation from the central light axe to outside, according to Warn restricted light model.
};

#endif