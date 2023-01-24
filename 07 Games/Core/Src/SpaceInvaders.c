#include "SpaceInvaders.h"

// graphics
// aliens

const char MotherShipGfx[][8] =
{
{ 0xfc, 0x3f, 0xb6, 0x6d, 0xff, 0xff, 0x9c, 0x39 },
{ 0xfc, 0x00, 0x4a, 0x01, 0xff, 0x03, 0xb5, 0x02 } };

const char InvaderTopGfx[][8] =
{
{ 0x18, 0x3c, 0x7e, 0xdb, 0xff, 0x24, 0x5a, 0xa5 },
{ 0x18, 0x3c, 0x7e, 0xdb, 0xff, 0x5a, 0x81, 0x42 } };

const char InvaderMiddleGfx[][16] =
{
{ 0x04, 0x01, 0x88, 0x00, 0xfc, 0x01, 0x76, 0x03, 0xff, 0x07, 0xfd, 0x05, 0x05,
		0x05, 0xd8, 0x00 },
{ 0x04, 0x01, 0x88, 0x00, 0xfd, 0x05, 0x75, 0x05, 0xff, 0x07, 0xfc, 0x01, 0x04,
		0x01, 0x02, 0x02 } };

const char InvaderBottomGfx[][16] =
{
{ 0xf0, 0x00, 0xfe, 0x07, 0xff, 0x0f, 0x67, 0x0e, 0xff, 0x0f, 0x9c, 0x03, 0x06,
		0x06, 0x0c, 0x03 },
{ 0xf0, 0x00, 0xfe, 0x07, 0xff, 0x0f, 0x67, 0x0e, 0xff, 0x0f, 0x9c, 0x03, 0x62,
		0x04, 0x01, 0x08 } };

const char TankGfx[] =
{ 0x40, 0x00, 0xe0, 0x00, 0xe0, 0x00, 0xfe, 0x0f, 0xff, 0x1f, 0xff, 0x1f, 0xff,
		0x1f, 0xff, 0x1f };

const char MissileGfx[] =
{ 0x01, 0x01, 0x01, 0x01 };

const char AlienBombGfx[] =
{ 0x01, 0x02, 0x01, 0x02 };

const char BaseGfx[] =
{ 0xf8, 0x1f, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xf8, 0x07,
		0xe0, 0x07, 0xe0 };

const char ExplosionGfx[] =
{ 0x10, 0x01, 0xa2, 0x08, 0x04, 0x04, 0x08, 0x02, 0x03, 0x18, 0x08, 0x02, 0xa4,
		0x04, 0x12, 0x09 };

//alien global vars
//The array of aliens across the screen
AlienStruct Alien[NUM_ALIEN_COLUMNS][NUM_ALIEN_ROWS];

// widths of aliens
// as aliens are the same type per row we do not need to store their graphic width per alien in the structure above
// that would take a byte per alien rather than just three entries here, 1 per row, saving significnt memory
uint8_t AlienWidth[] =
{ 8, 11, 12 }; // top, middle ,bottom widths

char AlienXMoveAmount = 1; // norm is 2 , this is pixel movement in X
signed char InvadersMoveCounter; // counts down, when 0 move invaders, set according to how many aliens on screen
uint8_t AnimationFrame = FALSE; // two frames of animation, if true show one if false show the other

// Player global variables
PlayerStruct Player;
GameObjectStruct Missile;

extern button BTNUP, BTNLEFT, BTNRIGHT;

void UpdateDisplay()
{
	ssd1306_Fill(Black);
	for (int across = 0; across < NUM_ALIEN_COLUMNS; across++)
	{
		for (int down = 0; down < NUM_ALIEN_ROWS; down++)
		{
			if (Alien[across][down].Ord.Status == ACTIVE)
			{
				switch (down)
				{
				case 0:
				{
					if (AnimationFrame)
					{
						ssd1306_DrawBitmap(Alien[across][down].Ord.X,
								Alien[across][down].Ord.Y, InvaderTopGfx[0], 8,
								INVADER_HEIGHT, White);

					}
					else
					{
						ssd1306_DrawBitmap(Alien[across][down].Ord.X,
								Alien[across][down].Ord.Y, InvaderTopGfx[1], 8,
								INVADER_HEIGHT, White);

					}
					break;
				}
				case 1:
				{
					if (AnimationFrame)
					{
						ssd1306_DrawBitmap(Alien[across][down].Ord.X,
								Alien[across][down].Ord.Y, InvaderMiddleGfx[0],
								16, INVADER_HEIGHT, White);

					}
					else
					{
						ssd1306_DrawBitmap(Alien[across][down].Ord.X,
								Alien[across][down].Ord.Y, InvaderMiddleGfx[1],
								16, INVADER_HEIGHT, White);

					}
					break;
				}
				default:
				{
					if (AnimationFrame)
					{
						ssd1306_DrawBitmap(Alien[across][down].Ord.X,
								Alien[across][down].Ord.Y, InvaderBottomGfx[0],
								16, INVADER_HEIGHT, White);
					}
					else
					{
						ssd1306_DrawBitmap(Alien[across][down].Ord.X,
								Alien[across][down].Ord.Y, InvaderBottomGfx[1],
								16, INVADER_HEIGHT, White);
					}
				}
				}
			}
		}
	}

	// player
	ssd1306_DrawBitmap(Player.Ord.X, Player.Ord.Y, TankGfx[0], TANKGFX_WIDTH,
	TANKGFX_HEIGHT, White);
	//missile
	if (Missile.Status == ACTIVE)
	{
		ssd1306_DrawBitmap(Missile.X, Missile.Y, MissileGfx[1], MISSILE_WIDTH,
		MISSILE_HEIGHT, White);
	}
	ssd1306_UpdateScreen();
}
void InitPlayer()
{
	Player.Ord.Y = PLAYER_Y_START;
	Player.Ord.X = PLAYER_X_START;
	Missile.Status = DESTROYED;
}

void InitAliens(int YStart)
{
	for (int across = 0; across < NUM_ALIEN_COLUMNS; across++)
	{
		for (int down = 0; down < 3; down++)
		{
			// we add down to centralise the aliens, just happens to be the right value we need per row!
			// we need to adjust a little as row zero should be 2, row 1 should be 1 and bottom row 0
			Alien[across][down].Ord.X =
					X_START_OFFSET
							+ (across
									* (LARGEST_ALIEN_WIDTH
											+ SPACE_BETWEEN_ALIEN_COLUMNS))
							- (AlienWidth[down] / 2);
			Alien[across][down].Ord.Y = YStart + (down * SPACE_BETWEEN_ROWS);
		}
	}
}
int RightMostPos()
{
	//returns x pos of right most alien
	int Across = NUM_ALIEN_COLUMNS - 1;
	int Down;
	int Largest = 0;
	int RightPos;
	while (Across >= 0)
	{
		Down = 0;
		while (Down < NUM_ALIEN_ROWS)
		{
			if (Alien[Across][Down].Ord.Status == ACTIVE)
			{
				// different aliens have different widths, add to x pos to get rightpos
				RightPos = Alien[Across][Down].Ord.X + AlienWidth[Down];
				if (RightPos > Largest)
					Largest = RightPos;
			}
			Down++;
		}
		if (Largest > 0) // we have found largest for this coloum
			return Largest;
		Across--;
	}
	return 0;  // should never get this far
}

int LeftMostPos()
{
	//returns x pos of left most alien
	int Across = 0;
	int Down;
	int Smallest = SCREEN_WIDTH * 2;
	while (Across < NUM_ALIEN_COLUMNS)
	{
		Down = 0;
		while (Down < 3)
		{
			if (Alien[Across][Down].Ord.Status == ACTIVE)
				if (Alien[Across][Down].Ord.X < Smallest)
					Smallest = Alien[Across][Down].Ord.X;
			Down++;
		}
		if (Smallest < SCREEN_WIDTH * 2) // we have found smalest for this coloum
			return Smallest;
		Across++;
	}
	return 0;  // should never get this far
}

uint8_t Collision(GameObjectStruct Obj1, unsigned char Width1,
		unsigned char Height1, GameObjectStruct Obj2, unsigned char Width2,
		unsigned char Height2)
{
	return ((Obj1.X + Width1 > Obj2.X) & (Obj1.X < Obj2.X + Width2)
			& (Obj1.Y + Height1 > Obj2.Y) & (Obj1.Y < Obj2.Y + Height2));
}

void MissileAndAlienCollisions()
{
	for (int across = 0; across < NUM_ALIEN_COLUMNS; across++)
	{
		for (int down = 0; down < NUM_ALIEN_ROWS; down++)
		{
			if (Alien[across][down].Ord.Status == ACTIVE)
			{
				if (Missile.Status == ACTIVE)
				{
					if (Collision(Missile, MISSILE_WIDTH, MISSILE_HEIGHT,
							Alien[across][down].Ord, AlienWidth[down],
							INVADER_HEIGHT))
					{
						// missile hit
						Alien[across][down].Ord.Status = DESTROYED;
						Missile.Status = DESTROYED;
					}
				}
			}
		}
	}
}
void CheckCollisions()
{
	MissileAndAlienCollisions();
}
void AlienControl()
{
	if ((InvadersMoveCounter--) < 0)
	{
		uint8_t Dropped = FALSE;
		if ((RightMostPos() + AlienXMoveAmount >= SCREEN_WIDTH)
				| (LeftMostPos() + AlienXMoveAmount < 0)) // at edge of screen
		{
			AlienXMoveAmount = -AlienXMoveAmount;       // reverse direction
			Dropped = TRUE;                  // and indicate we are dropping
		}
		// update the alien postions
		for (int Across = 0; Across < NUM_ALIEN_COLUMNS; Across++)
		{
			for (int Down = 0; Down < 3; Down++)
			{
				if (Alien[Across][Down].Ord.Status == ACTIVE)
				{
					if (Dropped == FALSE)
						Alien[Across][Down].Ord.X += AlienXMoveAmount;
					else
						Alien[Across][Down].Ord.Y += INVADERS_DROP_BY;
				}
			}
		}
		InvadersMoveCounter = INVADERS_SPEED;
		AnimationFrame = !AnimationFrame; ///swap to other frame
	}
}
void MissileControl()
{
	if (Missile.Status == ACTIVE)
	{
		Missile.Y -= MISSILE_SPEED;
		if (Missile.Y + MISSILE_HEIGHT < 0) // If off top of screen destroy so can be used again
			Missile.Status = DESTROYED;
	}
}
void PlayerControl()
{
	// user input checks
	if (Button_Check(BTNRIGHT) & (Player.Ord.X + TANKGFX_WIDTH < SCREEN_WIDTH))
		Player.Ord.X += PLAYER_X_MOVE_AMOUNT;
	if (Button_Check(BTNLEFT) & (Player.Ord.X > 0))
		Player.Ord.X -= PLAYER_X_MOVE_AMOUNT;
	if (Button_Check(BTNUP) & (Missile.Status != ACTIVE))
	{
		Missile.X = Player.Ord.X + (6); // offset missile so its in the mideel of the tank
		Missile.Y = PLAYER_Y_START;
		Missile.Status = ACTIVE;

	}

}

void Physics()
{
	AlienControl();
	PlayerControl();
	MissileControl();
	CheckCollisions();
}

void SpaceInvaders_GameLoop()
{
	InitAliens(0);
	InitPlayer();
	while (1)
	{
		Physics();
		UpdateDisplay();
		if (Button_Check(BTNRIGHT) && Button_Check(BTNLEFT))
		{
			return;
		}
	}
}
