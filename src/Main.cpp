#include "Game.h"
int main(int argc, const char **argv)
{
  Game game;
  bool success = game.Initialize();
  if (success)
  {
    game.RunLoop();
  }
  game.Shutdown();
  return 0;
}