/*
 *   Isometric Game Tutorial Part 1
 *   Author: Johan Forsblom
 *   Date: 1st October 2017
 *
 *   This tutorial covers the following:
 *
 *      * Single layer map rendering (multiple layer rendering in future tutorial if i get to it :D)
 *      * Smooth map scrolling (scrolling with less than the tile size)
 *      * Mouse tile picking with cartesian screen coordinates
 *
 *   Usage:
 *   Scroll the map with w,a,s,d
 *   Click on the map for "tile picking" (shows the selected tile up in the top left corner of the screen)
 *
 ******************************************************************************************************************
 *
 *   Copyright 2017 Johan Forsblom
 *
 *   You may use the code for whatever reason you want. If you do a commercial project and took inspiration from or
 *   copied this code, please do put my name in the credits section of your game :)
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 *   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "initclose.h"
#include "renderer.h"
#include "texture.h"
#include "isoEngine.h"

#define NUM_ISOMETRIC_TILES 5
#define MAP_HEIGHT 16
#define MAP_WIDTH 16

int worldTest[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,2,2,2,2,2,2,1,1,2,2,2,2,2,1},
    {1,1,1,1,2,1,1,2,1,1,2,2,2,2,2,1},
    {2,1,1,1,2,2,2,2,1,1,2,2,2,2,2,1},
    {2,1,1,2,2,1,1,2,1,1,2,2,2,2,2,1},
    {2,1,1,4,4,4,1,2,1,1,2,2,2,2,4,1},
    {2,1,1,4,4,4,1,2,1,1,2,2,2,2,2,1},
    {2,1,1,4,4,4,1,2,1,1,2,2,4,2,2,1},
    {2,2,2,4,4,4,2,1,2,3,3,3,4,2,2,1},
    {1,1,2,2,2,2,2,3,4,3,3,3,4,2,2,2},
    {1,1,1,1,2,1,1,2,1,3,3,3,2,2,2,3},
    {2,1,1,1,2,2,2,2,1,1,2,2,2,2,2,1},
    {2,1,1,2,2,1,1,2,1,1,3,2,2,2,4,4},
    {2,1,1,4,2,1,1,2,1,1,3,2,2,2,2,4},
    {2,1,1,1,2,1,1,2,1,1,3,3,3,3,3,4},
    {2,1,1,1,1,1,1,2,1,1,2,2,2,2,4,4},
    {2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,1}
};

typedef struct gameT
{
    SDL_Event event;
    int loopDone;
    SDL_Rect mouseRect;
    point2DT mousePoint;
    point2DT mapScroll2Dpos;
    int mapScrolllSpeed;
    isoEngineT isoEngine;
    int lastTileClicked;

}gameT;

gameT game;
textureT tilesTex;
SDL_Rect tilesRects[NUM_ISOMETRIC_TILES];


void setupRect(SDL_Rect *rect,int x,int y,int w,int h){
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

void initTileClip()
{
    int x=0,y=0;
    int i;

    textureInit(&tilesTex,0,0,0,NULL,NULL,SDL_FLIP_NONE);
    for(i=0;i<NUM_ISOMETRIC_TILES;++i){
        setupRect(&tilesRects[i],x,y,64,80);
        x+=64;
    }
}


void init()
{
    int tileSize = 32;
    game.loopDone = 0;
    initTileClip();
    InitIsoEngine(&game.isoEngine,tileSize);
    IsoEngineSetMapSize(&game.isoEngine,16,16);

    game.isoEngine.scrollX = 77;
    game.isoEngine.scrollY = -77;
    game.mapScroll2Dpos.x = -77;
    game.mapScroll2Dpos.y = 0;
    game.mapScrolllSpeed = 3;
    game.lastTileClicked = -1;

    if(loadTexture(&tilesTex,"data/isotiles.png")==0){
        fprintf(stderr,"Error, could not load texture: data/isotiles.png\n");
        exit(1);
    }
}

void drawIsoMouse()
{
    int correctX =(game.mapScroll2Dpos.x%32)*2;
    int correctY = game.mapScroll2Dpos.y%32;

    game.mousePoint.x = (game.mouseRect.x/TILESIZE) * TILESIZE;
    game.mousePoint.y = (game.mouseRect.y/TILESIZE) * TILESIZE;

    //For every other x position on the map
    if((game.mousePoint.x/TILESIZE)%2){
        //Move the mouse down by half a tile so we can
        //pick isometric tiles on that row as well.
        game.mousePoint.y+=TILESIZE*0.5;
    }
    textureRenderXYClip(&tilesTex,game.mousePoint.x-correctX,game.mousePoint.y+correctY,&tilesRects[0]);
}

void drawIsoMap(isoEngineT *isoEngine)
{
    int i,j;
    int tile = 4;
    point2DT point;

    //loop through the map
    for(i=0;i<isoEngine->mapHeight;++i)
    {
        for(j=0;j<isoEngine->mapWidth;++j)
        {
            point.x = (j *TILESIZE) + isoEngine->scrollX;
            point.y = (i *TILESIZE) + isoEngine->scrollY;

            tile = worldTest[i][j];

            Convert2dToIso(&point);

            textureRenderXYClip(&tilesTex,point.x,point.y,&tilesRects[tile]);
        }
    }
}

void getMouseTileClick(isoEngineT *isoEngine)
{
    point2DT point;
    point2DT tileShift, mouse2IsoPOint;

    int correctX =(game.mapScroll2Dpos.x%32)*2;
    int correctY = game.mapScroll2Dpos.y%32;

    //copy mouse point
    mouse2IsoPOint = game.mousePoint;
    ConvertIsoTo2D(&mouse2IsoPOint);

    //get tile coordinates
    GetTileCoordinates(&mouse2IsoPOint,&point);

    tileShift.x = correctX;
    tileShift.y = correctY;
    Convert2dToIso(&tileShift);

    point.x-=((float)isoEngine->scrollX+(float)tileShift.x)/(float)TILESIZE;
    point.y-=((float)isoEngine->scrollY-(float)tileShift.y)/(float)TILESIZE;

    if(point.x>=0 && point.y>=0 && point.x<MAP_WIDTH && point.y<MAP_HEIGHT)
    {
        game.lastTileClicked = worldTest[point.y][point.x];
    }
}

void draw()
{
    SDL_SetRenderDrawColor(getRenderer(),0x3b,0x3b,0x3b,0x00);
    SDL_RenderClear(getRenderer());

    drawIsoMap(&game.isoEngine);
    drawIsoMouse();

    if(game.lastTileClicked!=-1){
        textureRenderXYClip(&tilesTex,0,0,&tilesRects[game.lastTileClicked]);
    }

    SDL_RenderPresent(getRenderer());
    //Don't be a CPU HOG!! :D
    SDL_Delay(10);
}

void update()
{
    SDL_GetMouseState(&game.mouseRect.x,&game.mouseRect.y);
}

void updateInput()
{
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    while(SDL_PollEvent(&game.event) != 0)
    {
        switch(game.event.type)
        {
            case SDL_QUIT:
                game.loopDone=1;
            break;

            case SDL_KEYUP:
                switch(game.event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        game.loopDone=1;
                    break;

                    default:break;
                }
            break;

            case SDL_MOUSEBUTTONDOWN:
                if(game.event.button.button == SDL_BUTTON_LEFT)
                {
                    getMouseTileClick(&game.isoEngine);
                }
            break;

            default:break;
        }
    }

    if(keystate[SDL_SCANCODE_W]){
        game.isoEngine.scrollX+=game.mapScrolllSpeed;
        game.isoEngine.scrollY+=game.mapScrolllSpeed;
        game.mapScroll2Dpos.y+=game.mapScrolllSpeed;

        if(game.mapScroll2Dpos.y>0){
            game.mapScroll2Dpos.y=0;
            game.isoEngine.scrollX-=game.mapScrolllSpeed;
            game.isoEngine.scrollY-=game.mapScrolllSpeed;
        }
    }
    if(keystate[SDL_SCANCODE_A]){
        game.isoEngine.scrollX+=game.mapScrolllSpeed;
        game.isoEngine.scrollY-=game.mapScrolllSpeed;
        game.mapScroll2Dpos.x-=game.mapScrolllSpeed;
    }
    if(keystate[SDL_SCANCODE_S]){
        game.isoEngine.scrollX-=game.mapScrolllSpeed;
        game.isoEngine.scrollY-=game.mapScrolllSpeed;
        game.mapScroll2Dpos.y-=game.mapScrolllSpeed;

    }
    if(keystate[SDL_SCANCODE_D]){
        game.isoEngine.scrollX-=game.mapScrolllSpeed;
        game.isoEngine.scrollY+=game.mapScrolllSpeed;
        game.mapScroll2Dpos.x+=game.mapScrolllSpeed;
    }
}

int main(int argc, char *argv[])
{
    initSDL("Isometric Tutorial Series Part 1!");
    init();

    while(!game.loopDone){
        update();
        updateInput();
        draw();
    }

    closeDownSDL();
    return 0;
}
