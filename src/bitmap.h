// bitmap
#include <stdlib.h>
#include <strings.h>

typedef struct bitmap
{
  size_t len;
  unsigned int *data;
} bitmap;

#define bit_loop(bmp, expr)          \
  for (int i = 0; i < bmp->len; i++) \
  {                                  \
    if (bitmap_get(bmp, i) == 1)     \
    {                                \
      expr;                          \
    }                                \
  }

bitmap *bitmap_init(size_t size);

int bitmap_get(bitmap *bmp, int index);

int bitmap_set(bitmap *bmp, int index);

int bitmap_del(bitmap *bmp, int index);