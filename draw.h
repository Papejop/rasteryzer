
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 1920
#define HEIGHT 1080

typedef struct {
    int red;
    int green;
    int blue;
} color;

typedef struct {
    float x; // x coordinate is a float to allow pixel shading in interpolation function
    int y;
} point;

typedef struct {
    float *pointer;
    size_t size;
} arrayMetadata;

extern FILE *file;
extern color image[HEIGHT][WIDTH];

void initializeImageArray();
void swap(point *p0, point *p1);
void deallocateMemory(arrayMetadata array);
arrayMetadata interpolate(int i0, float d0, int i1, float d1);
void saveToPPM();
void drawPixel(int y, int x, color color);
void drawLine(point p0, point p1, color color);
void drawFilledTriangle(point p0, point p1, point p2, color color);
void drawShadedTriangle(point p0, point p1, point p2, color basicColor);
void drawWireFrameTriangle(point p0, point p1, point p2, color color);
