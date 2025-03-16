#ifndef CAMERA_H
#define CAMERA_H

#define WIDTH 1920
#define HEIGHT 1080

#include "draw.h"
#include "MatrixMath.h"

extern const color BLUE;
extern const color RED;
extern const color GREEN;
extern const color YELLOW;
extern const color PURPLE;
extern const color CYAN;
//TODO: refactor this triangle to have 3 pointers to vertex instead of 3 values - need to change a lot of code
typedef struct triangle{
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
    vertex4d boundsCenter;
    float boundsRadius;
} Shape;

typedef struct Object {
    Shape typeOfShape;
    matrix4x4 rotation;
    vertex locationOfObject;
    float scaleOfObject;
} Object;

typedef struct plane {
    vertex normal;
    float distance;
} plane;

typedef struct camera{
    vertex location;
    matrix4x4 rotationMatrix;
    plane clippingPlanes[5];
} cameraInstance;

extern vertex cubeVertices[8];
extern triangle cubeTriangles[11];

extern Shape cube;

void freeShape(Shape shape);
Shape initializeShape(Shape shapeTemplate, color ofnewColor);
Object initializeObject(Object obj, Shape shape, color c);
point ViewportToCanvas(point p);
point projectVertex(vertex v);
void RenderTriangle(triangle triangle, point *projected);
void RenderObject(Shape shape, matrix4x4 transform);
cameraInstance initializeCamera();
void RenderScene(cameraInstance camera, Object *instances, int numberOfInstaces);
void Render();

#endif // CAMERA_H