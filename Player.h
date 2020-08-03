#ifndef PLAYER_H
#define PLAYER_H

#include <Sprites.h>

const unsigned char PROGMEM player[] =
{
  // width, height,
  2, 16,
  0x00, 0xff, 0x00, 0xff,
  0x00, 0xff, 0x00, 0xff
};

class Player
{
public:
  Player() {}
  //offset added so i could increase collider size without it being bifger than the p1 paddkle
  Player(uint8_t x, uint8_t y, uint8_t offset)
  {
    setPosX(x);
    setPosY(y);
    collider.x = x - offset;
    collider.y = y;
    collider.width = 8;
    collider.height = 9;
    //collider.width = 2;
    //collider.height = 16;
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
  }
  void setPosY(int y)
  {
    this->y = y;
  }

  void setPosColliderX(int x)
  {
    collider.x = x;
  }
  void setPosColliderY(int y)
  {
    collider.y = y;
  }

  void updatePos()
  {
    
    
    uint8_t curx = getPosX();
    uint8_t cury = getPosY();

    setPosColliderX(curx);
    setPosColliderY(cury);
    //Sprites::drawOverwrite(curx,cury,player,0);
  }

  void enableAI() {
    }

  uint8_t getHeight() { return height; }
  uint8_t getWidth() { return width; }

  int x;
  int y;

  uint8_t upperbound = 0;
  uint8_t lowerbound = 48;

  Rect collider;

  uint8_t width = 2;
  uint8_t height = 16;
};

#endif //PLAYER_H
