#include <stdio.h>
#include "MyObjects.h"

using namespace std;

MyObjects::MyObjects() {
	faces[0][0] = 7; faces[0][1] = 4; faces[0][2] = 5;
	faces[1][0] = 7; faces[1][1] = 5; faces[1][2] = 6;
	faces[2][0] = 4; faces[2][1] = 1; faces[2][2] = 5;
	faces[3][0] = 5; faces[3][1] = 1; faces[3][2] = 2;
	faces[4][0] = 5; faces[4][1] = 2; faces[4][2] = 6;
	faces[5][0] = 6; faces[5][1] = 2; faces[5][2] = 3;
	faces[6][0] = 6; faces[6][1] = 3; faces[6][2] = 7;
	faces[7][0] = 7; faces[7][1] = 3; faces[7][2] = 0;
	faces[8][0] = 7; faces[8][1] = 0; faces[8][2] = 4;
	faces[9][0] = 4; faces[9][1] = 0; faces[9][2] = 1;
	faces[10][0] = 3; faces[10][1] = 2; faces[10][2] = 0;
	faces[11][0] = 0; faces[11][1] = 2; faces[11][2] = 1;
}

MyObjects::~MyObjects() {

}

void MyObjects::drawCube(GLfloat size){	
	vertices[0][0] = vertices[1][0] = vertices[4][0] = vertices[7][0] = -size / 2;
	vertices[2][0] = vertices[3][0] = vertices[5][0] = vertices[6][0] = size / 2;
	vertices[0][1] = vertices[1][1] = vertices[2][1] = vertices[3][1] = -size / 2;
	vertices[4][1] = vertices[5][1] = vertices[6][1] = vertices[7][1] = size / 2;
	vertices[0][2] = vertices[3][2] = vertices[6][2] = vertices[7][2] = -size / 2;
	vertices[1][2] = vertices[2][2] = vertices[4][2] = vertices[5][2] = size / 2;
	
	evaluateFaceNormals();
    evaluateVerticesNormals();
	
    GLuint i;

    /* Desenha todos os triângulos do objeto */
    glBegin(GL_TRIANGLES);
        for (i=0; i<N_FACES; i++){
            glNormal3fv(vertex_normals[faces[i][0]]);
            glVertex3fv(vertices[faces[i][0]]);

            glNormal3fv(vertex_normals[faces[i][1]]);
            glVertex3fv(vertices[faces[i][1]]);

            glNormal3fv(vertex_normals[faces[i][2]]);
            glVertex3fv(vertices[faces[i][2]]);
        }
    glEnd();
}

void MyObjects::normalize(ponto * n){
	GLfloat length;
	
	length = (GLfloat) sqrt((n->x * n->x) + (n->y * n->y) + (n->z * n->z));
		
	if (length == 0.0f)
		length =  1.0f;

	n->x = n->x / length;
	n->y = n->y / length;
	n->z = n->z / length;
}
    
void MyObjects::evaluateFaceNormals(void){
    float x[3], y[3], z[3];
    ponto a, b, n;
        
    for(int i = 0; i < N_FACES; i++){
        x[0] = vertices[faces[i][0]][0];
        y[0] = vertices[faces[i][0]][1];
        z[0] = vertices[faces[i][0]][2];
    
        x[1] = vertices[faces[i][1]][0];
        y[1] = vertices[faces[i][1]][1];
        z[1] = vertices[faces[i][1]][2];
    
        x[2] = vertices[faces[i][2]][0];
        y[2] = vertices[faces[i][2]][1];
        z[2] = vertices[faces[i][2]][2];
        
        a.x = x[2]- x[0];
        a.y = y[2]- y[0];
        a.z = z[2]- z[0];
        
        b.x = x[2]- x[1];
        b.y = y[2]- y[1];
        b.z = z[2]- z[1];
        
        n.x = a.y * b.z - a.z * b.y;
        n.y = a.z * b.x - a.x * b.z;
        n.z = a.x * b.y - a.y * b.x;
       
        normalize(&n);
        
        face_normals[i][0] = n.x;
        face_normals[i][1] = n.y;
        face_normals[i][2] = n.z;
    }
}

void MyObjects::evaluateVerticesNormals(void){
    ponto n;
    for(unsigned int i = 0; i < N_VERTICES; i++){
        n.x = 0.0;
        n.y = 0.0;
        n.z = 0.0;
        for(int j = 0; j < N_FACES; j++){
            if(faces[j][0] == i || faces[j][1] == i || faces[j][2] == i){
                n.x += face_normals[j][0];
                n.y += face_normals[j][1];
                n.z += face_normals[j][2];
            }
        }
        
        n.x /= 3.0;
        n.y /= 3.0;
        n.z /= 3.0;        
        
        normalize(&n);
        
        vertex_normals[i][0] = n.x;
        vertex_normals[i][1] = n.y;
        vertex_normals[i][2] = n.z;        

    }
    
} 