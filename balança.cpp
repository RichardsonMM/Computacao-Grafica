//
//  main.cpp
//  02-Otho_Win_View_Zoom_Pan
//
//  Created by Viapiana on 17/05/17.
//  Copyright © 2017 Viapas. All rights reserved.
//
//  Projecao Ortogonal
//  Window
//  Viewport
//  Zoom
//  Pan
//  Teclas especiais
//  Transparencia

#include <iostream>
#include <cstdlib>
#include <math.h>

#if __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <gl/glut.h>
#endif

#ifndef M_PI
#define M_PI 3.1415926
#endif

using namespace std;

#define PASSO 5

struct tipo_win{
    GLfloat xi, xf, yi, yf;
};

// definição da window de seleção
tipo_win window;

float aspecto_win;

// =======================================================================
// Funcao responsavel por desenhar os objetos
void desenha(void)
{
    // Limpa a janela de visualizacao com a cor de fundo especificada
    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    //glLineWidth( 5 );

    // BASE
    glPushMatrix();
    glTranslated(0, 70, 0);
    glColor3f(0.5, 0.0, 0.5);
    glBegin(GL_POLYGON);
        glVertex2f(-20, 10);
        glVertex2f(20, 10);
        glVertex2f(30, -20);
        glVertex2f(-30, -20);
    glEnd();

    // RETA VERTICAL

    glPushMatrix();
    glColor3f(0.0, 0.5, 0.5);
    glBegin(GL_POLYGON);
        glVertex2f(-5, 50);
        glVertex2f(5, 50);
        glVertex2f(5, -50);
        glVertex2f(-5, -50);
    glEnd();

    glPopMatrix();

    // RETA HORIZONTAL

    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(-60, 7);
        glVertex2f(60, 7);
        glVertex2f(60, -7);
        glVertex2f(-60, -7);
    glEnd();

    glPopMatrix();

    // CABEÇA

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(-35, 10);
        glVertex2f(35, 10);
        glVertex2f(20, -10);
        glVertex2f(-20, -10);
    glEnd();
    glPopMatrix();

    // CORRENTE ESQUERDA
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
        glVertex2f(-2, 50);
        glVertex2f(2, 50);
        glVertex2f(2, -20);
        glVertex2f(-2, -20);
    glEnd();

    // CORRENTE DIREITA
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
        glVertex2f(-2, 50);
        glVertex2f(2, 50);
        glVertex2f(2, -20);
        glVertex2f(-2, -20);
    glEnd();
    glPopMatrix();

    // PESO ESQUERDO
    glColor3f(0.5, 0.5, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(0, 5);
        glVertex2f(20, -10);
        glVertex2f(-20, -10);
    glEnd();
    glPopMatrix();

    // PESO DIREITO
    glColor3f(0.5, 0.5, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(0, 5);
        glVertex2f(20, -10);
        glVertex2f(-20, -10);
    glEnd();
    glPopMatrix();

    glPopMatrix();

    // Executa os comandos OpenGL
    glFlush();
}

// =======================================================================
// funcao para configurar a projecao ortogonal
void define_projecao( void )
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D ( window.xi , window.xf , window.yi , window.yf );
}

// =======================================================================
void inicializa( void )
{
    // Define a cor de fundo da janela de visualizacao como preta
    //             R     G     B    alfa
    glClearColor( 0.0 , 0.0 , 0.0 , 1.0 );

    window.xi = -160;
    window.xf =  160;
    window.yi = -90;
    window.yf =  90;

    aspecto_win = ( window.xf - window.xi ) / ( window.yf - window.yi );
    cout << "\nAspecto win : " << aspecto_win;

    // habilita a transparenica
    glEnable( GL_BLEND );

    // define a forma de calculo da transparencia
    glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
}

// =======================================================================
// Funcao de tratamento do teclado
void teclado( unsigned char key , GLint x , GLint y )
{
    if ( key == 27 )
        exit( 0 );

    // reinicializa o desenho
    if ( key == 'I'  || key == 'i' ){
        inicializa();
        define_projecao();
    }

    // como foram mudados dados que influenciam na visualizacao dos objetos,
    // este comando obriga a executar a funcao de desenho (desenha)
    glutPostRedisplay();

}

// =======================================================================
// tratamento das teclas especiais (teclas de funcao e de navegacao).
// os parametros que recebe sao a tecla pressionada e a posicao x e y
void teclas_especiais( GLint key , GLint x , GLint y )
{

    // movimenta a window para a esquerda
    if ( key == GLUT_KEY_LEFT ){
        window.xi -= PASSO;
        window.xf -= PASSO;
    }

    // movimenta a window para a direita
    if ( key == GLUT_KEY_RIGHT ){
        window.xi += PASSO;
        window.xf += PASSO;
    }

    // movimenta a window para cima
    if ( key == GLUT_KEY_UP ){
        window.yi += PASSO;
        window.yf += PASSO;
    }

    // movimenta a window para cima
    if ( key == GLUT_KEY_DOWN ){
        window.yi -= PASSO;
        window.yf -= PASSO;
    }

    // zoom out
    if ( key == GLUT_KEY_PAGE_DOWN ){
        window.xi -= PASSO;
        window.xf += PASSO;
        window.yi -= PASSO/aspecto_win;
        window.yf += PASSO/aspecto_win;
    }

    // zoom in
    if ( key == GLUT_KEY_PAGE_UP ){
        if( window.xi+PASSO < window.xf-PASSO && window.yi+PASSO < window.yf-PASSO )
        {
            window.xi += PASSO;
            window.xf -= PASSO;
            window.yi += PASSO/aspecto_win;
            window.yf -= PASSO/aspecto_win;
        }
    }

    // como foram mudadas as variaveis de controle da window
    // deve-se executar novamente a funcao de definicao da projecao
    define_projecao();

    // como foi mudado dados que influenciam na visualizacao dos objetos,
    // este comando obriga a executar a funcao de desenho (desenha)
    glutPostRedisplay();
}

// =======================================================================
// Programa Principal
int main( int argc , char *argv[] )
{
    glutInit(&argc, argv);

    // especifica a utilizacao dos buffers e indica o esquema de cores RGB
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );

    // Especifica as dimensoes da janela na tela, em pixels
    glutInitWindowSize( 640  , 360 );

    // Cria a janela com o titulo especificado
    glutCreateWindow( "ORTO - WINDOW - VIEWPORT - PAN - ZOOM" );

    // Modo full screen - OPCIONAL
    //glutFullScreen();

    // Determina qual a funcao do programa que desenha os objetos
    glutDisplayFunc( desenha );

    // Funcao callback de tratamento do teclado - OPCIONAL
    glutKeyboardFunc( teclado );

    // Funcao callback de tratamento das teclas especiais digitadas
    glutSpecialFunc( teclas_especiais );

    // funcao que tem as inicilizacoes de variaveis e estados do OpenGL
    inicializa();

    // configurar a projecao
    define_projecao();

    // Ultimo comando. Faz com que todas as janelas criadas sejam mostradas
    // Uma vez neste loop, somente sai quando o programa encerra
    glutMainLoop();

    return 0;
}
