#include "Point.h"

Point::Point() :
  x_(0), y_(0)
{}

Point::Point(int x, int y) :
  x_(x), y_(y)
{}

Point& Point::operator+=(const Point& other)
{
  x_ += other.x_;
  y_ += other.y_;
  return *this;
}

Point Point::operator+(const Point& other)
{
  return Point(x_ + other.x_, y_ + other.y_);
}

bool Point::operator==(const Point& other)
{
  return x_ == other.x_ && y_ == other.y_;
}
