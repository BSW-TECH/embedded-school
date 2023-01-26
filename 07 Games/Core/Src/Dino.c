/*
 * Dino.c
 *
 *  Created on: Jan 4, 2023
 *      Author: artur
 */
#include "Dino.h"
extern button BTNUP, BTNLEFT, BTNRIGHT;

static const unsigned char dino1[] =
{
		// 'dino', 25x26px
		0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x06, 0xff, 0x00,
		0x00, 0x0e, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00,
		0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x0f, 0xfc, 0x00,
		0x40, 0x0f, 0xc0, 0x00, 0x40, 0x1f, 0x80, 0x00, 0x40, 0x7f, 0x80, 0x00,
		0x60, 0xff, 0xe0, 0x00, 0x71, 0xff, 0xa0, 0x00, 0x7f, 0xff, 0x80, 0x00,
		0x7f, 0xff, 0x80, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x00, 0x00,
		0x1f, 0xff, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00,
		0x01, 0xdc, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00,
		0x01, 0x0c, 0x00, 0x00, 0x01, 0x8e, 0x00, 0x00 };

static const unsigned char tree1[] =
{
		// 'tree1', 11x23px
		0x1e, 0x00, 0x1f, 0x00, 0x1f, 0x40, 0x1f, 0xe0, 0x1f, 0xe0, 0xdf, 0xe0,
		0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0,
		0xff, 0xc0, 0xff, 0x00, 0xff, 0x00, 0x7f, 0x00, 0x1f, 0x00, 0x1f, 0x00,
		0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00 };

static const unsigned char tree2[] =
{
		// 'tree2', 22x23px
		0x1e, 0x01, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x4f, 0xe8, 0x1f, 0xff, 0xfc,
		0x1f, 0xff, 0xfc, 0xdf, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc,
		0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xef, 0xfc,
		0xff, 0x83, 0xfc, 0xff, 0x03, 0xfc, 0xff, 0x03, 0xf8, 0x7f, 0x03, 0xe0,
		0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0,
		0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0 };

// Move dino function
void moveDino(int16_t *y, int type)
{
	ssd1306_DrawBitmap(DINO_INIT_X, *y, dino1, DINO_WIDTH, DINO_HEIGHT, White);
}

// Move tree funciton
void moveTree(int16_t *x, int type)
{
	if (type == 0)
	{
		ssd1306_DrawBitmap(*x, TREE_Y, tree1, TREE1_WIDTH, TREE1_HEIGHT, White);
	}
	else if (type == 1)
	{
		ssd1306_DrawBitmap(*x, TREE_Y, tree2, TREE2_WIDTH, TREE2_HEIGHT, White);
	}

}

// Game over display with score
void gameOver(int score)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(10, 5);
	ssd1306_WriteString("Game Over", Font_6x8, White);
	ssd1306_SetCursor(10, 30);
	ssd1306_WriteString("Score: ", Font_6x8, White);
	char str[16] =
	{ 0 };
	sprintf(str, "%d", score);
	ssd1306_WriteString(str, Font_6x8, White);
	ssd1306_UpdateScreen();
	HAL_Delay(1000);
	return;
}

// Display score while running the game
void displayScore(int score)
{
	ssd1306_SetCursor(64, 10);
	ssd1306_WriteString("Score: ", Font_6x8, White);
	char str[16] =
	{ 0 };
	sprintf(str, "%d", score);
	ssd1306_WriteString(str, Font_6x8, White);
}


void renderScene(int16_t i)
{
	ssd1306_DrawBitmap(10, 29, dino1, 25, 26, White);
	ssd1306_DrawBitmap(50, TREE_Y, tree1, 11, 23, White);
	ssd1306_DrawBitmap(100, TREE_Y, tree2, 22, 23, White);
	ssd1306_Line(0, 54, 127, 54, White);
	ssd1306_DrawPixel(i, 60, White);
}

void DinoGameLoop()
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(20, 20);
	ssd1306_WriteString("DinoDev", Font_11x18, White);
	HAL_Delay(1000);
	ssd1306_UpdateScreen();
	while (1)
	{
		int16_t tree_x = 127;
		int16_t tree1_x = 195;
		int tree_type = rand() % 2;
		int tree_type1 = rand() % 2;

		int16_t dino_y = DINO_INIT_Y;
		//int input_command;
		int jump = 0;
		int score = 0;

		for (;;)
		{
			ssd1306_Fill(Black);
			if (Button_Check(BTNRIGHT) && Button_Check(BTNLEFT))
				{
					return;
				}
			if (Button_Check(BTNUP))
			{
				if (jump == 0)
					jump = 1;
			}
			if (jump == 1)
			{
				dino_y--;
				if (dino_y == (DINO_INIT_Y - JUMP_PIXEL))
				{
					jump = 2;
					score++;
				}
			}
			else if (jump == 2)
			{
				dino_y++;
				if (dino_y == DINO_INIT_Y)
				{
					jump = 0;
				}
			}

			if (tree_x <= (DINO_INIT_X + DINO_WIDTH)
					&& tree_x >= (DINO_INIT_X + (DINO_WIDTH / 2)))
			{
				if (dino_y >= (DINO_INIT_Y - 3))
				{
					// Collision Happened
					break;
				}
			}

			if (tree1_x <= (DINO_INIT_X + DINO_WIDTH)
					&& tree1_x >= (DINO_INIT_X + (DINO_WIDTH / 2)))
			{
				if (dino_y >= (DINO_INIT_Y - 3))
				{
					// Collision Happened
					break;
				}
			}

			displayScore(score);
			moveTree(&tree_x, tree_type);
			moveTree(&tree1_x, tree_type1);
			moveDino(&dino_y, 0);
			ssd1306_Line(0, 54, 127, 54, White);

			tree_x--;
			tree1_x--;
			if (tree_x == 0)
			{
				tree_x = 127;
				tree_type = rand() % 1;
			}

			if (tree1_x == 0)
			{
				tree1_x = 195;
				tree_type1 = rand() % 1;
			}
			ssd1306_UpdateScreen();
		}
		gameOver(score);
		ssd1306_UpdateScreen();
	}
}
