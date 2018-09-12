#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "initclose.h"
#include "renderer.h"

void initSDL(char *windowName)
{
    if(SDL_Init(SDL_INIT_VIDEO)< 0){
        fprintf(stderr,"Could not initialize SDL! SDL Error:%s\n",SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING,"1");

    if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"0")){
        fprintf(stdout,"Warning: Linear texture filtering was not enabled!");
    }

    initRenderer(windowName);

    if( !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        fprintf(stderr,"Could not initialize SDL_Image!) SDL_image error:%s\n",IMG_GetError());
        exit(1);
    }
}

void closeDownSDL()
{
    closeRenderer();
    IMG_Quit();
    SDL_Quit();
}
