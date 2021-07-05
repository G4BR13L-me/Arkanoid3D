#ifndef config_h
#define config_h

// Force Redraw timer
#define TIMER 16.6

// Window settings
#define WINTITLE "Arkanoid 3D"
const int WINWIDTH  = 800;
const int WINHEIGHT = 600;

// Brick settings
const int BRICKCOLS = 8;
const int BRICKROWS = 6;      

// Pedal Settings
const float PEDALWIDTH = 150.0f;
const float PEDALHEIGHT = 12.0f;

// Ball settings
const float BALL_RADIUS = 0.1f;
const int SPHERE_SEGMENTS = 10; // used in drawing ball and the life/heart symbol

#endif
