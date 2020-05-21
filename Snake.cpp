#include "Snake.h"

const char SNAKE_BODY = '@';

Snake::Snake() :
  headSymbol_('<')
{}

void Snake::reset(Point startPos)
{
  snake_.clear();
  snake_.reserve(1000);
  snake_.push_back(startPos);
  snake_.push_back(startPos);
  snake_[0].x_++;
}

void Snake::draw(Screen& screen)
{
  unsigned int sSize = snake_.size() - 1;
  for(unsigned i = 0; i < sSize; i++)
  {
    screen.setPos(snake_[i].x_, snake_[i].y_, SNAKE_BODY);
  }
  screen.setPos(snake_[sSize].x_, snake_[sSize].y_, headSymbol_);
  drawn_ = snake_.size();
}

void Snake::move(const Point& delta, Screen& screen)
{
  if (drawn_ == snake_.size())
  {
    screen.setPos(snake_[0].x_, snake_[0].y_, ' ');
  }
  else drawn_++;

  for(unsigned int i = 1; i < snake_.size(); i++)
  {
    snake_[i - 1] = snake_[i];
  }

  snake_[snake_.size() - 1] += delta;

  if (delta.x_ < 0)
    headSymbol_ = '<';
  else if (delta.x_ > 0)
    headSymbol_ = '>';
  else if (delta.y_ < 0)
    headSymbol_ = 'A';
  else headSymbol_ = 'V';

  screen.setPos(snake_[snake_.size() - 1].x_, 
    snake_[snake_.size() - 1].y_, headSymbol_);
  screen.setPos(snake_[snake_.size() - 2].x_, 
    snake_[snake_.size() - 2].y_, SNAKE_BODY);
}

void Snake::grow(const Point& pos, int growCoeficient)
{
  for(int i = 0; i < growCoeficient; i++)
  {
    snake_.insert(snake_.begin(), pos);
  }
}

bool Snake::into(const Point& pos)
{
  for(unsigned int i = 0; i < snake_.size(); i++)
  {
    if (snake_[i].x_ == pos.x_ && snake_[i].y_ == pos.y_)
      return true;
  }
  return false;
}

Point Snake::head()
{
  return snake_[snake_.size() - 1];
}

int Snake::size()
{
  return snake_.size();
}
