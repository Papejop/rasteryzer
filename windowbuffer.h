#ifndef WINDOWBUFFER_H
#define WINDOWBUFFER_H

#include <windows.h>

typedef struct WindowBuffer
{
    unsigned char* memory;
    int width;
    int height;
    BITMAPINFO bitmapInfo;
} WindowBuffer;

void InitializeBuffer(WindowBuffer* buffer, int width, int height);
void RenderBitmap(WindowBuffer* buffer, HDC hdc);

#endif // WINDOWBUFFER_H