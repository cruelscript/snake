#ifndef __SNAKEGAME_H
#define __SNAKEGAME_H

#include "Screen.h"
#include "Snake.h"
#include "Point.h"

const int PLAYER_NAME_LENGTH = 16;

struct Record
{
  void asString(char* buffer);
  char name_[PLAYER_NAME_LENGTH];
  double rating_;
  int length_;
  double gameTime;
  time_t date_;
};

class SnakeGame
{
public:
  SnakeGame(Screen& screen, int width = 80, int heigth = 24, int latency = 100);
  void game();
  void top10(bool afterGame);
  void readTop10();
  void writeTop10();
  bool playAgain();
  void pressAnyKey(int y);
  void logo();
  void goodbye();
private:
  enum Command
  {
    CMD_NOCOMMAND,
    CMD_EXIT,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_UP,
    CMD_DOWN
  };
  enum State
  {
    OK,
    DEAD,
    EXIT
  };
  typedef std::pair<int, Command> CmdPair;
  void drawField();
  Point makeFood();
  void printStatistics();
  Command getCommand();
  void top10Table();

  Screen screen_;
  Snake snake_;
  Record top10_[10];
  CmdPair cmdTable[5];
  int width_, height_;
  int latency_;
  double duration_;
  double rating_, rating_i_;
};

#endif