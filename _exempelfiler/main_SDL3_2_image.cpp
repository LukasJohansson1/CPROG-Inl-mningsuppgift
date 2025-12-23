/*
  main_SDL3_2_image
  laddar in och visar en bild från mappen "resources/images/"
*/  

// Headers

#include <iostream>
#include <string>

#include <SDL3/SDL.h> 		        //Grundbiblioteket
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>   //För utökad bildhantering
//#include <SDL3_ttf/SDL_ttf.h>     //För utökad texthantering
#include "Constants.h"				//Fönsterstorlek och sökvägar till programmets 'resources'(ex bild,typsnitt,ljud)

int main(int argc, char* argv[]) 
{
	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		//std::cout << "Error Initializing SDL Video: " << SDL_GetError();
	  	SDL_Log("Couldn't initialize SDL Video: %s", SDL_GetError());
    	return SDL_APP_FAILURE;
	}

	SDL_Window* window{ SDL_CreateWindow( "main_SDL3_2_image", constants::gScreenWidth, constants::gScreenHeight, 0 ) };
    if (window == nullptr) {
        //std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Log( "SDL_CreateWindow Error: %s", SDL_GetError() );
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer{ SDL_CreateRenderer(window, NULL) };
    if (renderer == nullptr) {
        //std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Log( "SDL_CreateRenderer Error: %s", SDL_GetError() );
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

	SDL_Surface* bg_sur{ IMG_Load( constants::bg_str.c_str() ) };
	if (bg_sur == nullptr) {
        //std::cerr << "IMG_Load Error: " << SDL_GetError() << std::endl;
        SDL_Log( "IMG_Load Error: %s", SDL_GetError() );
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

	SDL_Texture* bg_tex{ SDL_CreateTextureFromSurface( renderer, bg_sur ) };
	SDL_DestroySurface( bg_sur );
	if (bg_tex == nullptr) {
        //std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Log( "SDL_CreateTextureFromSurface Error: %s", SDL_GetError() );
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

	std::cout << "End Program using the application\'s windows menu \"quit\" or just close the window!" << std::endl;

	// Loop till dess att programmet avslutas!
	bool running{ true };
	while ( running ) 
	{
		SDL_Event e;
		if (SDL_PollEvent( &e )) 
		{
			if ( e.type == SDL_EVENT_QUIT ) 
			{
				running = false;
			}
		}

		SDL_RenderClear( renderer );
		SDL_RenderTexture( renderer, bg_tex, NULL, NULL );
		SDL_RenderPresent( renderer );
	}

	// Städa innan programmet avslutas!
	SDL_DestroyTexture( bg_tex );
	bg_tex = nullptr;

	SDL_DestroyRenderer( renderer );
	renderer = nullptr;

	SDL_DestroyWindow( window );
	window = nullptr;

	SDL_Quit();
	
	return EXIT_SUCCESS;
}