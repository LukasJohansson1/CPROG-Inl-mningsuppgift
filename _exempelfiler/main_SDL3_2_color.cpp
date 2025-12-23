/*
  sdl3_2_color
  Visar ett fönster med bakgrundsfärg på skärmen.
  Lyssnar efter tangentnedtryckningar ('q'/'Q'/PILUPP/PILNER).
  PILUPP ändrar bakgrundsfärg
  'q'/'Q' avslutar programmet
*/ 

// Headers

#include <string>
#include <iostream>
#include <SDL3/SDL.h>               //Grundbiblioteket
#include <SDL3/SDL_main.h>
//#include <SDL3_image/SDL_image.h> //För utökad bildhantering
//#include <SDL3_ttf/SDL_ttf.h>     //För utökad texthantering
#include "Constants.h"              //Fönsterstorlek och sökvägar till programmets 'resources'(ex bild,typsnitt,ljud)


int main(int argc, char* argv[] ) 
{
   // Initialisera SDL, så att det kan användas!
   if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		//std::cerr << "Error Initializing SDL Video: " << SDL_GetError();
	  	SDL_Log("Couldn't initialize SDL Video: %s", SDL_GetError());
    	return SDL_APP_FAILURE;
   }

   SDL_Window *window{ SDL_CreateWindow("main_SDL3_2_color", constants::gScreenWidth, constants::gScreenHeight, 0) };
   if (window == nullptr) {
      //std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      SDL_Log( "SDL_CreateWindow Error: %s", SDL_GetError() );
      SDL_Quit();
      return 1;
   }

   SDL_Renderer *renderer{ SDL_CreateRenderer(window, NULL) };
   if (renderer == nullptr) {
      //std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
      SDL_Log( "SDL_CreateRenderer Error: %s", SDL_GetError() );
      SDL_DestroyWindow(window);
      SDL_Quit();
      return 1;
   }

   // Istället för de två ovanstående SDL anropen så kan du också använda
   // nedanstående SDL_CreateWindowAndRenderer(), 
/*
   if (!SDL_CreateWindowAndRenderer("main_SDL3_2_color", screenWidth, screenHeight, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
*/

   std::cout << "End Program using the application\'s windows menu \"quit\" or just close the window!" << std::endl;

   double now{ 0 };
   float red{ 0 };
   float green{ 0 };
   float blue{ 0 };
   
   // Loop till dess att programmet avslutas!
   bool running{ true };
   while (running){
      SDL_Event event;
      while( SDL_PollEvent( &event ) )
      {
         switch(event.type)
         {
            case SDL_EVENT_QUIT:
               running = false;
               break;
            case SDL_EVENT_KEY_DOWN:
            {
               std::cout << "SDL_EVENT_KEY_DOWN\n";
               if ( event.key.key == SDLK_UP)
               {
                  std::cout << "SDLK_UP\n";
                  // convert from milliseconds to seconds.
                  now = ((double)SDL_GetTicks()) / 1000.0;  
                  // choose the color for the frame we will draw. 
                  //The sine wave trick makes it fade between colors smoothly.
                  red = (float) (0.5 + 0.5 * SDL_sin(now));
                  green = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
                  blue = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
               }

               if ( event.key.key == SDLK_DOWN)
               {
                  std::cout << "Använd Pil UPP-knappen istället!\n";
               }

               if (event.key.key == 'Q' || event.key.key == 'q')
               {
                  running = false;
               }
            }

            default:
               break;
         }
      }
      
      // choose the color for the frame we will draw, new color, full alpha
      SDL_SetRenderDrawColorFloat( renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT );

      // clear the window to the draw color.
      SDL_RenderClear( renderer );

      // put the newly-cleared rendering on the screen.
      SDL_RenderPresent( renderer );

   }

   const std::string message = "Nu avslutas programmet";
   std::cout << message << std::endl;

   //Destroy window
   SDL_DestroyRenderer( renderer );
   renderer = nullptr;

   SDL_DestroyWindow( window );
   window = nullptr;

   //Quit SDL subsystems
   SDL_Quit();

   return 0;   
}