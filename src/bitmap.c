#include "bitmap.h"

bitmap *init_bitmap(size_t size)
{
    struct bitmap *bmp = (bitmap *)malloc(sizeof(bitmap));
    size_t len = (size >> 5) + (size & 1);
    bmp->len = len * 32;
    bmp->data = (unsigned int *)malloc(len * 4);
    return bmp;
}

int bit_get(bitmap *ptr, int index)
{
    if (index > ptr->len)
    {
        return -1;
    }
    int i1 = index >> 5;
    int i2 = index & 31;

    unsigned int value = ptr->data[i1];

    unsigned int left = value << i2;
    return left >> 31;
}

int bit_set(bitmap *ptr, int index)
{
    if (index > ptr->len)
    {
        return -1;
    }
    int i1 = index >> 5;
    int i2 = index & 31;

    unsigned int value = ptr->data[i1];

    ptr->data[i1] = value | (1 << (31 - i2));
    return 0;
}

int bit_clear(bitmap *ptr, int index)
{
    if (index > ptr->len)
    {
        return -1;
    }
    int i1 = index >> 5;
    int i2 = index & 31;

    unsigned int value = ptr->data[i1];

    ptr->data[i1] = value & (~(1 << (31 - i2)));

    return 0;
}