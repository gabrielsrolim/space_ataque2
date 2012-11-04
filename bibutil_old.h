#ifndef __BIBUTIL_H
#define __BIBUTIL_H

//*****************************************************
//
// bibutil.h
// Cont�m as estruturas e rotinas implementadas na
// biblioteca de rotinas auxiliares (bibutil.cpp).
//
// Isabel H. Manssour e Marcelo Cohen
//
// Este c�digo acompanha o livro
// "OpenGL - Uma Abordagem Pr�tica e Objetiva"
//
//*****************************************************

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include <string>
#include <iostream>
#include <vector>

extern "C" {
	#include "jpeglib.h"
}
#ifndef M_PI
#define M_PI 3.1415926
#endif

using namespace std;

// Define a estrutura de uma imagem
struct TEX
{
    std::string nome;		// nome do arquivo carregado
	int ncomp;				// n�mero de componentes na textura (1-intensidade, 3-RGB)
	GLint dimx;				// largura
	GLint dimy;				// altura
	GLuint texid;			// identifi��o da textura em OpenGL
	unsigned char *data;	// apontador para a imagem em si
};

// Define a estrutura de um v�rtice
/*typedef struct {
	GLfloat x,y,z;
} VERT;*/

class VERT
{
public:

    // Constructors
    VERT() : x(0), y(0), z(0)
	{}

	VERT(float vx, float vy, float vz)
	{
		x = vx;
		y = vy;
		z = vz;
	}

	VERT(const VERT& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	// Destructor
	~VERT() {}

	// A minimal set of vector operations
	VERT operator * (float mult) const // result = this * arg
	{
		return VERT(x * mult, y * mult, z * mult);
	}

	VERT operator + (const VERT& v) const // result = this + arg
	{
		return VERT(x + v.x, y + v.y, z + v.z);
	}

	VERT operator - (const VERT& v) const // result = this - arg
	{
		return VERT(x - v.x, y - v.y, z - v.z);
	}

    friend ostream& operator << (ostream& out, const VERT& v)
    {
        return out << "(" << v.x << " ," << v.y << " ," << v.z << ")";
    }

	float x, y, z;
};

// Define a estrutura de uma face
struct FACE {
	GLint nv;		// n�mero de v�rtices na face
	std::vector<GLint> vert;	// �ndices dos v�rtices
	std::vector<GLint> norm;	// �ndices das normais
	std::vector<GLint> tex;		// �ndices das texcoords
	GLint mat;		// �ndice para o material (se houver)
	//GLint texid;	// �ndice para a textura (se houver)
};

// Define a estrutura de uma coordenada
// de textura (s,t,r) - r n�o � usado
struct TEXCOORD {
	GLfloat s,t,r;
};

// Define a estrutura de um modelo 3D
struct MESH {
    std::string id;             // nome do modelo
	GLint dlist;				// display list, se houver
	std::vector<VERT> vertices;
	std::vector<VERT> normais;
	std::vector<FACE> faces;
	std::vector<TEXCOORD> texcoords;

	// Inicializa novo mesh.
    MESH()
    {
        dlist = -1;	// sem display list
    }
};

// Define a estrutura de um objeto (um conjunto de modelos)
struct OBJ {
	std::vector<MESH*> meshes;  // lista com um ou mais modelos
    bool normais_por_vertice;	// true se houver normais por v�rtice
	bool tem_materiais;			// true se houver materiais
	GLint textura;				// cont�m a id da textura a utilizar, caso o objeto n�o tenha textura associada

    // Procura e retorna ponteiro para o modelo ou NULL se n�o achar
	MESH* procuraModelo(std::string nome)
	{
	    for(int o=0; o<meshes.size(); o++)
            if(meshes[o]->id == nome)
                return meshes[o];
        return NULL;
	}
};

// Define um material
struct MAT {
    std::string nome;	// Identifica��o do material
	GLfloat ka[4];	// Ambiente
	GLfloat kd[4];	// Difuso
	GLfloat ks[4];	// Especular
	GLfloat ke[4];	// Emiss�o
	GLint texid;    // id da textura (ou -1 se n�o houver)
	GLfloat spec;	// Fator de especularidade
};

// Prot�tipos das fun��es
// Fun��es para c�lculos diversos
void Normaliza(VERT &norm);
void ProdutoVetorial (VERT &v1, VERT &v2, VERT &vresult);
void VetorNormal(VERT vert1, VERT vert2, VERT vert3, VERT &n);
void RotaZ(VERT &in, VERT &out, float ang);
void RotaY(VERT &in, VERT &out, float ang);
void RotaX(VERT &in, VERT &out, float ang);

// Fun��es para carga e desenho de objetos
OBJ *CarregaObjeto(std::string nomeArquivo, bool mipmap);
void CriaDisplayList(OBJ *obj);
void DesabilitaDisplayList(OBJ *ptr);
void DesenhaObjeto(OBJ* obj);
void DesenhaMesh(MESH* mesh, bool temMateriais=false, bool normaisPorVertice=false, GLint textura=-1);
void SetaModoDesenho(char modo);

// Fun��es para libera��o de mem�ria
void LiberaObjeto(OBJ *obj);
void LiberaMateriais();

// Fun��es para c�lculo e exibi��o da taxa de quadros por segundo
float CalculaQPS(void);
void Escreve2D(float x, float y, const unsigned char *str);

// Fun��es para c�lculo de normais
void CalculaNormaisPorFace(OBJ *obj);
void CalculaNormaisPorFace(MESH *mesh);

// Fun��es para manipula��o de texturas e materiais
TEX *CarregaTextura(std::string arquivo, bool mipmap);
TEX *CarregaTexturasCubo(std::string arquivo, bool mipmap);
void SetaFiltroTextura(GLint tex, GLint filtromin, GLint filtromag);
MAT *ProcuraMaterial(std::string nome);
TEX *CarregaJPG(std::string filename, bool inverte=true);

void setaDesenhaNormais(bool flag);
// Constantes utilizadas caso n�o existam em GL/gl.h
#ifndef GL_ARB_texture_cube_map
# define GL_NORMAL_MAP					0x8511
# define GL_REFLECTION_MAP				0x8512
# define GL_TEXTURE_CUBE_MAP			0x8513
# define GL_TEXTURE_BINDING_CUBE_MAP	0x8514
# define GL_TEXTURE_CUBE_MAP_POSITIVE_X	0x8515
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_X	0x8516
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Y	0x8517
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	0x8518
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Z	0x8519
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	0x851A
# define GL_PROXY_TEXTURE_CUBE_MAP		0x851B
# define GL_MAX_CUBE_MAP_TEXTURE_SIZE	0x851C
#endif

#ifndef __FREEGLUT_EXT_H__
// Fun��o para desenhar um texto na tela com fonte bitmap
void glutBitmapString(void *fonte,const unsigned char *texto);
#endif

//Fun��o para escalar o objeto
void escalaObjeto(OBJ *obj, float scaleFactor);
#endif

