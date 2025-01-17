#include "draw.h"

FILE *file;
color image[HEIGHT][WIDTH];

void initializeImageArray() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            image[i][j] = (color){255, 255, 255};
        }
    }
}

void swap(point *p0, point *p1) {
    point temp = *p0;
    *p0 = *p1;
    *p1 = temp;
}

void deallocateMemory(arrayMetadata array) {
    free(array.pointer);
    array.pointer = NULL;
}

arrayMetadata interpolate(int i0, float d0, int i1, float d1) {
    float a, d = d0, *interpolateValuesArray;
    size_t size = (i0 == i1) ? 1 : (i1 - i0 + 1);
    interpolateValuesArray = (float *)malloc(size * sizeof(float));

    if (interpolateValuesArray == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    if (i0 == i1) {
        interpolateValuesArray[0] = d;
        return (arrayMetadata){interpolateValuesArray, 1};
    }

    a = (float)(d1 - d0) / (i1 - i0);

    for (int i = i0; i <= i1; ++i) {
        interpolateValuesArray[i - i0] = d;
        d = d + a;
    }

    return (arrayMetadata){interpolateValuesArray, size};
}

void saveToPPM() {
    file = fopen("image.ppm", "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "P3\n");
    fprintf(file, "# Hello\n");
    fprintf(file, "%d %d\n", WIDTH, HEIGHT);
    fprintf(file, "255\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            fprintf(file, "%d %d %d  ", image[i][j].red, image[i][j].green, image[i][j].blue);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void drawPixel(int y, int x, color color) {
    int a,b;
    if (y < -HEIGHT/2 || y >= HEIGHT/2 - 1 || x < -WIDTH/2 || x >= WIDTH/2 - 1) {
        return;
    }
    a=HEIGHT / 2 + y;  b=WIDTH / 2 + x;
    image[HEIGHT / 2 + y][WIDTH / 2 + x] = color;
}

void drawLine(point p0, point p1, color color) {
    arrayMetadata array;
    float a, y, x;
    float dx = p1.x - p0.x;
    float dy = p1.y - p0.y;
    if (abs(dx) > abs(dy)) {
        if (p0.x > p1.x) {
            swap(&p0, &p1);
        }

        array = interpolate(p0.x, p0.y, p1.x, p1.y);

        for (x = p0.x; x <= p1.x; ++x) {
            drawPixel(array.pointer[(int)(x - p0.x)], x, color);
        }
    } else {
        if (p0.y > p1.y) {
            swap(&p0, &p1);
        }

        array = interpolate(p0.y, p0.x, p1.y, p1.x);

        for (y = p0.y; y <= p1.y; ++y) {
            drawPixel(y, array.pointer[(int)(y - p0.y)], color);
        }
    }
    deallocateMemory(array);
}

void drawFilledTriangle(point p0, point p1, point p2, color color) {
    arrayMetadata x01, x12, x02, x012, xLeft, xRight;
    int i, x, y, middleOfline;

    if (p0.y > p1.y) swap(&p0, &p1);
    if (p0.y > p2.y) swap(&p0, &p2);
    if (p1.y > p2.y) swap(&p1, &p2);

    x01 = interpolate(p0.y, p0.x, p1.y, p1.x);
    x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
    x02 = interpolate(p0.y, p0.x, p2.y, p2.x);

    x01.size = x01.size - 1;

    x012.pointer = (float *)malloc((x01.size + x12.size) * sizeof(float));
    x012.size = x01.size + x12.size;

    for (i = 0; i <= x01.size; i++) {
        x012.pointer[i] = x01.pointer[i];
    }

    for (i = 0; i <= x12.size; i++) {
        x012.pointer[i + x01.size] = x12.pointer[i];
    }

    middleOfline = x02.size / 2;

    if (x012.pointer[middleOfline] > x02.pointer[middleOfline]) {
        xLeft = x02;
        xRight = x012;
    } else {
        xLeft = x012;
        xRight = x02;
    }

    for (y = p0.y; y < p2.y; ++y) {
        for (x = xLeft.pointer[(int)y - (int)p0.y]; x <= xRight.pointer[(int)y - (int)p0.y]; ++x) {
            drawPixel(y, x, color);
        }
    }

    deallocateMemory(x01);
    deallocateMemory(x12);
    deallocateMemory(x02);
    deallocateMemory(x012);
}

// TODO add dynamic shading to this function, currently its static, declared inside this function
void drawShadedTriangle(point p0, point p1, point p2, color basicColor) {
    arrayMetadata x01, x12, x02, x012, xLeft, xRight;
    arrayMetadata h01, h12, h02, h012, hLeft, hRight, hSegment;
    color shadedColor;
    int i, x, y, middleOfline;
    float h0 = 0.5, h1 = 0.75, h2 = 0.25;

    if (p0.y > p1.y) swap(&p0, &p1);
    if (p0.y > p2.y) swap(&p0, &p2);
    if (p1.y > p2.y) swap(&p1, &p2);

    x01 = interpolate(p0.y, p0.x, p1.y, p1.x);
    h01 = interpolate(p0.y, h0, p1.y, h1);

    x12 = interpolate(p1.y, p1.x, p2.y, p2.x);
    h12 = interpolate(p1.y, h1, p2.y, h2);

    x02 = interpolate(p0.y, p0.x, p2.y, p2.x);
    h02 = interpolate(p0.y, h0, p2.y, h2);

    x01.size = x01.size - 1; // we remove the last value to avoid overlap of the lines

    h01.size = h01.size - 1; // same here but for the shading values

    x012.pointer = (float *)malloc((x01.size + x12.size) * sizeof(float));
    x012.size = x01.size + x12.size;

    h012.pointer = (float *)malloc((h01.size + h12.size) * sizeof(float));
    h012.size = h01.size + h12.size;

    for (i = 0; i <= x01.size; i++)
        x012.pointer[i] = x01.pointer[i];

    for (i = 0; i <= x12.size; i++)
        x012.pointer[i + x01.size] = x12.pointer[i];

    for (i = 0; i <= h01.size; i++)
        h012.pointer[i] = h01.pointer[i];

    for (i = 0; i <= h12.size; i++)
        h012.pointer[i + h01.size] = h12.pointer[i];

    middleOfline = x012.size / 2;

    if (x012.pointer[middleOfline] > x02.pointer[middleOfline]) {
        xLeft = x02;
        hLeft = h02;

        xRight = x012;
        hRight = h012;
    } else {
        xLeft = x012;
        hLeft = h012;

        xRight = x02;
        hRight = h02;
    }

    for (y = p0.y; y < p2.y; ++y) {
        hSegment = interpolate(xLeft.pointer[(int)y - (int)p0.y], hLeft.pointer[(int)y - (int)p0.y], xRight.pointer[(int)y - (int)p0.y], hRight.pointer[(int)y - (int)p0.y]);
        for (x = xLeft.pointer[(int)y - (int)p0.y]; x <= xRight.pointer[(int)y - (int)p0.y]; ++x) {
            shadedColor.red = basicColor.red * hSegment.pointer[(int)x - (int)xLeft.pointer[(int)y - (int)p0.y]];
            shadedColor.green = basicColor.green * hSegment.pointer[(int)x - (int)xLeft.pointer[(int)y - (int)p0.y]];
            shadedColor.blue = basicColor.blue * hSegment.pointer[(int)x - (int)xLeft.pointer[(int)y - (int)p0.y]];

            drawPixel(y, x, shadedColor);
        }

        deallocateMemory(hSegment);
    }

    deallocateMemory(x01);
    deallocateMemory(x12);
    deallocateMemory(x02);
    deallocateMemory(x012);

    deallocateMemory(h01);
    deallocateMemory(h12);
    deallocateMemory(h02);
    deallocateMemory(h012);
}

void drawWireFrameTriangle(point p0, point p1, point p2, color color)
{
    if (p0.y > p1.y) swap(&p0, &p1);
    if (p0.y > p2.y) swap(&p0, &p2);
    if (p1.y > p2.y) swap(&p1, &p2);

    drawLine(p0, p1, color);
    drawLine(p1, p2, color);
    drawLine(p2, p0, color);
}