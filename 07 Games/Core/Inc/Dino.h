/*
 * Dino.h
 *
 *  Created on: Jan 4, 2023
 *      Author: artur
 */

#ifndef INC_DINO_H_
#define INC_DINO_H_

#include "main.h"
#include "ssd1306.h"
#include "button.h"

#define DINO_WIDTH 25
#define DINO_HEIGHT 26
#define DINO_INIT_X 10 // Dino initial spawn location
#define DINO_INIT_Y 29 // Dino initial spawn location

#define BASE_LINE_X 0
#define BASE_LINE_Y 54
#define BASE_LINE_X1 127
#define BASE_LINE_Y1 54


#define TREE1_WIDTH 11
#define TREE1_HEIGHT 23


#define TREE2_WIDTH 22
#define TREE2_HEIGHT 23

#define TREE_Y 35

#define JUMP_PIXEL 32 // Number of pixel dino will jump


void DinoGameLoop();
#endif /* INC_DINO_H_ */
