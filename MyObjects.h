#ifndef MyObjects_h
#define MyObjects_h

#include<GL/glut.h>
#include <math.h>

#define N_VERTICES 8
#define N_FACES 12

//Estrutura criada para facilitar o entendimento
struct ponto{
    float x;
    float y;
    float z;
};

// Ball
struct Ball {
	bool launch;
    GLfloat xpos, ypos;
    GLfloat xvel, yvel;
    GLfloat radius;
    GLfloat r, g, b;
};

// Paddle
struct Paddle {
    GLfloat xpos, ypos;
    GLfloat width, height;
    GLfloat r, g, b;
};

// Brick
struct Brick {
    GLfloat xpos, ypos;
    GLfloat width, height;
    GLfloat r, g, b;
    GLint health;
    GLint value;
};

class MyObjects {

public:
	MyObjects();
	~MyObjects();
	
    // 3D model handle
	GLfloat vertices[N_VERTICES][3];
	
	GLuint faces[N_FACES][3];
	
	GLfloat vertex_normals[N_VERTICES][3];
	
	GLfloat face_normals[N_FACES][3];
	
	void drawCube(GLfloat size);
	
	void normalize(ponto * n);
	    
	void evaluateFaceNormals(void);
	
	void evaluateVerticesNormals(void);
	
};

#endif