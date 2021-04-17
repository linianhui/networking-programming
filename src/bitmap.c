#include "bitmap.h"

bitmap *bitmap_init(size_t size)
{
    struct bitmap *bmp = (bitmap *)malloc(sizeof(bitmap));
    size_t len = (size >> 5) + (size & 1);
    bmp->len = len * 32;
    bmp->data = (unsigned int *)malloc(len * 4);
    bzero(bmp->data, len * 4);
    return bmp;
}

int bitmap_get(bitmap *bmp, int index)
{
    if (index > bmp->len)
    {
        return -1;
    }
    int i1 = index >> 5;
    int i2 = index & 31;

    unsigned int value = bmp->data[i1];

    unsigned int left = value << i2;
    return left >> 31;
}

int bitmap_set(bitmap *bmp, int index)
{
    if (index > bmp->len)
    {
        return -1;
    }
    int i1 = index >> 5;
    int i2 = index & 31;

    unsigned int value = bmp->data[i1];

    bmp->data[i1] = value | (1 << (31 - i2));
    return 0;
}

int bitmap_del(bitmap *bmp, int index)
{
    if (index > bmp->len)
    {
        return -1;
    }
    int i1 = index >> 5;
    int i2 = index & 31;

    unsigned int value = bmp->data[i1];

    bmp->data[i1] = value & (~(1 << (31 - i2)));

    return 0;
}