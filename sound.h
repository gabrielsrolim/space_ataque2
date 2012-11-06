#ifndef _SOUND_H_
  #define _SOUND_H_

/* Example of audio mixing with SDL. */

#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Structure for loaded sounds. */
typedef struct sound_s {
    Uint8 *samples;		/* raw PCM sample data */
    Uint32 length;		/* size of sound data in bytes */
} sound_t, *sound_p;

/* Structure for a currently playing sound. */
typedef struct playing_s {
    int active;                 /* 1 if this sound should be played */
    sound_p sound;              /* sound data to play */
    Uint32 position;            /* current position in the sound buffer */
} playing_t, *playing_p;

/* Array for all active sound effects. */
#define MAX_PLAYING_SOUNDS      100


/* The higher this is, the louder each currently playing sound will be.
   However, high values may cause distortion if too many sounds are
   playing. Experiment with this. */
#define VOLUME_PER_SOUND        SDL_MIX_MAXVOLUME / 2

extern void AudioCallback(void *user_data, Uint8 *audio, int length);
extern int LoadAndConvertSound(char *filename, SDL_AudioSpec *spec,
			sound_p sound);
extern void ClearPlayingSounds(void);
extern int PlaySound(sound_p sound);
extern int getStatusSound(sound_t sound);

#endif
