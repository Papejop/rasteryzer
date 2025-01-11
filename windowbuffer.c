#include <windows.h>
#include "windowbuffer.h"
#include "camera.h"

void InitializeBuffer(WindowBuffer* buffer, int width, int height)
{
    buffer->width = width;
    buffer->height = height;

    buffer->bitmapInfo.bmiHeader.biSize = sizeof(buffer->bitmapInfo.bmiHeader);
    buffer->bitmapInfo.bmiHeader.biWidth = buffer->width;
    buffer->bitmapInfo.bmiHeader.biHeight = -buffer->height; // Top-down DIB
    buffer->bitmapInfo.bmiHeader.biPlanes = 1;
    buffer->bitmapInfo.bmiHeader.biBitCount = 32;
    buffer->bitmapInfo.bmiHeader.biCompression = BI_RGB;

    int bitmapMemorySize = buffer->width * buffer->height * 4;
    buffer->memory = (unsigned char*)VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    unsigned int* pixel = (unsigned int*)buffer->memory;
    for (int y = 0; y < buffer->height; ++y)
    {
        for (int x = 0; x < buffer->width; ++x)
        {
            *pixel++ = 0x00FF0000; // Red color in 0xAARRGGBB format (Alpha, Red, Green, Blue)
        }
    }
}

void RenderBitmap(WindowBuffer* buffer, HDC hdc)
{
    StretchDIBits(hdc,
                  0, 0, buffer->width, buffer->height,
                  0, 0, buffer->width, buffer->height,
                  buffer->memory,
                  &buffer->bitmapInfo,
                  DIB_RGB_COLORS, SRCCOPY);
}