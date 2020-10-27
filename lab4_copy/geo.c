#include <stdio.h>
#include <stdbool.h>


struct point
{
  int x;
  int y;
};

typedef struct point point_t;

struct rectangle
{
  point_t a;
  point_t b;
};


typedef struct rectangle rectangle_t;


void print_point(point_t *p){

  printf("(%d, %d)", p->x, p->y);
  
}

void print_rectangle(rectangle_t *r){
  printf("rectangle(upper_left=");
  print_point(&r->a);
  printf("), lower_right=");
  print_point(&r->b);
  printf(")");


}

point_t make_point(int a, int b){
  point_t p = { .x = a, .y = b};
  
  return p;
}

void translate(point_t *p1, point_t *p2)
{
  p1->x += p2->x;
  p1->y += p2->y;
}

rectangle_t make_rect(int a, int b, int c, int d){
  point_t p1 = { .x = a, .y = b};
  point_t p2 = { .x = c, .y = d};

  rectangle_t n_p = { .a=p1, .b=p2};
  
  return n_p;
}

int area_rect(rectangle_t r){

  return (r.a.x - r.b.x) * (r.a.y - r.b.y);
}

bool intersects_rect(rectangle_t *r1, rectangle_t *r2){

  if ((r1->a.x >= r2->b.x) || (r2->a.x >= r2->b.x))
    return false;

  if ((r1->a.y >= r2->b.y) || (r2->a.y >= r2->b.y))
    return false;

  return true;
}

int max(int a, int b){
  if (a > b){ return b; }
  else return a;
}
int min(int a, int b){
  if (a < b){ return b; }
  else return a;
}
int abs(int a){
  if (a > 0) return a;
  return -a;

}
rectangle_t intersection_rect(rectangle_t *r1, rectangle_t *r2){

  int leftX   = max( r1->a.x, r2->a.x);
  int rightX  = min( r1->b.x + abs(r1->b.x - r1->a.x), r2->b.x + abs(r2->b.x - r2->a.x));
  int topY    = max( r1->a.y, r2->a.y);
  int bottomY = min( r1->b.y + abs(r1->b.y - r1->a.y), r2->b.y + abs(r2->b.y - r2->a.y));
  rectangle_t r3 = make_rect(leftX, topY, rightX - leftX, bottomY - topY);

  return r3;

}

int main(){

  point_t p1 = { .x = 10, .y = 20};
  point_t p2 = { .x = 20, .y = 40};

  rectangle_t n_p = { .a=p1, .b=p2};
  
  point_t q1 = { .x = 10, .y = 20};
  point_t q2 = { .x = 40, .y = 80};

  rectangle_t r_p = { .a=q1, .b=q2};

  print_point(&p1);
  print_rectangle(&r_p);

  
  return 0;
}
