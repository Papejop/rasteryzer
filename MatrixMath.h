#include <math.h>  


typedef struct matrix4x4{
    float matrix[4][4];
} matrix4x4;

typedef struct {
    float x;
    float y;
    float z;
} vertex;

typedef struct vertex4d {
    float x;
    float y;
    float z;
    float w;    
} vertex4d;

extern matrix4x4 identityMatrix;

matrix4x4 makeScailingMatrix(float scale);
matrix4x4 multiplyMxM(matrix4x4 a, matrix4x4 b);
matrix4x4 makeOYRotationMatrix(float degrees);
matrix4x4 makeOZRotationMatrix(float degrees);
matrix4x4 makeTranslationMatrix(vertex translation);
vertex4d multiplyMxV(matrix4x4 a, vertex4d b);
matrix4x4 transposeMatrix(matrix4x4 origianl);
matrix4x4 transformObjectMatrix(matrix4x4 rotation, vertex location, float scale);