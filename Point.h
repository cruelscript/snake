#ifndef __POINT_H
#define __POINT_H

class Point
{
public:
  Point();
  Point(int x, int y);
  Point& operator+=(const Point& other);
  Point operator+(const Point& other);
  bool   operator==(const Point& other);
  friend class SnakeGame;
  friend class Snake;
private:
  int x_;
  int y_;
};

#endif
