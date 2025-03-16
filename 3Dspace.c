#include "3Dspace.h"

float projectionPlaneZ = 1;
float viewportSize = 1;
const color BLUE = {0, 0, 255};
const color RED = {255, 0, 0};
const color GREEN = {0, 255, 0};
const color YELLOW = {255, 255, 0};
const color PURPLE = {255, 0, 255};
const color CYAN = {0, 255, 255};

vertex cubeVertices[] = {
    {-1, 1, -1}, // front bottom left
    {-1, -1, -1}, // front top left
    {1, -1, -1}, // front top right
    {1, 1, -1}, // front bottom right
    {-1, 1, 1}, // back bottom left
    {-1, -1, 1}, // back top left
    {1, -1, 1}, // back top right
    {1, 1, 1}  // back bottom right
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
    .numberOfTriangles = 11,
    .boundsCenter = (vertex4d){0, 0, 0, 1},
    .boundsRadius = 1.73205 // sqrt(3)
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

    newShape.boundsCenter = shapeTemplate.boundsCenter;
    newShape.boundsRadius = shapeTemplate.boundsRadius;

    return newShape;
}

Object initializeObject(Object obj, Shape shape, color c)
{
    obj.typeOfShape = initializeShape(shape, c);
    obj.scaleOfObject = 1;
    obj.locationOfObject = (vertex){0,0,0};
    obj.rotation = identityMatrix;

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
    //drawWireFrameTriangle(projected[triangle.v0], projected[triangle.v1], projected[triangle.v2], triangle.c);
    drawFilledTriangle(projected[triangle.v0], projected[triangle.v1], projected[triangle.v2], triangle.c);
}

void RenderObject(Shape shape, matrix4x4 transform)
{
    vertex4d vertexH;
    point *projected = (point *)malloc(shape.numberOfVertex * sizeof(point));
    int i;
    for(i = 0; i < shape.numberOfVertex; i++)
    { 
        vertexH.x = shape.vertices[i].x;
        vertexH.y = shape.vertices[i].y;
        vertexH.z = shape.vertices[i].z;
        vertexH.w = 1;
        //vertexH = multiplyMxV(transform, vertexH); we are transforming in transformAndClip function so removing this line is a temporary fix
        projected[i] = projectVertex((vertex){vertexH.x, vertexH.y, vertexH.z});
    }
    for(i = 0; i < shape.numberOfTriangles; i++)
        RenderTriangle(shape.triangles[i], projected);
    free(projected);
}

float dotProduct(vertex a, vertex b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float computeIntersectionParameter(plane clippingPlane, vertex4d v0, vertex4d v1) //TODO: check for division by zero
{
    float d0 = -(dotProduct(clippingPlane.normal, (vertex){v0.x, v0.y, v0.z}) + clippingPlane.distance);
    float d1 = dotProduct(clippingPlane.normal, (vertex){v1.x - v0.x, v1.y - v0.y, v1.z - v0.z});
    return d0 / d1;
}

vertex4d computeIntersection(plane clippingPlane, vertex4d v0, vertex4d v1)
{
    float t = computeIntersectionParameter(clippingPlane, v0, v1);
    return (vertex4d){v0.x + t * (v1.x - v0.x), v0.y + t * (v1.y - v0.y), v0.z + t * (v1.z - v0.z), 1};
}

//TODO: vertecies are overlapping - also refactor this function - have it return a struct with 2 or 1 triangles and have memory alloc hapen outside
void clipTrianges(vertex4d *vertices, triangle inputTriangle, plane clippingPlane, triangle **clippedTriangles, int *cTCounter, int *cTSize, vertex4d **clippedVertices, int *cVCounter, int *cVSize)
{

    int i, inCount = 0, nCount = 0;
    vertex4d p[3], n[3];
    vertex v[3];
    float insidePlane[3];

    for(i = 0; i < 3; ++i)
    {
        v[i] = (vertex){vertices[inputTriangle.v0].x,vertices[inputTriangle.v0].y,vertices[inputTriangle.v0].z};
        insidePlane[i] = dotProduct(clippingPlane.normal , v[i]) + clippingPlane.distance;
        if(insidePlane[i] > 0)
        {
            p[inCount] = (vertex4d){v[i].x, v[i].y, v[i].z, 1};
            inCount++;
        }
        else
        {
            n[nCount] = (vertex4d){v[i].x, v[i].y, v[i].z, 1};
            nCount++;
        }
    }

    if(*cTCounter + 2  >= *cTSize)
    {
        *cTSize *= 2;
        *clippedTriangles = (triangle *)realloc(*clippedTriangles, *cTSize * sizeof(triangle));
    }

    if(*cVCounter + 4 >= *cVSize)
    {
        *cVSize *= 2;
        *clippedVertices = (vertex4d *)realloc(*clippedVertices, *cVSize * sizeof(vertex4d));
    }

    if(inCount == 3) // all vertecies are inside the plane - output is the input triangle
    {
        (*clippedVertices)[*cVCounter] = vertices[inputTriangle.v0];
        (*clippedVertices)[*cVCounter + 1] = vertices[inputTriangle.v1];
        (*clippedVertices)[*cVCounter + 2] = vertices[inputTriangle.v2];
        (*clippedTriangles)[*cTCounter] = (triangle){*cVCounter, *cVCounter + 1, *cVCounter + 2, inputTriangle.c};
        *cTCounter += 1;
        *cVCounter += 3;
        
    }
    else if(inCount == 1) // 1 vertex is inside the plane - output is 1 clipped triangle
    {
        (*clippedVertices)[*cVCounter] = p[0];
        (*clippedVertices)[*cVCounter + 1] = computeIntersection(clippingPlane, n[0], p[0]);
        (*clippedVertices)[*cVCounter + 2] = computeIntersection(clippingPlane, n[1], p[0]);
        (*clippedTriangles)[*cTCounter] = (triangle){*cVCounter, *cVCounter + 1, *cVCounter + 2, inputTriangle.c};
        *cTCounter++;
        *cVCounter += 3;
    }
    else if(inCount == 2) // 2 vertecies are inside the plane - output is 2 clipped triangles
    {
        (*clippedVertices)[*cVCounter] = p[0];
        (*clippedVertices)[*cVCounter + 1] = p[1];
        (*clippedVertices)[*cVCounter + 2] = computeIntersection(clippingPlane, n[0], p[0]);
        (*clippedVertices)[*cVCounter + 3] = computeIntersection(clippingPlane, n[0], p[1]);
        (*clippedTriangles)[*cTCounter] = (triangle){*cVCounter, *cVCounter + 1, *cVCounter + 2, inputTriangle.c};
        (*clippedTriangles)[*cTCounter + 1] = (triangle){*cVCounter + 1, *cVCounter + 2, *cVCounter + 3, inputTriangle.c};
        *cTCounter += 2;
        *cVCounter += 4;
    }
}

int transformAndClip(plane clippingPlanes[5], Shape model, matrix4x4 transformed, float scale, Shape *clippedModel)
{
    int i;
    vertex4d center = multiplyMxV(transformed, model.boundsCenter);
    float radius = model.boundsRadius * scale, distance;

    for (i = 0; i < 5; i++)
    {
        distance = dotProduct(clippingPlanes[i].normal, (vertex){center.x, center.y, center.z}) + clippingPlanes[i].distance;
        if (distance < -radius)
            return 0;
    }


    int p;
    int cTSize = model.numberOfTriangles, cTCounter = 0;
    int cVSize = model.numberOfVertex, cVCounter = 0;

    vertex4d *vertices = (vertex4d *)malloc(model.numberOfVertex * sizeof(vertex4d));
    triangle *newClippedTriangles = (triangle*)malloc(model.numberOfTriangles * sizeof(triangle));
    vertex4d *clippedVertices = (vertex4d *)malloc(model.numberOfVertex * sizeof(vertex4d));

    for(i = 0; i < model.numberOfVertex; i++)
    {
        vertices[i] = multiplyMxV(transformed, (vertex4d){model.vertices[i].x, model.vertices[i].y, model.vertices[i].z, 1});
    }

    for(p = 0; p < 5; ++p)
    {

        for(i = 0; i < model.numberOfTriangles; ++i)
        {
           clipTrianges(vertices, model.triangles[i], clippingPlanes[p], &newClippedTriangles, &cTCounter, &cTSize, &clippedVertices, &cVCounter, &cVSize); 
        }
    }

    (*clippedModel).vertices = (vertex *)malloc(cVCounter * sizeof(vertex));
    (*clippedModel).triangles = (triangle *)malloc(cTCounter * sizeof(triangle));
    (*clippedModel).numberOfVertex = cVCounter;
    (*clippedModel).numberOfTriangles = cTCounter;
    (*clippedModel).boundsCenter = center;
    (*clippedModel).boundsRadius = radius;

    for(i = 0; i < cVCounter; i++)
    {
        (*clippedModel).vertices[i] = (vertex){clippedVertices[i].x, clippedVertices[i].y, clippedVertices[i].z};
    }

    for(i = 0; i < cTCounter; i++)
    {
        (*clippedModel).triangles[i] = newClippedTriangles[i];
    }

    free(vertices);
    free(newClippedTriangles);
    free(clippedVertices);

    return 1;

}

void RenderScene(cameraInstance camera, Object *instances, int numberOfInstaces)
{
    //first scale - then rotate - then translate
    int clipResult;
    matrix4x4 transformed;
    matrix4x4 cameraMatrix = multiplyMxM(transposeMatrix(camera.rotationMatrix), makeTranslationMatrix((vertex){-camera.location.x, -camera.location.y, -camera.location.z}));
    Shape clippedModel;

    for(int i = 0; i < numberOfInstaces; i++)
    {
        clippedModel = instances[i].typeOfShape;
        transformed =  multiplyMxM(cameraMatrix, transformObjectMatrix(instances[i].rotation, instances[i].locationOfObject, instances[i].scaleOfObject));
        clipResult = transformAndClip(camera.clippingPlanes, instances[i].typeOfShape, transformed ,instances[i].scaleOfObject, &clippedModel);
        //clipResult = 1;
        if(clipResult == 1)
        {
            RenderObject(clippedModel, transformed);
        }
        freeShape(clippedModel);

    }
}

cameraInstance initializeCamera()
{
    // this is 90deg FOV
    cameraInstance camera;
    float s = 1/sqrt(2);
    camera.location = (vertex){0, 0, 0};
    camera.rotationMatrix = identityMatrix;
    // near plane
    camera.clippingPlanes[0].normal = (vertex){0, 0, 1}; 
    camera.clippingPlanes[0].distance = -1;
    // left
    camera.clippingPlanes[1].normal = (vertex){s, 0, s};
    camera.clippingPlanes[1].distance = 0;
    // right
    camera.clippingPlanes[2].normal = (vertex){-s, 0, s};
    camera.clippingPlanes[2].distance = 0;
    // top
    camera.clippingPlanes[3].normal = (vertex){0, -s, s};
    camera.clippingPlanes[3].distance = 0;
    // bottom 
    camera.clippingPlanes[4].normal = (vertex){0, s, s};
    camera.clippingPlanes[4].distance = 0;

    return camera;
}

void Render()
{
    int numberOfInstaces = 2;
    Object *instances = (Object *)malloc(numberOfInstaces*sizeof(Object));
    cameraInstance camera = initializeCamera();
    camera.location = (vertex){0, 0, 0};

    
    instances[0] = initializeObject(instances[0], cube, RED);

    instances[0].scaleOfObject = 1;
    instances[0].locationOfObject = (vertex){2, 0, 2};

    instances[0].typeOfShape.triangles[0].c = BLUE;
    instances[0].typeOfShape.triangles[1].c = GREEN;
    instances[0].typeOfShape.triangles[2].c = CYAN;
    instances[0].typeOfShape.triangles[3].c = YELLOW;
    instances[0].typeOfShape.triangles[4].c = PURPLE;
    instances[0].typeOfShape.triangles[5].c = GREEN;
    instances[0].typeOfShape.triangles[6].c = BLUE;
    instances[0].typeOfShape.triangles[7].c = GREEN;


    instances[1] = initializeObject(instances[1], cube, RED);

    instances[1].locationOfObject = (vertex){2, 0, 10};
    instances[1].rotation = makeOYRotationMatrix(45);

    instances[1].typeOfShape.triangles[0].c = GREEN;
    instances[1].typeOfShape.triangles[1].c = YELLOW;
    instances[1].typeOfShape.triangles[2].c = PURPLE;
    instances[1].typeOfShape.triangles[3].c = CYAN;
    instances[1].typeOfShape.triangles[4].c = GREEN;
    instances[1].typeOfShape.triangles[5].c = YELLOW;
    instances[1].typeOfShape.triangles[6].c = BLUE;
    instances[1].typeOfShape.triangles[7].c = BLUE;

    //camera.rotationMatrix = makeOYRotationMatrix(-30);
    //camera.location = (vertex){-3, 1, 2};
    //instances[0].locationOfObject = (vertex){2, 2, 0};
    
    initializeImageArray();
    RenderScene(camera, instances, numberOfInstaces);

    free(instances);
}