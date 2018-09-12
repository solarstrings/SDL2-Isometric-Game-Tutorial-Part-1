#ifndef __RENDERER_H
#define __RENDERER_H

#define WINDOW_WIDTH     1200
#define WINDOW_HEIGHT    720

void initRenderer(char *windowCaption);
SDL_Renderer *getRenderer();
SDL_Window *getWindow();
void closeRenderer();


#endif // __RENDERER_H
