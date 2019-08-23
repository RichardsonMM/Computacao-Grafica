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
int ang = 45;

// =======================================================================
// Funcao responsavel por desenhar os objetos
void desenha(void)
{
    GLfloat raio, ang;
    GLint   segmentos;

    // Limpa a janela de visualizacao com a cor de fundo especificada
    glClear( GL_COLOR_BUFFER_BIT );

    //glLineWidth( 5 );
    glBegin( GL_LINES );

    // Especifica quam cor corrente
    glColor4f( 1.0 , 0.0 , 0.0 , 1.0 );
    glVertex2f( -150.0 , 0.0 );
    glVertex2f(  150.0 , 0.0 );

    glColor4f( 0.0 , 1.0 , 0.0 , 1.0 );
    glVertex2f( 0.0 , -150.0 );
    glVertex2f( 0.0 ,  150.0 );

    glEnd();

    glBegin( GL_POLYGON );

    glColor4f( 1.0 , 1.0 , 0.0 , 0.523542342 );
    glVertex2f( -30.0 ,  80.0 );
    glVertex2f( -60.0 ,   0.0 );
    glVertex2f(   0.0 , -70.0 );
    glVertex2f(  40.0 , -70.0 );
    glVertex2f(  80.0 ,   0.0 );
    glVertex2f(  20.0 ,  50.0 );

    glEnd();

    raio = 30;
    segmentos = 50;

    glColor4f( 0.0 , 0.5 , 0.8 , 1.0 );
    glBegin( GL_POLYGON );
    for ( GLint i = 0 ; i < segmentos ; i ++ ){
        ang =  2 * M_PI * i / segmentos;
        glVertex2f( raio * cos( ang ) , raio * sin( ang ) );
    }
    glEnd();

    glBegin( GL_TRIANGLES );

    glColor4f( 1.0 , 1.0 , 1.0 , 1.0 );
    glVertex2f(  250.0 , -50.0 );
    glVertex2f(  310.0 , -50.0 );
    glVertex2f(  280.0 ,  50.0 );

    glEnd();
    glColor3f(0.0, 1.0, 1.0);
    glutSolidTeapot(15);
    

    // Executa os comandos OpenGL
    glFlush();
}

// =======================================================================
// funcao para configurar a projecao ortogonal
void define_projecao( void )
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D ( window.xi , window.xf , window.yi , window.yf ); // projecao ortogonal 2D
}

// =======================================================================
void inicializa( void )
{
    // Define a cor de fundo da janela de visualizacao como preta
    //             R     G     B    alfa
    glClearColor( 0.0 , 0.0 , 0.0 , 1.0 );

    window.xi = -160; // x inicial
    window.xf =  160; // x final
    window.yi = -90;  // y inicial
    window.yf =  90;  // y final

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
        if( window.xi+PASSO < window.xf-PASSO && window.yi+PASSO < window.yf-PASSO ) // para que a visualização nao inverta apos dar o zoom
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
    
    
    //SRU = SISTEMA DE REFERENCIA PARA O UNIVERSO

    return 0;
}
