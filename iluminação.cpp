//
//  main.cpp
//  07-3D-Projecao
//
//  Created by Viapiana on 29/05/17.
//  Copyright © 2017 Viapas. All rights reserved.
//
//  3D
//  Camera sintetica
//  PAN - ZOOM
//  duas janelas


#include <iostream>
#include <cstdlib>
#include <math.h>
#include <sstream>

#if __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/freeglut.h>
#endif

#ifndef M_PI
#define M_PI 3.1415926
#endif

using namespace std;

#define JANELAS         2

#define LAR_MAIN        600
#define ALT_MAIN        600

#define ALT_BARRA       30

#define LAR_HELP        600
#define ALT_HELP        280

// prototipagem
//void especifica_parametros_visualizacao( void );

GLfloat angle, fAspect, rotX, rotY;
GLdouble obsX, obsY, obsZ;


struct tipo_camera {
    GLfloat posx;               // posicao x da camera
    GLfloat posy;               // posicao y da camera
    GLfloat posz;               // posicao z da camera
    GLfloat alvox;              // alvo x da visualizacao
    GLfloat alvoy;              // alvo y da visualizacao
    GLfloat alvoz;              // alvo z da visualizacao
    GLfloat ang;                // abertura da 'lente' - efeito de zoom
    GLfloat inicio;             // inicio da area de visualizacao em profundidade
    GLfloat fim;                // fim da area de visualizacao em profundidade
};

struct tipo_transformacao{
    GLfloat dx, dy, dz;         // paramatros de translacao
    GLfloat sx, sy, sz;         // parametros de escala
    GLfloat angx , angy , angz; // parametros de rotacao
};

struct tipo_janela{
    GLfloat largura;            // largura da janela
    GLfloat altura;             // altura da jaanela
    GLfloat aspecto;            // aspecto da janela (relacao entre largura e altura)
};

// define a perspectiva da camera
tipo_camera camera;

// definicao dos valores de transformacao
tipo_transformacao transf;

// definicao dos janela principal
tipo_janela janela;

// transformacao atual, eixo atual
char  transformacao, eixo;

// para visualizar ou nao os eixos, forma do objeto: aramado ou solido
bool visual_eixo , wireframe;

// controle das janelas, passo de atualizacao dos parametros das transformacoes
GLint jan[ JANELAS ], passo;

GLfloat vertices[7][3] = {
    -30, 20, 30,
    -30, -20, 30,
    30, -20, 30,
    30, 20, 30,
    0, 40, -30,
    -30, -20, -30,
    30, -20, -30
};

GLfloat texCoords[] = {
    0.0f, 0.0f,  // lower-left corner
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
};

// =======================================================================
void mostra_texto_bitmap( float x , float y , string texto )
{
    glRasterPos2f ( x , y );

    for( int i = 0 ; i < texto.length() ; i++ )
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13 , texto[ i ] );

    // GLUT_BITMAP_8_BY_13
    // GLUT_BITMAP_9_BY_15
    // GLUT_BITMAP_TIMES_ROMAN_10
    // GLUT_BITMAP_TIMES_ROMAN_24
    // GLUT_BITMAP_HELVETICA_10
    // GLUT_BITMAP_HELVETICA_12
    // GLUT_BITMAP_HELVETICA_18
}

// =======================================================================
void mensagem( void )
{
    ostringstream msg;

    // Especifica as dimensoes da viewport para a barra superior
    glViewport( 0 , janela.altura , janela.largura , ALT_BARRA );

    msg << "TRANSF: " << transformacao << " - EIXO: " << eixo << " - PASSO: " << passo;

    glColor4f( 1.0 , 1.0 , 1.0 , 1.0 );

    mostra_texto_bitmap( 0 , 0 , msg.str() );

    // Volta a viewport para o tamanho normal
    glViewport( 0 , 0 , janela.largura , janela.altura );
}

// =======================================================================
//desenha os eixos x y z
void desenha_eixos()
{
    glLineWidth( 4 );

    glBegin( GL_LINES );

    //eixo x
    glColor4f( 0.0 , 1.0 , 0.0 , 0.8 );
    glVertex3i( -100 , 0 , 0 );
    glVertex3i(  100 , 0 , 0 );

    //eixo y
    glColor4f( 0.0 , 0.0 , 1.0 , 0.8);
    glVertex3i( 0 , -100 , 0 );
    glVertex3i( 0 ,  100 , 0 );

    //eixo z
    glColor4f( 1.0 , 0.0 , 0.0 , 0.8 );
    glVertex3i( 0 , 0 , -100 );
    glVertex3i( 0 , 0 ,  100 );

    glEnd();
}
// =======================================================================
// Funcao responsavel por mostrar o help
void desenha_help(void)
{
    // Limpa a janela de visualizao com a cor de fundo especificada
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // seleciona o tipo de matriz para a visualizacao dos objetos (modelos)
    glMatrixMode(GL_MODELVIEW);

    glClear( GL_COLOR_BUFFER_BIT );
    glColor4f( 1.0 , 1.0 , 0.0 , 1.0 );

    mostra_texto_bitmap( 0 , 270 , "ESC     : finaliza o programa" );
    mostra_texto_bitmap( 0 , 255 , "ALT +   : aumenta o passo" );
    mostra_texto_bitmap( 0 , 240 , "ALT -   : diminui o passo" );
    mostra_texto_bitmap( 0 , 225 , "I       : reinicializa" );
    mostra_texto_bitmap( 0 , 210 , "L       : liga/desliga anti-aliasing" );
    mostra_texto_bitmap( 0 , 195 , "W       : liga/desliga wireframe" );
    mostra_texto_bitmap( 0 , 180 , "E       : mostra/esconde os eixos");
    mostra_texto_bitmap( 0 , 165 , "X Y Z   : seleciona o eixo" );
    mostra_texto_bitmap( 0 , 150 , "S R T   : seleciona a transformacao" );
    mostra_texto_bitmap( 0 , 135 , "A       : aumenta o angulo de abertura da perspectiva" );
    mostra_texto_bitmap( 0 , 120 , "a       : diminui o angulo de abertura da perspectiva" );
    mostra_texto_bitmap( 0 , 105 , "+       : executa a transformacao corrente sobre o eixo corrente" );
    mostra_texto_bitmap( 0 ,  90 , "-       : executa a transformacao corrente sobre o eixo corrente" );
    mostra_texto_bitmap( 0 ,  75 , "ESQ     : movimenta a camera para a esquerda" );
    mostra_texto_bitmap( 0 ,  60 , "DIR     : movimenta a camera para a direita" );
    mostra_texto_bitmap( 0 ,  45 , "ACIMA   : movimenta a camera para cima" );
    mostra_texto_bitmap( 0 ,  30 , "ABAIXO  : movimenta a camera para baixo" );
    mostra_texto_bitmap( 0 ,  15 , "PG UP   : movimenta a camera para frente" );
    mostra_texto_bitmap( 0 ,   0 , "PG DOWN : movimenta a camera para tras" );

    // Executa os comandos OpenGL
    glutSwapBuffers();
}

// =======================================================================
// Funcao usada para especificar a projecao ortogonal do help
void especifica_parametros_visualizacao_help( void )
{
    // seleciona o tipo de matriz para a projecao
    glMatrixMode( GL_PROJECTION );

    // limpa (zera) as matrizes
    glLoadIdentity();

    glClearColor( 0.0 , 0.0 , 0.0 , 1.0 );

    gluOrtho2D(  0 , LAR_HELP , 0 , ALT_HELP );

}

// =======================================================================
// Funcao callback chamada quando o tamanho da janela do help eh alterado
void altera_tamanho_janela_help( GLsizei largura , GLsizei altura )
{
    glutReshapeWindow( LAR_HELP , ALT_HELP );

    especifica_parametros_visualizacao_help();
}

void DefineIluminacao (void)
{
        GLfloat luzAmbiente[4]={0.3,0.3,0.3,1.0};
        GLfloat luzDifusa[4]={1.0,1.0,1.0,1.0};          // "cor"
        GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho"
        GLfloat posicaoLuz[4]={0.0, 100.0, 100.0, 1.0};

        // Capacidade de brilho do material
        GLfloat especularidade[4]={1.0,1.0,1.0,1.0};
        GLint especMaterial = 60;

        // Define a refletância do material
        glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
        // Define a concentração do brilho
        glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

        // Ativa o uso da luz ambiente
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

        // Define os parâmetros da luz de número 0
        glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
        glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
        glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );
}

// =======================================================================
// Funcao responsavel por desenhar os objetos
void desenha(void)
{
    // Limpa a janela de visualizao com a cor de fundo especificada
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    DefineIluminacao();

    // seleciona o tipo de matriz para a visualizacao dos objetos (modelos)
    glMatrixMode(GL_MODELVIEW);

    mensagem();
    glPushMatrix();

    if ( visual_eixo )
        desenha_eixos();

    glPushMatrix();

    glTranslatef( transf.dx , transf.dy , transf.dz );
    glRotatef( transf.angx , 1 , 0 , 0 );
    glRotatef( transf.angy , 0 , 1 , 0 );
    glRotatef( transf.angz , 0 , 0 , 1 );
    glScalef( transf.sx , transf.sy , transf.sz );

    glLineWidth( 1 );

    glColor4f( 0.0 , 0.0 , 0.2 , 1.0 );

    /*glBegin(GL_POLYGON);
    glColor4f( 1.0 , 0.0 , 0.0 , 1.0 );
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[3]);
    glEnd();

    glBegin(GL_POLYGON);
    glColor4f( 0.0 , 1.0 , 0.0 , 1.0 );
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[3]);
    glEnd();

    glBegin(GL_POLYGON);
    glColor4f( 0.0 , 1.0 , 1.0 , 1.0 );
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[6]);
    glEnd();

    glBegin(GL_POLYGON);
    glColor4f( 1.0 , 1.0 , 0.0 , 1.0 );
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[5]);
    glEnd();

    glBegin(GL_POLYGON);
    glColor4f( 0.0 , 1.0 , 1.0 , 1.0 );
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[0]);
    glEnd();

    glBegin(GL_POLYGON);
    glColor4f( 0.0 , 0.5 , 0.5 , 1.0 );
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[0]);
    glEnd();

    glBegin(GL_POLYGON);
    glColor4f( 1.0 , 1.0 , 1.0 , 1.0 );
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[3]);
    glEnd();

    glBegin(GL_POLYGON);
    glColor4f( 1.0 , 0.0 , 1.0 , 1.0 );
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[6]);
    glEnd();
    */

    glutSolidCone(30.0, 70.0, 30, 30);

    //
    glPopMatrix();

    glPopMatrix();

    // Executa os comandos OpenGL
    glutSwapBuffers();
}

// =======================================================================
void inicializa( void )
{
    // Define a cor de fundo da janela de visualizacao como preta
    //             R     G     B    alfa
    glClearColor( 1.0 , 1.0 , 1.0 , 1.0 );

    transf.dx   = 0.0;
    transf.dy   = 0.0;
    transf.dz   = 0.0;
    transf.sx   = 1.0;
    transf.sy   = 1.0;
    transf.sz   = 1.0;
    transf.angx = 0.0;
    transf.angy = 0.0;
    transf.angz = 0.0;

    camera.posx   = 0;
    camera.posy   = 0;
    camera.posz   = 500;
    camera.alvox  = 0;
    camera.alvoy  = 0;
    camera.alvoz  = 0;
    camera.inicio = 0.1;
    camera.fim    = 5000.0;
    camera.ang    = 45;

    transformacao = 'T';
    eixo = 'X';

    visual_eixo = true;
    wireframe = true;

    passo = 5;

    // habilita a transparenica
    glEnable( GL_BLEND );

    // define a forma de calculo da transparencia
    glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

    // Habilita antialiasing
    glEnable( GL_LINE_SMOOTH );

    // Solicita melhor qualidade
    glHint( GL_LINE_SMOOTH_HINT , GL_NICEST );

    glutSetWindow( jan[ 0 ] );
    glutReshapeWindow( LAR_HELP , ALT_HELP );
    glutPositionWindow( 100 , 100 );

    glutSetWindow( jan[ 1 ] );
    glutReshapeWindow( LAR_MAIN , ALT_MAIN+ALT_BARRA );
    glutPositionWindow( ( glutGet( GLUT_SCREEN_WIDTH  ) - LAR_MAIN ) / 2 ,
                        ( glutGet( GLUT_SCREEN_HEIGHT ) - (ALT_MAIN+ALT_BARRA)  ) / 2 );

    glEnable(GL_DEPTH_TEST); // habilita o Z buffer

    glEnable(GL_CULL_FACE);

    // Habilita a definição da cor do material a partir da cor corrente
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de iluminação
    glEnable(GL_LIGHTING);
    // Habilita a luz de número 0
    glEnable(GL_LIGHT0);
    // Habilita o depth-buffering
    //glEnable(GL_DEPTH_TEST);

    // Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);

    // Inicializa a variável que especifica o ângulo da projeção
    // perspectiva
    angle=50;

    // Inicializa as variáveis usadas para alterar a posição do
    // observador virtual
    rotX = 30;
    rotY = 0;
    obsZ = 180;
}

// =======================================================================
// Funcao usada para especificar o volume de visualizacao
void especifica_parametros_visualizacao( void )
{
    // seleciona o tipo de matriz para a projecao
    glMatrixMode( GL_PROJECTION );

    // limpa (zera) as matrizes
    glLoadIdentity();

    DefineIluminacao();

    // Especifica e configura a projecao perspectiva
    gluPerspective( camera.ang , janela.aspecto , camera.inicio , camera.fim );

    // Especifica sistema de coordenadas do modelo
    glMatrixMode( GL_MODELVIEW );

    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();

    // Especifica posicao da camera (o observador) e do alvo
    gluLookAt( camera.posx , camera.posy , camera.posz , camera.alvox , camera.alvoy , camera.alvoz ,0,1,0);
}

// =======================================================================
// Funcao callback chamada quando o tamanho da janela eh alterado
void altera_tamanho_janela( GLsizei largura , GLsizei altura )
{
    janela.largura = largura;
    janela.altura  = altura-ALT_BARRA;

    // Para previnir uma divis„o por zero
    if ( janela.altura == 0 ) janela.altura = 1;

    // Especifica as dimensıes da viewport
    glViewport( 0 , 0 , janela.largura , janela.altura );

    // Calcula o aspecto
    janela.aspecto = janela.largura / janela.altura;

    especifica_parametros_visualizacao();
}

// =======================================================================
// Funcao de tratamento do teclado
void teclado( unsigned char key , GLint x , GLint y )
{
    GLint modificador = glutGetModifiers();

    if ( modificador & GLUT_ACTIVE_ALT)
    {
        // ALT pressionado
        if( key == '-' || key == '_' )
            if( passo - 1 > 0 )
                passo--;

        if( key == '+' || key == '=' )
            passo++;
    }
    else
    {
        if ( key == 27 )
            exit( 0 );

        if ( toupper( key ) == 'I' )
            inicializa();

        if ( toupper( key ) == 'W' )
            wireframe = !wireframe;

        if ( toupper( key ) == 'X' || toupper( key ) == 'Y' || toupper( key ) == 'Z')
            eixo = toupper( key );

        if ( toupper( key ) == 'S' || toupper( key ) == 'R' || toupper( key ) == 'T' )
            transformacao = toupper( key );

        if ( key == 'A')
            if ( camera.ang+passo < 180 )
                camera.ang += passo;

        if ( key == 'a')
            if ( camera.ang-passo > 0 )
                camera.ang -= passo;

        if ( key == 'l' || key == 'L' )
        {
            if( glIsEnabled( GL_LINE_SMOOTH ) )
                glDisable( GL_LINE_SMOOTH );
            else
                glEnable( GL_LINE_SMOOTH );
        }

        if ( key == 'e' || key == 'E' )
            visual_eixo = !visual_eixo;

        if ( key == '+' || key == '=')
            switch( transformacao ){
                case 'S':
                    if ( eixo == 'X')
                        transf.sx += 0.1;
                    if ( eixo == 'Y')
                        transf.sy += 0.1;
                    if ( eixo == 'Z')
                        transf.sz += 0.1;
                    break;

                case 'R':
                    if ( eixo == 'X')
                        transf.angx += passo;
                    if ( eixo == 'Y')
                        transf.angy += passo;
                    if ( eixo == 'Z')
                        transf.angz += passo;
                    break;

                case 'T':
                    if ( eixo == 'X')
                        transf.dx += passo;
                    if ( eixo == 'Y')
                        transf.dy += passo;
                    if ( eixo == 'Z')
                        transf.dz += passo;
                    break;
            }

        if ( key == '-' || key == '_' )
            switch( transformacao ){
                case 'S':
                    if ( eixo == 'X')
                        transf.sx -= 0.1;
                    if ( eixo == 'Y')
                        transf.sy -= 0.1;
                    if ( eixo == 'Z')
                        transf.sz -= 0.1;
                    break;

                case 'R':
                    if ( eixo == 'X')
                        transf.angx -= passo;
                    if ( eixo == 'Y')
                        transf.angy -= passo;
                    if ( eixo == 'Z')
                        transf.angz -= passo;
                    break;

                case 'T':
                    if ( eixo == 'X')
                        transf.dx -= passo;
                    if ( eixo == 'Y')
                        transf.dy -= passo;
                    if ( eixo == 'Z')
                        transf.dz -= passo;
                    break;
            }

    }
    especifica_parametros_visualizacao();

    glutPostRedisplay();
}

// =======================================================================
// tratamento das teclas especiais (teclas de funcao e de navegacao).
// os parametros que recebe sao a tecla pressionada e a posicao x e y
void teclas_especiais( GLint key , GLint x , GLint y )
{
    GLint modificador = glutGetModifiers();

    if ( modificador & GLUT_ACTIVE_ALT)
    {
        // ALT pressionado
    }
    else
    {
        if ( key == GLUT_KEY_LEFT )
        {
            camera.posx  -= passo;
            camera.alvox -= passo;
        }

        if ( key == GLUT_KEY_RIGHT )
        {
            camera.posx  += passo;
            camera.alvox += passo;
        }

        if ( key == GLUT_KEY_UP )
        {
            camera.posy  += passo;
            camera.alvoy += passo;
        }

        if ( key == GLUT_KEY_DOWN )
        {
            camera.posy  -= passo;
            camera.alvoy -= passo;
        }

        if ( key == GLUT_KEY_PAGE_UP ) // aumenta o tamanho da window
        {
            camera.posz  -= passo;
            camera.alvoz -= passo;
        }

        if ( key == GLUT_KEY_PAGE_DOWN) // diminui o tamanho da window
        {
            camera.posz  += passo;
            camera.alvoz += passo;
        }
    }

    especifica_parametros_visualizacao();

    // como foi mudado dados que influenciam na visualizacao dos objetos,
    // este comando obriga a executar a funcao de desenho (desenha)
    glutPostRedisplay();
}

// =======================================================================
// Programa Principal
int main( int argc , char *argv[] )
{
    glutInit( &argc , argv );

    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // janela do help
    glutInitWindowSize( LAR_HELP , ALT_HELP );
    glutInitWindowPosition( 100 , 100 );
    jan[ 0 ] = glutCreateWindow( "HELP" );

    // janela principal
    glutInitWindowSize( LAR_MAIN , ALT_MAIN+ALT_BARRA );
    glutInitWindowPosition( ( glutGet( GLUT_SCREEN_WIDTH  ) - LAR_MAIN ) / 2 ,
                            ( glutGet( GLUT_SCREEN_HEIGHT ) - ALT_MAIN+ALT_BARRA  ) / 2 );
    jan[ 1 ] = glutCreateWindow( "PROJECAO 3D" );

    // callbacks da segunda de help
    glutSetWindow( jan[ 0 ] );
    glutDisplayFunc( desenha_help );
    glutReshapeFunc( altera_tamanho_janela_help );

    // callbacks da janela principal
    glutSetWindow( jan[ 1 ] );
    glutDisplayFunc( desenha );
    glutKeyboardFunc( teclado );
    glutSpecialFunc( teclas_especiais );
    glutReshapeFunc( altera_tamanho_janela );

    // funcao que tem as inicializacoes de variaveis e estados do OpenGL
    inicializa();

    // Ultimo comando. Faz com que todas as janelas criadas sejam mostradas
    // Uma vez neste loop, somente sai quando o programa encerra
    glutMainLoop();

    return 0;
}
