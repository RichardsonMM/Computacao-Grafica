//  Aluno: Richardson Patrick Matos de Moraes
//  main.cpp
//  Meu Primeiro Programa em OpenGL
//
//  Created by imac on 01/08/2019.
//  Copyright © 2019 imac. All rights reserved.
//

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#pragma clang diagnostic ignored "-Wdeprecated-declarations"

using namespace std;

//=====================================================================

void teclado(unsigned char tecla, int x, int y){
    if(tecla == 27){ // se foi clicado na tecla "Esc"
        exit(0);
    }
}

//=====================================================================

void banana(void ){
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(0.0, 0.0, 0.0);
    
    glLineWidth(20);
    
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0.6, -0.2);
        glColor3f(0.0, 1.0, 0.0);

        glVertex2f(0.3, 0.4);
        glColor3f(0.0, 0.0, 1.0);

        glVertex2f(-0.2, 0.3);

    
    glEnd();
  
    
    glFlush();

}

//=====================================================================

void inicializa(void){
    glClearColor(0.0, 1.0, 0.0, 1.0); // cor de fundo
}

//=====================================================================


int main(int argc, char * argv[]) {
    
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    
    glutInitWindowSize(500, 500);
    
    glutCreateWindow("Primeiro Programa OpenGL");
    
    glutDisplayFunc(banana);
    
    glutKeyboardFunc(teclado);
    
    inicializa();
    
    glutMainLoop();
    
    
    
    
    return 0;
}
