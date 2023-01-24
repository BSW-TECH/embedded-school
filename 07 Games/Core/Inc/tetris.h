/**
* @file tetris.h
*
* @brief tetris game header file
*
* @par
* COPYRIGHT NOTICE: 2022 BSW TECH
*/
#ifndef _TETRIS_H_
#define _TETRIS_H_

#include "main.h"

/***********Structure for current shape***********/
typedef struct
{
	char** array;
	int width, row, col;
} Shape;




/***********Playing field sizes***********/
#define ROWS 8			// Height
#define COLS 10			// Width

#define FALLINGTIME 1000 // Current tile falling time(milliseconds)

/***********Actions that can be performed by player using buttons**********/
#define ACTION_UP 'w'
#define ACTION_DOWN 's'
#define ACTION_RIGHT 'a'
#define ACTION_LEFT 'd'


/**
  * @brief  Shape copy function
  *
  * @param  Shape you want to copy
  *
  * @return Shape
  */
Shape CopyShape(Shape shape);

/**
  * @brief  Shape delete function
  *
  * @param  Shape you want to delete
  *
  * @return
  */
void DeleteShape(Shape shape);

/**
  * @brief  Position checking function
  *
  * @param  Shape wich position you want to get
  *
  * @return Shape position
  */
uint8_t CheckPosition(Shape shape);

/**
  * @brief  Random shape get function
  *
  *
  * @return
  */
void GetNewShape();

/**
  * @brief  Shape rotation function
  *
  * @param  Shape you want to rotate
  *
  * @return
  */
void RotateShape(Shape shape);

/**
  * @brief  Update table with new shape positions
  *
  *
  * @return
  */
void WriteToTable();

/**
  * @brief  Full line checking function and score updating
  *
  *
  * @return
  */
void CheckFullRow();

/**
  * @brief  Table printing function
  *
  *
  * @return
  */
void PrintTable();

/**
  * @brief  Change position function
  *
  * @param Action you want to perform
  *
  * @return
  */
void ManipulateCurrent(char action);

/**
  * @brief  tetris main game loop
  *
  * @param
  *
  * @return
  */
void Tetris_GameLoop();

#endif /*_TETRIS_H_*/
/*** end of file ***/
