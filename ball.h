#ifndef BALL_H
#define BALL_H

#include <Sprites.h>

const unsigned char PROGMEM ballsprite[] = {
// width, height,
2, 2,
0x00, 0x03,0x00, 0x03
};

enum Direction {up, down, left, right, upright = 4, upleft = 5, downright = 6, downleft = 7};


class Ball {
  public:
  Ball() {
   
  }
  Ball(int x, int y) {
    setPosX(x);
    setPosY(y);
    collider.x = x;
    collider.y = y;
    collider.width = 2;
    collider.height = 2;
    Sprites::drawOverwrite(x,y,ballsprite,0);

  }

  int getPosX()
  {
    return x;
  }
  int getPosY()
  {
    return y;
  }
  void setPosX(int x)
  {
    this->x = x;
    collider.x = x;
  }
  void setPosY(int y)
  {
    this->y = y;
    collider.y = y;
  }
 
   void updatePos(uint8_t gameState) {
    float curx = getPosX();
    float cury = getPosY();

    
    // if the game is being played
    if(gameState == 1){
      switch (direction) {
        case left:
          curx-=ballspeed;
          break;
        case upleft:
          curx-=ballspeed;
          cury-=ballspeed;
          break;
        case right:
          curx+=ballspeed;
          break;
        case upright:
          curx+=ballspeed;
          cury-=ballspeed;
          break;
        case up:
          cury-=ballspeed;
          break;
        case down:
          cury+=ballspeed;
          break;
        case downleft:
          curx-=ballspeed;
          cury+=ballspeed;
          break;
        case downright:
          curx+=ballspeed;
          cury+=ballspeed;
          break;
      }
    }
    // change the x and y of the ball
    setPosX(curx);
    setPosY(cury);
    // redraw ball at new coordinates
    Sprites::drawOverwrite(curx,cury,ballsprite,0);

  }
  void resetPos() {
    this->setPosX(63);
    this->setPosY(31);
  }
  int getHeight() {return height;}
  int getWidth() {return width;}  


 
  Rect collider;
  
  Direction direction;

  uint8_t ballspeed = 3;
  int x = 0;
  uint8_t y = 8;
  uint8_t width = 2;
  uint8_t height = 2;
};

#endif //BALL_H
