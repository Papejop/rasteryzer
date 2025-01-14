#ifndef CAMERA_H
#define CAMERA_H

#define WIDTH 1920
#define HEIGHT 1080

#include "draw.h"

extern float projectionPlaneZ;
extern float viewportSize;
extern const color BLUE;
extern const color RED;
extern const color GREEN;
extern const color YELLOW;
extern const color PURPLE;
extern const color CYAN;

typedef struct {
    float x;
    float y;
    float z;
} vertex;

typedef struct {
    int v0;
    int v1;
    int v2;
    color c;
} triangle;

typedef struct Shape {
    vertex* vertices;
    triangle* triangles;
    int numberOfVertex;
    int numberOfTriangles;
} Shape;

extern vertex pyramidVertices[];
extern triangle pyramidTriangles[];
extern vertex cubeVertices[];
extern triangle cubeTriangles[];

extern Shape cube;
extern Shape pyramid;

void freeShape(Shape shape);
Shape initializeShape(Shape shapeTemplate, color ofnewColor);
point ViewportToCanvas(point pHelper);
point projectVertex(vertex v);
void RenderTriangle(triangle triangle, point *projected);
void RenderObject(triangle *triangles, int sizeT, vertex *vertecies, int sizeV);
void RenderScene();

#endif // CAMERA_H