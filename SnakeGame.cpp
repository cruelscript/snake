#include "SnakeGame.h"
#include <ctime>
#include "iostream"
#include <conio.h>
#include <fstream>

const char* RECORD_FORMAT_STRING = "%-15s  %9.4f  %4u  %7.2f  %s";
const char* TOP10_DATA_FILENAME = "snake.txt";
const char* VERSION_NUMBER = "v 1.0";
const char* COPYRIGHT = "(c) cruelscript, 2020.";
const char BORDER = '#';
const char FOOD = '$';

void Record::asString(char* buffer)
{
  sprintf(buffer, RECORD_FORMAT_STRING, name_, rating_, 
    length_, gameTime, ctime(&date_));
}

std::ostream& operator<<(std::ostream& out, const Record& record)
{
  out << record.rating_ << ' ' << record.length_ << ' '
    << record.gameTime << ' ' << record.date_ << ' ' << record.name_ << std::endl;
  return out;
}

std::istream& operator>>(std::istream& in, Record& record)
{
  in >> record.rating_ >> record.length_ >> record.gameTime >> record.date_;
  in.ignore(1);
  in.getline(&record.name_[0], 16);
  return in;
}

int recordCompare(const void* first, const void* second)
{
  const Record* first_ = reinterpret_cast<const Record*>(first);
  const Record* second_ = reinterpret_cast<const Record*>(second);
  return static_cast<int>(second_->rating_ - first_->rating_);
}

void clearKeys()
{
  while(_kbhit())
  {
    _getch();
  }
}

SnakeGame::SnakeGame(Screen& screen, int width, int heigth, int latency) :
   screen_(screen), width_(width), height_(heigth), latency_(latency),
  duration_(0), rating_(0), rating_i_(0)
{
  srand(static_cast<unsigned int>(time(NULL)));

  cmdTable[0] = CmdPair(27, CMD_EXIT);
  cmdTable[1] = CmdPair('K', CMD_LEFT);
  cmdTable[2] = CmdPair('M', CMD_RIGHT);
  cmdTable[3] = CmdPair('H', CMD_UP);
  cmdTable[4] = CmdPair('P', CMD_DOWN);
}

void SnakeGame::game()
{
  duration_ = 0;
  rating_ = rating_i_ = 0.0;

  drawField();

  snake_.reset(Point(width_ / 2, height_ / 2));

  Command cmd = CMD_NOCOMMAND;
  State state = OK;
  Point delta(-1, 0);

  Point food = makeFood();
  screen_.setPos(food.x_, food.y_, FOOD);
  snake_.draw(screen_);
  printStatistics();

  clock_t beginTime, endTime;
  beginTime = clock();
  do
  {
    if(_kbhit())
    {
      cmd = getCommand();
    }
    switch (cmd)
    {
    case CMD_LEFT:
      delta = Point(-1, 0);
      break;
    case CMD_RIGHT:
      delta = Point(1, 0);
      break;
    case CMD_UP:
      delta = Point(0, -1);
      break;
    case CMD_DOWN:
      delta = Point(0, 1);
      break;
    default:
      break;
    }
    Point head = snake_.head();
    head += delta;

    if (head.x_ == 0 || head.x_ == width_ - 1 || head.y_ == 0
      || head.y_ == height_ - 1 || snake_.into(head))
    {
      state = DEAD;
    }
    if(state == OK)
    {
      snake_.move(delta, screen_);

      if(snake_.head() == food)
      {
        snake_.grow(food, 3);
        food = makeFood();
        screen_.setPos(food.x_, food.y_, FOOD);

        endTime = clock();
        duration_ += static_cast<double>(endTime - beginTime) / CLOCKS_PER_SEC;
        rating_i_ = snake_.size() / duration_;
        rating_ += rating_i_;
        beginTime = endTime;

        printStatistics();
      }
      Sleep(latency_);
    }
  }
  while (state == OK);

  screen_.setPosString(width_ / 2 - 8, 10, " G a m e    o v e r ");
  clearKeys();
  _getch();
  clearKeys();
}

void SnakeGame::top10(bool afterGame)
{
  char buffer[80];
  char bufferEncoded[PLAYER_NAME_LENGTH];

  top10Table();

  time_t date = time(NULL);
  if(afterGame)
  {
    screen_.setPos(5, 16);
    _cprintf(RECORD_FORMAT_STRING, "Your result", rating_, 
      snake_.size(), duration_, ctime(&date));
  }
  if(rating_ > top10_[9].rating_)
  {
    screen_.setPosString(5, 20, "Your name: _");
    screen_.setPos(16, 20);
    std::cin.getline(&buffer[0], PLAYER_NAME_LENGTH);
    clearKeys();

    OemToCharBuffA(buffer, bufferEncoded, static_cast<DWORD>(PLAYER_NAME_LENGTH));

    strcpy(top10_[9].name_, bufferEncoded);
    top10_[9].date_ = date;
    top10_[9].gameTime = duration_;
    top10_[9].length_ = snake_.size();
    top10_[9].rating_ = rating_;

    qsort(top10_, 10, sizeof(Record), recordCompare);

    top10Table();
    writeTop10();
  }
}

void SnakeGame::readTop10()
{
  std::ifstream fin(TOP10_DATA_FILENAME);
  if(fin)
  {
    for(int i = 0; i < 10; i++)
    {
      fin >> top10_[i];
    }
  }
  fin.close();
}

void SnakeGame::writeTop10()
{
  std::ofstream fout(TOP10_DATA_FILENAME);
  if(fout)
  {
    for(int i = 0; i < 10; i++)
    {
      fout << top10_[i];
    }
  }
  fout.close();
}

bool SnakeGame::playAgain()
{
  screen_.setPosString(width_ / 2 - 12, height_ - 3, "O n c e    m o r e ?");
  int ch = _getch();
  clearKeys();
  if (ch == 'N' || ch == 'n' || ch == 27)
    return false;
  return true;
}

void SnakeGame::pressAnyKey(int y)
{
  screen_.setPosString(width_ / 2 - 15, y, "Press any key for continue...");
  _getch();
  clearKeys();
}

void SnakeGame::logo()
{
  screen_.setPosString(width_ / 2 - 9, 10, "O l d s c h o o l");
  screen_.setPosString(width_ / 2 - 7, 12, "S  N  A  K  E");
  screen_.setPosString(width_ / 2 - 3, 16, VERSION_NUMBER);
  screen_.setPosString(width_ / 2 - 11, height_, COPYRIGHT);
  pressAnyKey(22);
}

void SnakeGame::goodbye()
{
  screen_.cls();
  _cprintf("Oldschool Snake %s\n%s\n", VERSION_NUMBER, COPYRIGHT);
}

void SnakeGame::drawField()
{
  screen_.cls();

  for(int y = 0; y < height_; y++)
  {
    if(y== 0 || y == height_ - 1)
    {
      for(int x = 0; x < width_; x++)
      {
        screen_.setPos(x, y, BORDER);
      }
    }
    else
    {
      screen_.setPos(0, y, BORDER);
      screen_.setPos(width_ - 1, y, BORDER);
    }
  }
  screen_.setPos(0, height_);
  _cprintf("Length: ****  Rating: ****.**** (****.****)  Time: ****.**");
}

Point SnakeGame::makeFood()
{
  Point food;
  do
  {
    food.x_ = rand() % (width_ - 2) + 1;
    food.y_ = rand() % (height_ - 2) + 1;
  }
  while (snake_.into(food));
  return food;
}

void SnakeGame::printStatistics()
{
  screen_.setPos(8, height_);
  _cprintf("%04u", snake_.size());
  screen_.setPos(22, height_);
  _cprintf("%09.4f", rating_);
  screen_.setPos(33, height_);
  _cprintf("%09.4f", rating_i_);
  screen_.setPos(51, height_);
  _cprintf("%07.2f", duration_);
}

SnakeGame::Command SnakeGame::getCommand()
{
  int ch;
  ch = _getch();
  if(ch == 0 || ch == 0xe0)
  {
    ch = _getch();
  }
  for(int i = 0; i < 5; i++)
  {
    if(cmdTable[i].first == ch)
    {
      return cmdTable[i].second;
    }
  }
  return CMD_NOCOMMAND;
}

void SnakeGame::top10Table()
{
  screen_.cls();
  char buffer[80];

  screen_.setPosString(width_ / 2 - 12, 2,"***** T O P    1 0 *****");
  screen_.setPosString(5, 4, "Name                Rating  Length  Time   Date");

  for(int i = 0; i < 10; i++)
  {
    top10_[i].asString(buffer);
    screen_.setPosString(5, 5 + i, buffer);
  }
}
