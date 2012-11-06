#include "sound.h"

int main (int argc,char **argv)
{
    SDL_Surface *screen;
    SDL_Event event;
    int quit_flag = 0;		/* we'll set this when we want to exit. */

    /* Audio format specifications. */
    SDL_AudioSpec desired, obtained;

    /* Our loaded sounds and their formats. */
    sound_t cannon, explosion;

    /* Initialize SDL's video and audio subsystems.
       Video is necessary to receive events. */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
	printf("Unable to initialize SDL: %s\n", SDL_GetError());
	return 1;
    }

    /* Make sure SDL_Quit gets called when the program exits. */
    atexit(SDL_Quit);

    /* We also need to call this before we exit. SDL_Quit does
       not properly close the audio device for us. */
    atexit(SDL_CloseAudio);

    /* Attempt to set a 256x256 hicolor (16-bit) video mode. */
    screen = SDL_SetVideoMode(256, 256, 16, 0);
    if (screen == NULL) {
	printf("Unable to set video mode: %s\n", SDL_GetError());
	return 1;
    }

    /* Open the audio device. The sound driver will try to give us
       the requested format, but it might not succeed. The 'obtained'
       structure will be filled in with the actual format data. */
    desired.freq = 44100;	/* desired output sample rate */
    desired.format = AUDIO_S16;	/* request signed 16-bit samples */
    desired.samples = 4096;	/* this is more or less discretionary */
    desired.channels = 2;	/* ask for stereo */
    desired.callback = AudioCallback;
    desired.userdata = NULL;	/* we don't need this */
    if (SDL_OpenAudio(&desired, &obtained) < 0) {
	printf("Unable to open audio device: %s\n", SDL_GetError());
	return 1;
    }

    /* Load our sound files and convert them to the sound card's format. */
    if (LoadAndConvertSound("teste.wav", &obtained, &cannon) != 0) {
	printf("Unable to load sound.\n");
	return 1;
    }

    if (LoadAndConvertSound("teste.wav", &obtained, &explosion) != 0) {
	printf("Unable to load sound.\n");
	return 1;
    }

    /* Clear the list of playing sounds. */
    ClearPlayingSounds();

    /* SDL's audio is initially paused. Start it. */
    SDL_PauseAudio(0);

    printf("Press 'Q' to quit. C and E play sounds.\n");

    /* Start the event loop. Keep reading events until there is
       an event error or the quit flag is set. */
    while (SDL_WaitEvent(&event) != 0 && quit_flag == 0) {
	SDL_keysym keysym;

	switch (event.type) {

	case SDL_KEYDOWN:
	    keysym = event.key.keysym;

	    /* If the user pressed Q, exit. */
	    if (keysym.sym == SDLK_q) {
		printf("'Q' pressed, exiting.\n");
		quit_flag = 1;
	    }

	    /* 'C' fires a cannon shot. */
	    if (keysym.sym == SDLK_c) {
		printf("Firing cannon!\n");
		PlaySound(&cannon);
	    }

	    /* 'E' plays an explosion. */
	    if (keysym.sym == SDLK_e) {
		printf("Kaboom!\n");
		PlaySound(&explosion);
	    }

	    break;

	case SDL_QUIT:
	    printf("Quit event. Bye.\n");
	    quit_flag = 1;
	}
    }

    /* Pause and lock the sound system so we can safely delete our sound data. */
    SDL_PauseAudio(1);
    SDL_LockAudio();

    /* Free our sounds before we exit, just to be safe. */
    free(cannon.samples);
    free(explosion.samples);

    /* At this point the output is paused and we know for certain that the
       callback is not active, so we can safely unlock the audio system. */
    SDL_UnlockAudio();

    return 0;
}
