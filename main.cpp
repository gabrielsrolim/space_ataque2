#include <cstdlib>
#include <iostream>

#include <GL/glut.h>
#include <math.h>
#include <string.h>

#include <stdlib.h>
#include <string>
#include <vector>

#include "bibutil.h"
#include "importar.h"

//using namespace std;


tPosicao missel_esfera,missel_nave,nave_espacial;
tRegFaces SA_faces;
tRegPoints points;
tEsfera esfera[QTD_ESFERAS];
int width_window=800,height_window=600,num_esfera=0;
int SA_Time=1,dispara=0,dispara_missel_esfera=0,missel_esfera_habilita=0,missel_selecionado,vidas=3,habilita_desenho=1;
double rx = 0.0,ry=0.0,rz=0.0,trans_missel_nave=0.0,trans_ovni=0.0,trans_missel_esfera=0.0,vez=0.0,vira_nave=0;
double z=0.5,cor_esferas=0.0,mov_esferas=0.0,rodar_ovne=0.0;
double trans_ini_nave=-15,escala_missel=0.2;//padrão escala 0.2m translação inicial: -19(eixo y)
double tamanho_vida = 5.0,velovidade_missel_ovni = VELOCIDADE_MISSEL;
double velocidade_ovni = 0.4,pos_lanca_missel_ovni = 0;
bool habilitar_linhas = false;

OBJ *ovni = NULL,*aviao = NULL,*background = NULL;
tObjetoJogos OB_ovni,OB_nave,OB_missel1,OB_missel2[QTD_MISSEL_OVNI];
char /*aviao,*/missel/*,ovni*/;
int num=0,pontos=0; 
int i;
char textpontos[50],textRodada[50];
TEX *imagem;

int teste = 0;



// Desenha um texto na janela GLUT
void Texto(){
	int i;
	glColor3f(0,0,0);     
	// Posição onde o textpontos será colocado
	sprintf(textpontos,"Pontos: %d",pontos);
	glRasterPos3d(10, 0, 0);
	for(i=0; textpontos[i] != '\0'; ++i)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,textpontos[i]);
}

void Texto2(){
	int i;
	glColor3f(1,1,1);     
	// Posição onde o textpontos será colocado
	sprintf(textRodada,"%s","Game Over!");
	glRasterPos3d(-4, 0, 0);
	for(i=0; textRodada[i] != '\0'; ++i)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,textRodada[i]);
	sprintf(textRodada,"%s","Para Encerrar o jogo Aperte 'q'.");
	glRasterPos3d(-4, -2, 0);
	for(i=0; textRodada[i] != '\0'; ++i)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,textRodada[i]);
	sprintf(textRodada,"%s","Para Continuar o jogo Aperte 'e'.");
	glRasterPos3d(-4, -4, 0);
	for(i=0; textRodada[i] != '\0'; ++i)
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,textRodada[i]);
	if(pontos >20){ 
		sprintf(textRodada,"Parabens! Voce Fez %d pontos.",pontos);
		glRasterPos3d(-4, -6, 0);
		for(i=0; textRodada[i] != '\0'; ++i)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,textRodada[i]);    
	}
	
}

void RecomecaJogo(){
	rx = 0.0;ry=0.0;rz=0.0;trans_missel_nave=0.0;trans_missel_esfera=0.0;vez=0.0; dispara=0; mov_esferas=0.0;
	tamanho_vida = 5;pontos=0;
	vidas=3;
	for(i=0;i<QTD_ESFERAS;i++){
		esfera[i].status = DISPONIVEL;
	}
}




void ResetaJogo(int fim){
	rx = 0.0;ry=0.0;rz=0.0;trans_missel_nave=0.0;trans_missel_esfera=0.0;vez=0.0; dispara=0; mov_esferas=0.0;

	if(tamanho_vida<=-5){
		tamanho_vida = 5;
	}
	if(fim){
		habilita_desenho=0;
		Desenha();
		vidas=3;
	}
	
	for(i=0;i<QTD_ESFERAS;i++){
		esfera[i].status = DISPONIVEL;
	}
}


void ResetaNave(){
	rx = 0.0;ry=0.0;vez=0.0; 
}


void iluminacao(void){
	GLfloat luzAmbiente[4] = {1,1,1,1.0};
	GLfloat luzDifusa[4] = {1,1,1,1.0};//cor
	GLfloat luzEspecular[4] = {1.0,1.0,1.0,1.0};//brilho
	GLfloat posicaoLuz[4] = {0.0,0.0,1,0.0};
	
	GLfloat especularidade[4] = {1.0,1.0,1.0,1.0};
	GLint especMaterial = 60;
	
	
	//Define a refletância do material
	glMaterialfv(GL_FRONT,GL_SPECULAR,especularidade);
	
	//Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
	
	//Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,luzAmbiente);
	
	//define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,luzAmbiente);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,luzDifusa);
	glLightfv(GL_LIGHT0,GL_SPECULAR,luzEspecular);
	glLightfv(GL_LIGHT0,GL_POSITION,posicaoLuz);
	
	glEnable(GL_COLOR_MATERIAL);
	
	glEnable(GL_LIGHTING);
	
	glShadeModel(GL_FLAT);
	
	
	//GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0};
	/*GLfloat luzAmbiente[4]={0.5,0.5,0.5,1}; 
	GLfloat luzDifusa[4]={0.5,0.5,0.5,1};	   // "cor" 
	GLfloat luzEspecular[4]={1, 1, 1, 1};// "brilho" 
	GLfloat posicaoLuz[4]={20.0, 0.0, 20, 1};;//{20.0, 0.0, 20, 1};
	GLfloat posicaoLuz2[4]={-20.0, 0.0, 20, 1};;
	
	GLfloat posicaoLuz3[4]={20.0, 0.0, 20, 1};//{20.0, 0.0, 20, 1};
	GLfloat posicaoLuz4[4]={-20.0, 0.0, 20, 1};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={0.2,0.2,0.2,0.1}; 
	GLint especMaterial = 90;

	// Especifica que a cor de fundo da janela sera preta
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);*/
	
	// Habilita o modelo de colorizacao de Gouraud
	//glShadeModel(GL_FLAT);

	/*// Define a refletancia do material 
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentracao do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

// Define a concentracao do brilho
	glMaterialfv(GL_FRONT,GL_DIFFUSE ,luzDifusa);

// Define a concentracao do brilho
	glMaterialfv(GL_FRONT,GL_SPECULAR ,luzEspecular);

	// Ativa o uso da luz ambiente 
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os par�metros da luz de n�mero 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

	glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz2 );

	// Habilita a definicao da cor do material a partir da cor corrente
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminacao
	glEnable(GL_LIGHTING);  
	// Habilita a luz de numero 0
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);*/


	
	
}


void Desenha(){
	float fps;
	char msg[40+1];



	
	/*glPushMatrix();  
	iluminacao();
	glPopMatrix();*/

	glEnable(GL_CULL_FACE); // habilita remoção de superficies escondidas.
	glCullFace(GL_BACK);
	
	glDrawBuffer(GL_BACK);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20.0, 20.0, -20.0, 20.0, 1, 1000.0);

	glMatrixMode(GL_VIEWPORT);
	glViewport(0, 50, width_window, height_window);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0, 0.0, 0.0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	
	glEnable(GL_DEPTH_TEST);  

	if(habilita_desenho){
		glPushMatrix();
		glTranslated(0, 0, -60);
		Esferas();
		desenha_aviao();
		misseis();
		glPopMatrix();  
	}else{
		glPushMatrix();
		glTranslated(0, 0, -60);
		Texto2(); 
		glPopMatrix(); 
	}

	fps = CalculaQPS();
	sprintf(msg,"fps: %f\n",fps);
	//printf(msg);
	Escreve2D(0, 0.005, (unsigned char *)msg);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20.0, 20.0, -20.0, 20.0, 1, 60.0);

	glViewport(0, 0, width_window, 50);
	glPushMatrix();
	glTranslated(0, 0, -20);

	//glDisable(GL_DEPTH_TEST); 
	glMatrixMode(GL_MODELVIEW);

	glDisable(GL_LIGHTING);

	barravida();
	//chances();
	Texto();

	glBegin(GL_QUAD_STRIP);
	//glNormal3d(0.0, 0.0, 1.0);
	glColor3d(0.5, 0.4, 0.3);//vida vermelha
	glVertex3d(-20,20,-10);
	glVertex3d(-20,-20,-10);
	glVertex3d(20,20,-10);
	glVertex3d(20,-20,-10);
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix(); 

	glDisable(GL_CULL_FACE);
}

void desenha_aviao(){
	//Aviao
	glPushMatrix();
	glColor3d(0.234, 0.234,0.234 );

	OB_nave.ponto.x = 0.0;
	OB_nave.ponto.y = -18.4;
	OB_nave.ponto.z = 3.0;
	
	//glTranslated(rx,0, 0);
	OB_nave.ponto.x += rx;
	
	glTranslatef(OB_nave.ponto.x,OB_nave.ponto.y, OB_nave.ponto.z);

	glRotated(90,1,0,0);
	glRotated(-54,1,0,0);

	glRotated(vira_nave,0,0,1);
	glScaled(1.5,1.5,1.5);

	//glCallList(aviao);
	SetaModoDesenho('s');
	//printf("Avião\n");
	//glPushMatrix();
	//glTranslated(1, 2, 1);
	DesenhaObjeto(aviao);
	//glPopMatrix(); 
		if(habilitar_linhas){
	glBegin(GL_LINES);

	glColor3d(1, 0, 0);//x
	glVertex3d(-20,0,0);
	glVertex3d(20,0,0);
	
	glColor3d(0, 1, 0);//y
	glVertex3d(0,-20,0);
	glVertex3d(0,20,0);
	
	glColor3d(0, 0, 1);//Z
	glVertex3d(0,0,-20);
	glVertex3d(0,0,20);
	glEnd();
}
	glPopMatrix(); 

}

void chances(){

	//Vida 1
	if (vidas!=0){
		glPushMatrix();
		glColor3d(0, 0, 1);
		glTranslated(-14,0, 0);
		glScaled(0.5,7,0.8);//glScaled(0.1,1,1);
		glRotated(90,1,0,0);
		glRotated(90,0,1,0);
		SetaModoDesenho('s');
		DesenhaObjeto(aviao);
		glPopMatrix();
	} 
	if(vidas>1){
		//Vida 2
		glPushMatrix();
		glColor3d(0, 0, 1);
		glTranslated(-16.3,0, 0);
		glScaled(0.5,7,0.8);//glScaled(0.1,1,1);
		glRotated(90,1,0,0);
		glRotated(90,0,1,0);
		SetaModoDesenho('s');
		DesenhaObjeto(aviao);
		glPopMatrix();
	}
	if(vidas==3){
		//Vida 3
		glPushMatrix();
		glColor3d(0, 0, 1);
		glTranslated(-18.5,0, 0);
		glScaled(0.5,7,0.8);//glScaled(0.1,1,1);
		glRotated(90,1,0,0);
		glRotated(90,0,1,0);
		SetaModoDesenho('s');
		DesenhaObjeto(aviao);
		glPopMatrix();
	}
}


void barravida(){

	glBegin(GL_QUAD_STRIP);
	glNormal3d(0.0, 0.0, 1.0);
	glColor3d(0, 0, 1);//vida Azul
	glVertex3d(-5,5,1);
	glVertex3d(-5,0,1);
	glVertex3d(tamanho_vida,5,1);
	glVertex3d(tamanho_vida,0,1);
	glEnd();

	glPopMatrix();

	glPushMatrix();


	glBegin(GL_QUAD_STRIP);
	glNormal3d(0.0, 0.0, 1.0);
	glColor3d(1, 0, 0);//vida vermelha
	glVertex3d(-5,5,0);
	glVertex3d(-5,0,0);
	glVertex3d(5,5,0);
	glVertex3d(5,0,0);
	glEnd();

	glPopMatrix();
	

}

void display(void){

	Desenha();

	glutSwapBuffers();
}

void teclasEspeciais(int key, int x, int y){
	switch(key){
	case GLUT_KEY_RIGHT:
		if(vira_nave>=-10){
			vira_nave=-14;            
		}else{
			if(rx>=20.0){
				rx=20;
			}else{
				rx+=VELOCIDADE_DELC_AVIAO;
			}
			
		}
		//vira_nave=-20; 
		glutPostRedisplay();
		//vira_nave=0;
		break;
	case GLUT_KEY_LEFT:
		if(vira_nave<=10){
			vira_nave=14;            
		}else{
			if(rx<=-20.0){
				rx=-20;
			}else{
				rx-=VELOCIDADE_DELC_AVIAO;            
			} 
		}            
		glutPostRedisplay();
		break;
    }
}

void keyboard(unsigned char k,int x,int y){
	switch(k){
	case ' ':
		dispara=1;
		break;
		case'A':
		case'a':
		if(vira_nave<=10){
			vira_nave=14;            
		}else{
			if(rx<=-20.0){
				rx=-20;
			}else{
				rx-=VELOCIDADE_DELC_AVIAO;            
			} 
		}            
		glutPostRedisplay();
		//vira_nave=0;
		break;
		case'S':
		case's':
		ry-=VELOCIDADE_DELC_AVIAO;
		glutPostRedisplay();
		break;
		case'D':
		case'd':
		if(vira_nave>=-10){
			vira_nave=-14;            
		}else{
			if(rx>=20.0){
				rx=20;
			}else{
				rx+=VELOCIDADE_DELC_AVIAO;
			}
			
		}
		//vira_nave=-20; 
		glutPostRedisplay();
		//vira_nave=0;
		break;
		case'W':
		case'w':
		ry+=VELOCIDADE_DELC_AVIAO;
		glutPostRedisplay();
		break;
		case'R':
		case'r':
		RecomecaJogo();
		
		glutPostRedisplay();
		break;
		case'E':
		case'e':
		if(!habilita_desenho){
			sprintf(textpontos,"%s","Pontos: 0");
			habilita_desenho = 1;
			tamanho_vida = 5;
			pontos = 0;  
		}
		
		glutPostRedisplay();
		break;
	case 't':
	case 'T': rz+=VELOCIDADE_DELC_AVIAO;
		glutPostRedisplay();
		break;
	case 'g':
	case 'G': rz-=VELOCIDADE_DELC_AVIAO;
		glutPostRedisplay();
		break;
	case 'm':
	case 'M': glEnable(GL_LIGHT0);
		glutPostRedisplay();
		break;
	case 'n':
	case 'N': glDisable(GL_LIGHT0);
		glutPostRedisplay();
		break;
	case 'q':
	case 'Q': 
		exit(0);
		break;
	case '+':
		teste+=1;
		printf("teste %d\n",teste);
		break;
	case '-':
		teste-=1;
		printf("teste %d\n",teste);
		break;
	case 'h':
    case 'H':
         habilitar_linhas = habilitar_linhas?false:true;
	default:
		printf("%c\n",k);
		break;    
	}
}

void Esferas(){

	if(num_esfera!=0 || cor_esferas!=0){
		num_esfera=0;
		cor_esferas=0;
	}

	glPushMatrix();
	cor_esferas+=0.001;
	glColor3d(0.5, 0.6, 1);
	glRotated(11,1,0,0);
	OB_ovni.ponto.x = 0.0 + trans_ovni;
	OB_ovni.ponto.y = 0.0 ;
	OB_ovni.ponto.z = 0.0;
	glTranslated(OB_ovni.ponto.x,OB_ovni.ponto.y, OB_ovni.ponto.z);
	glRotated(rodar_ovne,0,1,0);//rodar ovne.
	glScaled(2,2,2);
	SetaModoDesenho('s');
	DesenhaObjeto(ovni);
	if(habilitar_linhas){
	glBegin(GL_LINES);
	
	glColor3d(0.5, 0.4, 0.3);//x
	glVertex3d(-20,0,0);
	glVertex3d(20,0,0);
	
	glColor3d(0, 1, 0);//y
	glVertex3d(0,-20,0);
	glVertex3d(0,20,0);
	
	glColor3d(0, 0, 1);//Z
	glVertex3d(0,0,-20);
	glVertex3d(0,0,20);
	glEnd();
 }
	glPopMatrix(); 

	
}


void misseis(){

	//missel nave center
	glPushMatrix();
	glColor3d(1, 0, 0);
	
	OB_missel1.ponto.x = OB_nave.ponto.x;
	OB_missel1.ponto.y = OB_nave.ponto.y + trans_missel_nave - 2;
	OB_missel1.ponto.z = 0;
	
	glTranslated(OB_missel1.ponto.x, OB_missel1.ponto.y, 0);
	glRotated(-90,1,0,0);
	missel_nave.translacao_x = rx;
	missel_nave.translacao_y = trans_missel_nave;

	glScaled(escala_missel,escala_missel,escala_missel);

	glCallList(missel);
	glPopMatrix();

	//missel ovni
	for(i=0;i<QTD_MISSEL_OVNI;i++){
        //printf("habilitado %s dispara %s\n",OB_missel2[i].habilitado?"ok":"nao",OB_missel2[i].dispara_missel_esfera?"ok":"nao");                           
		if(OB_missel2[i].habilitado && OB_missel2[i].dispara_missel_esfera){
			glPushMatrix();
			glColor3d(1, 0, 0);
			/*if(!dispara_missel_esfera){
				OB_missel2[i].ponto.x = OB_ovni.ponto.x;
				}else{
				OB_missel2[i].ponto.x = pos_lanca_missel_ovni;
				}*/
			//printf("trans_missel_esfera %f\n",trans_missel_esfera);
			OB_missel2[i].ponto.y; //= (OB_ovni.ponto.y-trans_missel_esfera);
			OB_missel2[i].ponto.z =  OB_ovni.ponto.z;
			glTranslated(OB_missel2[i].ponto.x,OB_missel2[i].ponto.y,OB_missel2[i].ponto.z);
			//glTranslated(esfera[missel_selecionado].x, 0, 0);
			//glTranslated(0, trans_missel_esfera, 0);
			glRotated(90,1,0,0);
			
			glScaled(escala_missel,escala_missel,escala_missel);
			
			glCallList(missel);
			glPopMatrix();
		}
		
	}


}

void Inicializa(){
	char str[40+1];
	
	OB_ovni.ponto.x = 0.0;
	OB_ovni.ponto.y = 0.0;
	OB_ovni.ponto.z = 0.0;
	
	OB_nave.ponto.x = 0.0;
	OB_nave.ponto.y = -17.5;
	OB_nave.ponto.z = 3.0;
	
	OB_missel1.ponto.x = OB_nave.ponto.x;
	OB_missel1.ponto.y = OB_nave.ponto.y + trans_missel_nave;
	OB_missel1.ponto.z = 0;
	
	for(i=0;i<QTD_MISSEL_OVNI;i++){
		OB_missel2[i].ponto.x = 0.0;
		OB_missel2[i].ponto.y = 0.0;
		OB_missel2[i].ponto.z = 0.0;
		OB_missel2[i].habilitado = false;
		OB_missel2[i].dispara_missel_esfera = false;
	}
	
	imagem = CarregaJPG("490020730_5d1ef91b20_o.jpg");

	//importarBlenderWrl("nave_pronta3.wrl",&SA_faces,&points);
	sprintf(textpontos,"%s","Pontos: 0");
	//printf("Inicializa 1\n");


	aviao = CarregaObjeto("nave_pronta_gabriel2.obj", true);
	if(aviao == NULL){
		printf("ERRO teste.obj\n");
	}else{
		printf("Nave Importado\n");
	}

	missel=glGenLists(2);
	glNewList(missel,GL_COMPILE);
	importarBlenderWrl("missel.wrl",&SA_faces,&points);
	glEndList();

	DestroiRegFaces(&SA_faces);
	DestroiRegPoints(&points);


	ovni = CarregaObjeto("ovni.obj", true);
	if(ovni == NULL){
		printf("ERRO ovni.obj\n");
	}else{
		printf("Ovni Importado\n");
	}

	iluminacao();
	InitEsferas(esfera);



}

void ColisaoMisselNavelOvni(){
	/*printf(" OB_missel1.ponto.x - OB_ovni.ponto.x %f\n",OB_missel1.ponto.x - OB_ovni.ponto.x);
	printf(" OB_missel1.ponto.y - OB_ovni.ponto.y %f\n",OB_missel1.ponto.y - OB_ovni.ponto.y);*/
	if(OB_missel1.ponto.x <= OB_ovni.ponto.x+6.5 &&  OB_missel1.ponto.x >= OB_ovni.ponto.x-6.5  && OB_missel1.ponto.y <=OB_ovni.ponto.y+1.5 && OB_missel1.ponto.y >=OB_ovni.ponto.y-1.5 ){
		//printf("perde vida!\n");
		pontos+=10;
		sprintf(textpontos,"Pontos: %d",pontos);
		trans_missel_nave=0;
		dispara = 0;
	}
	if(OB_missel1.ponto.y >=OB_ovni.ponto.y+3){
		trans_missel_nave=0;
		dispara = 0;
	}
	
}

void ColisaoMisselOvniNave(){

	/*printf(" OB_missel2.ponto.x - OB_nave.ponto.x %f\n",OB_missel2.ponto.x - OB_nave.ponto.x);
	printf(" OB_missel2.ponto.y - OB_nave.ponto.y %f\n",OB_missel2.ponto.y - OB_nave.ponto.y);*/
	for(i=0;i<QTD_MISSEL_OVNI;i++){
		if(OB_missel2[i].habilitado){
			if(OB_missel2[i].ponto.x <= OB_nave.ponto.x+2.5 &&  OB_missel2[i].ponto.x >= OB_nave.ponto.x-2.5  && OB_missel2[i].ponto.y <=OB_nave.ponto.y+1.5 && OB_missel2[i].ponto.y >=OB_nave.ponto.y-1.5 ){
				//printf("atingiu!\n");
				
				vez=0.0;
				trans_missel_esfera = 0;
				tamanho_vida-= PERDE_VIDA;
				trans_missel_nave=0;
				dispara = 0;
				OB_missel2[i].dispara_missel_esfera = false;
				OB_missel2[i].habilitado = false;
				OB_missel2[i].ponto.x = 0.0;
          		OB_missel2[i].ponto.y = 0.0;
		        OB_missel2[i].ponto.z = 0.0;
				if(tamanho_vida<=-5.0){
					ResetaJogo(1); 
				}
			}
			if(OB_missel2[i].ponto.y <=OB_nave.ponto.y-5){
				vez=0.0;
				trans_missel_esfera = 0;
				OB_missel2[i].dispara_missel_esfera = false;
				OB_missel2[i].habilitado = false;
				OB_missel2[i].ponto.x = 0.0;
          		OB_missel2[i].ponto.y = 0.0;
		        OB_missel2[i].ponto.z = 0.0;
			}
		}
	}
}

void ColisaoMisselOvniMisselNave(){
	//printf("OB_missel2.ponto.y - OB_missel1.ponto.y,%f\n", OB_missel2.ponto.y - OB_missel1.ponto.y);
	for(i=0;i<QTD_MISSEL_OVNI;i++){
		if(OB_missel2[i].habilitado){
			if(OB_missel2[i].ponto.x - OB_missel1.ponto.x <=0.5 && OB_missel2[i].ponto.x - OB_missel1.ponto.x >=-0.5  && OB_missel2[i].ponto.y - OB_missel1.ponto.y <=3 && OB_missel2[i].ponto.y - OB_missel1.ponto.y >-1 ){
				// printf("Colidiu\n");
				vez=0.0;
				trans_missel_esfera = 0;
				trans_missel_nave=0;
				dispara = 0; 
                OB_missel2[i].dispara_missel_esfera = false;
				OB_missel2[i].habilitado = false;
				OB_missel2[i].ponto.x = 0.0;
          		OB_missel2[i].ponto.y = 0.0;
		        OB_missel2[i].ponto.z = 0.0; 
                               
			}
		}
	}
}


void TimerFunction(int value){
	//printf("tamanho_vida %f\n",tamanho_vida);
	//vira_nave=0;
	if(habilita_desenho){
		
		if(dispara){ 
			trans_missel_nave+=VELOCIDADE_MISSEL;
			/*if((trans_missel_nave+trans_ini_nave)==(esfera[0].y+mov_esferas+1) || trans_missel_nave==35){
				dispara = 0;
				trans_missel_nave=0;
			}*/
		}
		vez+=1.0;
		//printf("vez %f\n",vez);
		if(vez>=VEZES && trans_missel_esfera==0){
			//missel_esfera_habilita=1;
			vez=0.0;
			for(i=0;i<QTD_MISSEL_OVNI;i++){
               // printf("%d habilitado %s dispara %s\n",i,OB_missel2[i].habilitado?"ok":"nao",OB_missel2[i].dispara_missel_esfera?"ok":"nao");                                                      
				if(!OB_missel2[i].habilitado || !OB_missel2[i].dispara_missel_esfera){
                     
					OB_missel2[i].ponto.x = OB_ovni.ponto.x;
					OB_missel2[i].habilitado = true;
					OB_missel2[i].dispara_missel_esfera = true;
					//printf("Habilitou %d\n",i);
					break;
				}
			}
			
		}
		//printf("tamanho_vida %f trans_missel_esfera %f\n",tamanho_vida,trans_missel_esfera);
		for(i=0;i<QTD_MISSEL_OVNI;i++){
			if(OB_missel2[i].dispara_missel_esfera && OB_missel2[i].habilitado){
                 
				if(tamanho_vida <= -1){
					//printf("Aumenta vel\n");
					velovidade_missel_ovni = 1.0;
				}
				//trans_missel_esfera+=velovidade_missel_ovni;
				OB_missel2[i].ponto.y -= velovidade_missel_ovni;
			}
		}
		rodar_ovne+=1;
		rodar_ovne = (int)rodar_ovne % 360;
		//printf("trans_ovni %f\n",trans_ovni);
		if(trans_ovni <= -18 || trans_ovni >= 18){
			velocidade_ovni *= (-1);
		}
		trans_ovni += velocidade_ovni;
		ColisaoMisselNavelOvni();
		ColisaoMisselOvniNave();
		ColisaoMisselOvniMisselNave();
		
		//ColisoesMisselNave();
		// ColisoesMisselEsfera();
		// ColisoesEsferaNave();

		/*num= rand() % 100;
		vez+=1.0;
		if(vez==20.0 && trans_missel_esfera==0){
			dispara_missel_esfera = 1;
			vez=0.0;
			missel_esfera_habilita=1;
			if(num>=QTD_ESFERAS){
				missel_selecionado = (num - 49); 
			}else{
				missel_selecionado = num;
			}
		}
		if(vez==20.0){
			mov_esferas-=VELOCIDADE_DESC_ESFERA;
		}

		if(dispara_missel_esfera){
			trans_missel_esfera-=VELOCIDADE_MISSEL;
			if(trans_missel_esfera==-30){
				dispara_missel_esfera = 0;
				trans_missel_esfera=0;
				missel_esfera_habilita=0;
				vez=0.0;
				tamanho_vida-= PERDE_VIDA;
				if(tamanho_vida<=-5.0){
					ResetaJogo(1); 
				} 
			}
		}*/

		
		
		glutPostRedisplay();
	}
	glutPostRedisplay();

	
	glutTimerFunc(SA_Time, TimerFunction, 0); 

}



int main (int argc,char **argv)
{


	glutInit(&argc,argv);

	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(50,50);

	glutInitWindowSize(width_window,height_window);

	glutCreateWindow("Space Ataque");

	glutTimerFunc(SA_Time, TimerFunction, 0);

	Inicializa();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(teclasEspeciais); 	
	glutMainLoop();

	return 0;


}

