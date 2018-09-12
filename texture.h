#ifndef TEXTURE_H_
#define TEXTURE_H_

typedef struct textureT
{
    int x;
    int y;
    int width;
    int height;
    double angle;
    SDL_Point *center;
    SDL_Rect *cliprect;
    SDL_RendererFlip fliptype;
    SDL_Texture *texture;
}textureT;

int loadTexture(textureT *texture, char *filename);
void textureInit(textureT *texture, int x,int y, double angle, SDL_Point *center, SDL_Rect *cliprect, SDL_RendererFlip fliptype);
void textureRenderXYClip(textureT *texture, int x, int y, SDL_Rect *cliprect);

#endif // TEXTURE_H_
