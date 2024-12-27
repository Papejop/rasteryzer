#include "draw.h"

int projectionPlaneZ = 1;
int viewportSize = 1;
color BLUE = (color){0, 0, 255};
color RED = (color){255, 0, 0};
color GREEN = (color){0, 255, 0};
color YELLOW = (color){255, 255, 0};
color PURPLE = (color){255, 0, 255};
color CYAN = (color){0, 255, 255};

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
    vertex A, B, C, D, E, F, G, H;

    A = (vertex){600.0, 1000.0, 5};
    B = (vertex){600.0, 600.0, 5};
    C = (vertex){1200.0, 600.0, 5};
    D = (vertex){1200.0, 1000.0, 5};

    E = (vertex){600.0, 1000.0, 7.5};
    F = (vertex){600.0, 600.0, 7.5};
    G = (vertex){1200.0, 600.0, 7.5};
    H = (vertex){1200.0, 1000.0, 7.5};
    

    vertex *vertices = (vertex *)malloc(8 * sizeof(vertex));
    triangle *triangles = (triangle *)malloc(11 * sizeof(triangle));

    triangles[0] = (triangle){0, 1, 2, BLUE};
    triangles[1] = (triangle){0, 2, 3, BLUE};
    triangles[2] = (triangle){0, 1, 5, GREEN};
    triangles[3] = (triangle){0, 5, 4, GREEN};
    triangles[4] = (triangle){1, 2, 6, YELLOW};
    triangles[5] = (triangle){1, 6, 5, YELLOW};
    triangles[6] = (triangle){2, 3, 7, RED};
    triangles[7] = (triangle){2, 7, 6, RED};
    triangles[8] = (triangle){3, 0, 4, PURPLE};
    triangles[9] = (triangle){3, 4, 7, PURPLE};
    triangles[10] = (triangle){4, 5, 6, CYAN};
    vertices[0] = A;
    vertices[1] = B;
    vertices[2] = C;
    vertices[3] = D;
    vertices[4] = E;
    vertices[5] = F;
    vertices[6] = G;
    vertices[7] = H;


    RenderObject(triangles, 11, vertices, 8);

    saveToPPM();

    return 0;
}