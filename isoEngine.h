#ifndef ISOENGINE_H_
#define ISOENGINE_H_

unsigned int TILESIZE;

typedef struct isoEngineT
{
    int scrollX;
    int scrollY;
    int mapHeight;
    int mapWidth;
}isoEngineT;

typedef struct point2DT
{
    int x;
    int y;
}point2DT;

void InitIsoEngine(isoEngineT *isoEngine, int tileSizeInPixels);
void IsoEngineSetMapSize(isoEngineT *isoEngine,int width, int height);
void Convert2dToIso(point2DT *point);
void ConvertIsoTo2D(point2DT *point);
void GetTileCoordinates(point2DT *point,point2DT *point2DCoord);
#endif // ISOENGINE_H_
