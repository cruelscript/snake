#ifndef __SNAKE_H
#define __SNAKE_H

#include <vector>

#include "Point.h"
#include "Screen.h"

typedef std::vector<Point> PointVector;

class Snake
{
public:
  Snake();
  void reset(Point startPos);
  void draw(Screen& screen);
  void move(const Point& delta, Screen& screen);
  void grow(const Point& pos, int growCoeficient);
  bool into(const Point& pos);
  Point head();
  int size();
private:
  PointVector snake_;
  char headSymbol_;
  unsigned int drawn_;
};

#endif