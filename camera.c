#include "draw.h"

int projectionPlaneZ = 1;
int viewportSize = 1;
const color BLUE = (color){0, 0, 255};
const color RED = (color){255, 0, 0};
const color GREEN = (color){0, 255, 0};
const color YELLOW = (color){255, 255, 0};
const color PURPLE = (color){255, 0, 255};
const color CYAN = (color){0, 255, 255};

typedef struct{
    float x;
    float y;
    float z;
} vertex;

typedef struct{
    int v0;
    int v1;
    int v2;
    color c;
} triangle;

typedef struct Shape{
    vertex* vertices;
    triangle* triangles;
    int numberOfVertex;
    int numberOfTriangles;
} Shape;

vertex pyramidVertices[] = {
    {600, 1000, 8},
    {1200, 1000, 8},
    {600, 1000, 6},
    {1200, 1000, 6},
    {900, 600, 7} 
};

triangle pyramidTriangles[] = {
    {0, 1, 4, (color){255, 0, 0}},
    {0, 2, 4, (color){255, 0, 0}},
    {1, 3, 4, (color){255, 0, 0}},
    {2, 3, 4, (color){255, 0, 0}},
    {0, 2, 3, (color){255, 0, 0}}
};

vertex cubeVertices[] = {
    {600, 1000, 7.5},
    {600, 600, 7.5},
    {1200, 600, 7.5},
    {1200, 1000, 7.5},
    {600, 1000, 10},
    {600, 600, 10},
    {1200, 600, 10},
    {1200, 1000, 10}
};

triangle cubeTriangles[] = {
    {0, 1, 2, (color){255, 0, 0}},
    {0, 2, 3, (color){255, 0, 0}},
    {0, 1, 5, (color){255, 0, 0}},
    {0, 5, 4, (color){255, 0, 0}},
    {1, 2, 6, (color){255, 0, 0}},
    {1, 6, 5, (color){255, 0, 0}},
    {2, 3, 7, (color){255, 0, 0}},
    {2, 7, 6, (color){255, 0, 0}},
    {3, 0, 4, (color){255, 0, 0}},
    {3, 4, 7, (color){255, 0, 0}},
    {4, 5, 6, (color){255, 0, 0}}
};

struct Shape cube = {
    .vertices = &cubeVertices[0],
    .triangles = &cubeTriangles[0],
    .numberOfVertex = 8,
    .numberOfTriangles = 11
};

struct Shape pyramid = {
    .vertices = &pyramidVertices[0],
    .triangles = &pyramidTriangles[0],
    .numberOfVertex = 5,
    .numberOfTriangles = 5
};

Shape initializeShape(Shape shapeTemplate, color ofnewColor)
{
    Shape newShape;
    int i;

    newShape.vertices = malloc(shapeTemplate.numberOfVertex * sizeof(vertex));
    newShape.triangles = malloc(shapeTemplate.numberOfTriangles * sizeof(triangle));
    if(!newShape.vertices || !newShape.triangles)
    {
        printf("Failed to allocate memory for shape vertices\n");
        exit(1);
    }

    newShape.numberOfVertex = shapeTemplate.numberOfVertex;
    newShape.numberOfTriangles = shapeTemplate.numberOfTriangles;

    for(i = 0; i < shapeTemplate.numberOfVertex; ++i)
        newShape.vertices[i] = shapeTemplate.vertices[i];

    for(i = 0; i < shapeTemplate.numberOfTriangles; ++i)
    {
        newShape.triangles[i] = shapeTemplate.triangles[i];
        newShape.triangles[i].c = ofnewColor;
    }

    return newShape;
}

point ViewportToCanvas(float x, float y)
{
    point p;
    p.x = x / viewportSize;
    p.y = y / viewportSize;
    return p;
}

point projectVertex(vertex v)
{
    point p;
    p.x = v.x * projectionPlaneZ / v.z;
    p.y = (float)(v.y * projectionPlaneZ / v.z);
    p = ViewportToCanvas(p.x, p.y);
    return p;
}

void RenderTriangle(triangle triangle,  point *projected)
{
    drawWireFrameTriangle(projected[triangle.v0], projected[triangle.v1], projected[triangle.v2], triangle.c);
}

void RenderObject(triangle *triangles, int sizeT, vertex *vertecies, int sizeV)
{
    point *projected = (point *)malloc(sizeV * sizeof(point));
    int i, t;
    for(i = 0; i < sizeV; i++)
        projected[i] = projectVertex(vertecies[i]);
    for(t = 0; t < sizeT; t++)
        RenderTriangle(triangles[t], projected);
}

int main()
{
    initializeImageArray();
    
    int viewportSize = 1;
    int projectionPlaneZ = 1; 

    Shape newShape = initializeShape(pyramid, BLUE);
    //Shape newShape2 = initializeShape(pyramid, BLUE);

    RenderObject(newShape.triangles, newShape.numberOfTriangles, newShape.vertices, newShape.numberOfVertex);
    //RenderObject(triangles, 11, vertices, 8);

    saveToPPM();

    return 0;
}