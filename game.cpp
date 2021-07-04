#include "game.h"

using namespace std;

MyObjects objects;

void recomputeFrame(int value);
float Sx=-15.0,Tz=0;

Game::Game() {
    init();
}

Game::~Game() {
	
}

void Game::display(void) {
	
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
                newBall(paddle.xpos);
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

void Game::set3DView(void) {
	// Turn on lighting
    GLfloat light_position[] = {-1.0, 1.0, 1.5, 0.0};
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
	
	// set black to be the erase color
    glClearColor(0.3, 0.3, 0.3, 0.0);
}

void Game::init(void) {
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
    newBall(0);
    
    // Start game play
    gameState = Game::Gameplay;
    PlaySoundEffect(BACKGROUND);
}

void Game::drawBackground(void) {
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

void Game::drawGame(void) {
    // Draw balls
    drawBalls();
    
    // Draw bricks
    drawBricks();
    
    // Draw paddle
    drawPaddle();
    
    // Draw game statistics (lifes, score)
    drawGameStats();

}

void Game::newBall(float x) {
    Ball b1;
    b1.xpos = x;
    b1.ypos = -4.8f;
    b1.launch = false;
    
    // Set X speed
    if ((float) rand() / (RAND_MAX) < 0.5){
		if((float) rand() / (RAND_MAX) < 0.5)
			b1.xvel = 0.1f;
        else
			b1.xvel = 0.125f;	
	} else {
		if((float) rand() / (RAND_MAX) < 0.5)
			b1.xvel = -0.1f;
        else
			b1.xvel = -0.125f;
	}
    
	// Set Y speed    
    if ((float) rand() / (RAND_MAX) < 0.5)
		b1.yvel = 0.032f;
	else 
		b1.yvel = 0.040f;
    
    b1.radius = BALL_RADIUS;
    b1.r = 0.4f + (float) rand() / (RAND_MAX);
    b1.g = 0.25f + (float) rand() / (RAND_MAX);
    b1.b = 0.4f + (float) rand() / (RAND_MAX);
    balls.push_back(b1);
}

void Game::drawBalls(void) {
    for( vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it) {
        glPushMatrix();
		   	glColor3f(it->r, it->g, it->b);
		   	glTranslatef(it->xpos, it->ypos, 0.0);
			glutSolidSphere(it->radius,SPHERE_SEGMENTS,SPHERE_SEGMENTS);
		glPopMatrix();
		
		// Wait launch     
  		if(!it->launch)
			continue;
  		
        // Set new position
        it->xpos += it->xvel;
        it->ypos += it->yvel;
        
        // Collision with left/right/top window sides
        if(it->xpos + it->radius > 4.7 || it->xpos - it->radius < -4.7)
            it->xvel *= -1;
        if(it->ypos + it->radius > 4.0)
            it->yvel *= -1;
        if(it->ypos - it->radius < -6.2) {
            it = balls.erase(it);
            --it;
            continue;
        }
        
        // Collission with the bricks
        for( std::vector<Brick>::iterator br = bricks.begin(); br != bricks.end(); ) {
            // Collission between ball and vertical brick sides
            if( (it->ypos + it->radius >= br->ypos - br->height/2) && (it->ypos - it->radius <= br->ypos + br->height/2) ) {
                // brick right edge and left point on ball
                if( (it->xpos - it->radius <= br->xpos + br->width/2) && (it->xpos + it->radius > br->xpos + br->width/2) ) {
                    it->xvel *= -1;
                    br = hitBrick(br);
                    continue;
                }
                
                // brick left edge and right point on ball
                if( (it->xpos + it->radius >= br->xpos - br->width/2) && (it->xpos - it->radius < br->xpos - br->width/2) ) {
                    it->xvel *= -1;
                    br = hitBrick(br);
                    continue;
                }
            }
            
            // Collission between ball and horizontal brick sides
            if( (it->xpos - it->radius <= br->xpos + br->width/2) && (it->xpos + it->radius >= br->xpos - br->width/2) ) {
                // brick bottom edge and top point on ball
                if( (it->ypos + it->radius >= br->ypos - br->height/2) && (it->ypos - it->radius < br->ypos - br->height/2) ) {
                    it->yvel *= -1;
                    br = hitBrick(br);
                    continue;
                }
                
                // brick top edge and bottom point on ball
                if( (it->ypos - it->radius <= br->ypos + br->height/2) && (it->ypos + it->radius > br->ypos + br->height/2) ) {
                    it->yvel *= -1;
                    br = hitBrick(br);
                    continue;
                }
            }
            
            ++br; // next brick
        }
        
        // Collission between paddle's top edge and bottom point on ball
        if( (it->xpos <= paddle.xpos + paddle.width/2) && (it->xpos >= paddle.xpos - paddle.width/2) ) {
            if( (it->ypos - it->radius <= paddle.ypos + paddle.height/2) && (it->ypos + it->radius > paddle.ypos + paddle.height/2) ) {
                it->yvel *= -1;
                reward = 100;
                score += reward;
                continue;
            }
        }
    }
}

void Game::initPaddle(void) {
    paddle.r = 0.2f;
    paddle.g = 0.5f;
    paddle.b = 1.0f;
    paddle.width = 1.5f;
    paddle.height = 0.25f;
    paddle.xpos = 0.0f;
    paddle.ypos = -5.4f;
}

void Game::drawPaddle() {
	glPushMatrix();
		glColor3f(paddle.r, paddle.g, paddle.b);
		glTranslatef(paddle.xpos, paddle.ypos, 0.0);
		glScalef (paddle.width, paddle.height, paddle.height);
		glutSolidCube(1.0);
	glPopMatrix();
}

void Game::drawBricks(void) {
    for (std::vector<Brick>::iterator it = bricks.begin(); it != bricks.end(); ++it) {
		glPushMatrix();
    		glColor3f(it->r, it->g, it->b);
			glTranslatef(it->xpos, it->ypos, 0);
			glScalef(it->width, it->height, it->height);
			objects.drawCube(1.0);
		glPopMatrix();
    }
}

template <typename Iterator>
Iterator Game::hitBrick(Iterator brick) {
    score += reward;
    reward += 25;
    
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

void Game::initBricks(void) {
    if (level == 1)
        generateBricks(1);
    else
        generateBricks(2);
}

void Game::generateBricks(int level) {
	bool strongerBrick;
	int rows = WALLROWS, cols = WALLCOLS;
	if(level ==1)
		rows += -2;
		
    for ( int i = 0; i < rows; ++i) {
        for ( int j = 0; j < cols; ++j) {
			Brick newBrick;
			newBrick.width = 0.5;
			newBrick.height = 0.5;
			newBrick.xpos = -3.4;
			newBrick.ypos = 2.0;
        	
        	// Set stronger bricks
        	strongerBrick = ( level == 1 
					? ((i+j)%2 != 0) // Level 1 
					: (i == 1 || i == WALLROWS - 2 || j == 2 || j == WALLCOLS - 3)); //Level 2
			
			if (strongerBrick) {
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
            
            newBrick.xpos += j;
            newBrick.ypos -= i;
            bricks.push_back(newBrick);
        }
    }
}

void Game::drawGameStats(void) {
    float offset = 25.0f;
	glPushMatrix();
		glTranslatef(10.0,10.0,0.0);
		glRotatef(180,0.0,0.0,1.0);
		glRotatef(Sx,1.0,0.0,0.0);
		glScalef(0.07, 0.07, 0.07);
		for(int i = 0; i < lifesCount && i < 10; ++i)
		{
			drawLife(35 + offset * (i%3), 15 + 25*(i/3));  
		}
	glPopMatrix();
		
    drawScoreLevel();
}

void Game::drawLife(float x, float y) {
    // Scale the heart symbol
    float const scale = 0.5;
    
    // Heart symbol equations from Walfram Mathworld: http://mathworld.wolfram.com/HeartCurve.html
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.0f, 0.0f);
    for(int j = 0; j < SPHERE_SEGMENTS; j++) {
        float const theta = 2.0f * 3.1415926f * (float)j / (float)SPHERE_SEGMENTS;
        float const xx = scale * 16.0f * sinf(theta) * sinf(theta) * sinf(theta);
        float const yy = -1 * scale * (13.0f * cosf(theta) - 5.0f * cosf(2.0f * theta) - 2 * cosf(3.0f * theta) - cosf(4.0f * theta));
        glVertex3f(x + xx, y + yy, 0.1);
    }
    glEnd();
}

void drawWord(string s) { 
	for(int i=0; i <(int) s.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
}

void Game::drawScoreLevel(void) {
    glPushMatrix();
	    // Write Score
	    string word = "Score: ", value;
	    stringstream ss;  
		ss << score;  
		ss >> value;
	    glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos3f(-8,8,0);
		glScalef(2.2, 2.2, 2.2);
		drawWord(word+value);
		
		// Write Level
		word = "  Level: ";  
		ss << level;  
		ss >> value;
		drawWord(word+value);
    glPopMatrix();
}

void Game::reshape(int width, int height) {
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

void Game::mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        newBall(paddle.xpos);
    }
    
    // Force redraw
	glutPostRedisplay();
}

void Game::keyStroke(unsigned char key, int x, int y) {
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
        case 27: // Esc
            exit(0);
            break;
        default:
            break;
    }
}

void Game::specialKeyPos(int key, int x, int y) {
    switch(key)
	{
		case GLUT_KEY_LEFT: // Move paddlle Left
			if (paddle.xpos - paddle.width/2 > -4.7f) {
                paddle.xpos -= 0.15f;
                
                // Sync ball with paddle move
                for( vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it) {     
			  		if(!it->launch){
						it->xpos -= 0.15f;  
					}
		    	}
            }
            break;
        case GLUT_KEY_RIGHT: // Move paddlle Right
            if (paddle.xpos + paddle.width/2 < 4.7f) {
                paddle.xpos += 0.15f;
                
                // Sync ball with paddle move
                for( vector<Ball>::iterator it = balls.begin(); it != balls.end(); ++it) {     
			  		if(!it->launch){
						it->xpos += 0.15f;  
					}
			    }
            } 
            break;
        default:
            break;
    }
}
