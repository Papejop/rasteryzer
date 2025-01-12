#include "camera.h"

float projectionPlaneZ = 1;
float viewportSize = 1;
const color BLUE = {0, 0, 255};
const color RED = {255, 0, 0};
const color GREEN = {0, 255, 0};
const color YELLOW = {255, 255, 0};
const color PURPLE = {255, 0, 255};
const color CYAN = {0, 255, 255};

vertex pyramidVertices[] = {
    {600, 1000, 8},
    {1200, 1000, 8},
    {600, 1000, 6},
    {1200, 1000, 6},
    {900, 600, 7}
};

triangle pyramidTriangles[] = {
    {0, 1, 4, {255, 0, 0}},
    {0, 2, 4, {255, 0, 0}},
    {1, 3, 4, {255, 0, 0}},
    {2, 3, 4, {255, 0, 0}},
    {0, 2, 3, {255, 0, 0}}
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

vertex cubeVertices2[] = {
    {-0.5, 0.5, 6}, // front bottom left
    {-0.5, -0.5, 6}, // front top left
    {0.5, -0.5, 6}, // front top right
    {0.5, 0.5, 6}, // front bottom right
    {-0.5, 0.5, 8}, // back bottom left
    {-0.5, -0.5, 8}, // back top left
    {0.5, -0.5, 8}, // back top right
    {0.5, 0.5, 8}  // back bottom right
};

triangle cubeTriangles[] = {
    {0, 1, 2, {265, 0, 0}}, // V
    {0, 2, 3, {255, 0, 0}}, // V
    {0, 1, 5, {255, 0, 0}}, // V
    {0, 5, 4, {255, 0, 0}}, // V
    {1, 2, 6, {255, 0, 0}}, // V
    {1, 6, 5, {255, 0, 0}}, // V
    {2, 3, 7, {255, 0, 0}}, // V
    {2, 7, 6, {255, 0, 0}}, // V
    {0, 3, 4, {255, 0, 0}}, // V
    {3, 4, 7, {255, 0, 0}}, // V
    {4, 5, 6, {255, 0, 0}}  // V
};

Shape cube = {
    .vertices = cubeVertices2,
    .triangles = cubeTriangles,
    .numberOfVertex = 8,
    .numberOfTriangles = 11
};

Shape pyramid = {
    .vertices = pyramidVertices,
    .triangles = pyramidTriangles,
    .numberOfVertex = 5,
    .numberOfTriangles = 5
};

void freeShape(Shape shape)
{
    free(shape.vertices);
    free(shape.triangles);
}

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

point ViewportToCanvas(point pHelp) // TODO: this 2 functions are highly sus
{
    point p;
    p.x = (WIDTH * pHelp.x / viewportSize);
    p.y = (HEIGHT * pHelp.y / viewportSize);
    return p;
}

point projectVertex(vertex v)
{
    point p;
    p.x = v.x * (float)projectionPlaneZ / v.z;
    p.y = v.y * (float)projectionPlaneZ / v.z;
    p = ViewportToCanvas(p);
    return p;
}

void RenderTriangle(triangle triangle, point *projected)
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
    free(projected);
}

void RenderScene()
{
    initializeImageArray();
    
    int viewportSize = 1;
    int projectionPlaneZ = 1; 
    //Shape newShape = initializeShape(pyramid, BLUE);
    Shape newShape = initializeShape(cube, RED);
    //Shape newShape2 = initializeShape(pyramid, BLUE);
    RenderObject(newShape.triangles, newShape.numberOfTriangles, newShape.vertices, newShape.numberOfVertex);

    //RenderObject(triangles, 11, vertices, 8);
    //saveToPPM();
}