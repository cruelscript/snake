#ifndef __SCREEN_H
#define __SCREEN_H

#include <windows.h>
#include <exception>

class Screen
{
public:
  Screen();
  ~Screen();
  void showCursor(bool visible);
  void textAttribute(WORD attribute);
  void setPos(int x, int y, char ch = 0);
  void setPosString(int x, int y, const char* str);
  void cls();
private:
  HANDLE consoleOutput_;
  CONSOLE_CURSOR_INFO oldCursorInfo_, currentCursorInfo_;
  WORD oldTextAttribute_;
};

class ScreenException : std::exception
{
public:
  ScreenException(const char* reason) : reason_(reason) {}
  const char* what() const noexcept override { return reason_; }
private:
  const char* reason_;
};

#endif
