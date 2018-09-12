#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

void initRenderer(char *windowCaption)
{
    window = SDL_CreateWindow(windowCaption,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_RESIZABLE);
    if(window == NULL){
        fprintf(stderr,"SDL_CreateWindow failed:%s",SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL)
    {
        fprintf(stderr,"SDL_CreateRenderer failed:%s",SDL_GetError());
        exit(1);
    }
}

SDL_Renderer *getRenderer()
{
    return renderer;
}

SDL_Window *getWindow()
{
    return window;
}

void closeRenderer()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
