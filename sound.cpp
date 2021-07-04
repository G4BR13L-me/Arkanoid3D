#include "sound.h"

void PlaySoundEffect(Effect e){
	switch(e)
	{
		case BACKGROUND:
			PlaySound(TEXT("audios/background.wav"), NULL, SND_ASYNC | SND_LOOP);
			break;
		case HIT:
			// TODO
			break;
		case DIE:
			// TODO
			break;
		default:
			break;
	}
}
