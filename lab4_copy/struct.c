#include <stdio.h>

struct point
{
  int x;
  int y;
};

typedef struct point point_t;

struct point translate(point_t p1, point_t p2)
{
  p1.x += p2.x;
  p1.y += p2.y;
  return p1;
}

int main(){


  point_t p1 = { .x = 10 };
  point_t p2 = { .y = -42 };
  point_t p3 = { };

  printf("point(x=%d,y=%d)\n", p1.x, p1.y);
  printf("point(x=%d,y=%d)\n", p2.x, p2.y);
  printf("point(x=%d,y=%d)\n", p3.x, p3.y);

  return 0;
}
