#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>
#include <sstream>

// Local Files
#include "MyObjects.h"    
#include "config.h"   
#include "sound.h"

class Game {

public:
    // Class Constructor/Destructor
	Game();
	~Game();
    
    // Public functions (handle GLUT calls)
	void display(void);
	void set3DView(void);
	void init(void);
	void reshape(int width, int height);
	void mouseClick(int button, int state, int x, int y);
	void keyStroke(unsigned char key, int x, int y);
	void specialKeyPos(int key, int x, int y);
    
private:
    // Game statistics
    int score;
    int level;
    int reward;
    int lifesCount;
    
    // Possible ame mode
    enum State {INIT, Menus, Gameplay, Scoreboard};
    Game::State gameState;
    
    // Balls
    std::vector <Ball> balls;
    
    // Paddle
    Paddle paddle;
    
    // Bricks
    std::vector<Brick> bricks;
    
    // Private functions
    void drawBackground(void);
    void drawGame(void);
    void newBall(float x);
    void drawBalls(void);
    void initPaddle(void);
    void drawPaddle(void);
    void initBricks(void);
    void bricksLevel1(void);
    void bricksLevel2(void);
    void generateBricks(int level);
    void drawBricks(void);
    template <typename Iterator>
    int wallCollision(Iterator it);
    template <typename Iterator>
    bool brickCollision(Iterator it, Iterator br);
    template <typename Iterator>
    Iterator hitBrick(Iterator brick);
    void drawLife(float x, float y);
    void drawGameStats(void);
    void drawScoreLevel(void);
};

#endif // GAME_H