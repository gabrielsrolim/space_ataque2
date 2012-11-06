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
#include "glm.h"
#include "sound.h"
#include <SDL\SDL.h>
#define HAVE_LIBJPEG


tPosicao missel_esfera,missel_nave,nave_espacial;
tRegFaces SA_faces;
tRegPoints points;
tEsfera esfera[QTD_ESFERAS];
int width_window=800,height_window=600,num_esfera=0,limite_ovni=22;
int SA_Time=1,dispara=0,dispara_missel_esfera=0,missel_esfera_habilita=0,missel_selecionado,vidas=3,habilita_desenho=1;
double rx = 0.0,ry=0.0,rz=0.0,trans_missel_nave=0.0,trans_ovni=0.0,trans_missel_esfera=0.0,vez=0.0,vira_nave=0;
double z=0.5,cor_esferas=0.0,mov_esferas=0.0,rodar_ovne=0.0;
double trans_ini_nave=-15,escala_missel=2.4;//padrão escala 0.2m translação inicial: -19(eixo y)
double tamanho_vida = 5.0,velovidade_missel_ovni = VELOCIDADE_MISSEL_OVNI;
double velocidade_ovni = 0.3,pos_lanca_missel_ovni = 0;
bool habilitar_linhas = false;
float ajeita_nave = 0;
GLMmodel *GLM_ovni = NULL,*GLM_aviao = NULL,*GLM_aviao2 = NULL,*GLM_background = NULL,*GLM_missel = NULL,*GLM_missel2 = NULL;
GLuint GLM_ovni_list = 0,GLM_aviao_list = 0,GLM_aviao_list2,GLM_background_list=0,GLM_missel_list=0,GLM_missel_list2=0;
int qtd_colisoes = 0;
 /* Audio format specifications. */
SDL_AudioSpec desired, obtained;

    /* Our loaded sounds and their formats. */
sound_t cannon, musica;

OBJ *ovni = NULL,*aviao = NULL,*background = NULL;
tObjetoJogos OB_ovni,OB_nave,OB_missel1[QTD_MISSEL_NAVE],OB_missel2[QTD_MISSEL_OVNI];
char /*aviao,*/missel/*,ovni*/;
int num=0,pontos=0; 
int i;
char textpontos[50],textRodada[50];
TEX *imagem;
double velocidade_disparo = VEZES;

int teste = 0;

GLfloat angle = 45,fAspect = (GLfloat)width_window/(GLfloat)height_window;



// Desenha um texto na janela GLUT
void Texto(){
	int i;
	glColor3f(0,0,0);     
	// Posição onde o textpontos será colocado
	//sprintf(textpontos,"Pontos: %d",pontos);
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
	
	if(vidas>0){
       sprintf(textRodada,"Parabens! Voce Salvou a Galaxia!!",pontos);
		glRasterPos3d(-4, -6, 0);
		for(i=0; textRodada[i] != '\0'; ++i)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,textRodada[i]);
   }else{
	    sprintf(textRodada,"Naaaaooo!!! A Galaxia foi destruida!!",pontos);
		glRasterPos3d(-4, -6, 0);
		for(i=0; textRodada[i] != '\0'; ++i)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,textRodada[i]);
   }
	
	
	/*if(pontos >20){ 
		sprintf(textRodada,"Parabens! Voce Fez %d pontos.",pontos);
		glRasterPos3d(-4, -8, 0);
		for(i=0; textRodada[i] != '\0'; ++i)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,textRodada[i]);    
	}*/
	
}

void RecomecaJogo(){
	rx = 0.0;ry=0.0;rz=0.0;trans_missel_nave=0.0;trans_missel_esfera=0.0;vez=0.0; dispara=0; mov_esferas=0.0;
	tamanho_vida = 5;pontos=0;
	vidas=3;
	velovidade_missel_ovni = VELOCIDADE_MISSEL_OVNI;
	velocidade_ovni = 0.3;
	velocidade_disparo = VEZES;
	qtd_colisoes = 0;
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

		velovidade_missel_ovni = VELOCIDADE_MISSEL_OVNI;
		velocidade_ovni = 0.3;
		velocidade_disparo = VEZES;
		ajeita_nave = 0;
		
		OB_ovni.ponto.x = 0.0;
		OB_ovni.ponto.y = 0.0;
		OB_ovni.ponto.z = 0.0;
		
		OB_nave.ponto.x = 0.0;
		OB_nave.ponto.y = -17.5;
		OB_nave.ponto.z = 3.0;
		
		for(i=0;i<QTD_MISSEL_NAVE;i++){
			OB_missel1[i].ponto.x = OB_nave.ponto.x;
			OB_missel1[i].ponto.y = OB_nave.ponto.y + trans_missel_nave;
			OB_missel1[i].ponto.z = 0;
			OB_missel1[i].habilitado = false;
			OB_missel1[i].dispara_missel_esfera = false;
		}
		
		for(i=0;i<QTD_MISSEL_OVNI;i++){
			OB_missel2[i].ponto.x = 0.0;
			OB_missel2[i].ponto.y = 0.0;
			OB_missel2[i].ponto.z = 0.0;
			OB_missel2[i].habilitado = false;
			OB_missel2[i].dispara_missel_esfera = false;
		}
		qtd_colisoes = 0;
	}
	
	
}


void ResetaNave(){
	rx = 0.0;ry=0.0;vez=0.0; 
}


void iluminacao(void){
	GLfloat luzAmbiente[4] = {0.8,0.8,0.8,1.0};;
	GLfloat luzDifusa[4] = {0.1,0.1,0.1,0.8};//cor
	GLfloat luzEspecular[4] = {0.1,0.1,0.1,1.0};//brilho
	GLfloat posicaoLuz[4] = {0.0,0.0,20.0,0.0};
	GLfloat posicaoLuz2[4]={0.0, 20.0, 0.0, 1};
	GLfloat posicaoLuz3[4]={0.0, -20.0, 0.0, 1};
	
	GLfloat especularidade[4] = {1.0,1.0,1.0,1.0};
	GLint especMaterial = 60;
	
	
	//Define a refletância do material
	//glMaterialfv(GL_FRONT,GL_SPECULAR,especularidade);
	
	//Define a concentração do brilho
	//glMateriali(GL_FRONT,GL_SHININESS,especMaterial);
	
	//Ativa o uso da luz ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,luzAmbiente);
	
	//define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0,GL_AMBIENT,luzAmbiente);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,luzDifusa);
	glLightfv(GL_LIGHT0,GL_SPECULAR,luzEspecular);
	glLightfv(GL_LIGHT0,GL_POSITION,posicaoLuz);

	glLightfv(GL_LIGHT1,GL_AMBIENT,luzAmbiente);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,luzDifusa);
	glLightfv(GL_LIGHT1,GL_SPECULAR,luzEspecular);
	glLightfv(GL_LIGHT1,GL_POSITION,posicaoLuz2);

	glLightfv(GL_LIGHT2,GL_AMBIENT,luzAmbiente);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,luzDifusa);
	glLightfv(GL_LIGHT2,GL_SPECULAR,luzEspecular);
	glLightfv(GL_LIGHT2,GL_POSITION,posicaoLuz3);
	
	glEnable(GL_COLOR_MATERIAL);
	
	glEnable(GL_LIGHTING);
	
	glShadeModel(GL_SMOOTH);

	// Habilita a luz de numero 0
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	
	
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
	//glOrtho(-20.0, 20.0, -20.0, 20.0, 1, 1000.0);

	gluPerspective(angle,fAspect,0.1,1200);

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
		glPushMatrix();
		glTranslated(0,0,-30);
		glScaled(20,20,20);
		glRotated(90,1,0,0);
		glCallList(GLM_background_list);
		glPopMatrix();
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
	chances();
	//Texto();

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
	glRotated(ajeita_nave,0,1,0);

	glScaled(1.5,1.5,1.5);
	
	glCallList(GLM_aviao_list);
	
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
	glCallList(GLM_ovni_list);
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
	for(i=0;i<QTD_MISSEL_NAVE;i++){
		if(OB_missel1[i].habilitado && OB_missel1[i].dispara_missel_esfera){
			glPushMatrix();
			glColor3d(0, 0, 1);
			glTranslated(OB_missel1[i].ponto.x, OB_missel1[i].ponto.y, 0);
			glRotated(90,1,0,0);

			glScaled(escala_missel,escala_missel,escala_missel);

			glCallList(GLM_missel_list);
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
	}


	//missel ovni
	for(i=0;i<QTD_MISSEL_OVNI;i++){
		//printf("habilitado %s dispara %s\n",OB_missel2[i].habilitado?"ok":"nao",OB_missel2[i].dispara_missel_esfera?"ok":"nao");                           
		if(OB_missel2[i].habilitado && OB_missel2[i].dispara_missel_esfera){
			glPushMatrix();
			glColor3d(1, 0, 0);
			//printf("trans_missel_esfera %f\n",trans_missel_esfera);
			OB_missel2[i].ponto.y; //= (OB_ovni.ponto.y-trans_missel_esfera);
			OB_missel2[i].ponto.z =  OB_ovni.ponto.z;
			glTranslated(OB_missel2[i].ponto.x,OB_missel2[i].ponto.y,OB_missel2[i].ponto.z);
			//glTranslated(esfera[missel_selecionado].x, 0, 0);
			//glTranslated(0, trans_missel_esfera, 0);
			glRotated(-90,1,0,0);
			
			glScaled(escala_missel,escala_missel,escala_missel);
			
			glCallList(GLM_missel_list2);
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
		
	}


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
		//SetaModoDesenho('s');
		//DesenhaObjeto(aviao);
		glCallList(GLM_aviao_list2);
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
		//SetaModoDesenho('s');
		//DesenhaObjeto(aviao);
		glCallList(GLM_aviao_list2);
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
		//SetaModoDesenho('s');
		//DesenhaObjeto(aviao);
		glCallList(GLM_aviao_list2);
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
bool primeira = true;

void display(void){

	Desenha();
	

	glutSwapBuffers();
	if(primeira){
     SDL_Delay(3000);
     primeira = false;
   }
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
				if((int)rx%2==0){
					ajeita_nave -=1;
				}
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
				if((int)rx%2==0){
					ajeita_nave+=1;
				}
				
			} 
		}           
		glutPostRedisplay();
		break;
	}
}

void keyboard(unsigned char k,int x,int y){
	switch(k){
	case ' ':
		for(i=0;i<QTD_MISSEL_NAVE;i++){
			if(!OB_missel1[i].habilitado && !OB_missel1[i].dispara_missel_esfera){
				
				OB_missel1[i].ponto.x = OB_nave.ponto.x;
				OB_missel1[i].ponto.y = OB_nave.ponto.y - 2;
				OB_missel1[i].ponto.z = 0;
				OB_missel1[i].habilitado = true;
				OB_missel1[i].dispara_missel_esfera = true;
				PlaySound(&cannon);
				break;   
			}
		}
		
		//SDL_Delay(2000);
		glutPostRedisplay();
		
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
				if((int)rx%2==0){
					ajeita_nave+=1;
				}
				
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
				if((int)rx%2==0){
					ajeita_nave -=1;
				}
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
 		   vidas=3;  
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

void Inicializa(){
	char str[40+1];
	
	OB_ovni.ponto.x = 0.0;
	OB_ovni.ponto.y = 0.0;
	OB_ovni.ponto.z = 0.0;
	
	OB_nave.ponto.x = 0.0;
	OB_nave.ponto.y = -17.5;
	OB_nave.ponto.z = 3.0;
	
	for(i=0;i<QTD_MISSEL_NAVE;i++){
		OB_missel1[i].ponto.x = OB_nave.ponto.x;
		OB_missel1[i].ponto.y = OB_nave.ponto.y + trans_missel_nave;
		OB_missel1[i].ponto.z = 0;
		OB_missel1[i].habilitado = false;
		OB_missel1[i].dispara_missel_esfera = false;
	}
	
	for(i=0;i<QTD_MISSEL_OVNI;i++){
		OB_missel2[i].ponto.x = 0.0;
		OB_missel2[i].ponto.y = 0.0;
		OB_missel2[i].ponto.z = 0.0;
		OB_missel2[i].habilitado = false;
		OB_missel2[i].dispara_missel_esfera = false;
	}
	
	//imagem = CarregaJPG("490020730_5d1ef91b20_o.jpg");

	//importarBlenderWrl("nave_pronta3.wrl",&SA_faces,&points);
	sprintf(textpontos,"%s","Pontos: 0");
	//printf("Inicializa 1\n");



	/*aviao = CarregaObjeto("nave_pronta_gabriel2.obj", true);
	if(aviao == NULL){
		printf("ERRO teste.obj\n");
	}else{
		printf("Nave Importado\n");
	}*/
	
	GLM_background = glmReadOBJ("fundo.obj");
	GLM_background_list = glGenLists(1);
	glNewList(GLM_background_list, GL_COMPILE);
	glmDraw(GLM_background, GLM_NONE|GLM_SMOOTH|GLM_TEXTURE|GLM_COLOR);
	glEndList();
	
	GLM_aviao = glmReadOBJ("nave_pronta_gabriel2.obj");
	GLM_aviao_list = glGenLists(1);
	glNewList(GLM_aviao_list, GL_COMPILE);
	glmDraw(GLM_aviao, GLM_SMOOTH|GLM_TEXTURE|GLM_COLOR);
	glEndList();
	
	GLM_aviao2 = glmReadOBJ("nave_pronta_gabriel2.obj");
	GLM_aviao_list2 = glGenLists(1);
	glNewList(GLM_aviao_list2, GL_COMPILE);
	glmDraw(GLM_aviao2, GLM_SMOOTH);
	glEndList();
	
	
	GLM_missel = glmReadOBJ("missel.obj");
	GLM_missel_list = glGenLists(1);
	glNewList(GLM_missel_list, GL_COMPILE);
	glmDraw(GLM_missel, GLM_NONE|GLM_SMOOTH|GLM_TEXTURE|GLM_COLOR);
	glEndList();
	
	GLM_missel2 = glmReadOBJ("missel2.obj");
	GLM_missel_list2 = glGenLists(1);
	glNewList(GLM_missel_list2, GL_COMPILE);
	glmDraw(GLM_missel2, GLM_NONE|GLM_SMOOTH|GLM_TEXTURE|GLM_COLOR);
	glEndList();
	
	/*missel=glGenLists(1);
	glNewList(missel,GL_COMPILE);
	importarBlenderWrl("missel.wrl",&SA_faces,&points);
	glEndList();

	DestroiRegFaces(&SA_faces);
	DestroiRegPoints(&points);*/


	/*ovni = CarregaObjeto("ovni.obj", true);
	if(ovni == NULL){
		printf("ERRO ovni.obj\n");
	}else{
		printf("Ovni Importado\n");
	}*/
	
	GLM_ovni = glmReadOBJ("ovni.obj");
	GLM_ovni_list = glGenLists(1);
	glNewList(GLM_ovni_list, GL_COMPILE);
	glmDraw(GLM_ovni, GLM_NONE|GLM_SMOOTH|GLM_TEXTURE|GLM_MATERIAL);
	glEndList();
	
	/*background = CarregaObjeto("background.obj", false);
	if(background == NULL){
		printf("ERRO background.obj\n");
	}else{
		printf("background Importado\n");
	}*/
	
	CriaDisplayList(NULL);

	iluminacao();
	InitEsferas(esfera);



}

void ColisaoMisselNavelOvni(){
	/*printf(" OB_missel1.ponto.x - OB_ovni.ponto.x %f\n",OB_missel1.ponto.x - OB_ovni.ponto.x);
	printf(" OB_missel1.ponto.y - OB_ovni.ponto.y %f\n",OB_missel1.ponto.y - OB_ovni.ponto.y);*/
	
	for(i=0;i<QTD_MISSEL_NAVE;i++){
		if(OB_missel1[i].habilitado && OB_missel1[i].dispara_missel_esfera){
			if(OB_missel1[i].ponto.x <= OB_ovni.ponto.x+6.5 &&  OB_missel1[i].ponto.x >= OB_ovni.ponto.x-6.5  && OB_missel1[i].ponto.y <=OB_ovni.ponto.y+1.5 && OB_missel1[i].ponto.y >=OB_ovni.ponto.y-1.5 ){
				//printf("perde vida!\n");
				pontos+=10;
				sprintf(textpontos,"Pontos: %d",pontos);
				OB_missel1[i].habilitado = false;
				OB_missel1[i].dispara_missel_esfera = false;
				tamanho_vida-= PERDE_VIDA;
				if(tamanho_vida<-5){
                 ResetaJogo(1);
                }
			}



			if(OB_missel1[i].ponto.y >=OB_ovni.ponto.y+3){
				OB_missel1[i].habilitado = false;
				OB_missel1[i].dispara_missel_esfera = false;
			}
		}
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
				qtd_colisoes += 1;
				if(qtd_colisoes == 5){
                   qtd_colisoes = 0;
                   vidas -= 1;
                   if(vidas <=0){
                    ResetaJogo(1);
                   }
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
	int j=0;
	//printf("OB_missel2.ponto.y - OB_missel1.ponto.y,%f\n", OB_missel2.ponto.y - OB_missel1.ponto.y);
	for(j=0;j<QTD_MISSEL_NAVE;j++){
		for(i=0;i<QTD_MISSEL_OVNI;i++){
			if(OB_missel2[i].habilitado){
				if(OB_missel2[i].ponto.x - OB_missel1[j].ponto.x <=0.5 && OB_missel2[i].ponto.x - OB_missel1[j].ponto.x >=-0.5  && OB_missel2[i].ponto.y - OB_missel1[j].ponto.y <=3 && OB_missel2[i].ponto.y - OB_missel1[j].ponto.y >-1 ){
					// printf("Colidiu\n");
					vez=0.0;
					OB_missel1[j].dispara_missel_esfera = false;
					OB_missel1[j].habilitado = false;
					OB_missel1[j].ponto.x = 0.0;
					OB_missel1[j].ponto.y = 0.0;
					OB_missel1[j].ponto.z = 0.0;




					OB_missel2[i].dispara_missel_esfera = false;
					OB_missel2[i].habilitado = false;
					OB_missel2[i].ponto.x = 0.0;
					OB_missel2[i].ponto.y = 0.0;
					OB_missel2[i].ponto.z = 0.0; 
					
				}
			}
		}//for missel_ovni
	}//for missel_nave




}


void TimerFunction(int value){
	//printf("tamanho_vida %f\n",tamanho_vida);
	//vira_nave=0;
	if(habilita_desenho){
		
		for(i=0;i<QTD_MISSEL_NAVE;i++){                                                      
			if(OB_missel1[i].habilitado || OB_missel1[i].dispara_missel_esfera){
				OB_missel1[i].ponto.y += VELOCIDADE_MISSEL;
			}
			






		}

		vez+=1.0;
		//printf("vez %f\n",vez);
		if(vez>=velocidade_disparo){
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
				
				if(tamanho_vida <= 0){
					//printf("Aumenta vel\n");
					//velovidade_missel_ovni = 1.0;
					if(velocidade_ovni < 0){
						velocidade_ovni = -(VELOCIDADE_MISSEL_OVNI + 0.3);
					}else{
						velocidade_ovni = (VELOCIDADE_MISSEL_OVNI + 0.3);
					}      
					velocidade_disparo = 5;
				}
				//trans_missel_esfera+=velovidade_missel_ovni;
				OB_missel2[i].ponto.y -= velovidade_missel_ovni;
			}
		}
		rodar_ovne+=1;
		rodar_ovne = (int)rodar_ovne % 360;
		//printf("trans_ovni %f\n",trans_ovni);
		if(trans_ovni <= -limite_ovni || trans_ovni >= limite_ovni){
			velocidade_ovni *= (-1);
		}
		trans_ovni += velocidade_ovni;
		if(habilita_desenho){
    		ColisaoMisselNavelOvni();
    		ColisaoMisselOvniNave();
    		ColisaoMisselOvniMisselNave();
        }
		
		if(getStatusSound(musica)!= 1){
          printf("TOCA NOVAMENTE\n");
          PlaySound(&musica);
        }
		
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

void InicializaSound(){

    /* Initialize SDL's video and audio subsystems.
       Video is necessary to receive events. */
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
	printf("Unable to initialize SDL: %s\n", SDL_GetError());
	return;
    }

    /* Make sure SDL_Quit gets called when the program exits. */
    atexit(SDL_Quit);

    /* We also need to call this before we exit. SDL_Quit does
       not properly close the audio device for us. */
    atexit(SDL_CloseAudio);

    /* Open the audio device. The sound driver will try to give us
       the requested format, but it might not succeed. The 'obtained'
       structure will be filled in with the actual format data. */
    desired.freq = 44100;	/* desired output sample rate */
    desired.format = AUDIO_S16;	/* request signed 16-bit samples */
    desired.samples = 4096;	/* this is more or less discretionary */
    desired.channels = 2;	/* ask for stereo */
    desired.callback = AudioCallback;
    desired.userdata = NULL;	/* we don't need this */
    if (SDL_OpenAudio(&desired, &obtained) < 0) {
	printf("Unable to open audio device: %s\n", SDL_GetError());
	return;
    }

    /* Load our sound files and convert them to the sound card's format. */
    if (LoadAndConvertSound("teste.wav", &obtained, &cannon) != 0) {
	printf("Unable to load sound.\n");
	return;
    }

    if (LoadAndConvertSound("sw.wav", &obtained, &musica) != 0) {
	printf("Unable to load sound.\n");
	return;
    }

    /* Clear the list of playing sounds. */
    ClearPlayingSounds();

    /* SDL's audio is initially paused. Start it. */
    SDL_PauseAudio(0);
    
    PlaySound(&musica);
}



int main (int argc,char **argv)
{

	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(50,50);

	glutInitWindowSize(width_window,height_window);

	glutCreateWindow("Space Ataque");

	glutTimerFunc(SA_Time, TimerFunction, 0);

	Inicializa();
	InicializaSound();
	printf("Carregando Musica\n");
	//SDL_Delay(3000);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(teclasEspeciais); 	
	glutMainLoop();
    

	return 0;


}

