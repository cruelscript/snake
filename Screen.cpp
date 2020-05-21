#include "Screen.h"

#include <conio.h>

Screen::Screen() :
  consoleOutput_(GetStdHandle(STD_OUTPUT_HANDLE))
{
  if (consoleOutput_ == INVALID_HANDLE_VALUE)
    throw ScreenException("Error. INVALID_HANDLE_VALUE");
  if (!GetConsoleCursorInfo(consoleOutput_, &oldCursorInfo_))
    throw ScreenException("Error. GetConsoleCursorInfo()");

  currentCursorInfo_.dwSize = oldCursorInfo_.dwSize;
  currentCursorInfo_.bVisible = oldCursorInfo_.bVisible;

  CONSOLE_SCREEN_BUFFER_INFO CSBI;
  GetConsoleScreenBufferInfo(consoleOutput_, &CSBI);
  oldTextAttribute_ = CSBI.wAttributes;
}

Screen::~Screen()
{
  SetConsoleCursorInfo(consoleOutput_, &oldCursorInfo_);
  SetConsoleTextAttribute(consoleOutput_, oldTextAttribute_);
}

void Screen::showCursor(bool visible)
{
  currentCursorInfo_.bVisible = visible;
  if (!SetConsoleCursorInfo(consoleOutput_, &currentCursorInfo_))
    throw ScreenException("Error. SetConsoleCursorInfo()");
}

void Screen::textAttribute(WORD attribute)
{
  SetConsoleTextAttribute(consoleOutput_, attribute);
}

void Screen::setPos(int x, int y, char ch)
{
  COORD point;
  point.X = static_cast<SHORT>(x);
  point.Y = static_cast<SHORT>(y);

  if (!SetConsoleCursorPosition(consoleOutput_, point))
    throw ScreenException("Error. SetConsoleCursorPosition()");
  if (ch > 0)
    _putch(ch);
}

void Screen::setPosString(int x, int y, const char* str)
{
  setPos(x, y);
  _cprintf("%s", str);
}

void Screen::cls()
{
  system("cls");
}
