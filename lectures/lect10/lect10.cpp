
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

///easy way
//In ClassDemoApp::Init()
Mix_OpenAudio(44100, MIX_DEFAULT_FO

//In ClassDemoApp
Mix_Chunk* someSound;
someSound = Mix_LoadWAV("some_sound.wav");
Mix_PlayChannel(-1, someSound, 0);

Mix_Music* music;
music = Mix_LoadMUS("music.mp3");
Mix_PlayMusic(music, -1);

DemoApp::~DemoApp() {
	Mix_FreeChunk(someSound);
	Mix_FreeMusic(music);

	SDL_Quit();
}
