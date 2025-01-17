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
    {2, 1, 6},
    {4, 1, 6},
    {2, 1, 2},
    {4, 1, 2},
    {3, 2, 4}
};

triangle pyramidTriangles[] = {
    {0, 1, 4, {255, 0, 0}},
    {0, 2, 4, {255, 0, 0}},
    {1, 3, 4, {255, 0, 0}},
    {2, 3, 4, {255, 0, 0}},
    {0, 2, 3, {255, 0, 0}}
};

vertex cubeVertices[] = {
    {-1, 1, 6}, // front bottom left
    {-1, -1, 6}, // front top left
    {1, -1, 6}, // front top right
    {1, 1, 6}, // front bottom right
    {-1, 1, 8}, // back bottom left
    {-1, -1, 8}, // back top left
    {1, -1, 8}, // back top right
    {1, 1, 8}  // back bottom right
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
    .vertices = cubeVertices,
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

vertex calculateCenterOfObject(vertex* vertices, int numberOfVertex)
{
    if (numberOfVertex == 0) {
        return (vertex){0, 0, 0};
    }

    vertex center;
    float minX = vertices[0].x, maxX = vertices[0].x;
    float minY = vertices[0].y, maxY = vertices[0].y;
    float minZ = vertices[0].z, maxZ = vertices[0].z;

    for (int i = 1; i < numberOfVertex; ++i)
    {
        if (vertices[i].x < minX) minX = vertices[i].x;
        if (vertices[i].x > maxX) maxX = vertices[i].x;
        if (vertices[i].y < minY) minY = vertices[i].y;
        if (vertices[i].y > maxY) maxY = vertices[i].y;
        if (vertices[i].z < minZ) minZ = vertices[i].z;
        if (vertices[i].z > maxZ) maxZ = vertices[i].z;
    }

    center.x = (minX + maxX) / 2.0f;
    center.y = (minY + maxY) / 2.0f;
    center.z = (minZ + maxZ) / 2.0f;

    return center;
}

Object initializeObject(Object obj, Shape shape, color c)
{
    obj.typeOfShape = initializeShape(shape, c);
    obj.scaleOfObject = (vertex){1,1,1};
    obj.locationOfObject = (vertex){0,0,0};
    obj.centerOfObject = calculateCenterOfObject(obj.typeOfShape.vertices, obj.typeOfShape.numberOfVertex);

    return obj;
}

point ViewportToCanvas(point p) // TODO: this works in the current aspect ratio but will need to be modified to work with any aspect ratio/window size
{
    p.x = HEIGHT * p.x / viewportSize;
    p.y = HEIGHT * p.y / viewportSize;
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

void localTranslateObject(Object *object, float x, float y, float z)
{
    for(int i = 0; i < object->typeOfShape.numberOfVertex; i++)
    {
        object->typeOfShape.vertices[i].x += x;
        object->typeOfShape.vertices[i].y += y;
        object->typeOfShape.vertices[i].z += z;
    }

    object->locationOfObject.x += x;
    object->locationOfObject.y += y;
    object->locationOfObject.z += z;
}

void localScaleObject(Shape *shape, float scale)
{
    for(int i = 0; i < shape->numberOfVertex; i++)
    {
        shape->vertices[i].x *= scale;
        shape->vertices[i].y *= scale;
        shape->vertices[i].z *= -scale;
    }
}

void localRotateObject(Shape *shape, float rotateX, float rotateY, float rotateZ)
{
    double sinX = sin(rotateX), cosX = cos(rotateX);
    double sinY = sin(rotateY), cosY = cos(rotateY);
    double sinZ = sin(rotateZ), cosZ = cos(rotateZ);

    vertex center = calculateCenterOfObject(shape->vertices, shape->numberOfVertex);

    for(int i = 0; i < shape->numberOfVertex; ++i)
    {
        shape->vertices[i].x -= center.x;
        shape->vertices[i].y -= center.y;
        shape->vertices[i].z -= center.z;
    }


    for(int i = 0; i < shape->numberOfVertex; ++i)
    {
        float x = shape->vertices[i].x;
        float y = shape->vertices[i].y;
        float z = shape->vertices[i].z;
        float newX;
        float newY;
        float newZ;

        // x axis rotation
        newY = y * cosX - z * sinX;
        newZ = y * sinX + z * cosX;

        // y axis rotation
        newX = x * cosY + newZ * sinY;
        newZ = -x * sinY + newZ * cosY;

        // z axis rotation
        x = newX * cosZ - newY * sinZ;
        y = newX * sinZ + newY * cosZ;

        shape->vertices[i].x = x;
        shape->vertices[i].y = y;
        shape->vertices[i].z = newZ;
    }

    for(int i = 0; i < shape->numberOfVertex; ++i)
    {
        shape->vertices[i].x += center.x;
        shape->vertices[i].y += center.y;
        shape->vertices[i].z += center.z;
    }
}

void RenderObject(Shape shape)
{
    point *projected = (point *)malloc(shape.numberOfVertex * sizeof(point));
    int i, t;
    for(i = 0; i < shape.numberOfVertex; i++)
        projected[i] = projectVertex(shape.vertices[i]);
    for(t = 0; t < shape.numberOfTriangles; t++)
        RenderTriangle(shape.triangles[t], projected);
    free(projected);
}

void RenderScene()
{
    initializeImageArray();
    
    int viewportSize = 1;
    int projectionPlaneZ = 1; 

    Object news1, news2;
    
    news1 = initializeObject(news1, cube, RED);
    news2 = initializeObject(news2, cube, BLUE);
    
    localRotateObject(&news1.typeOfShape, 1, 1, 0);

    localTranslateObject(&news1, 1, 0, 0);
    localTranslateObject(&news2, -2, 0, 0);

    RenderObject(news1.typeOfShape);
    RenderObject(news2.typeOfShape);

    freeShape(news1.typeOfShape);
    freeShape(news2.typeOfShape);
}