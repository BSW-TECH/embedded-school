/**
 * @file tetris.c
 *
 * @brief tetris game for stm32 game console
 *
 * @par
 * COPYRIGHT NOTICE: 2022 BSW TECH
 */

/*************************************TETRIS****************************************************************/
#include "tetris.h"

extern button BTNUP, BTNDOWN, BTNLEFT, BTNRIGHT;

// Playing field
char Table[ROWS][COLS] =
{ 0 };
// Player score counter
int T_score = 0;
// Game state flag FALSE->Game Over TRUE->Current playing
char GameOn = TRUE;
// Falling time multiplexer the more points the player has, the more difficult the game (the faster the pieces fall)
double Tmultiplex = 0;

Shape current;

/***********Tile Shapes***********/
const Shape ShapesArray[7] =
{
{ (char*[]
		)
		{ (char[]
				)
				{ 0, 1, 1 }, (char[]
						)
						{ 1, 1, 0 }, (char[]
								)
								{ 0, 0, 0 } }, 3 },                    //S_shape
		{ (char*[]
				)
				{ (char[]
						)
						{ 1, 1, 0 }, (char[]
								)
								{ 0, 1, 1 }, (char[]
										)
										{ 0, 0, 0 } }, 3 },            //Z_shape
		{ (char*[]
				)
				{ (char[]
						)
						{ 0, 1, 0 }, (char[]
								)
								{ 1, 1, 1 }, (char[]
										)
										{ 0, 0, 0 } }, 3 },            //T_shape
		{ (char*[]
				)
				{ (char[]
						)
						{ 0, 0, 1 }, (char[]
								)
								{ 1, 1, 1 }, (char[]
										)
										{ 0, 0, 0 } }, 3 },            //L_shape
		{ (char*[]
				)
				{ (char[]
						)
						{ 1, 0, 0 }, (char[]
								)
								{ 1, 1, 1 }, (char[]
										)
										{ 0, 0, 0 } }, 3 },           //ML_shape
		{ (char*[]
				)
				{ (char[]
						)
						{ 1, 1 }, (char[]
								)
								{ 1, 1 } }, 2 },					//SQ_shape
		{ (char*[]
				)
				{ (char[]
						)
						{ 0, 0, 0, 0 }, (char[]
								)
								{ 1, 1, 1, 1 }, (char[]
										)
										{ 0, 0, 0, 0 }, (char[]
												)
												{ 0, 0, 0, 0 } }, 4 } //R_shape
};

/**
 * @brief  Shape copy function
 *
 * @param  Shape you want to copy
 *
 * @return Shape
 */
Shape CopyShape(Shape shape)
{
	Shape new_shape = shape;
	char **copyshape = shape.array;
	new_shape.array = (char**) malloc(new_shape.width * sizeof(char*));
	int i, j;
	for (i = 0; i < new_shape.width; i++)
	{
		new_shape.array[i] = (char*) malloc(new_shape.width * sizeof(char));
		for (j = 0; j < new_shape.width; j++)
		{
			new_shape.array[i][j] = copyshape[i][j];
		}
	}
	return new_shape;
}

/**
 * @brief  Shape delete function
 *
 * @param  Shape you want to delete
 *
 * @return
 */
void DeleteShape(Shape shape)
{
	int i;
	for (i = 0; i < shape.width; i++)
	{
		free(shape.array[i]);
	}
	free(shape.array);
}

/**
 * @brief  Position checking function
 *
 * @param  Shape wich position you want to get
 *
 * @return Shape position
 */
uint8_t CheckPosition(Shape shape)
{ //Check the position of the copied shape
	char **array = shape.array;
	int i, j;
	for (i = 0; i < shape.width; i++)
	{
		for (j = 0; j < shape.width; j++)
		{
			if ((shape.col + j < 0 || shape.col + j >= COLS
					|| shape.row + i >= ROWS))
			{ //Out of borders
				if (array[i][j]) //but is it just a phantom?
					return FALSE;
			}
			else if (Table[shape.row + i][shape.col + j] && array[i][j])
				return FALSE;
		}
	}
	return TRUE;
}

/**
 * @brief  Random shape get function
 *
 *
 * @return
 */
void GetNewShape()
{ //returns random shape
	Shape new_shape = CopyShape(ShapesArray[rand() % 7]);

	new_shape.col = rand() % (COLS - new_shape.width + 1);
	new_shape.row = 0;
	DeleteShape(current);
	current = new_shape;
	if (!CheckPosition(current))
	{
		GameOn = FALSE;
	}
}

/**
 * @brief  Shape rotation function
 *
 * @param  Shape you want to rotate
 *
 * @return
 */
void RotateShape(Shape shape)
{ //rotates clockwise
	Shape temp = CopyShape(shape);
	int i, j, k, width;
	width = shape.width;
	for (i = 0; i < width; i++)
	{
		for (j = 0, k = width - 1; j < width; j++, k--)
		{
			shape.array[i][j] = temp.array[k][i];
		}
	}
	DeleteShape(temp);
}

/**
 * @brief  Update table with new shape positions
 *
 *
 * @return
 */
void WriteToTable()
{
	uint8_t i, j;
	for (i = 0; i < current.width; i++)
	{
		for (j = 0; j < current.width; j++)
		{
			if (current.array[i][j])
				Table[current.row + i][current.col + j] = current.array[i][j];
		}
	}
}

/**
 * @brief  Full line checking function and score updating
 *
 *
 * @return
 */
void CheckFullRow()
{ //checks lines
	int i, j, sum, count = 0;
	for (i = 0; i < ROWS; i++)
	{
		sum = 0;
		for (j = 0; j < COLS; j++)
		{
			sum += Table[i][j];
		}
		if (sum == COLS)
		{
			count++;
			int l, k;
			for (k = i; k >= 1; k--)
				for (l = 0; l < COLS; l++)
					Table[k][l] = Table[k - 1][l];
			for (l = 0; l < COLS; l++)
				Table[k][l] = 0;
		}
	}
	Tmultiplex += 10;
	T_score += 100 * count;
}

/**
 * @brief  Table printing function
 *
 *
 * @return
 */
void PrintTable()
{
	char Buffer[ROWS][COLS] =
	{ 0 };
	int i, j;
	for (i = 0; i < current.width; i++)
	{
		for (j = 0; j < current.width; j++)
		{
			if (current.array[i][j])
				Buffer[current.row + i][current.col + j] = current.array[i][j];
		}
	}
	//clear();
	ssd1306_Fill(Black);
	ssd1306_Line(11, 0, 11, 63, White); // координаты замени
	ssd1306_Line(74, 0, 74, 63, White);
	ssd1306_SetCursor(77, 12);
	ssd1306_WriteString("Tetris", Font_6x8, Black);
	ssd1306_SetCursor(77, 23);
	char str[16] =
	{ 0 };
	sprintf(str, "%d", T_score);
	ssd1306_WriteString("score:", Font_6x8, Black);
	ssd1306_WriteString(str, Font_6x8, Black);
	uint8_t cursor = 0;
	for (i = 0; i < ROWS; i++)
	{
		ssd1306_SetCursor(13, cursor);
		for (j = 0; j < COLS; j++)
		{
			//printw("%c ", (Table[i][j] + Buffer[i][j])? 'O': '.');
			ssd1306_WriteChar((Table[i][j] + Buffer[i][j]) ? '.' : ' ',
					Font_6x8, Black);
		}
		//printw("\n");
		cursor += 8;
	}
	ssd1306_UpdateScreen();
	//printw("\nScore: %d\n", score);
}

/**
 * @brief  Change position function
 *
 * @param Action you want to perform
 *
 * @return
 */
void ManipulateCurrent(char action)
{
	Shape temp = CopyShape(current);
	switch (action)
	{
	case 's':
		temp.row++;  //move down
		if (CheckPosition(temp))
			current.row++;
		else
		{
			WriteToTable();
			CheckFullRow(); //check full lines, after putting it down
			GetNewShape();
		}
		break;
	case 'd':
		temp.col++;  //move right
		if (CheckPosition(temp))
			current.col++;
		break;
	case 'a':
		temp.col--;  //move left
		if (CheckPosition(temp))
			current.col--;
		break;
	case 'w':
		RotateShape(temp);  //yes
		if (CheckPosition(temp))
			RotateShape(current);
		break;
	}
	DeleteShape(temp);
	PrintTable();
}

void Tetris_GameLoop()
{
	T_score = 0;
	uint32_t timer = 0;
	//gettimeofday(&before, NULL);
	//nodelay(stdscr, TRUE);
	GetNewShape();
	PrintTable();
	while (GameOn)
	{
		/*if ((c = getch()) != ERR) {
		 ManipulateCurrent(c);
		 }Оработка кнопок*/
		if (Button_Check(BTNRIGHT) && Button_Check(BTNLEFT))
		{
			return;
		}
		if (Button_Check(BTNUP))
		{
			ManipulateCurrent(ACTION_UP);
		}
		else if (Button_Check(BTNRIGHT))
		{
			ManipulateCurrent(ACTION_RIGHT);
		}
		else if (Button_Check(BTNLEFT))
		{
			ManipulateCurrent(ACTION_LEFT);
		}

		if (HAL_GetTick() - timer > FALLINGTIME - Tmultiplex)
		{ //time difference
			timer = HAL_GetTick();
			ManipulateCurrent('s');
		}

		srand(timer);
	}
	// printw("\nGame over\n");
	DeleteShape(current);
	ssd1306_SetCursor(10, 10);
	ssd1306_WriteString("Game Over", Font_7x10, Black);
	TIM2->CCR4 = 15000;
	HAL_Delay(200);
	TIM2->CCR4 = 0;
	ssd1306_UpdateScreen();
	while (!GameOn)
	{
		ssd1306_Fill(Black);
		ssd1306_SetCursor(10, 10);
		ssd1306_WriteString("Play Again?", Font_7x10, Black);
		ssd1306_SetCursor(7, 30);
		ssd1306_WriteString("Press any button", Font_7x10, Black);
		ssd1306_UpdateScreen();
		Button_Check(BTNUP);
		if (Button_Check(BTNUP) || Button_Check(BTNRIGHT))
		{
			GameOn = TRUE;
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLS; j++)
				{
					Table[i][j] = 0;
				}
			}
		}
		else if (Button_Check(BTNRIGHT) && Button_Check(BTNLEFT))
		{
			return;
		}
	}
}
/*** end of file ***/
