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
    if (bit_get(bmp, i))             \
    {                                \
      expr;                          \
    }                                \
  }

bitmap *init_bitmap(size_t size);

int bit_get(bitmap *bmp, int index);

int bit_set(bitmap *bmp, int index);

int bit_clear(bitmap *bmp, int index);