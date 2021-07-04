#ifndef Sound_h
#define Sound_h

#include <windows.h>  
#include <mmsystem.h> 
	
// Game Sounds
enum Effect {BACKGROUND, HIT, DIE};
void PlaySoundEffect(Effect e);

#endif