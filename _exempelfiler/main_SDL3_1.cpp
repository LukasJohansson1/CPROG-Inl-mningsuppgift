/*
  sdl3_1
  initialiserar SDL3, och avslutar programmet genom att rensa upp.
*/   

// Headers

// Alla dessa SDL inkluderingsfiler används inte i detta testprogram.
// Bifogas endast för test av SDL installation! 
#include <iostream>
#include <SDL3/SDL.h>             //Grundbiblioteket
#include <SDL3/SDL_main.h>        
#include <SDL3_image/SDL_image.h> //För utökad bildhantering
#include <SDL3_ttf/SDL_ttf.h>     //För utökad texthantering

int main(int argc, char* argv[])
{
  // Initialisera SDL, så att det kan användas!
  // (förenklad, saknar felkontroll)
  SDL_Init(SDL_INIT_VIDEO); 
  std::cout << "SDL är nu initialiserad och redo för användning!" << std::endl;

  // Här ska naturligtvis någonting hända, men inte denna gång.
  
  // Nu avslutas programmet.

  // Rensa upp SDL innan programmet avslutas!
  SDL_Quit();
  std::cout << "SDL är nu upprensat innan programavslut!" << std::endl;
  
  return 0;
}
