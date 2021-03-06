#include <Arduboy2.h>
#include <Sprites.h>
#include <ArduboyTones.h>
#include "Player.h"
#include "Ball.h"
#include "Stage.h"


void detectCollision(Rect ball, Rect player1, Rect player2);
uint8_t priorstate;
uint8_t paused = 0;
uint8_t paddlespeed = 3;
const uint8_t SCREENX = 128;
const uint8_t SCREENY = 64;
const uint8_t upperbound = 0;
const uint8_t lowerbound = 52;
Rect btmcollider(0, SCREENY, 900, 8);
Rect topcollider(0, -6, 900, 8);
void controls();

Sprites sprites;

Player player1(0, 4, 8);
Player player2(SCREENX - 4, SCREENY - 12, -4);
Ball daball((SCREENX / 2 - 1), (SCREENY / 2 - 1));
Ball *ballptr = &daball;
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Stage stage(arduboy, ballptr);

const uint16_t bouncesfx[] PROGMEM = {
    220, 100,
    TONES_END};
const uint16_t pausesfx[] PROGMEM = {
    500, 100, 220, 100, 500, 100,
    TONES_END};
const uint16_t winsfx[] PROGMEM = {
    220, 1000, 0, 250, 440, 500, 880, 2000,
    TONES_END};
const uint16_t scoresfx[] PROGMEM = {
    500, 100,
    TONES_END};
const uint16_t menusfx[] PROGMEM = {
    600, 100,
    TONES_END};
const uint16_t selectsfx[] PROGMEM = {
    700, 100,
    TONES_END};

void setup()
{
  arduboy.begin();
  arduboy.setFrameRate(30);
}

void loop()
{
  if (!(arduboy.nextFrame()))
    return;

  arduboy.pollButtons();
  controls();

  if (paused == 0)
  {

    arduboy.clear();
  if(stage.getGameState() == 3) {
      if(arduboy.everyXFrames(daball.cpulevel)){
      if(player2.getPosY() > daball.getPosY()) {
          player2.y-=(paddlespeed*2);
        }  else 
        player2.y+=(paddlespeed*2);
      }
    }
    player1.updatePos();
    player2.updatePos();
    //had to increase size for faster speeds player1 is is drawn off -4 offscreen so collider can be 12 and not 4
    arduboy.fillRect(player1.getPosX(), player1.getPosY(), 4, 9, WHITE);
    arduboy.fillRect(player2.getPosX(), player2.getPosY(), 4, 9, WHITE);
    daball.updatePos(stage.getGameState());
    
  
    
    stage.updateState();

    detectCollision(daball.collider, player1.collider, player2.collider);

    arduboy.drawFastHLine(0, SCREENY - 1, 900, WHITE);
    arduboy.drawFastHLine(0, 0, 900, WHITE);

    if (updateScore() == 10)
    {
      sound.tones(winsfx);
      stage.displayScore();
      stage.displayWinner();
      arduboy.display();
      delay(5000);
      reset();
      stage.setGameState(0);
      stage.menuselection = 10;
    }

    arduboy.display();
  }
  else
  {
    arduboy.clear();
    arduboy.setCursor(30, 12 + stage.menuselection);
    arduboy.print("->");
    arduboy.setCursor(48, 12);
    arduboy.print("Paused");
    arduboy.setCursor(45, 22);
    arduboy.print("Unpause");
    arduboy.setCursor(45, 32);
    arduboy.print("Main Menu");
    arduboy.display();
  }
}

void controls()
{

  if (arduboy.justPressed(DOWN_BUTTON) && (stage.getGameState() == 0 | stage.getGameState() == 4))
  {
    sound.tones(menusfx);
    
    if (stage.menuselection < 30 && stage.getGameState() == 0)
      stage.menuselection += 10;
    else if (stage.menuselection < 20 && stage.getGameState() == 4) {
      stage.menuselection += 10;
      }
  }
  if (arduboy.justPressed(UP_BUTTON) && (stage.getGameState() == 0 | stage.getGameState() == 4))
  {
    sound.tones(menusfx);
    //&& stage.getGameState() == 0
    if (stage.menuselection > 10 )  {
      stage.menuselection -= 10;
    }
      //else if (stage.menuselection < 20 && stage.getGameState() == 4) {
      //stage.menuselection -= 10;
      //}
  }
  if (arduboy.justPressed(A_BUTTON) && stage.getGameState() == 0)
  {
    switch (stage.menuselection)
    {
    case 10: //1player
    stage.setGameState(3);
      arduboy.initRandomSeed();
      daball.direction = (Direction)random(4, 7);
      break;
    case 20: //2player
      stage.setGameState(1);
      arduboy.initRandomSeed();
      daball.direction = (Direction)random(4, 7);
      break;
    case 30: //options
      stage.menuselection = 10;
      stage.setGameState(2);

      break;
    }
  }
  if (arduboy.justPressed(LEFT_BUTTON) & (stage.getGameState() == 1 | stage.getGameState() == 3 | stage.getGameState() == 4))
  {
    // if not paused pause it
    if (paused == 0)
    {
      sound.tones(pausesfx);
      delay(300);
      sound.noTone();

      // get the priorstate so can go back to it after unpausing
      priorstate = stage.getGameState();
      stage.menuselection = 10;
      stage.setGameState(4);
      paused = 1;
    }
    // unpause
    else
    {
      stage.setGameState(priorstate);
      paused = 0;
    }
  }
  if (arduboy.justPressed(RIGHT_BUTTON) && stage.getGameState() == 1)
  {
    reset();
  }
  //Pressed A while in options menu
  if (arduboy.justPressed(A_BUTTON) && stage.getGameState() == 2)
  {
    sound.tones(selectsfx);
    switch (stage.menuselection)
    {
    case 10: //ballspeed
      break;
     case 20:
     break; //cpulevel
    case 30: //back
      stage.menuselection = 10;
      stage.setGameState(0);
    }
  }
  // Pressed A int he pause menu
  if (arduboy.justPressed(A_BUTTON) && stage.getGameState() == 4)
  {
    sound.tones(selectsfx);
    switch (stage.menuselection)
    {
    case 10: //unpause
      stage.setGameState(priorstate);
      paused = 0;
      break;
    case 20: //main menu
      paused = 0;
      stage.setGameState(0);
      stage.menuselection = 10;
      reset();
      break;
    }
  }
  // Pressing down while in options menu
  if (arduboy.justPressed(DOWN_BUTTON) && stage.getGameState() == 2)
  {
    sound.tones(menusfx);
    if (stage.menuselection < 30)
      stage.menuselection += 10;
  }
  // Pressing up on the options menu
  if (arduboy.justPressed(UP_BUTTON) && stage.getGameState() == 2)
  {
    sound.tones(menusfx);
    // menuselection controls what will be selected first option == 10 no wrap
    if (stage.menuselection > 10)
      stage.menuselection -= 10;
  }
  // Pressing left on the options screen
  if (arduboy.justPressed(LEFT_BUTTON) && stage.getGameState() == 2)
  {
    sound.tones(menusfx);
    switch (stage.menuselection)
    {
      // Lower ballspeed 2 being min
      case 10: //ballspeed
        if (daball.ballspeed > 2) {
          daball.ballspeed -= 1;
          paddlespeed -= 1;
        }
        break;
      // Lower CPU level
      case 20:
        if(daball.cpulevel < 4)
          daball.cpulevel+=1;
        break;
      // back
      case 30:
        stage.menuselection = 10;
        stage.setGameState(0);
    }
  }
  // Pressed right and in the options menu
  if (arduboy.justPressed(RIGHT_BUTTON) && stage.getGameState() == 2)
  {
    sound.tones(menusfx);
    switch (stage.menuselection)
    {
      // Change Ballspeed max 5
      case 10: //ballspeed
      if(daball.ballspeed < 5) {
        daball.ballspeed += 1;
        paddlespeed += 1;
      }
      break;
      // Chane CPU difficulty
      case 20:
      if(daball.cpulevel > 2)
        daball.cpulevel-=1;
      break;
      // Go back
      case 30:
        stage.menuselection = 10;
        stage.setGameState(0);
    }
    
  }
  // Pressed B while playing 2 player
  if (arduboy.pressed(B_BUTTON) && stage.getGameState() == 1)
  {
    int tmp = player2.getPosY();
    tmp -= paddlespeed;
    player2.setPosY(tmp);
    if (Arduboy2Base::collide(topcollider, player2.collider) | player2.getPosY() < upperbound)
    {
      tmp += paddlespeed;
      player2.setPosY(tmp);
    }
  }
  // Pressed A while playing 2 player 
  if (arduboy.pressed(A_BUTTON) && stage.getGameState() == 1)
  {
    int tmp = player2.getPosY();
    tmp += paddlespeed;
    player2.setPosY(tmp);
    if (Arduboy2Base::collide(btmcollider, player2.collider) | player2.getPosY() > lowerbound)
    {
      tmp -= paddlespeed;
      player2.setPosY(tmp);
    }
  }
  // Move player 1 up
  if (arduboy.pressed(UP_BUTTON))
  {
    int tmp = player1.getPosY();
    tmp -= paddlespeed;
    player1.setPosY(tmp);
    if (Arduboy2Base::collide(topcollider, player1.collider) | player1.getPosY() < upperbound)
    {
      tmp += paddlespeed;
      player1.setPosY(tmp);
    }
  }
  // Move player 1 down
  if (arduboy.pressed(DOWN_BUTTON))
  {
    int tmp = player1.getPosY();
    tmp += paddlespeed;
    player1.setPosY(tmp);
    if (Arduboy2Base::collide(btmcollider, player1.collider) | player1.getPosY() > lowerbound)
    {
      tmp -= paddlespeed;
      player1.setPosY(tmp);
    }
  }
}
// detect collisions between the ball, players, and the uppser / lower boundaries
void detectCollision(Rect ball, Rect player1, Rect player2)
{

  if (Arduboy2Base::collide(ball, btmcollider))
  {
    bounce(0);
  }
  else if (Arduboy2Base::collide(ball, player1))
  {
    bounce(1);
  }
  else if (Arduboy2Base::collide(ball, topcollider))
  {
    bounce(2);
  }
  else if (Arduboy2Base::collide(ball, player2))
  {
    bounce(3);
  }
}
//bounces in different direction depending on who the ball hit
void bounce(uint8_t who)
{
  sound.tones(bouncesfx);

  if (who == 0)
  {
    switch (daball.direction)
    {
    case downright:
      daball.direction = upright;
      break;
    case downleft:
      daball.direction = upleft;
      break;
    }
  }
  else if (who == 1)
  {
    switch (daball.direction)
    {
    case upleft:
      daball.direction = upright;
      break;
    case downleft:
      daball.direction = downright;
      break;
    }
  }
  else if (who == 2)
  {
    switch (daball.direction)
    {
    case upleft:
      daball.direction = downleft;
      break;
    case upright:
      daball.direction = downright;
      break;
    }
  }
  else
  {
    switch (daball.direction)
    {
    case downright:
      daball.direction = downleft;
      break;
    case upright:
      daball.direction = upleft;
      break;
    }
  }
}

uint8_t updateScore()
{

  if (daball.x >= 128)
  {
    stage.scores[0] += 1;
    daball.resetPos();
    sound.tones(scoresfx);
  }
  if (daball.x <= 0)
  {
    stage.scores[1] += 1;
    daball.resetPos();
    sound.tones(scoresfx);
  }
  if (stage.scores[0] > stage.scores[1])
  {
    return stage.scores[0];
  }
  else
    return stage.scores[1];
}

void reset()
{
  stage.scores[0] = 0;
  stage.scores[1] = 0;
  daball.resetPos();
}
