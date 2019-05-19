#include "FirstMap.h"

namespace fs = std::experimental::filesystem;

using namespace std;

Sprites block[COUNT_OF_BLOCKS];
Sprites rope[COUNT_OF_ROPES];
Sprites lader[COUNT_OF_LADERS];
Sprites prize[COUNT_OF_PRIZE];
Sprites enemy[COUNT_OF_ENEMY];

bool MapUpload = false;
bool enemyControl = true;
 string MAP1 = "DefaultMaps/Map1";
 string MAP2 = "DefaultMaps/Map2";
 string MAP3 = "DefaultMaps/Map3";
 string MAP_HELP1 = "DefaultMaps/Map1";
 string MAP_HELP2 = "DefaultMaps/Map2";
 string MAP_HELP3 = "DefaultMaps/Map3";

void PlayGame()
{
	GotoXY(WIDTH / 2, INDENT_TOP / 2);
	SetColor(Black, White);
	cout << "Upload map and press ";
	SetColor(White, Black);
	cout << "SPACE";
	SetColor(Black, White);
	cout << " to start";
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	string FirstmMenuItems[ITEMS_COUNT] = { "NEW GAME","CREATE MAP","LOAD MAP","EXIT" };

	int startX = MENU_INDENT;
	int startY = INDENT_TOP + 1;
	int margin = 3;

	COORD position = { startX, startY };
	DrawBorder();
	for (auto item : FirstmMenuItems)
	{
		SetColor(Black, White);
		SetConsoleCursorPosition(h, position);
		cout << item;
		position.Y += margin;
	}

	int currentItem = 0; // new game
	SetColor(White, Black);
	position.Y = startY + currentItem * margin;
	SetConsoleCursorPosition(h, position);
	cout << FirstmMenuItems[currentItem];
	int code;
	while (true)
	{
		if (!CreateThread(0, 0, MainMusic, 0, 0, 0))
		{
			CreateThread(0, 0, MainMusic, 0, 0, 0);
		}
		code = _getch();
		if (code == 224)
			code = _getch();
		SetColor(Black, White);
		position.Y = startY + currentItem * margin;
		SetConsoleCursorPosition(h, position);
		cout << FirstmMenuItems[currentItem];
		if (code == SPACE && MapUpload == true)
		{
			Move();
		}
		else if ((code == DOWN || code == RIGHT) && currentItem < ITEMS_COUNT - 1) // down arrow
		{
			Beep(640, 10);
			currentItem++;
		}
		else if ((code == UP || code == LEFT) && currentItem > 0) // up arrow
		{
			Beep(640, 10);
			currentItem--;
		}
		else if (code == ENTER)
		{
			Beep(1000, 20);
			switch (currentItem)
			{
			case 0:
				Play();
				startX = MENU_INDENT;
				startY = INDENT_TOP + 1;
				position.Y = startY;
				for (auto item : FirstmMenuItems)
				{
					SetColor(Black, White);
					SetConsoleCursorPosition(h, position);
					cout << item;
					position.Y += margin;
				}
				break;
			case 1:

				Create();
				startX = MENU_INDENT;
				startY = INDENT_TOP + 1;
				position.Y = startY;
				for (auto item : FirstmMenuItems)
				{
					SetColor(Black, White);
					SetConsoleCursorPosition(h, position);
					cout << item;
					position.Y += margin;
				}
				break;
			case 2:
				Load();
				startX = MENU_INDENT;
				startY = INDENT_TOP + 1;
				position.Y = startY;
				for (auto item : FirstmMenuItems)
				{
					SetColor(Black, White);
					SetConsoleCursorPosition(h, position);
					cout << item;
					position.Y += margin;
				}
				break;
			case 3:
				exit(0);
			}
		}
		SetColor(White, Black);
		position.Y = startY + currentItem * margin;
		SetConsoleCursorPosition(h, position);
		cout << FirstmMenuItems[currentItem];
	}
}

void CreateMap()
{
	MapCreator(block, COUNT_OF_BLOCKS, BLOCK_WIDTH, 1, FULL_SIZE_BLOCK, 0);
	MapCreator(rope, COUNT_OF_ROPES, ROPE_WIDTH, 1, ROPE, 2);
	MapCreator(lader, COUNT_OF_LADERS, LADER_WIDTH, LADER_HEIGHT, LADER, 0);
	MapCreator(prize, COUNT_OF_PRIZE, 1, 1, PRIZE, 1);
	MapUpload = true;
}

void DrawBorder()
{
	GotoXY(0, 0);
	for (int i = 0; i < HEIGHT + INDENT_TOP; i++)
	{
		if (i >= INDENT_TOP)
		{
			for (int j = 0; j < WIDTH + 1; j++)
			{
				if (j < WIDTH && j > INDENT_SIDE && (i == INDENT_TOP || i == HEIGHT + INDENT_TOP - 1)) cout << FULL_SIZE_BLOCK;
				else if (j == INDENT_SIDE || j == WIDTH || j == INDENT_SIDE + 1 || j == WIDTH - 1) cout << FULL_SIZE_BLOCK;
				//else if (((i  - INDENT_TOP) % 6 == 0) && j < WIDTH && j > INDENT_SIDE)cout << FULL_SIZE_BLOCK;
				else cout << " ";
			}
		}
		else cout << " ";
		cout << endl;
	}
}

void MapCreator(Sprites arr[], const int countSign, const int width, const int height, const char sprite, int shift)
{
	char* input = new char;
	*input = 0;
	int tmpX = INDENT_SIDE + 2;
	int tmpY = HEIGHT + INDENT_TOP - 7 - shift;
	ChangePos(countSign, sprite, &tmpX, &tmpY, width, height, 0, 0);
	for (int i = countSign; i > 0; *input = _getwch())
	{
		// left
		if (*input == 75 && tmpX > INDENT_SIDE + 2) 
		{
			Beep(540, 5);
			if (FloorCollision(width, height, tmpX - 1, tmpY) && RopeCollision(width, height, tmpX - 1, tmpY) && LaderCollision(width, height, tmpX - 1, tmpY))
			{
				tripleChangePosition(sprite, FULL_SIZE_BLOCK, ROPE, LADER, width, height, &tmpX, &tmpY, -1, 0);
			}
			else if (FloorCollision(width, height, tmpX - 1, tmpY) && RopeCollision(width, height, tmpX - 1, tmpY))
			{
				doubleChangePosition(sprite, FULL_SIZE_BLOCK, ROPE, width, height, &tmpX, &tmpY, -1, 0);
			}
			else if (FloorCollision(width, height, tmpX - 1, tmpY) && LaderCollision(width, height, tmpX - 1, tmpY))
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, -1, 0);
			}
			else if (FloorCollision(width, height, tmpX - 1, tmpY))
			{
				ChangePos(countSign, FULL_SIZE_BLOCK, sprite, &tmpX, &tmpY, width, height, -1, 0);
			}
			else if (LaderCollision(width, height, tmpX - 1, tmpY - LADER_WIDTH) && sprite == PRIZE)
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, -1, 0);
			}
			else if (LaderCollision(width, height, tmpX - 1, tmpY ) && sprite != PRIZE)
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, -1, 0);
			}
			else if (RopeCollision(width, height, tmpX - 1, tmpY))
			{
				ChangePos(countSign, ROPE, sprite, &tmpX, &tmpY, width, height, -1, 0);
			}
			else if (PrizeCollision(width, height, tmpX - 1, tmpY))
			{
				ChangePos(countSign, PRIZE, sprite, &tmpX, &tmpY, width, height, -1, 0);
			}
			else ChangePos(countSign, sprite, &tmpX, &tmpY, width, height, -1, 0);
		}
		// rigth
		else if (*input == 77 && tmpX + width < WIDTH - 1)
		{
			Beep(540, 5);
			if (FloorCollision(width, height, tmpX - width, tmpY) && RopeCollision(width, height, tmpX - width, tmpY)&& LaderCollision(width, height, tmpX - width, tmpY))
			{
				tripleChangePosition(sprite, FULL_SIZE_BLOCK, ROPE, LADER, width, height, &tmpX, &tmpY, 1, 0);
			}
			else if (FloorCollision(width, height, tmpX - width, tmpY) && RopeCollision(width, height, tmpX - width, tmpY))
			{
				doubleChangePosition(sprite, FULL_SIZE_BLOCK, ROPE, width, height, &tmpX, &tmpY, 1, 0);
			}
			else if (FloorCollision(width, height, tmpX - width, tmpY) && (LaderCollision(width, height, tmpX - width, tmpY)))
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, 1, 0);
			}
			else if (FloorCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, FULL_SIZE_BLOCK, sprite, &tmpX, &tmpY, width, height, 1, 0);
			}
			else if (LaderCollision(width, height, tmpX - 1, tmpY - LADER_WIDTH ) && sprite == PRIZE)
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, 1, 0);
			}
			else if (LaderCollision(width, height, tmpX - width, tmpY) && sprite != PRIZE)
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, 1, 0);
			}
			else if (RopeCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, ROPE, sprite, &tmpX, &tmpY, width, height, 1, 0);
			}
			else if (PrizeCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, PRIZE, sprite, &tmpX, &tmpY, width, height, 1, 0);
			}
			else ChangePos(countSign, sprite, &tmpX, &tmpY, width, height, 1, 0);
		}
		// up
		else if (*input == 72 && tmpY > INDENT_TOP + 6 )
		{
			Beep(680, 5);
			if (FloorCollision(width, height, tmpX - width, tmpY) && RopeCollision(width, height, tmpX - width, tmpY) && LaderCollision(width, height, tmpX - width, tmpY))
			{
				tripleChangePosition(sprite, FULL_SIZE_BLOCK, ROPE, LADER, width, height, &tmpX, &tmpY, 0, -6);
			}
			else if (FloorCollision(width, height, tmpX - width, tmpY) && RopeCollision(width, height, tmpX - width, tmpY))
			{
				doubleChangePosition(sprite, FULL_SIZE_BLOCK, ROPE, width, height, &tmpX, &tmpY, 0, -6);
			}
			else if (FloorCollision(width, height, tmpX - width, tmpY) && LaderCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, 0, -6);
			}
			else if (FloorCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, FULL_SIZE_BLOCK, sprite, &tmpX, &tmpY, width, height, 0, -6);
			}
			else if (LaderCollision(width, height, tmpX - width, tmpY - LADER_WIDTH - 1) && sprite == PRIZE)
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, 0, -6);
			}
			else if (LaderCollision(width, height, tmpX - width, tmpY) && sprite != PRIZE)
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, 0, -6);
			}
			else if (RopeCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, ROPE, sprite, &tmpX, &tmpY, width, height, 0, -6);
			}
			else if (PrizeCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, PRIZE, sprite, &tmpX, &tmpY, width, height, 0, -6);
			}
			else ChangePos(countSign, sprite, &tmpX, &tmpY, width, height, 0, -6);
		}
		// down
		else if ((*input == 80 && tmpY < HEIGHT - 3 && sprite != ROPE) || (*input == 80 && tmpY < HEIGHT - 4 && sprite == ROPE) )
		{
			Beep(680, 5);
			if (FloorCollision(width, height, tmpX - width, tmpY) && RopeCollision(width, height, tmpX - width, tmpY) && LaderCollision(width, height, tmpX - width, tmpY))
			{
				tripleChangePosition(sprite, FULL_SIZE_BLOCK, ROPE, LADER ,width, height, &tmpX, &tmpY, 0, 6);
			}
			else if (FloorCollision(width, height, tmpX - width, tmpY) && RopeCollision(width, height, tmpX - width, tmpY))
			{
				doubleChangePosition(sprite, FULL_SIZE_BLOCK, ROPE, width, height, &tmpX, &tmpY, 0, 6);
			}
			else if (FloorCollision(width, height, tmpX - width, tmpY) && LaderCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, 0, 6);
			}
			else if (FloorCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, FULL_SIZE_BLOCK, sprite, &tmpX, &tmpY, width, height, 0, 6);
			}
			else if (LaderCollision(width, height, tmpX - width, tmpY - LADER_WIDTH) && sprite == PRIZE)
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, 0, 6);
			}
			else if (LaderCollision(width, height, tmpX - width, tmpY) && sprite != PRIZE)
			{
				ChangePos(countSign, LADER, sprite, &tmpX, &tmpY, width, height, 0, 6);
			}
			else if (RopeCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, ROPE, sprite, &tmpX, &tmpY, width, height, 0, 6);
			}
			else if (PrizeCollision(width, height, tmpX - width, tmpY))
			{
				ChangePos(countSign, PRIZE, sprite, &tmpX, &tmpY, width, height, 0, 6);
			}
			else ChangePos(countSign, sprite, &tmpX, &tmpY, width, height, 0, 6);
		}

		// Enter
		else if (*input == 13 )
		{
			Beep(940, 5);
			if (LaderCollision(width, height, tmpX - 1, tmpY - LADER_WIDTH) && sprite == PRIZE);
			//left move
			else if (
				FloorCollision(width, height, tmpX, tmpY) && !FloorCollision(width, height, tmpX - width, tmpY) ||
				LaderCollision(width, height, tmpX, tmpY) && !LaderCollision(width, height, tmpX - width, tmpY) ||
				RopeCollision(width, height, tmpX, tmpY) && !RopeCollision(width, height, tmpX - width, tmpY) ||
				PrizeCollision(width, height, tmpX, tmpY) && !PrizeCollision(width, height, tmpX - width, tmpY)
				)
			{
				if (tmpX - width > INDENT_SIDE + 2) {
					for (int i = 0; i < height; i++)
					{
						for (int j = 0; j < width; j++)
						{
							SetColor(Black, White);
							GotoXY(tmpX + j, tmpY + i); cout << sprite;
						}
					}
					arr[countSign - i].x = tmpX;
					arr[countSign - i].y = tmpY;
					i -= 1;
					tmpX -= width;
					GotoXY(tmpX, tmpY);
					for (int i = 0; i < height; i++)
					{
						for (int j = 0; j < width; j++)
						{
							SetColor(Red, White);
							GotoXY(tmpX + j, tmpY + i); cout << sprite;
						}
					}
				}
				else {
					for (int i = 0; i < height; i++)
					{
						for (int j = 0; j < width; j++)
						{
							SetColor(Black, White);
							GotoXY(tmpX + j, tmpY + i); cout << sprite;
						}
					}
					arr[countSign - i].x = tmpX;
					arr[countSign - i].y = tmpY;
					i -= 1;
					if((tmpX == INDENT_SIDE + 2)) tmpX -= tmpX - INDENT_SIDE - 2 - width;
					else tmpX -= tmpX - INDENT_SIDE - 2;
					for (int i = 0; i < height; i++)
					{
						for (int j = 0; j < width; j++)
						{
							SetColor(Red, White);
							GotoXY(tmpX + j, tmpY + i); cout << sprite;
						}
					}
				}
			}
			//right move
			else if (tmpX + width + width < WIDTH ) {
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Black, White);
						GotoXY(tmpX + j, tmpY + i); cout << sprite;
					}
				}
				arr[countSign - i].x = tmpX;
				arr[countSign - i].y = tmpY;
				i -= 1;
				tmpX += width;
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Red, White);
						GotoXY(tmpX + j, tmpY + i); cout << sprite;
					}
				}
			}
			else {
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Black, White);
						GotoXY(tmpX + j, tmpY + i); cout << sprite;
					}
				}
				arr[countSign - i].x = tmpX;
				arr[countSign - i].y = tmpY;
				i -= 1;
				if (sprite == LADER)
				{
					tmpX = (WIDTH - 5) + (WIDTH - 4 - tmpX - width * 2);
				}
				else if(sprite == FULL_SIZE_BLOCK && (tmpX == WIDTH - 6 || tmpX == WIDTH - 5) )
					tmpX = (WIDTH - 4);
				else if(sprite == ROPE)
					tmpX = (WIDTH +1 ) + (WIDTH - 4 - tmpX - width * 2);
				else 
					tmpX = (WIDTH - 1) + (WIDTH - 4 - tmpX - width * 2);

				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Red, White);
						GotoXY(tmpX + j, tmpY + i); cout << sprite;
					}
				}
			}
		}

		// esc
		else if (*input == 27)
		{
			if (FloorCollision(width, height, tmpX, tmpY) && RopeCollision(width, height, tmpX, tmpY))
			{
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Black, White);
						GotoXY(tmpX + j, tmpY + i); cout << sprite;
					}
				}
			}
			else if (FloorCollision(width, height, tmpX , tmpY))
			{
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						GotoXY(tmpX + j, tmpY + i); cout << " ";
					}
				}
				for (int j = 0; j < width; j++)
				{
					SetColor(Black, White);
					GotoXY(tmpX + j, tmpY); cout << FULL_SIZE_BLOCK;
				}
	
			}
			else if (LaderCollision(width, height, tmpX , tmpY))
			{
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Black, White);
						GotoXY(tmpX + j, tmpY + i); cout << LADER;
					}
				}
			}
			else if (RopeCollision(width, height, tmpX , tmpY) && sprite == LADER)
			{
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Black, White);
						GotoXY(tmpX + j, tmpY + i); cout << " ";
					}
				}

				for (int j = 0; j < width; j++)
				{
					SetColor(Black, White);
					GotoXY(tmpX + j, tmpY + 4); cout << ROPE;
				}
		
			}
			else if (RopeCollision(width, height, tmpX, tmpY))
			{
				for (int j = 0; j < width; j++)
				{
					SetColor(Black, White);
					GotoXY(tmpX + j, tmpY); cout << ROPE;
				}
			}
			else if (LaderCollision(width, height, tmpX, tmpY - LADER_WIDTH) && sprite == PRIZE)
			{
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Black, White);
						GotoXY(tmpX + j, tmpY + i); cout << LADER;
					}
				}
			}
			else if (LaderCollision(width, height, tmpX , tmpY - LADER_WIDTH))
			{
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Black, White);
						GotoXY(tmpX + j, tmpY + i); cout << sprite;
					}
				}
			}
			else if (PrizeCollision(width, height, tmpX, tmpY) )
			{
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Black, White);
						GotoXY(tmpX + j, tmpY + i); cout << sprite;
					}
				}
			}
			else { 
				for (int i = 0; i < height; i++)
				{
					for (int j = 0; j < width; j++)
					{
						SetColor(Black, White);
						GotoXY(tmpX + j, tmpY + i); cout << " ";
					}
				}
			}
			break;
		}
	}
	delete input;
}

void ChangePos(string look, int* x, int* y, int width, int xChanges, int yChanges)
{
	GotoXY(*x, *y);
	for (int j = 0; j < width; j++)
	{
		GotoXY(*x + j, *y);
		cout << " ";
	}
	if (enemyControl == true)
	{
		SetColor(Red, White);
	}
	else
		SetColor(Black, White);
	*x += xChanges;
	*y += yChanges;
	GotoXY(*x, *y);
	cout << look;
}

void ChangePos(string look, char spriteToChange, int *x, int *y, int width, int xChanges, int yChanges)
{
	for (int j = 0; j < width; j++)
	{
		GotoXY(*x + j, *y);
		cout << " ";
	}
	if (enemyControl == true)
	{
		SetColor(Red, White);
	}
	else
		SetColor(Black, White);
	for (int j = 0; j < width; j++)
	{
		GotoXY(*x + j, *y);
		SetColor(Black, White);
		cout << spriteToChange;
	}
	*x += xChanges;
	*y += yChanges;
	GotoXY(*x , *y);
	if (enemyControl == true)
	{
		SetColor(Red, White);
		cout << look;
	}
	else {
		SetColor(Black, White);
		cout << look;
	}
}

void ChangePos(const int countSign, const char sprite, int* x, int* y, int width, int height ,int xChanges, int yChanges)
{
	GotoXY(*x, *y);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			GotoXY(*x + j, *y + i); 
			SetColor(Black, White);
			cout << " ";
		}
	}
	if (enemyControl == true)
	{
		SetColor(Red, White);
	}
	else
		SetColor(Black, White);
	*x += xChanges;
	*y += yChanges;
	GotoXY(*x, *y);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			
			GotoXY(*x + j, *y + i); 
			SetColor(Red, White);
			cout << sprite;
		}
	}
}

void ChangePos(const int countSign, const char spriteToChange ,const char sprite, int* x, int* y, int width, int height, int xChanges, int yChanges)
{
	GotoXY(*x, *y);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			GotoXY(*x + j, *y + i);
			SetColor(Black, White);
			cout << " ";
		}
	}
	GotoXY(*x, *y);
	if (enemyControl == true)
	{
		SetColor(Red, White);
	}
	else
		SetColor(Black, White);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (spriteToChange == LADER)
			{
				GotoXY(*x + j, *y + i);
				SetColor(Black, White);
				cout << spriteToChange;
			}
			else if (spriteToChange == ROPE && sprite == LADER) {
				GotoXY(*x + j, *y + 4);
				SetColor(Black, White);
				cout << spriteToChange;
			}

			else {
				GotoXY(*x + j, *y);
				SetColor(Black, White);
				cout << spriteToChange;
			}
			
		}
	}
	*x += xChanges;
	*y += yChanges;
	GotoXY(*x, *y);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			GotoXY(*x + j, *y + i);
			SetColor(Red, White);
			cout << sprite;
		}
	}
}

void doubleChangePosition(char charFromArr, const char spriteToChange1, const char spriteToChange2, const int width, const int height, int *x, int *y, int xChanges,int yChanges)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			GotoXY(*x + j, *y + i);
			SetColor(Black, White);
			cout << " ";
		}
	}
	for (int j = 0; j < width; j++)
	{
		GotoXY(*x + j, *y);
		SetColor(Black, White);
		cout << spriteToChange1;
	}
	for (int j = 0; j < width; j++)
	{
		GotoXY(*x + j, *y + 4);
		SetColor(Black, White);
		cout << spriteToChange2;
	}
	*x += xChanges;
	*y += yChanges;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			GotoXY(*x + j, *y + i);
			SetColor(Red, White);
			cout << charFromArr;
		}
	}
}

void tripleChangePosition(char charFromArr, const char spriteToChange1, const char spriteToChange2, const char spriteToChange3, const int width, const int height, int* x, int* y, int xChanges, int yChanges)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			GotoXY(*x + j, *y + i);
			SetColor(Black, White);
			cout << " ";
		}
	}
	for (int j = 0; j < width; j++)
	{
		GotoXY(*x + j, *y);
		SetColor(Black, White);
		cout << spriteToChange1;
	}
	for (int j = 0; j < width; j++)
	{
		GotoXY(*x + j, *y + 4);
		SetColor(Black, White);
		cout << spriteToChange2;
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			GotoXY(*x + j, *y + i);
			SetColor(Black, White);
			cout << spriteToChange3;
		}
	}
	*x += xChanges;
	*y += yChanges;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			GotoXY(*x + j, *y + i);
			SetColor(Red, White);
			cout << charFromArr;
		}
	}
}

bool FloorCollision(int width ,int height,int x, int y)
{
	for (int i = 0; i < COUNT_OF_BLOCKS; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = 0; k < BLOCK_WIDTH; k++)
			{
				if ((x + width == block[i].x + k || y + height == block[i].y + j) && y == block[i].y)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool RopeCollision(int width, int height, int x, int y)
{
	for (int i = 0; i < COUNT_OF_ROPES; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = 0; k < ROPE_WIDTH; k++)
			{
				if ((x + width == rope[i].x && y + j  == rope[i].y ) )
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool LaderCollision(int width, int height, int x, int y)
{
	for (int i = 0; i < COUNT_OF_LADERS; i++)
	{
		for (int j = 0; j < LADER_HEIGHT; j++)
		{
			for (int k = 0; k < LADER_WIDTH; k++)
			{
				if ((x + width == lader[i].x + k && y + j == lader[i].y))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool PrizeCollision(int width, int height, int x, int y)
{
	for (int i = 0; i < COUNT_OF_PRIZE; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = 0; k < width; k++)
			{
				if ((x + width == prize[i].x && y + j == prize[i].y))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void DrawSpace(int x1, int x2, int y1, int y2)
{
	for (int i = 0; i < y2; i++)
	{
		for (int j = 0; j < x2; j++)
		{
			GotoXY(j + x1, i + y1);
			cout << " ";
		}
	}
}

void DrawMap(Sprites block[], Sprites rope[], Sprites lader[], Sprites prize[])
{
	SetColor(Black, White);
	for (int i = 0; i < COUNT_OF_BLOCKS; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < BLOCK_WIDTH; k++)
			{
				if (block[i].x != 0)
				{
					GotoXY(block[i].x + k, block[i].y + j);
					cout << FULL_SIZE_BLOCK;
				}
				
			}
		}
	}
	for (int i = 0; i < COUNT_OF_ROPES; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < ROPE_WIDTH; k++)
			{
				if (rope[i].x != 0)
				{
					GotoXY(rope[i].x + k, rope[i].y + j);
					cout << ROPE;
				}
			}
		}
	}
	for (int i = 0; i < COUNT_OF_LADERS; i++)
	{
		for (int j = 0; j < LADER_HEIGHT; j++)
		{
			for (int k = 0; k < LADER_WIDTH; k++)
			{
				if (lader[i].x != 0)
				{
					GotoXY(lader[i].x + k, lader[i].y + j);
					cout << LADER;
				}
			}
		}
	}
	for (int i = 0; i < COUNT_OF_PRIZE; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < 1; k++)
			{
				if (prize[i].x != 0)
				{
					GotoXY(prize[i].x + k, prize[i].y + j);
					SetColor(Red, White);
					cout << PRIZE;
					SetColor(Black, White);
				}
			}
		}
	}
}

/*bool EnemyCollision(char) // right collision
{
	for (int j = 0; j < COUNT_OF_ENEMY; j++)
	{
		int tmp = enemy[j].x;

		for (int i = 0; i < COUNT_OF_ENEMY; i++)
		{
			if (tmp == enemy[i].x + 2 && j != i)
			{
				return true;
			}
		}
	}
	return false;
}
*/

void WriteToFile(string name) 
{
	ofstream Out;
	fs::create_directories("Maps/UserMaps");
	Out.open("Maps/UserMaps/" + name + ".bin");
	if (!Out.is_open())
	{
		cout << "ERROR!";
	}
	else
	{
		for (int i = 0; i < COUNT_OF_BLOCKS; i++)
		{
			Out << block[i].x << " ";
			Out << block[i].y << " ";
			if (i == COUNT_OF_BLOCKS - 1)
			{
				for (int i = 0; i < COUNT_OF_ROPES; i++)
				{
					Out << rope[i].x << " ";
					Out << rope[i].y << " ";
					if (i == COUNT_OF_ROPES - 1)
					{
						for (int i = 0; i < COUNT_OF_LADERS; i++)
						{
							Out << lader[i].x << " ";
							Out << lader[i].y << " ";
							if (i == COUNT_OF_LADERS - 1)
							{
								for (int i = 0; i < COUNT_OF_PRIZE; i++)
								{
									Out << prize[i].x << " ";
									Out << prize[i].y << " ";
								}
							}
						}
					}
				}
			}
		}
	}
	Out.close();
}

void LoadFromFile(string name)
{
	ifstream Out;
	Out.open("Maps/" + name + ".bin");
	if (!Out.is_open())
	{
		cout << endl << "\t  ERROR";
		MapUpload = false;
		Sleep(500);
	}
	else 
	{
		for (int i = 0; i < COUNT_OF_BLOCKS; i++)
		{
			Out >> block[i].x;
			Out >> block[i].y;
			if (i == COUNT_OF_BLOCKS - 1)
			{
				for (int i = 0; i < COUNT_OF_ROPES; i++)
				{
					Out >> rope[i].x;
					Out >> rope[i].y;
					if (i == COUNT_OF_ROPES - 1)
					{
						for (int i = 0; i < COUNT_OF_LADERS; i++)
						{
							Out >> lader[i].x;
							Out >> lader[i].y;
							if (i == COUNT_OF_LADERS - 1)
							{
								for (int i = 0; i < COUNT_OF_PRIZE; i++)
								{
									Out >> prize[i].x;
									Out >> prize[i].y;
								}
							}
						}
					}
				}
			}
		}
		MapUpload = true;
	}
	Out.close();
}

void getFilesFromDir()
{
	for (const auto & entry : fs::directory_iterator("Maps/UserMaps")) 
	{
		cout << "\n\n   " << entry.path() << endl;
	}
}

bool PrizeCollision(int x, int y)
{
	for (int i = 0; i < COUNT_OF_PRIZE; i++)
	{
		if ((x == prize[i].x && y == prize[i].y))
		{
			prize[i].x = 0;
			prize[i].y = 0;
			return true;
		}
	}
	return false;
}

void DeleteArray()
{
	for (int i = 0; i < COUNT_OF_LADERS; i++)
	{
		lader[i].x = 0;
		lader[i].y = 0;
	}
	for (int i = 0; i < COUNT_OF_BLOCKS; i++)
	{
		block[i].x = 0;
		block[i].y = 0;
	}
	for (int i = 0; i < COUNT_OF_ROPES; i++)
	{
		rope[i].x = 0;
		rope[i].y = 0;
	}
	for (int i = 0; i < COUNT_OF_PRIZE; i++)
	{
		prize[i].x = 0;
		prize[i].y = 0;
	}
	for (int i = 0; i < COUNT_OF_ENEMY; i++)
	{
		enemy[i].x = 0;
		enemy[i].y = 0;
	}
	for (int i = 0; i < COUNT_OF_ENEMY; i++)
	{
		//SetColor(Red, White);
		enemy[i].look.frontLook = "";

		enemy[i].look.leftLook = "";

		enemy[i].look.rightLook = "";

		enemy[i].look.width = 2;

		enemy[i].x = 0;
		enemy[i].y = 0;
	}

}

void EnemyMove(Sprites hero, char *enemyPosition, char *heroPosition)
{
	enemyControl = true;
	int moveCount = 0;
	bool touchLeftBorder = false;
	bool touchRightBorder = true;
	
	for (int i = 0 ; i < COUNT_OF_ENEMY; i++)
	{
		for (int i = 0; i < COUNT_OF_PRIZE; i++)
		{
			if (prize[i].x != 0 && prize[i].y != 0)
			{
				GotoXY(prize[i].x, prize[i].y);
				SetColor(Red, White);
				cout << PRIZE;
			}
		}
		moveCount = 0;
		if (enemy[i].x == hero.x)
		{
			enemy[i].sameXPos = true;
		}
		enemy[i].yChanges = false;


		// up
	
		if (enemy[i].y > hero.y && enemy[i].y > INDENT_TOP + 5 )
		{
			int tmp = enemy[i].y;
			if (LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width, enemy[i].y - LADER_HEIGHT + 1))
			{

				if (*enemyPosition == 'l') {
					if (!LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width + 1, enemy[i].y - LADER_HEIGHT + 1))
					{

					}
					else {
						ChangePos(enemy[i].look.leftLook, LADER, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 0, -1);
					}
				}
				else if (*enemyPosition == 'r') {
					if (!LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width + 1, enemy[i].y - LADER_HEIGHT + 1))
					{

					}
					else {
						ChangePos(enemy[i].look.rightLook, LADER, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 0, -1);
					}
				}
			}
			if (enemy[i].y == tmp)
			{
				enemy[i].yChanges = false;
			}
			else enemy[i].yChanges = true;
			//moveCount = 1;
			//else ChangePos(hero.look.leftLook, &hero.x, &hero.y, hero.look.width, 0, -1);
		}
		// down
		else if ((enemy[i].y < hero.y && enemy[i].y < HEIGHT + 2))
		{
			int tmp = enemy[i].y;
			if (LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width, enemy[i].y + 1) && !LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width, enemy[i].y - LADER_HEIGHT + 1)) //(LaderCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y + 1)
			{
				if (*enemyPosition == 'l') {
					if (!LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width + 1, enemy[i].y - LADER_HEIGHT + 2))
					{

					}
					else ChangePos(enemy[i].look.leftLook, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 0, 1);
				}
				else if (*enemyPosition == 'r') {
					if (!LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width + 1, enemy[i].y - LADER_HEIGHT + 2))
					{

					}
					else  ChangePos(enemy[i].look.rightLook, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 0, 1);
				}
			}
			if (LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width, enemy[i].y - 4))
			{
				if (*enemyPosition == 'r') {
					if (!LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width + 1, enemy[i].y - LADER_HEIGHT + 1))
					{
					}
					else {
						ChangePos(enemy[i].look.rightLook, LADER, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 0, 1);
					}
				}
				if (*enemyPosition == 'l') {
					if (!LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width + 1, enemy[i].y - LADER_HEIGHT + 2))
					{

					}
					else ChangePos(enemy[i].look.leftLook, LADER, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 0, 1);
					
				}
			}
			if ((RopeCollision(enemy[i].look.width, 1, enemy[i].x - 2, enemy[i].y - 2)) || (RopeCollision(enemy[i].look.width, 1, enemy[i].x - 1, enemy[i].y - 1)))
			{
				if (*enemyPosition == 'l') {
					if (!RopeCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width, enemy[i].y - 1))
					{

					}
					else ChangePos(enemy[i].look.leftLook, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 0, 1);
				}
				else if (*enemyPosition == 'r') {
					if (!RopeCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width + 1, enemy[i].y - 1))
					{

					}
					else  ChangePos(enemy[i].look.rightLook, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 0, 1);
				}
				goto EnemyFall;
			}
			if (enemy[i].y == tmp)
			{
				enemy[i].yChanges = false;
			}
			else enemy[i].yChanges = true;
			//moveCount = 1;
		}
		//left
		 if (enemy[i].x > hero.x && enemy[i].x > INDENT_SIDE + 2 && moveCount == 0 && enemy[i].sameXPos == false && enemy[i].yChanges == false)
		{
			*enemyPosition = 'l';
			/*
			if (EnemyCollision())
			{
				break;
			}
			*/
			if (LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width + 1, enemy[i].y - LADER_HEIGHT + 1))
			{
				if (FloorCollision(1, 1, enemy[i].x, enemy[i].y) && !LaderCollision(1, 1, enemy[i].x - enemy[i].look.width, enemy[i].y))
				{
					//GotoXY(hero.x - 1, hero.y);
					//cout << "s";
				}
				else ChangePos(enemy[i].look.leftLook, LADER, &enemy[i].x, &enemy[i].y, enemy[i].look.width, -1, 0);
			}
			
			else ChangePos(enemy[i].look.leftLook, &enemy[i].x, &enemy[i].y, enemy[i].look.width, -1, 0);
			moveCount = 1;
		}
		
		// right
		else if (enemy[i].x < hero.x && enemy[i].x < WIDTH - 1 && moveCount == 0 && enemy[i].sameXPos == false && enemy[i].yChanges == false)
		 {
			*enemyPosition = 'r';
			/*
			if (EnemyCollision(*enemyPosition))
			{
				break;
			}
			*/
			if (LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width, enemy[i].y - LADER_HEIGHT + 1))
			{
				if (FloorCollision(1, 1, enemy[i].x + enemy[i].look.width, enemy[i].y) && !LaderCollision(1, 1, enemy[i].x + 1, enemy[i].y))
				{
					//GotoXY(hero.x - 1, hero.y);
					//cout << "s";
				}
				else ChangePos(enemy[i].look.rightLook, LADER, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 1, 0);
			}
			
			else ChangePos(enemy[i].look.rightLook, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 1, 0);
			moveCount = 1;
		}
		 // Same x coord
	
		 // right move
		 if (enemy[i].x <= (WIDTH - 1) / 2  && enemy[i].sameXPos == true)
		 {
			 *enemyPosition = 'r';
			 if (LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width, enemy[i].y - LADER_HEIGHT + 1))
			 {
				 if (FloorCollision(1, 1, enemy[i].x + enemy[i].look.width, enemy[i].y) && !LaderCollision(1, 1, enemy[i].x + 1, enemy[i].y))
				 {
					 //GotoXY(hero.x - 1, hero.y);
					 //cout << "s";
				 }
				 else ChangePos(enemy[i].look.rightLook, LADER, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 1, 0);
			 }
			 else ChangePos(enemy[i].look.rightLook, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 1, 0);
			 /*if (enemy[i].x == (WIDTH - 3) / 2)
			 {
				 enemy[i].sameXPos = false;
			 }
			 */
			 if (enemy[i].yChanges == true)
			 {
				 enemy[i].sameXPos = false;
			 }
		 }
		 // left move
		 else if (enemy[i].x >= (WIDTH - 1) / 2 && enemy[i].sameXPos == true)
		 {
			 if (enemy[i].x == WIDTH / 2 + 1)
			 {
				 enemy[i].sameXPos = false;
			 }
			 *enemyPosition = 'l';
			 if (LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width + 1, enemy[i].y - LADER_HEIGHT + 1))
			 {
				 if (FloorCollision(1, 1, enemy[i].x, enemy[i].y) && !LaderCollision(1, 1, enemy[i].x - enemy[i].look.width, enemy[i].y))
				 {
					 //GotoXY(hero.x - 1, hero.y);
					 //cout << "s";
				 }
				 else ChangePos(enemy[i].look.leftLook, LADER, &enemy[i].x, &enemy[i].y, enemy[i].look.width, -1, 0);
			 }
			 else ChangePos(enemy[i].look.leftLook, &enemy[i].x, &enemy[i].y, enemy[i].look.width, -1, 0);
			 moveCount = 1;
			 if (enemy[i].yChanges == true)
			 {
				 enemy[i].sameXPos = false;
			 }
		 }
		
	 EnemyFall:
		while (((!FloorCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width, enemy[i].y + 1) && *enemyPosition == 'r') || ((!FloorCollision(enemy[i].look.width, 1, enemy[i].x - 1, enemy[i].y + 1) && *enemyPosition == 'l'))) && enemy[i].y < HEIGHT + INDENT_TOP - 2)
		{

			if (((PrizeCollision(enemy[i].x, enemy[i].y) || PrizeCollision(enemy[i].x + 1, enemy[i].y)) && *enemyPosition == 'r') || (PrizeCollision(enemy[i].x, enemy[i].y) || PrizeCollision(enemy[i].x - 1, enemy[i].y)) && *enemyPosition == 'l')
			{
				//change position
			}
			if ((RopeCollision(enemy[i].look.width, 1, enemy[i].x - 2, enemy[i].y - 1) && *enemyPosition == 'r') || (RopeCollision(enemy[i].look.width, 1, enemy[i].x - 1, enemy[i].y - 1) && *enemyPosition == 'l'))
			{
				if (RopeCollision(enemy[i].look.width, 1, enemy[i].x - 2, enemy[i].y - 1) && !RopeCollision(enemy[i].look.width, 1, enemy[i].x + 1, enemy[i].y - 1) && *enemyPosition == 'r')
				{	
					SetColor(Black, White);
					//SetColor(Black, White);
					GotoXY(enemy[i].x - 1, enemy[i].y - 1);
					cout << "_";
					break;
				}
				else if (RopeCollision(enemy[i].look.width, 1, enemy[i].x - 1, enemy[i].y - 1) && !RopeCollision(enemy[i].look.width, 1, enemy[i].x - 2, enemy[i].y - 1) && * enemyPosition == 'l')
				{
					SetColor(Black, White);
					//SetColor(Red, White);
					GotoXY(enemy[i].x + 1, enemy[i].y - 1);
					cout << "_";
					break;
				}
				else {
					SetColor(Black, White);
					//SetColor(Red, White);
					GotoXY(enemy[i].x, enemy[i].y - 1);
					cout << "_";
					GotoXY(enemy[i].x + 1, enemy[i].y - 1);
					cout << "_";
					break;
				}
			}
			else if (LaderCollision(enemy[i].look.width, 1, enemy[i].x - enemy[i].look.width, enemy[i].y - 4))
			{
				break;
			}
			else if (LaderCollision(enemy[i].look.width, 1, enemy[i].x - 1, enemy[i].y - 4))
			{
				break;
			}
			else if (*enemyPosition == 'l')
			{
				Sleep(50);
				ChangePos(enemy[i].look.leftLook, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 0, 1);
			}
			else if (*enemyPosition == 'r')
			{
				Sleep(50);
				ChangePos(enemy[i].look.rightLook, &enemy[i].x, &enemy[i].y, enemy[i].look.width, 0, 1);
			}
		}
		//Sleep(100);
	}

	
}

void Move()
{

	for (int i = 0; i < COUNT_OF_ENEMY; i++)
	{
		//SetColor(Red, White);
		enemy[i].look.frontLook = (char)192;
		enemy[i].look.frontLook += (char)219;
		enemy[i].look.frontLook += (char)217;

		enemy[i].look.leftLook = "-";
		enemy[i].look.leftLook += (char)219;

		enemy[i].look.rightLook += (char)219;
		enemy[i].look.rightLook += "-";

		enemy[i].look.width = 2;

		enemy[i].x = INDENT_SIDE + 2 + i * 2;
		enemy[i].y = INDENT_TOP + 4;
	}
	enemy[0].x = INDENT_SIDE + 10;
	enemy[0].y = INDENT_TOP + 4;
	enemy[1].x = INDENT_SIDE + 100;
	enemy[1].y = INDENT_TOP + 4;
	enemy[2].x = INDENT_SIDE + 50;
	enemy[2].y = INDENT_TOP + 14;
	enemy[3].x = INDENT_SIDE + 80;
	enemy[3].y = INDENT_TOP + 21;

	for (int i = 0; i < COUNT_OF_ENEMY; i++)
	{
		//GotoXY(enemy[i].x, enemy[i].y);
		//cout << enemy[i].look.rightLook;
	}
	char *enemyPosition = new char;

	*enemyPosition = 'r';
	int *skipCount = new int;
	*skipCount = 0;
	/////////////////////////////////////////////

	Sprites hero;
	hero.look.frontLook = (char)192;
	hero.look.frontLook += (char)219;
	hero.look.frontLook += (char)217;

	hero.look.leftLook = "-";
	hero.look.leftLook += (char)219;

	hero.look.rightLook += (char)219;
	hero.look.rightLook += "-";

	hero.look.width = 2;

	hero.x = INDENT_SIDE + 25;
	hero.y = INDENT_TOP + HEIGHT - 8;

	char *position = new char;
	char *input = new char;
	bool *win = new bool;
	int* countPrize = new int;
	*countPrize = 0;
	int *prizeOnMap = new int;
	*prizeOnMap = 0;
	PrizeCount(prizeOnMap);
	*win = false;
	GotoXY(hero.x, hero.y);
	cout << hero.look.rightLook;
	*position = 'r';
	
	for (; !*win;)
	{
		for (int i = 0; i < COUNT_OF_ENEMY; i++)
		{
			if (enemy[i].x == hero.x && enemy[i].y == hero.y)
			{
				Sleep(200);
				mciSendString("Play sound/Lose.wav", 0, 0, 0);
				//DrawSpace(INDENT_SIDE + 2, WIDTH - INDENT_SIDE - 3, INDENT_TOP + 1, HEIGHT - 2);
				GotoXY(0, 0);
				//DrawBorder();
				DrawSpace(INDENT_SIDE + 2, WIDTH - INDENT_SIDE - 3, INDENT_TOP + 1, HEIGHT - 2);
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						GotoXY(WIDTH - 1 + j, HEIGHT - 2 + i);
						SetColor(Black, White);
						cout << FULL_SIZE_BLOCK;
						MapUpload = false;
					}
				}
				for (int i = 0; i < COUNT_OF_PRIZE; i++)
				{
					GotoXY(WIDTH + i + 1, INDENT_TOP + 2);
					cout << " ";
				}
				MapUpload = false;
				delete input, win, position, prizeOnMap, enemyPosition, skipCount;
				prizeOnMap = nullptr;
				win = nullptr;
				input = nullptr;
				position = nullptr;
				skipCount = nullptr;
				return;
			}
		}
		if (_kbhit()) {
			enemyControl = false;
			*input = _getch();
			SetColor(Black, White);
			if (((PrizeCollision(hero.x, hero.y) || PrizeCollision(hero.x + 1, hero.y)) && *position == 'r') || (PrizeCollision(hero.x, hero.y) || PrizeCollision(hero.x - 1, hero.y)) && *position == 'l')
			{
				GotoXY(WIDTH + *countPrize + 2, INDENT_TOP + 2);
				cout << PRIZE;
				*countPrize += 1;
				mciSendString("Play sound/TakePrize.wav", 0, 0, 0); // If prize is taken play sound
				if (*countPrize == *prizeOnMap)
				{
					for (int i = 0; i < 5; i++)
					{
						for (int j = 0; j <	 2; j++)
						{
							mciSendString("Play sound/OpenTheDoor.wav", 0, 0, 0);
							GotoXY(WIDTH - 1 + j, HEIGHT - 2 + i);
							SetColor(Red, White);
							cout << FULL_SIZE_BLOCK;
							MapUpload = false;
						}
					}
				}
			}
			// left
			if (*input == ESCAPE)
			{
				MapUpload = false;
				break;
			}
			if (*input == 75 && hero.x > INDENT_SIDE + 2)
			{

				*position = 'l';
				if (LaderCollision(hero.look.width, 1, hero.x - hero.look.width + 1, hero.y - LADER_HEIGHT + 1))
				{
					if (FloorCollision(1, 1, hero.x, hero.y) && !LaderCollision(1, 1, hero.x - hero.look.width, hero.y))
					{
						//GotoXY(hero.x - 1, hero.y);
						//cout << "s";
					}
					else ChangePos(hero.look.leftLook, LADER, &hero.x, &hero.y, hero.look.width, -1, 0);
				}
				else ChangePos(hero.look.leftLook, &hero.x, &hero.y, hero.look.width, -1, 0);
			}
			// if won
			else if ((*input == 77 && *countPrize == *prizeOnMap) && hero.x + hero.look.width == WIDTH - 1 && hero.y > INDENT_TOP + HEIGHT - 5)
			{
				
				*position = 'r';
				if (LaderCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y - LADER_HEIGHT + 1))
				{
					if (FloorCollision(1, 1, hero.x + hero.look.width, hero.y) && !LaderCollision(1, 1, hero.x + 1, hero.y))
					{
						//GotoXY(hero.x - 1, hero.y);
						//cout << "s";
					}
					else ChangePos(hero.look.rightLook, LADER, &hero.x, &hero.y, hero.look.width, 1, 0);
				}
				else ChangePos(hero.look.rightLook, &hero.x, &hero.y, hero.look.width, 1, 0);
				mciSendString("Play sound/Win.wav", 0, 0, 0);
				delete input, win, position, prizeOnMap, enemyPosition, skipCount;
				prizeOnMap = nullptr;
				win = nullptr;
				input = nullptr;
				position = nullptr;
				skipCount = nullptr;
				return;

			}
			// right
			else if (*input == 77 && hero.x + hero.look.width < WIDTH - 1)
			{
				
				*position = 'r';
				if (LaderCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y - LADER_HEIGHT + 1))
				{
					if (FloorCollision(1, 1, hero.x + hero.look.width, hero.y) && !LaderCollision(1, 1, hero.x + 1, hero.y))
					{
						//GotoXY(hero.x - 1, hero.y);
						//cout << "s";
					}
					else ChangePos(hero.look.rightLook, LADER, &hero.x, &hero.y, hero.look.width, 1, 0);
				}
				else ChangePos(hero.look.rightLook, &hero.x, &hero.y, hero.look.width, 1, 0);

			}

			// up
			else if (*input == 72 && hero.y > INDENT_TOP + 5)
			{
				
				if (LaderCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y - LADER_HEIGHT + 1))
				{
					if (*position == 'l') {
						if (!LaderCollision(hero.look.width, 1, hero.x - hero.look.width + 1, hero.y - LADER_HEIGHT + 1))
						{

						}
						else {
							ChangePos(hero.look.leftLook, LADER, &hero.x, &hero.y, hero.look.width, 0, -1);
						}
					}
					else if (*position == 'r') {
						if (!LaderCollision(hero.look.width, 1, hero.x - hero.look.width + 1, hero.y - LADER_HEIGHT + 1))
						{

						}
						else {
							ChangePos(hero.look.rightLook, LADER, &hero.x, &hero.y, hero.look.width, 0, -1);
						}
					}
				}
				//else ChangePos(hero.look.leftLook, &hero.x, &hero.y, hero.look.width, 0, -1);
			}
			// down
			else if ((*input == 80 && hero.y < HEIGHT + 2))
			{
				if ((RopeCollision(hero.look.width, 1, hero.x - 2, hero.y - 2)) || (RopeCollision(hero.look.width, 1, hero.x - 1, hero.y - 1)))
				{
					if (*position == 'l') {
						if (!RopeCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y - 1))
						{

						}
						else ChangePos(hero.look.leftLook, &hero.x, &hero.y, hero.look.width, 0, 1);
					}
					else if (*position == 'r') {
						if (!RopeCollision(hero.look.width, 1, hero.x - hero.look.width + 1, hero.y - 1))
						{

						}
						else  ChangePos(hero.look.rightLook, &hero.x, &hero.y, hero.look.width, 0, 1);
					}
					goto Fall;
				}
				if (LaderCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y + 1) && !LaderCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y - LADER_HEIGHT + 1)) //(LaderCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y + 1)
				{
					if (*position == 'l') {
						if (!LaderCollision(hero.look.width, 1, hero.x - hero.look.width + 1, hero.y - LADER_HEIGHT + 2))
						{

						}
						else ChangePos(hero.look.leftLook, &hero.x, &hero.y, hero.look.width, 0, 1);
					}
					else if (*position == 'r') {
						if (!LaderCollision(hero.look.width, 1, hero.x - hero.look.width + 1, hero.y - LADER_HEIGHT + 2))
						{

						}
						else  ChangePos(hero.look.rightLook, &hero.x, &hero.y, hero.look.width, 0, 1);
					}
				}
				else if (LaderCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y - 4))
				{
					if (*position == 'r') {
						if (!LaderCollision(hero.look.width, 1, hero.x - hero.look.width + 1, hero.y - LADER_HEIGHT + 1))
						{
						}
						else {
							ChangePos(hero.look.rightLook, LADER, &hero.x, &hero.y, hero.look.width, 0, 1);
						}
					}
					if (*position == 'l') {
						ChangePos(hero.look.leftLook, LADER, &hero.x, &hero.y, hero.look.width, 0, 1);
					}
				}
				//else ChangePos(hero.look.leftLook, &hero.x, &hero.y, hero.look.width, 0, 1);
			}
			// enter
			else if (*input == 13)
			{

			}
		Fall:
			//int wav = 10000;
			while (((!FloorCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y + 1) && *position == 'r') || ((!FloorCollision(hero.look.width, 1, hero.x - 1, hero.y + 1) && *position == 'l'))) && hero.y < HEIGHT + INDENT_TOP - 2)
			{
				if (((PrizeCollision(hero.x, hero.y) || PrizeCollision(hero.x + 1, hero.y)) && *position == 'r') || (PrizeCollision(hero.x, hero.y) || PrizeCollision(hero.x - 1, hero.y)) && *position == 'l')
				{
					GotoXY(WIDTH + *countPrize + 2, INDENT_TOP + 2);
					cout << PRIZE;
					*countPrize += 1;
				}
				if ((RopeCollision(hero.look.width, 1, hero.x - 2, hero.y - 1) && *position == 'r') || (RopeCollision(hero.look.width, 1, hero.x - 1, hero.y - 1) && *position == 'l'))
				{
					if (RopeCollision(hero.look.width, 1, hero.x - 2, hero.y - 1) && !RopeCollision(hero.look.width, 1, hero.x + 1, hero.y - 1) && *position == 'r')
					{
						GotoXY(hero.x - 1, hero.y - 1);
						cout << "_";
						break;
					}
					else if (RopeCollision(hero.look.width, 1, hero.x - 1, hero.y - 1) && !RopeCollision(hero.look.width, 1, hero.x - 2, hero.y - 1) && * position == 'l')
					{
						GotoXY(hero.x + 1, hero.y - 1);
						cout << "_";
						break;
					}
					else {
						GotoXY(hero.x, hero.y - 1);
						cout << "_";
						GotoXY(hero.x + 1, hero.y - 1);
						cout << "_";
						break;
					}
				}
				else if (LaderCollision(hero.look.width, 1, hero.x - hero.look.width, hero.y - 4))
				{
					break;
				}
				else if (LaderCollision(hero.look.width, 1, hero.x - 1, hero.y - 4))
				{
					break;
				}
				else if (*position == 'l')
				{
					//Beep(wav, 2);
					Sleep(50);
					ChangePos(hero.look.leftLook, &hero.x, &hero.y, hero.look.width, 0, 1);
				}
				else if (*position == 'r')
				{
					//Beep(wav, 2);
					Sleep(50);
					ChangePos(hero.look.rightLook, &hero.x, &hero.y, hero.look.width, 0, 1);
				}
				//wav -= 300;
			}
		}
		else {
			SetColor(Red, White);
			//Sleep(130);
			if (*skipCount == ENEMY_SPEED)
			{
				EnemyMove(hero, enemyPosition, position);
				*skipCount = 0;
			}
			else *skipCount += 1;
		}
	}
}

void PrizeCount(int *count)
{
	for (int i = 0; i < COUNT_OF_PRIZE; i++)
	{
		if (prize[i].x != 0 )
		{
			*count += 1;
		}
	}
}

void Play()
{

	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	string SecondMenuItem[ITEMS_COUNT - 1] = { "MAP1","MAP2","MAP3" };

	int startX = MENU_INDENT;
	int startY = INDENT_TOP + 1;
	int margin = 3;

	COORD position = { startX, startY };
	DrawSpace(0, INDENT_SIDE - 2, 0, 50);
	for (auto item : SecondMenuItem)
	{
		SetColor(Black, White);
		SetConsoleCursorPosition(h, position);
		cout << item;
		position.Y += margin;
	}

	int currentItem = 0; // new game
	SetColor(White, Black);
	position.Y = startY + currentItem * margin;
	SetConsoleCursorPosition(h, position);
	cout << SecondMenuItem[currentItem];

	int code;
	while (true)
	{
		int tmp = 0;
		code = _getch();
		if (code == 224)
			code = _getch();
		SetColor(Black, White);
		position.Y = startY + currentItem * margin;
		SetConsoleCursorPosition(h, position);
		cout << SecondMenuItem[currentItem];
		if (code == SPACE && MapUpload == true)
		{
			Move();
			break;
		}
		else if (code == ESCAPE)
		{
			break;
		}
		else if ((code == DOWN || code == RIGHT) && currentItem < ITEMS_COUNT - 2) // down arrow
		{
			Beep(640, 10);
			currentItem++;
		}
		else if ((code == UP || code == LEFT) && currentItem > 0) // up arrow
		{
			Beep(640, 10);
			currentItem--;
		}
		else if (code == ENTER)
		{
			Beep(1000, 20);
			if (currentItem == 0)
			{
				MapUpload = true;
				DeleteArray();
				DrawSpace(INDENT_SIDE + 2, WIDTH - INDENT_SIDE - 3, INDENT_TOP + 1, HEIGHT - 2);
				//DrawBorder();
				LoadFromFile(MAP_HELP1);
				DrawMap(block, rope, lader, prize);
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						GotoXY(WIDTH - 1 + j, HEIGHT - 2 + i);
						SetColor(Black, White);
						cout << FULL_SIZE_BLOCK;
					}
				}
				for (int i = 0; i < COUNT_OF_PRIZE; i++)
				{
					GotoXY(WIDTH + i + 1, INDENT_TOP + 2);
					cout << " ";
				}
				GotoXY(INDENT_SIDE / 2 + WIDTH / 2, INDENT_TOP + HEIGHT + 1);
				cout << "MAP1                  ";
				//DrawSpace(0, INDENT_SIDE - 2, 0, 50);
				//break;
			}
			else if (currentItem == 1)
			{
				MapUpload = true;
				DeleteArray();
				DrawSpace(INDENT_SIDE + 2, WIDTH - INDENT_SIDE - 3, INDENT_TOP + 1, HEIGHT - 2);
				//DrawBorder();
				LoadFromFile(MAP_HELP2);
				DrawMap(block, rope, lader, prize);
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						GotoXY(WIDTH - 1 + j, HEIGHT - 2 + i);
						SetColor(Black, White);
						cout << FULL_SIZE_BLOCK;
					}
				}
				for (int i = 0; i < COUNT_OF_PRIZE; i++)
				{
					GotoXY(WIDTH + i + 1, INDENT_TOP + 2);
					cout << " ";
				}
				GotoXY(INDENT_SIDE / 2 + WIDTH / 2, INDENT_TOP + HEIGHT + 1);
				cout << "MAP2                ";
				//DrawSpace(0, INDENT_SIDE - 2, 0, 50);
				//break;
			}
			else if (currentItem == 2)
			{
				MapUpload = true;
				DeleteArray();
				DrawSpace(INDENT_SIDE + 2, WIDTH - INDENT_SIDE - 3, INDENT_TOP + 1, HEIGHT - 2);
				//DrawBorder();
				LoadFromFile(MAP_HELP3);
				DrawMap(block, rope, lader, prize);
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						GotoXY(WIDTH - 1 + j, HEIGHT - 2 + i);
						SetColor(Black, White);
						cout << FULL_SIZE_BLOCK;
					}
				}
				for (int i = 0; i < COUNT_OF_PRIZE; i++)
				{
					GotoXY(WIDTH + i + 1, INDENT_TOP + 2);
					cout << " ";
				}
				GotoXY(INDENT_SIDE / 2 + WIDTH / 2, INDENT_TOP + HEIGHT + 1);
				cout << "MAP3               ";
				//DrawSpace(0, INDENT_SIDE - 2, 0, 50);
				//break;
			}
		}
		SetColor(White, Black);
		position.Y = startY + currentItem * margin;
		SetConsoleCursorPosition(h, position);
		cout << SecondMenuItem[currentItem];
	}
}

void Create()
{
	DeleteArray();
	DrawSpace(INDENT_SIDE + 2, WIDTH - INDENT_SIDE - 3, INDENT_TOP + 1, HEIGHT - 2);
	DrawSpace(0, INDENT_SIDE - 2, 0, 50);
	//DrawBorder();
	GotoXY(MENU_INDENT, INDENT_TOP + 1);
	cout << "Write map name" << endl;
	ShowCursor(true);
	string *name = new string;
	GotoXY(MENU_INDENT, INDENT_TOP + 2);
	cin >> *name;
	ShowCursor(false);
	GotoXY(INDENT_SIDE / 2 + WIDTH / 2, INDENT_TOP + HEIGHT + 1);
	cout << *name <<"        ";
	DrawSpace(0, INDENT_SIDE - 2, 0, 50);
	DrawBorder();
	CreateMap();
	WriteToFile(*name);
	delete name;
	
}

void Load()
{
	DeleteArray();
	DrawSpace(INDENT_SIDE + 2, WIDTH - INDENT_SIDE - 3, INDENT_TOP + 1, HEIGHT - 2);
	DrawSpace(0, INDENT_SIDE - 2, 0, 50);
	GotoXY(0, INDENT_TOP);
	getFilesFromDir();
	string *str = new string;
	string *tmpStr = new string;
	*str += "UserMaps/";
	cout << "\n\n   Write map name\n\n\n   ";
	cin >> *tmpStr;
	*str += *tmpStr;
	LoadFromFile(*str);
	DrawSpace(INDENT_SIDE + 2, WIDTH - INDENT_SIDE - 3, INDENT_TOP + 1, HEIGHT - 2);
	DrawBorder();
	DrawMap(block, rope, lader, prize);
	DrawSpace(0, INDENT_SIDE - 2, 0, 50);
	if (MapUpload == true)
	{
		GotoXY(INDENT_SIDE / 2 + WIDTH / 2, INDENT_TOP + HEIGHT + 1);
		cout << *tmpStr << "            ";
	}
	delete str, tmpStr;
	str = nullptr;
	tmpStr = nullptr;
}

