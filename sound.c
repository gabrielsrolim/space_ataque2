#include "sound.h"

playing_t playing[MAX_PLAYING_SOUNDS];

int getStatusSound(sound_t sound){
    int i;
    for(i=0;i<MAX_PLAYING_SOUNDS;i++){
       if(playing[i].sound->length == sound.length && playing[i].sound->samples== sound.samples){
          return playing[i].active;
       }
    }
}

/* This function is called by SDL whenever the sound card
   needs more samples to play. It might be called from a
   separate thread, so we should be careful what we touch. */
void AudioCallback(void *user_data, Uint8 *audio, int length)
{
    int i;

    /* Avoid compiler warning. */
    //user_data += 0;

    /* Clear the audio buffer so we can mix samples into it. */
    memset(audio, 0, length);

    /* Mix in each sound. */
    for (i = 0; i < MAX_PLAYING_SOUNDS; i++) {
	if (playing[i].active) {
	    Uint8 *sound_buf;
	    Uint32 sound_len;

	    /* Locate this sound's current buffer position. */
	    sound_buf = playing[i].sound->samples;
	    sound_buf += playing[i].position;

	    /* Determine the number of samples to mix. */
	    if ((playing[i].position + length) > playing[i].sound->length) {
		sound_len = playing[i].sound->length - playing[i].position;
	    } else {
		sound_len = length;
	    }

	    /* Mix this sound into the stream. */
	    SDL_MixAudio(audio, sound_buf, sound_len, VOLUME_PER_SOUND);

	    /* Update the sound buffer's position. */
	    playing[i].position += length;

	    /* Have we reached the end of the sound? */
	    if (playing[i].position >= playing[i].sound->length) {
		playing[i].active = 0;	/* mark it inactive */
	    }
	}
    }
}

/* This function loads a sound with SDL_LoadWAV and converts
   it to the specified sample format. Returns 0 on success
   and 1 on failure. */
int LoadAndConvertSound(char *filename, SDL_AudioSpec *spec,
			sound_p sound)
{
    SDL_AudioCVT cvt;           /* audio format conversion structure */
    SDL_AudioSpec loaded;       /* format of the loaded data */
    Uint8 *new_buf;

    /* Load the WAV file in its original sample format. */
    if (SDL_LoadWAV(filename,
		    &loaded, &sound->samples, &sound->length) == NULL) {
	printf("Unable to load sound: %s\n", SDL_GetError());
	return 1;
    }

    /* Build a conversion structure for converting the samples.
       This structure contains the data SDL needs to quickly
       convert between sample formats. */
    if (SDL_BuildAudioCVT(&cvt, loaded.format,
			  loaded.channels,
			  loaded.freq,
			  spec->format, spec->channels, spec->freq) < 0) {
	printf("Unable to convert sound: %s\n", SDL_GetError());
	return 1;
    }

    /* Since converting PCM samples can result in more data
       (for instance, converting 8-bit mono to 16-bit stereo),
       we need to allocate a new buffer for the converted data.
       Fortunately SDL_BuildAudioCVT supplied the necessary
       information. */
    cvt.len = sound->length;
    new_buf = (Uint8 *) malloc(cvt.len * cvt.len_mult);
    if (new_buf == NULL) {
	printf("Memory allocation failed.\n");
	SDL_FreeWAV(sound->samples);
	return 1;
    }

    /* Copy the sound samples into the new buffer. */
    memcpy(new_buf, sound->samples, sound->length);

    /* Perform the conversion on the new buffer. */
    cvt.buf = new_buf;
    if (SDL_ConvertAudio(&cvt) < 0) {
	printf("Audio conversion error: %s\n", SDL_GetError());
	free(new_buf);
	SDL_FreeWAV(sound->samples);
	return 1;
    }

    /* Swap the converted data for the original. */
    SDL_FreeWAV(sound->samples);
    sound->samples = new_buf;
    sound->length = sound->length * cvt.len_mult;

    /* Success! */
    printf("'%s' was loaded and converted successfully.\n", filename);

    return 0;
}


/* Removes all currently playing sounds. */
void ClearPlayingSounds(void)
{
    int i;

    for (i = 0; i < MAX_PLAYING_SOUNDS; i++) {
	playing[i].active = 0;
    }
}

/* Adds a sound to the list of currently playing sounds.
   AudioCallback will start mixing this sound into the stream
   the next time it is called (probably in a fraction of a second). */
int PlaySound(sound_p sound)
{
    int i;

    /* Find an empty slot for this sound. */
    for (i = 0; i < MAX_PLAYING_SOUNDS; i++) {
	if (playing[i].active == 0)
	    break;
    }

    /* Report failure if there were no free slots. */
    if (i == MAX_PLAYING_SOUNDS)
	return 1;

    /* The 'playing' structures are accessed by the audio callback,
       so we should obtain a lock before we access them. */
    SDL_LockAudio();
    playing[i].active = 1;
    playing[i].sound = sound;
    playing[i].position = 0;
    SDL_UnlockAudio();

    return 0;
}
