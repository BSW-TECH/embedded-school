/**
* @file SpaceInvaders.h
*
* @brief SpaceInvaders game header file
*
* @par
* COPYRIGHT NOTICE: 2022 BSW TECH
*/
#ifndef INC_SPACEINVADERS_H_
#define INC_SPACEINVADERS_H_


#include "main.h"
// DISPLAY SETTINGS


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Input settings
#define FIRE_BUT 7
#define RIGHT_BUT A1
#define LEFT_BUT A2

// Alien Settings
#define NUM_ALIEN_COLUMNS 7
#define NUM_ALIEN_ROWS 3
#define X_START_OFFSET 6
#define SPACE_BETWEEN_ALIEN_COLUMNS 3
#define LARGEST_ALIEN_WIDTH 11
#define SPACE_BETWEEN_ROWS 9
#define INVADERS_DROP_BY 4            // pixel amount that invaders move down by
#define INVADERS_SPEED 20             // speed of movement, lower=faster.
#define INVADER_HEIGHT 8

// Player settingsc
#define TANKGFX_WIDTH 16
#define TANKGFX_HEIGHT 8
#define PLAYER_X_MOVE_AMOUNT 1
#define PLAYER_Y_START 56
#define PLAYER_X_START 0

#define MISSILE_HEIGHT 4
#define MISSILE_WIDTH 1
#define MISSILE_SPEED 1

// Status of a game object constants
#define ACTIVE 0
#define DESTROYED 2




// Game structures

typedef struct   {
  // base object which most other objects will include
  signed int X;
  signed int Y;
  unsigned char Status;  //0 active, 1 exploding, 2 destroyed
}GameObjectStruct;

typedef struct   {
  GameObjectStruct Ord;
}AlienStruct;

typedef struct   {
  GameObjectStruct Ord;
}PlayerStruct;

void SpaceInvaders_GameLoop();

#endif /* INC_SPACEINVADERS_H_ */
/*** end of file ***/
