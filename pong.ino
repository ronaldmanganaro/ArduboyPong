#include <Arduboy2.h>
#include <Sprites.h>
#include <ArduboyTones.h>
#include "Player.h"
#include "Ball.h"
#include "Stage.h"

uint8_t paused = 0;
uint8_t paddlespeed = 2;
const uint8_t SCREENX = 128;
const uint8_t SCREENY = 64;
const uint8_t upperbound = 0;
const uint8_t lowerbound = 52;
Rect btmcollider(0,SCREENY,900,8);
Rect topcollider(0,-6,900,8);
void controls();

Sprites sprites;

Player player1(0,4,8);
Player player2(SCREENX-4,SCREENY-12, -4);
Ball daball((SCREENX/2 - 1), (SCREENY/2 - 1));
Ball *ballptr = &daball;
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Stage stage(arduboy, ballptr);

const uint16_t bouncesfx[] PROGMEM = {
  220,100,
  TONES_END };
const uint16_t pausesfx[] PROGMEM = {
  500,100, 220,100, 500,100,
  TONES_END };
const uint16_t winsfx[] PROGMEM = {
  220,1000, 0,250, 440,500, 880,2000,
  TONES_END };
const uint16_t scoresfx[] PROGMEM = {
  500,100,
  TONES_END };
void setup() {
  arduboy.begin();
  arduboy.setFrameRate(15);

}

void loop() {
  if (!(arduboy.nextFrame()))
    return;


  arduboy.pollButtons();
  controls();

  if (paused == 0) {

  arduboy.clear();
  
  player1.updatePos();
  player2.updatePos();
  //had to increase size for faster speeds player1 is is drawn off -4 offscreen so collider can be 12 and not 4
  arduboy.fillRect(player1.getPosX(), player1.getPosY(), 4, 9, WHITE);
  arduboy.fillRect(player2.getPosX(), player2.getPosY(), 4, 9, WHITE);

  daball.updatePos(stage.getGameState());
  stage.updateState();
  
  detectCollision(daball.collider, player1.collider, player2.collider);
  
  arduboy.drawFastHLine(0,SCREENY-1,900, WHITE);
  arduboy.drawFastHLine(0,0,900, WHITE);
  
  if (updateScore() == 10) {
        sound.tones(winsfx);

    stage.displayScore();
    stage.displayWinner();
    arduboy.display();
    delay(5000);
    reset();  
  }
  
  arduboy.display();
  } else  {
    arduboy.setCursor(50, 32);
          arduboy.print("Paused");
          arduboy.display();
    }
}

void controls ()
{
 
  if(arduboy.justPressed(DOWN_BUTTON) && stage.getGameState() == 0){
    if(stage.menuselection < 30)
      stage.menuselection+=10;
  }
   if(arduboy.justPressed(UP_BUTTON) && stage.getGameState() == 0){
    if(stage.menuselection > 10)
      stage.menuselection-=10;
  }
  if(arduboy.justPressed(A_BUTTON) && stage.getGameState() == 0) {
    switch (stage.menuselection) {
      case 10://1player
      
      break;
      case 20: //2player
        stage.setGameState(1);
        arduboy.initRandomSeed();
        daball.direction = (Direction)random(4,7);
      break;
      case 30: //options
      stage.menuselection = 10;
      stage.setGameState(2);
      
      break;
    }
    
  }
  if(arduboy.justPressed(LEFT_BUTTON) & stage.getGameState() == 1){
        if(paused == 0) {
          paused = 1;
          sound.tones(pausesfx);
          delay(300);
        sound.noTone();
        } else {
          paused = 0;
        }  
        
  }
  if(arduboy.justPressed(RIGHT_BUTTON) && stage.getGameState() == 1) {
    reset();
  }
  if(arduboy.justPressed(A_BUTTON) && stage.getGameState() == 2) {
      switch (stage.menuselection) {
      case 10://ballspeed
        //daball.ballspeed = ;
      break;
      case 20: //back
        stage.setGameState(0);
        
      break;
      /*case 30: //options
      Serial.println("display option menu");
      stage.menuselection = 10;
      stage.setGameState(2);
      
      break;*/
    }
  }
  if(arduboy.justPressed(DOWN_BUTTON) && stage.getGameState() == 2){
    if(stage.menuselection < 20)
        stage.menuselection+=10;
  }
  if(arduboy.justPressed(UP_BUTTON) && stage.getGameState() == 2){
    if(stage.menuselection > 10)
        stage.menuselection-=10;
  }
  if(arduboy.justPressed(LEFT_BUTTON) && stage.getGameState() == 2){
      daball.ballspeed-=1;
      paddlespeed-=1;
  }
  if(arduboy.justPressed(RIGHT_BUTTON) && stage.getGameState() == 2){
    daball.ballspeed+=1;
          paddlespeed+=1;

  }
  if (arduboy.pressed(B_BUTTON)) {
      int tmp = player2.getPosY();
      tmp-=paddlespeed;
       player2.setPosY(tmp);
       //
      if(Arduboy2Base::collide(topcollider, player2.collider) | player2.getPosY() < upperbound   ){
       tmp+=paddlespeed;
        player2.setPosY(tmp);
      } 
  }
  if (arduboy.pressed(A_BUTTON)) {
      int tmp = player2.getPosY();
      tmp+=paddlespeed;
       player2.setPosY(tmp);
       // | player1.getPosY() > lowerbound
       if(Arduboy2Base::collide(btmcollider, player2.collider) | player2.getPosY() > lowerbound  ){ 
         tmp-=paddlespeed;
        player2.setPosY(tmp);
        }
  }
  if (arduboy.pressed(UP_BUTTON)) {
      int tmp = player1.getPosY();
      tmp-=paddlespeed;
       player1.setPosY(tmp);
       //
      if(Arduboy2Base::collide(topcollider, player1.collider) | player1.getPosY() < upperbound   ){
       tmp+=paddlespeed;
        player1.setPosY(tmp);
      } 
       
      
  }
  if (arduboy.pressed(DOWN_BUTTON)) {
      int tmp = player1.getPosY();
      tmp+=paddlespeed;
       player1.setPosY(tmp);
       // | player1.getPosY() > lowerbound
       if(Arduboy2Base::collide(btmcollider, player1.collider) | player1.getPosY() > lowerbound  ){ 
         tmp-=paddlespeed;
        player1.setPosY(tmp);
        }
      }
  }


void detectCollision(Rect ball, Rect player1, Rect player2) {

  if(Arduboy2Base::collide(ball, btmcollider)) {
    bounce(0);
  } else if(Arduboy2Base::collide(ball, player1)) {
    bounce(1);
  } else if(Arduboy2Base::collide(ball, topcollider)) {
    bounce(2);
  } else if(Arduboy2Base::collide(ball, player2)) {
    bounce(3);
  }
}

void bounce(uint8_t who) {
    sound.tones(bouncesfx);


  if (who == 0) {
    switch (daball.direction) {
      case downright:
        daball.direction = upright;
        break;
      case downleft:
        daball.direction = upleft;
        break;
      }
  } else if (who == 1) {
    switch (daball.direction) {
      case upleft:
        daball.direction = upright;
        break;
      case downleft:
        daball.direction = downright;
        break;
      }
  } else if (who == 2) {
    switch (daball.direction) {
      case upleft:
        daball.direction = downleft;
        break;
      case upright:
        daball.direction = downright;
        break;
      }
  } else {
        switch (daball.direction) {
          case downright:
          daball.direction = downleft;
          break;
      case upright:
        daball.direction = upleft;
        break;
   }
  }
}

uint8_t updateScore() {
  
  if(daball.x >= 128) {
    stage.scores[0]+=1;
    daball.resetPos();
 sound.tones(scoresfx);

  } 
  if(daball.x <= 0) {
    stage.scores[1]+=1;
    //Serial.println("p2scored");
    //Serial.println(daball.x);
    daball.resetPos();
 sound.tones(scoresfx);
  }
  if(stage.scores[0] > stage.scores[1]) {
    return stage.scores[0];
    } else return stage.scores[1];
}

void reset() {
  stage.scores[0] = 0;
  stage.scores[1] = 0;
  daball.resetPos();

}
