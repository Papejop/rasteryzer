#include "MatrixMath.h"

matrix4x4 identityMatrix = {.matrix = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}};

matrix4x4 makeScailingMatrix(float scale)
{
    matrix4x4 scaled = {.matrix = {{scale,0,0,0},{0,scale,0,0},{0,0,scale,0},{0,0,0,1}}};
    return scaled;
}

matrix4x4 multiplyMxM(matrix4x4 a, matrix4x4 b)
{
    matrix4x4 result = {.matrix = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}};
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                result.matrix[i][j] += a.matrix[i][k] * b.matrix[k][j];
            }
        }
    }
    return result;
}

matrix4x4 makeOYRotationMatrix(float degrees)
{
    degrees = degrees * 3.14159 / 180;
    float sinD = sin(degrees);
    float cosD = cos(degrees);

    matrix4x4 OYrotationMatrix = {.matrix = {{cosD,0,-sinD,0},{0,1,0,0},{sinD,0,cosD,0},{0,0,0,1}}};
    return OYrotationMatrix;
}

matrix4x4 makeOXRotationMatrix(float degrees)
{
    degrees = degrees * 3.14159 / 180;
    float sinD = sin(degrees);
    float cosD = cos(degrees);

    matrix4x4 OXrotationMatrix = {.matrix = {{1,0,0,0},{0,cosD,-sinD,0},{0,sinD,cosD,0},{0,0,0,1}}};
    return OXrotationMatrix;
}

matrix4x4 makeOZRotationMatrix(float degrees)
{
    degrees = degrees * 3.14159 / 180;
    float sinD = sin(degrees);
    float cosD = cos(degrees);

    matrix4x4 OZrotationMatrix = {.matrix  = {{cosD,-sinD,0,0},{sinD,cosD,0,0},{0,0,1,0},{0,0,0,1}}};
    return OZrotationMatrix;
}

matrix4x4 makeTranslationMatrix(vertex translation)
{
    matrix4x4 translationMatrix = {.matrix = {{1,0,0,translation.x},{0,1,0,translation.y},{0,0,1,translation.z},{0,0,0,1}}};
    return translationMatrix;
}

vertex4d multiplyMxV(matrix4x4 inputMatrix, vertex4d vertex4D) 
{
    float input[4] = {vertex4D.x, vertex4D.y, vertex4D.z, vertex4D.w};
    float result[4] = {0,0,0,0};

    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            result[i] += inputMatrix.matrix[i][j] * input[j];
        }
    }
    
    return (vertex4d){result[0], result[1], result[2], result[3]}; 
}

matrix4x4 transposeMatrix(matrix4x4 origianl)
{
    matrix4x4 transposed = {.matrix = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}};
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            transposed.matrix[i][j] = origianl.matrix[j][i];
        }
    }
    return transposed;
}

matrix4x4 transformObjectMatrix(matrix4x4 rotation, vertex location, float scale)
{
    return multiplyMxM(makeTranslationMatrix(location), multiplyMxM(rotation, makeScailingMatrix(scale)));
}
