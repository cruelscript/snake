#include <iostream>
#include "Screen.h"
#include "SnakeGame.h"

int main()
{
  try
  {
    Screen screen;
    screen.showCursor(false);
    screen.textAttribute((WORD)0x0a);
    screen.cls();

    SnakeGame game(screen, 80, 24, 120);
    game.logo();

    game.readTop10();
    game.top10(false);
    game.pressAnyKey(18);

    do
    {
      game.game();
      game.top10(true);
    }
    while (game.playAgain());

    game.goodbye();
  }
  catch (std::exception& e)
  {
    std::cerr << "***" << e.what() << std::endl;
  }
  return 0;
}

