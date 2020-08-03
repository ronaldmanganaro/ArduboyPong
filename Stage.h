#ifndef STAGE_H
#define STAGE_H

#include <Arduboy2.h>
#include "Player.h"

class Stage
{
public:
  Stage() {}

  Stage(Arduboy2 _arduboy, Ball *daball)
  {
    arduboy = _arduboy;
    ball = daball;
    scores[0] = 0;
    scores[1] = 0;
  }

  void updateState()
  {
    switch (GameState)
    {
    case 0:
      displayStartMenu();
      if (initialpos != 2)
        initialpos -= 4;
      break;
    case 1:

      displayScore();
      break;
    case 2:
      displayOptions();
      break;
    }
  }

  int getGameState()
  {
    return GameState;
  }
  void setGameState(int state)
  {
    GameState = state;
  }
  void displayStartMenu()
  {

    arduboy.setCursor(30, initialpos + 2 + menuselection);
    arduboy.print("->");
    arduboy.setCursor(45, initialpos + 12);
    arduboy.print("1 Player");
    arduboy.setCursor(45, initialpos + 22);
    arduboy.print("2 Player");
    arduboy.setCursor(45, initialpos + 32);
    arduboy.print("Options");
  }
  void displayScore()
  {
    arduboy.setCursor(50, 2);
    arduboy.print(scores[0]);
    arduboy.print(" | ");
    arduboy.print(scores[1]);
  }
  void displayWinner()
  {

    arduboy.setCursor(35, 22);
    if (scores[0] == 10)
    {
      arduboy.print("Player1");
    }
    else
    {
      arduboy.print("Player2");
    }
    arduboy.setCursor(55, 32);
    arduboy.print("IS");
    arduboy.setCursor(45, 42);
    arduboy.print("THE WINNER");
  }
  void displayOptions()
  {

    arduboy.setCursor(20, initialpos + 2 + menuselection);
    arduboy.print("->");
    arduboy.setCursor(35, initialpos + 12);
    arduboy.print("Ball/Pdle Spd:");
    arduboy.print(ball->ballspeed);
    arduboy.setCursor(35, initialpos + 22);
    arduboy.print("Back");
  }
  Arduboy2 arduboy;
  Ball *ball;
  uint8_t initialpos = 70;
  uint8_t GameState = 0;
  uint8_t scores[2];
  uint8_t menuselection = 10;
};

#endif //BALL_H
