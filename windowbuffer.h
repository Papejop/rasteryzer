#ifndef WINDOWBUFFER_H
#define WINDOWBUFFER_H

#include <windows.h>
#include "3Dspace.h"

typedef struct WindowBuffer
{
    unsigned char* memory;
    int width;
    int height;
    BITMAPINFO bitmapInfo;
} WindowBuffer;

void FillBuffer(WindowBuffer* buffer);
void InitializeBuffer(WindowBuffer* buffer, int width, int height);
void RenderBitmap(WindowBuffer* buffer, HDC hdc);

#endif // WINDOWBUFFER_H