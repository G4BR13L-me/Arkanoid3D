//
//  Breakout.cpp
//  ogamalBreakout
//
//  Created by Osama Attia on 9/21/14.
//  ogamal@iastate.edu
//
#include <stdio.h>
#include "Breakout.h"

using namespace std;

MyObjects objects;

void recomputeFrame(int value);
float Sx=-15.0,Tz=0;

Breakout::Breakout() {
    init();
}

Breakout::~Breakout() {

}


void Breakout::display(void) {
	
    // Clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Draw my cool gradient background
    //drawBackground();
	glTranslatef(0.0,0.0,Tz);
	glRotatef(Sx,1.0,0.0,0.0);
	
    // Select which state of the game to display
    switch (gameState) {
        case INIT:
            // Init values
            init();
            break;
            
        case Menus:
            // TODO List menu
            break;
            
        case Gameplay:
            // Draw the game
            drawGame();
            // If no balls, player loses the game
            if (balls.size() <= 0 && lifesCount > 0) {
                newBall(-1, -1);
                lifesCount--;
                reward = 100;
            } else if (balls.size() <= 0) {
                // TODO - GAME OVER
            }
            
            // If no bricks, player wins the level
            if (bricks.size() <= 0 && level <= 2) {
                level++;
                initBricks();
            } else if (bricks.size() <= 0) {
                // TODO - PLAYER WON
            }
            break;
            
        case Scoreboard:
            // TODO
            break;
        
        default:
            break;
    }
    
    glutTimerFunc(TIMER, recomputeFrame, 0);
    
    glutSwapBuffers();
}

void recomputeFrame(int value) {
	glutPostRedisplay();
}

void Breakout::set3DView(void) {
	// Turn on lighting
    GLfloat light_position[] = {-1.0, 1.0, 1.0, 0.0};
	GLfloat light_color[] = {1.0, 1.0, 1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	// Turn on material characteristics
	GLfloat mat_ambient_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambient_diffuse);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
	
	// Turn on Gouraud shading
    glShadeModel(GL_SMOOTH);
	
	// Turn on z-buffering
    glEnable(GL_DEPTH_TEST);
	
	// set white to be the erase color
    glClearColor(1.0, 1.0, 1.0, 1.0);
	
	// set initial color (blue)
    glColor3f(0.0, 0.0, 1.0);
}

void Breakout::init(void) {
    // Reset game statistics
    score = 0;
    level = 1;
    reward = 100;
    lifesCount = 3;
    
    // Remove all balls
    balls.clear();
    
    // Remove all bricks
    bricks.clear();
    
    // Init bricks
    initBricks();
    
    // Add ball and paddle
    initPaddle();
    newBall(-1, -1);
    
    // Start game play
    gameState = Breakout::Gameplay;
}

void Breakout::drawBackground(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    // Top color
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex2f(WINWIDTH, WINHEIGHT);
    glVertex2f(-WINWIDTH, WINHEIGHT);
    // Bottom color
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glEnd();
}

void Breakout::drawGame(void) {
    // // Draw coordinates for guidance
    // drawCoordinate();
    
    // Draw balls
    drawBalls();
    
    // Draw bricks
    drawBricks();
    
    // Draw paddle
    drawPaddle();
    
    // Draw game statistics (lifes, score)
    // drawGameStats();

}

void Breakout::newBall(float x = -1, float y = -1) {
    Ball b1;
    b1.xpos = 0.0;
    b1.ypos = -4.8f;
    b1.launch = false;
	b1.xvel = 0.15f/1.5;
    b1.yvel = 0.05f/1.5;
    b1.radius = BALL_RADIUS;
    b1.r = 0.4f + (float) rand() / (RAND_MAX);
    b1.g = 0.25f + (float) rand() / (RAND_MAX);
    b1.b = 0.4f + (float) rand() / (RAND_MAX);
    balls.push_back(b1);
}

void Breakout::drawBalls(void) {
    for( vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it) {
        glPushMatrix();
		   	glColor3f(it->r, it->g, it->b);
		   	glTranslatef(it->xpos, it->ypos, 0.0);
		   	// glScalef (2.5, 0.5, 0.5);
			glutSolidSphere(it->radius,SPHERE_SEGMENTS,SPHERE_SEGMENTS);
		   	// objects.drawCube(0.5);
		glPopMatrix();
		
		// Wait launch     
  		if(!it->launch)
			continue;
  		
        // Set new position
        it->xpos += it->xvel;
        it->ypos += it->yvel;
        
        // Collision with left/right/top window sides
        if(it->xpos > 4.7f || it->xpos < -4.7f) {
            it->xvel *= -1;
        }
        if(it->ypos > 5.2f) {
            it->yvel *= -1;
        }
        if(it->ypos < -5.2f) {
            it = balls.erase(it);
            --it;
            continue;
        }
        //br->width/2, br->height/2
        // Collission with the bricks
        for( std::vector<Brick>::iterator br = bricks.begin(); br != bricks.end(); ) {
            // Check collission between circle and vertical brick sides
            if( (it->ypos + it->radius >= br->ypos - br->height/2) && (it->ypos - it->radius <= br->ypos + br->height/2) ) {
                // brick right edge and left point on circle
                if( (it->xpos - it->radius <= br->xpos + br->width/2) && (it->xpos + it->radius > br->xpos + br->width/2) ) {
                    it->xvel *= -1;
                    br = hitBrick(br);
                    cout << "Direita" << endl;
                    continue;
                }
                
                // brick left edge and right point on circle
                if( (it->xpos + it->radius >= br->xpos - br->width/2) && (it->xpos - it->radius < br->xpos - br->width/2) ) {
                    it->xvel *= -1;
                    br = hitBrick(br);
                    cout << "Esquerda" << endl;
                    continue;
                }
            }
            
            // Check collission between circle and horizontal brick sides
            if( (it->xpos - it->radius <= br->xpos + br->width/2) && (it->xpos + it->radius >= br->xpos - br->width/2) ) {
                // brick bottom edge and top point on circle
                if( (it->ypos + it->radius >= br->ypos - br->height/2) && (it->ypos - it->radius < br->ypos - br->height/2) ) {
                    it->yvel *= -1;
                    br = hitBrick(br);
                    cout << "Baixo" << endl;
                    continue;
                }
                
                // brick top edge and bottom point on circle
                if( (it->ypos - it->radius <= br->ypos + br->height/2) && (it->ypos + it->radius > br->ypos + br->height/2) ) {
                    it->yvel *= -1;
                    br = hitBrick(br);
                    cout << "Cima" << endl;
                    continue;
                }
            }
            
            ++br; // next brick
        }
        
        // Check collission between paddle's top edge and bottom point on circle
        if( (it->xpos - it->radius <= paddle.xpos + paddle.width/2) && (it->xpos + it->radius >= paddle.xpos - paddle.width/2) ) {
            if( (it->ypos - it->radius <= paddle.ypos + paddle.height/2) && (it->ypos + it->radius > paddle.ypos + paddle.height/2) ) {
                it->yvel *= -1;
                reward = 100;
                score += reward;
                continue;
            }
        }
    }
}

void Breakout::initPaddle(void) {
    paddle.r = 0.2f;
    paddle.g = 0.5f;
    paddle.b = 1.0f;
    paddle.width = 2.5f;
    paddle.height = 0.5f;
    paddle.xpos = 0.0f;
    paddle.ypos = -5.4f;
}

void Breakout::drawPaddle() {
	glPushMatrix();
		glColor3f(paddle.r, paddle.g, paddle.b);
		glTranslatef(paddle.xpos, paddle.ypos, 0.0);
		glScalef (paddle.width, paddle.height, paddle.height);
		glutSolidCube(0.5);
		// objects.drawCube(0.5);
	glPopMatrix();
}

void Breakout::drawBricks(void) {
	
    for (std::vector<Brick>::iterator it = bricks.begin(); it != bricks.end(); ++it) {
		glPushMatrix();
    		glColor3f(it->r, it->g, it->b);
			glTranslatef(it->xpos, it->ypos, 0);
			glScalef(it->width, it->height, it->height);
			// glutSolidCube(0.5);
			objects.drawCube(0.5);
		glPopMatrix();
    }
}

template <typename Iterator>
Iterator Breakout::hitBrick(Iterator brick) {
    score += reward;
    reward += 25;
//    system("afpqlay ../../cartoon008.wav");
    
    // Decrease brick health
    if (brick->health > 1) {
        brick->r = 0.95f;
        brick->g = 0.95f;
        brick->b = 0.95f;
        brick->health -= 1;
        return ++brick;
    } else {
        return bricks.erase(brick);
    }
}

void Breakout::initBricks(void) {
    if (level == 1)
        bricksLevel1();
    else if (level == 2)
        bricksLevel2();
}

void Breakout::bricksLevel1(void) {
    Brick newBrick;
    newBrick.r = 0.95f;
    newBrick.g = 0.95f;
    newBrick.b = 0.95f;
    newBrick.health = 1;
    newBrick.width = 1.0;
    newBrick.height = 0.5;
    
    for (int i = 0; i < WALLROWS; ++i) {
        for (int j = 0; j < WALLCOLS; ++j) {
            // Set stronger bricks
            if(false) {
                newBrick.r = 1.0f;
                newBrick.g = 0.5f;
                newBrick.b = 0.5f;
                newBrick.health = 2;
            } else {
				newBrick.r = 1.0f;
                newBrick.g = 0.5f;
                newBrick.b = 0.5f;
                newBrick.health = 1;
            }
            
            newBrick.xpos = -2.6 + j * newBrick.width + j * WALLSPACE;
            newBrick.ypos = -1 + i * newBrick.height + i * WALLSPACE;
            bricks.push_back(newBrick);
        }
    }
}

void Breakout::bricksLevel2(void) {
    Brick newBrick;
    newBrick.width = (WALLWIDTH - (WALLCOLS - 2) * WALLSPACE) / WALLCOLS;
    newBrick.height = (WALLHEIGHT - (WALLROWS - 2) * WALLSPACE) / WALLROWS;
    
    for (int i = 0; i < WALLROWS; i++) {
        for (int j = 0; j < WALLCOLS; j++) {
            // Set stronger bricks
            if (i == 1 || i == WALLROWS - 2 || j == 1 || j == WALLCOLS - 2) {
                newBrick.r = 1.0f;
                newBrick.g = 0.5f;
                newBrick.b = 0.5f;
                newBrick.health = 2;
            } else {
                newBrick.r = 0.95f;
                newBrick.g = 0.95f;
                newBrick.b = 0.95f;
                newBrick.health = 1;
            }
            
            newBrick.xpos = WALLX + j * newBrick.width + j * WALLSPACE;
            newBrick.ypos = WALLY + i * newBrick.height + i * WALLSPACE;
            bricks.push_back(newBrick);
        }
    }
}

void Breakout::drawGameStats(void) {
    glBegin(GL_LINES);
    // Bottom right (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(20.0f, 30.0f);
    glVertex2f(WINWIDTH - 20.0f, 30.0f);
    glEnd();
    
    float offset = 25.0f;
    for (int i = 0; i < lifesCount && i < 10; ++i) {
        drawLife(35 + offset * i, 15);
    }
    
    drawScore();
}

void Breakout::drawLife(float x, float y) {
    // Scale the heart symbol
    float const scale = 0.5f;
    
    // Heart symbol equations from Walfram Mathworld: http://mathworld.wolfram.com/HeartCurve.html
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.2f, 0.2f);
    for(int j = 0; j < SPHERE_SEGMENTS; j++) {
        float const theta = 2.0f * 3.1415926f * (float)j / (float)SPHERE_SEGMENTS;
        float const xx = scale * 16.0f * sinf(theta) * sinf(theta) * sinf(theta);
        float const yy = -1 * scale * (13.0f * cosf(theta) - 5.0f * cosf(2.0f * theta) - 2 * cosf(3.0f * theta) - cosf(4.0f * theta));
        glVertex2f(x + xx, y + yy);
    }
    glEnd();
}

void Breakout::drawScore(void) {
    glPushMatrix();
    // Write score word
    glColor3f(1.0f, 0.7f, 0.7f);
    glRasterPos2f(WINWIDTH - 120, 20);
    char buf[300], *p;
    p = buf;
    sprintf(buf, "Score: ");
    do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); while(*(++p));
    // Print the score
    p = buf;
    sprintf(buf, "           %d", score);
    glColor3f(1.0f, 0.2f, 0.2f);
    glRasterPos2f(WINWIDTH - 120, 20);
    do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); while(*(++p));
    glPopMatrix();
}

void Breakout::drawCoordinate(void) {
    glBegin(GL_LINES);
        // Top left (white)
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(20.0f, 10.0f);
        glVertex2f(20.0f, 30.0f);
        glVertex2f(10.0f, 20.0f);
        glVertex2f(30.0f, 20.0f);
    
        // Bottom right (red)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(WINWIDTH - 20.0f, WINHEIGHT - 10.0f);
        glVertex2f(WINWIDTH - 20.0f, WINHEIGHT - 30.0f);
        glVertex2f(WINWIDTH - 10.0f, WINHEIGHT - 20.0f);
        glVertex2f(WINWIDTH - 30.0f, WINHEIGHT - 20.0f);
    glEnd();
}

void Breakout::reshape(int width, int height) {
    if (width != WINWIDTH || height != WINHEIGHT)
        glutReshapeWindow(WINWIDTH, WINHEIGHT);
    
	glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

    glViewport (0, 0, (GLsizei) WINWIDTH, (GLsizei) WINHEIGHT);

    gluPerspective(100, (float)WINHEIGHT/(float)WINHEIGHT, 1.0, 100.0);
    gluLookAt(0.0,0.0,6.5, 	// posição da câmera (olho) 
			  0.0,0.0,0.0, 	// centro da cena
			  0.0,1.0,0.0); // direção de cima 
    
    glMatrixMode (GL_MODELVIEW);
}

void Breakout::mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        newBall(x/100.0, y/100.0);
    }
    
    // Force redraw
	glutPostRedisplay();
}

void Breakout::mouseMove(int x, int y) {

}

void Breakout::keyStroke(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            Tz += 1.0;
            break;
        case 's':
        	Tz += -1.0;
            break;
        case 'd':
        	Sx += 1.0;
            break;
        case 'a':
        	Sx += -1.0;
            break;
        case 'q': // Exit
            exit(0);
            break;
        case 'n': // New game
            init();
            break;
        case 'h':
            lifesCount++;
            break;
        case 32: // SPACE bar
			for( vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it) {     
		  		if(it->launch)
	  				continue;
		  		it->launch = true;
		  		break;
		    }
            break;
        case 27: // Esc button
            exit(0);
            break;
        default:
            break;
    }
    cout << "Sx: " << Sx << endl;
}

void Breakout::specialKeyPos(int key, int x, int y) {
    switch(key)
	{
		case GLUT_KEY_LEFT:
			if (paddle.xpos > -5.0) {
                paddle.xpos -= 0.15f;
            }
            for( vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it) {     
		  		if(!it->launch){
					it->xpos -= 0.15f;  
				}
		    }
            break;
        case GLUT_KEY_RIGHT:
            if (paddle.xpos < 5.0) {
                paddle.xpos += 0.15f;
            }
            for( vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it) {     
		  		if(!it->launch){
					it->xpos += 0.15f;  
				}
		    }
            break;
        default:
            break;
    }
}
