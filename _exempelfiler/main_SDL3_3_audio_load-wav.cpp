/*
  sdl3_3_audio_load-wav
  Laddar in en wav-fil och spelar upp en audio-ström från mappen "resources/sounds/"
  
  Resurser:
  Ljudfil lånad från https://examples.libsdl.org/SDL3/

  Based on: 
  "https://examples.libsdl.org/SDL3/audio/"
*/

// Headers
#include <string>
#include <SDL3/SDL.h>                 //Grundbiblioteket
#include <SDL3/SDL_main.h>
//#include <SDL3_image/SDL_image.h>   //För utökad bildhantering
//#include <SDL3_ttf/SDL_ttf.h>       //För utökad texthantering
#include "Constants.h"                //Fönsterstorlek och sökvägar till programmets 'resources'(ex bild,typsnitt,ljud)

// Global Variables

//Audio
SDL_AudioDeviceID audio_device{ 0 };

// things that are playing sound (the audiostream itself, plus the original data, so we can refill to loop. 
typedef struct Sound 
{
    Uint8 *mWav_data{ nullptr };
    Uint32 mWav_data_len{ 0 } ;
    SDL_AudioStream *mAudioStream{ nullptr };
} Sound;

static Sound gSounds[1];


int main( int argc, char* args[] )
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) 
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    static SDL_Window* window{ nullptr }; 
	static SDL_Renderer* renderer{ nullptr }; 
    // Ett tomt fönster, då programmet endast är ett exempel på ljud.
    if (!SDL_CreateWindowAndRenderer("main_SDL3_3_audio_load-wav", constants::gScreenWidth, constants::gScreenHeight, 0, &window, &renderer)) 
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Öppna default audio device
    audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (audio_device == 0) 
    {
        SDL_Log("Couldn't open audio device: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_AudioSpec sample_AudioSpec;
    Sound *sample_Sound{ &gSounds[0] };
    const char *sample_wavPath{ constants::sample_str.c_str() };

    // Ladda in extern .wav-fil
    if (!SDL_LoadWAV(sample_wavPath, &sample_AudioSpec, &sample_Sound->mWav_data, &sample_Sound->mWav_data_len)) 
    {
        SDL_Log("Couldn't load .wav file: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Create an audio stream. Set the source format to the wav's format (what
       we'll input), leave the dest format NULL here (it'll change to what the
       device wants once we bind it). */
    sample_Sound->mAudioStream = SDL_CreateAudioStream(&sample_AudioSpec, NULL);
    if (!sample_Sound->mAudioStream) 
    {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    } else if (!SDL_BindAudioStream(audio_device, sample_Sound->mAudioStream)) 
    {  /* once bound, it'll start playing when there is data available! */
        SDL_Log("Failed to bind '%s' stream to device: %s", constants::sample_str.c_str(), SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Loop till dess att programmet avslutas!
	bool running{ true };
	while (running) 
    {
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}
	
        //audio
        unsigned int i{ 0 };
        for (i = 0; i < SDL_arraysize(gSounds); i++) {
            /* If less than a full copy of the audio is queued for playback, put another copy in there.
            This is overkill, but easy when lots of RAM is cheap. One could be more careful and
            queue less at a time, as long as the stream doesn't run dry.  */
            if (SDL_GetAudioStreamQueued(gSounds[i].mAudioStream) < ((int) gSounds[i].mWav_data_len)) {
                SDL_PutAudioStreamData(gSounds[i].mAudioStream, gSounds[i].mWav_data, (int) gSounds[i].mWav_data_len);
            }
        }

        // we're not doing anything with the renderer, so just blank it out.
        SDL_RenderClear(renderer);  
        SDL_RenderPresent(renderer); //Update screen

    } 

    //Clean up and close
    SDL_CloseAudioDevice(audio_device);
    unsigned int i{0};
    for (i = 0; i < SDL_arraysize(gSounds); i++) {
        if (gSounds[i].mAudioStream) {
            SDL_DestroyAudioStream(gSounds[i].mAudioStream);
        }
        SDL_free(gSounds[i].mWav_data);
    }
    
    //Destroy window
    SDL_DestroyRenderer( renderer );
    renderer = nullptr;
    SDL_DestroyWindow( window );
    window = nullptr;

    //Quit SDL subsystems
    SDL_Quit();


	return EXIT_SUCCESS;
}



