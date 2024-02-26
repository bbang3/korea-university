#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>

// COSE101, Spring Semester, 2019
// Computer Science and Engineering, College of Informatics, Korea University, Seoul.

#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define UP 72
#define SPACE 32 
#define p 112 
#define P 80 
#define ESC 27

#define MAP_ADJ_X 10
#define MAP_ADJ_Y 5
#define X 8
#define Y 5

typedef struct _block {
	int pos_x; //position of block
	int pos_y;
	int num;
	int nextnum;
	int isactive;
}Block;

Block block, block2;

/* 'DO NOT MODIFY THESE FUNCTIONS.'		'ÀÌ ÇÔ¼öµéÀ» °íÄ¡Áö ¸¶¼¼¿ä.'		 */
void removeCursorFromCMD();
void setCoordinate(int x, int y); //cmd¿¡¼­ Ä¿¼­¸¦ x, y À§Ä¡·Î ¼¼ÆÃ

								  /* display fuctions */
void goMainMenuScreen(); // ¸ÞÀÎ¸Þ´º·Î ÀÌµ¿ Moving to main menu.
void printGameScreen();  // °ÔÀÓÈ­¸é Ãâ·Â	 Printing game screen.
void printPauseScreen(); // ÀÏ½Ã Á¤Áö È­¸é	 Prining pause screen.
void printEndScreen(int endType); //  // ½ºÅ×ÀÌÁö Á¾·á È­¸é Ãâ·Â. ½ºÅ×ÀÌÁöÅ¬¸®¾î, ¸ðµç½ºÅ×ÀÌÁöÅ¬¸®¾î, ÆÐ¹è.
								  //Printing result of end of stages including 'cleared the stage', 'cleared whole stages', and 'lost'.
void clearScreen();
/* 'DO NOT MODIFY THESE FUNCTIONS.' ends	'ÀÌ ÇÔ¼öµéÀ» °íÄ¡Áö ¸¶¼¼¿ä' ³¡.	*/

/* @ --- You can modify following functions ----  ÀÌ ¾Æ·¡¿¡ÀÖ´Â ÇÔ¼öµéÀ» °íÄ¡¼¼¿ä.  */
//Implement these functions below to complete your project.
//ÀÌ ÇÔ¼öµéÀ» ±¸ÇöÇÏ¼Å¼­ ÇÁ·ÎÁ§Æ®¸¦ ¿Ï¼ºÇÏ¼¼¿ä.

/* Block generating functions */
int getRandomNumber();
char getRandomOper();
void newBlock(int);

/* block control and merging functions  */
int takeBlockControl();				//ºí·Ï Á¶ÀÛ ¹æÇâ ÀÔ·Â ¹ÞÀ½			Taking input for moving blocks
void flipBlock();					// Fliping function.
void moveBlock(int);		// ÀÔ·Â ¹ÞÀº ¹æÇâÀ¸·Î ºí·Ï ÀÌµ¿		Moving blocks to corresponding direction of input key.
int checkAdjacentBlock(int, int, int*);	//Checking adjacent blocks for merging conditions
void checkNumber(int*);			// ºí·Ï ÀÎÁ¢ ºí·Ï¼ýÀÚ È®ÀÎ			Checking number of adjacent block
int isNumberBlock(int); // ÇØ´ç Ä­ÀÌ ¼ýÀÚ ºí·ÏÀÎÁö È®ÀÎ
int isOperatorBlock(int); // ÇØ´ç Ä­ÀÌ ¿¬»êÀÚ ºí·ÏÀÎÁö È®ÀÎ

									/* game flow control functions */
int isStageEnd();  // ½ºÅ×ÀÌÁö°¡ ³¡³µ´ÂÁö Ã¼Å©. (½ºÅ×ÀÌÁö Å¬¸®¾î, ¸ðµç ½ºÅ×ÀÌÁö Å¬¸®¾î, ÆÐ¹è)         Checking condition for end of the stage. (stage clear, all stage clear, lost)
void setGameFlow(int type); // °¢ ½ºÅ×ÀÌÁö¿¡ ¸Â´Â °ÔÀÓ È¯°æ ¼¼ÆÃ.   Setting up status values for each stages.

int time_interval_moveBlockDown = 100;
int goal = 64;
int currentStage = 0;
int score = 0;
int best_score = 0;
int gameScreen[X][Y] = { 0 };

int main() {
	system("mode con cols=120 lines=38");
	srand(time(NULL));
	removeCursorFromCMD();
	goMainMenuScreen();

	int count64 = 0; // 64ºí·ÏÀÌ ¸¸µé¾îÁø È½¼ö
	while (1) {
		for (int j = 0; j < 5; j++) {
			int ret = takeBlockControl();
			if (ret == SPACE)
			{
				score += 5;
				break;
			}
			else if (ret == ESC) count64 = 0;
			printGameScreen();
			Sleep(time_interval_moveBlockDown);
		}
		moveBlock(DOWN);
		if (!block.isactive && !block2.isactive) { // ÀÌ ºÎºÐÀº ¶³¾îÁö´Â ºí·ÏÀÌ ¹Ù´ÚÀÌ³ª ´Ù¸¥ºí·Ï¿¡ ´ê¾Ò´ÂÁö Ã¼Å©ÇÕ´Ï´Ù. This statement is cheking that wether the falling blocks got touched by floor or other blocks.
			checkNumber(&count64);
			if (isStageEnd() != 1)
				newBlock(0);
			else if (currentStage == 1) count64 = 0;
		}
	}

	return 0;
}

int getRandomNumber() {
	//return 32;
	int randnum = rand() % 10 + 1;
	if (randnum <= 5) return 2;
	if (randnum <= 8) return 4;
	return 8;
	/*

	ÀÏÁ¤ È®·ü·Î 2, 4, 8 Áß ÇÏ³ª¸¦ »ý¼ºÇÏ¿© ±× °ªÀ» ¸®ÅÏÇÔ.
	Generate 2, 4, or 8 with certain percentage and return the number.

	*/

	// You should modify example value for complete this function.
	//return 2; // example retrun value
}

char getRandomOper() {
	if (currentStage <= 3)
		return '+';

	int randnum = rand() % 2;

	if (randnum == 0) return '+';
	return '-';
	/*

	ÀÏÁ¤ È®·ü·Î +, - Áß ÇÏ³ª¸¦ »ý¼ºÇÏ¿© ±× °ªÀ» ¸®ÅÏÇÔ.
	Generate +, or - with certain percentage and return the number.

	*/

	// You should modify example value for complete this function. ¿¹½Ã°ªÀ» °íÄ¡¼Å¼­ ÇÔ¼ö¸¦ ¿Ï¼ºÇÏ¼¼¿ä.
	//return '+'; // example retrun value 	
}

void newBlock(int isInit) {
	/*

	ÀÌ ÇÔ¼ö´Â »ý¼ºµÈ °ª°ú ¶³¾îÁö±â ½ÃÀÛÇÏ´Â À§Ä¡¸¦ ÀÔ·ÂÇØ¼­ »õ ºí·ÏÀ» ¸¸µì´Ï´Ù.
	±×¸®°í ³ª¼­, ´ÙÀ½ ºí·Ï¿¡ »ç¿ëµÉ »õ °ªÀ» »ý¼ºÇÕ´Ï´Ù.
	This fuction makes new blocks with setting generated value and the initial position where starts falling down.
	After that, generate new number for value of the next block.

	*/

	// You should modify these example values for complete this function.

	if (isInit)
	{
		block.num = getRandomNumber();
		block.nextnum = getRandomNumber();

		block2.num = getRandomOper();
		block2.nextnum = getRandomOper();
	}
	else
	{
		block.num = block.nextnum; // example of setting value
		block.nextnum = getRandomNumber();

		block2.num = block2.nextnum;  // example of setting value
		block2.nextnum = getRandomOper();
	}

	block.pos_x = 0; // example value of position ¿¹½Ã°ª
	block.pos_y = rand() % (Y - 1); // example value of position ¿¹½Ã°ª
	block.isactive = 1;
	gameScreen[block.pos_x][block.pos_y] = block.num;

	block2.pos_x = 0; // example value of position ¿¹½Ã°ª
	block2.pos_y = block.pos_y + 1; // example value of position ¿¹½Ã°ª
	block2.isactive = 1;
	gameScreen[block2.pos_x][block2.pos_y] = block2.num;

}

int takeBlockControl() {
	int input_blockControl = 0;

	if (_kbhit()) {
		input_blockControl = _getch();
		if (input_blockControl == 224 && block.isactive && block2.isactive) { //¹æÇâÅ°ÀÎ°æ¿ì  Cases for direction keys
			do { input_blockControl = _getch(); } while (input_blockControl == 224);//¹æÇâÅ°Áö½Ã°ªÀ» ¹ö¸²  dropping the value of direction
			switch (input_blockControl) {
			case UP:
				flipBlock();
				break;
			case DOWN:
				moveBlock(DOWN);
				break;
			case LEFT:
				moveBlock(LEFT);
				break;
			case RIGHT:
				moveBlock(RIGHT);
				break;
				/*

				Implement cases of each dilection key
				°¢ ¹æÇâÅ°¿¡ ´ëÇÑ °æ¿ìµéÀ» ±¸ÇöÇÏ¼¼¿ä

				*/
			}
		}
		else { //¹æÇâÅ°°¡ ¾Æ´Ñ°æ¿ì  Cases for other keys
			switch (input_blockControl) {
			case SPACE:
				if (block.isactive) {
					int lowestx = block.pos_x + 1;
					while (lowestx < X && gameScreen[lowestx][block.pos_y] == 0)
						lowestx++;

					gameScreen[block.pos_x][block.pos_y] = 0;
					gameScreen[lowestx - 1][block.pos_y] = block.num;
					block.pos_x = lowestx - 1;
					block.isactive = 0;
				}
				if (block2.isactive) {
					int lowestx = block2.pos_x + 1;
					while (lowestx < X && gameScreen[lowestx][block2.pos_y] == 0)
						lowestx++;

					gameScreen[block2.pos_x][block2.pos_y] = 0;
					gameScreen[lowestx - 1][block2.pos_y] = block2.num;
					block2.pos_x = lowestx - 1;
					block2.isactive = 0;
				}
				return SPACE;
				/*

				Implement hard drop
				ÇÏµåµå¶øÀ» ±¸ÇöÇÏ¼¼¿ä

				*/
			case P:
			case p:
				printPauseScreen();
				break;
			case ESC:
				printEndScreen(0);
				return ESC;
				break;
			}
		}
	}
	while (_kbhit()) _getch(); //Å°¹öÆÛ¸¦ ºñ¿ò. Emptying key buffer.
	return 0;
}

void flipBlock(void) {//ÁÂ¿ì ¹ÝÀüÇÔ¼ö		Block flipping function
	if (!block.isactive || !block2.isactive) return;

	Block temp;
	temp = block;

	block.num = block2.num;
	gameScreen[block.pos_x][block.pos_y] = block2.num;

	block2.num = temp.num;
	gameScreen[block2.pos_x][block2.pos_y] = temp.num;
	/*

	Fliping blocks when the 'up' butten is pressed.
	À§·Î ¹öÆ°ÀÌ ´­·ÈÀ»°æ¿ì ºí·ÏÀ» ¹Ù²ß´Ï´Ù.

	*/
}

void moveBlock(int direction) { // ÁÂ,¿ì,¾Æ·¡ ÀÔ·Â½Ã ¿òÁ÷ÀÓ ÇÔ¼ö	Moving blocks for left, reight, and down input
	switch (direction) {
	case LEFT:
		if (block.pos_y > 0 && gameScreen[block.pos_x][block.pos_y - 1] == 0) {
			gameScreen[block.pos_x][block.pos_y - 1] = block.num;
			gameScreen[block.pos_x][block.pos_y] = 0;
			block.pos_y--;
		}
		if (block2.pos_y > 0 && gameScreen[block2.pos_x][block2.pos_y - 1] == 0) {
			gameScreen[block2.pos_x][block2.pos_y - 1] = block2.num;
			gameScreen[block2.pos_x][block2.pos_y] = 0;
			block2.pos_y--;
		}

		break;
	case RIGHT:
		if (block2.pos_y < Y - 1 && gameScreen[block2.pos_x][block2.pos_y + 1] == 0) {
			gameScreen[block2.pos_x][block2.pos_y + 1] = block2.num;
			gameScreen[block2.pos_x][block2.pos_y] = 0;
			block2.pos_y++;
		}
		if (block.pos_y < Y - 1 && gameScreen[block.pos_x][block.pos_y + 1] == 0) {
			gameScreen[block.pos_x][block.pos_y + 1] = block.num;
			gameScreen[block.pos_x][block.pos_y] = 0;
			block.pos_y++;
		}

		break;
	case DOWN:
		if (block.pos_x < X - 1 && gameScreen[block.pos_x + 1][block.pos_y] == 0) {
			gameScreen[block.pos_x + 1][block.pos_y] = block.num;
			gameScreen[block.pos_x][block.pos_y] = 0;
			block.pos_x++;
		}
		else {
			block.isactive = 0;
		}

		if (block2.pos_x < X - 1 && gameScreen[block2.pos_x + 1][block2.pos_y] == 0) {
			gameScreen[block2.pos_x + 1][block2.pos_y] = block2.num;
			gameScreen[block2.pos_x][block2.pos_y] = 0;
			block2.pos_x++;
		}
		else {
			block2.isactive = 0;
		}
	}
}

int checkAdjacentBlock(int x, int y, int* count64) { //Merging Á¶°Ç È®ÀÎ ÇÔ¼ö		Checking merging condition

	// ¼öÆò ¹æÇâ °Ë»ç
	if (y <= Y - 3 && isOperatorBlock(gameScreen[x][y + 1]) && isNumberBlock(gameScreen[x][y + 2]))
	{
		if (y == 0 && isOperatorBlock(gameScreen[x][y + 3]) && isNumberBlock(gameScreen[x][y + 4]))
		{
			if (gameScreen[x][y + 1] == '-' && gameScreen[x][y + 3] == '+')
			{
				gameScreen[x][y] = abs(gameScreen[x][y] - (gameScreen[x][y + 2] + gameScreen[x][y + 4]));
			}
			else
			{
				gameScreen[x][y] = gameScreen[x][y + 1] == '+' ?
					(gameScreen[x][y] + gameScreen[x][y + 2]) : abs(gameScreen[x][y] - gameScreen[x][y + 2]);
				gameScreen[x][y] = gameScreen[x][y + 3] == '+' ?
					(gameScreen[x][y] + gameScreen[x][y + 4]) : abs(gameScreen[x][y] - gameScreen[x][y + 4]);

			}
			gameScreen[x][y + 1] = 0;
			gameScreen[x][y + 2] = 0;
			gameScreen[x][y + 3] = 0;
			gameScreen[x][y + 4] = 0;
		}
		else
		{

			if (gameScreen[x][y + 1] == '+')
				gameScreen[x][y] += gameScreen[x][y + 2];
			else
				gameScreen[x][y] = abs(gameScreen[x][y] - gameScreen[x][y + 2]);

			if (gameScreen[x][y] == 0)
				gameScreen[x][y] = 999;
			else if (gameScreen[x][y] == 64)
			{
				(*count64)++;
				score += (*count64 - 1) * 10;

				gameScreen[x][y] = 0;
			}
			gameScreen[x][y + 1] = 0;
			gameScreen[x][y + 2] = 0;
		}
		return 1;
	}
	// ¼öÁ÷ ¹æÇâ °Ë»ç
	if (x >= 3 && isOperatorBlock(gameScreen[x - 1][y]) && isNumberBlock(gameScreen[x - 2][y]))
	{
		if (gameScreen[x - 1][y] == '+')
			gameScreen[x][y] += gameScreen[x - 2][y];
		else
			gameScreen[x][y] = abs(gameScreen[x][y] - gameScreen[x - 2][y]);

		if (gameScreen[x][y] == 0)
			gameScreen[x][y] = 999;
		else if (gameScreen[x][y] == 64)
		{
			(*count64)++;
			score += (*count64 - 1) * 10;

			gameScreen[x][y] = 0;
		}

		gameScreen[x - 1][y] = 0;
		gameScreen[x - 2][y] = 0;
		return 1;
	}

	for (int i = X - 1; i >= 1; i--)
	{
		for (int j = 0; j < Y; j++)
		{
			if (gameScreen[i][j] == 0 && gameScreen[i - 1][j])
			{
				gameScreen[i][j] = gameScreen[i - 1][j];
				gameScreen[i - 1][j] = 0;
			}
		}
	}
	/*

	Á¶°ÇÈ®ÀÎÇÏ¿© ¿¬»êÇÏ°í Merging
	64 »ý¼º½Ã Á¡¼ö °è»ê

	Implement checking condition of Merging.
	If 64 is found, player earns points.

	*/

	return 0; //example value. 0 for none, 1 for mergable.  ¿¹½Ã°ª. 0ÀÌ¸é ÇÕÄ¥ ºí·ÏÀÌ ¾ø°í, 1ÀÌ¸é ÀÖÀ½.
}

void checkNumber(int* count64) {
	int isMergeable = 1;
	while (isMergeable)
	{
		isMergeable = 0;
		for (int i = X - 1; i > 0; i--) {
			for (int j = 0; j < Y; j++) {
				if (isNumberBlock(gameScreen[i][j]) && checkAdjacentBlock(i, j, count64)) {
					isMergeable = 1;
					Sleep(200);
				}
			}
			if (isMergeable)
				break;
		}
	}
}

int isNumberBlock(int blockValue)
{
	return blockValue != 0 && blockValue != 999 && blockValue != '+' && blockValue != '-';
}

int isOperatorBlock(int blockValue)
{
	return blockValue == '+' || blockValue == '-';
}

void setGameFlow(int setGameFlowType) { // Implement this function to set conditions of stages.  ½ºÅ×ÀÌÁöÀÇ Á¶°ÇµéÀ» ¼³Á¤ÇÏ±âÀ§ÇØ ÀÌ ÇÔ¼ö¸¦ ¿Ï¼ºÇÏ¼¼¿ä.
	switch (setGameFlowType) {
	case 0:  /* initialize game */
		if (best_score < score)
			best_score = score;

		currentStage = 0;
		score = 0;
	case 1: /* To next stage */
		currentStage++;

		switch (currentStage) {
		case 1:
			time_interval_moveBlockDown = 100;
			goal = 100;
			break;
		case 2:
			time_interval_moveBlockDown = 77;
			goal = 300;
			break;
		case 3:
			time_interval_moveBlockDown = 66;
			goal = 500;
			break;
		case 4:
			time_interval_moveBlockDown = 59;
			goal = 600;
			break;
		case 5:
			time_interval_moveBlockDown = 50;
			goal = 750;
			break;
		case 6:
			time_interval_moveBlockDown = 40;
			goal = 850;
			break;
		}
		clearScreen();
		newBlock(1);
		break;

	default:
		clearScreen();
		setCoordinate(MAP_ADJ_X + 2, MAP_ADJ_Y + 13); printf(" @System error: state end condition currupted.");
		exit(0);
	}
}

int isStageEnd() { // ½ºÅ×ÀÌÁö°¡ ³¡³µ´ÂÁö Ã¼Å©      Checking if stage ended up or not.
	for (int i = X - 1; i > 1; i--) {
		for (int j = 0; j < Y; j++) {
			if (score >= goal) {
				currentStage < 6 ? printEndScreen(1) : printEndScreen(2);
				return 1;
			}
		}
	}
	for (int i = 0; i < Y; i++) {
		if (gameScreen[0][i] != 0) { //ÆÐ¹è			Game over
			printEndScreen(0);
			return 1;
		}
	}
	return 0;
}

/* @ ¡é¡é¡é'DO NOT MODIFY FROM HERE.'¡é¡é¡é starts		 ÀÌ ¾Æ·¡·Î´Â ¼öÁ¤ÇÏÁö ¸¶¼¼¿ä.	*/
void removeCursorFromCMD() { // cmd¿¡¼­ ±ôºýÀÌ´Â Ä¿¼­¾ø¾Ú Concealing cursor from cmd.
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void setCoordinate(int x, int y) { //cmd¿¡¼­ Ä¿¼­ À§Ä¡¸¦ x, y·Î ¼¼ÆÃ   Setting curser to (x,y).
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printGameScreen() {
	int i, j, k;

	for (i = 0; i < X; i++) {
		for (j = 0; j < Y; j++) {
			setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
			if (gameScreen[i][j] == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
				if (i == 0) {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) - 1);
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("     ¦»      ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("-------------");
				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("     ¦»      ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("             ");
				}
			}

			else {
				for (k = 0; k < 12;) if (1 << (++k) == gameScreen[i][j]) break;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k + 1);
				if (gameScreen[i][j] == 45 || gameScreen[i][j] == 43)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

				if (i == 0) {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) - 1);
					printf("¦£¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤ ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					if (gameScreen[i][j] == 45)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 43)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 999)
						printf("      *", gameScreen[i][j]);
					else
						printf("   %4d", gameScreen[i][j]);
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥ ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);

				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("¦£¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤  ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					if (gameScreen[i][j] == 45)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 43)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 999)
						printf("      *", gameScreen[i][j]);
					else
						printf("   %4d", gameScreen[i][j]);
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥  ");
				}

			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y); printf(" Stage : %4d", currentStage);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 1); printf(" GOAL  : %4d", goal);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 2); printf(" SPEED  : %.1f", 100.0 / (float)time_interval_moveBlockDown);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 3); printf("+-  N E X T  -+ ");
	for (k = 0; k < 12;) if (1 << (++k) == block.nextnum) break;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k + 1);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 4); printf("  ¦£¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 5); printf("     %4d    ", block.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 6); printf("  ¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 7); printf("  ¦£¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¤    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 8); printf("     %4c    ", block2.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 9); printf("  ¦¦¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¥    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 10); printf("+-- -  -  - --+ ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 11); printf(" YOUR SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 12); printf("        %6d", score);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 13); printf(" BEST SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 14); printf("        %6d", best_score);

	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 15); printf("  ¡â   : Flip");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 16); printf("¢·  ¢¹ : Left / Right");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 17); printf("  ¡ä   : Soft Drop ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 18); printf(" SPACE : Hard Drop");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 19); printf("   P   : Pause");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 20); printf("  ESC  : Quit");

}

void goMainMenuScreen() { //½ÃÀÛÈ­¸é Ãâ·Â ÇÔ¼ö. Printing out main menu.
	int x = 19;
	int y = 9;
	int input_command = 0;

	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("o ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡ 64 PuyoPuyo ¡Ù¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡ 64 PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡ 64 PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡¦¡");
	Sleep(30);
	setCoordinate(x, y + 2); printf("¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡¦¡  64 PuyoPuyo ¡Ù ¦¡");


	setCoordinate(x + 2, y + 4); printf(" Press any key to start");

	setCoordinate(x, y + 6);  printf(" ¢Ç¢Ç¢Ç¢Ç HOW TO CONTROL ¢Ç¢Ç¢Ç¢Ç");
	setCoordinate(x, y + 7);  printf(" ¢Ç                            ¢Ç");
	setCoordinate(x, y + 8);  printf(" ¢Ç      ¡â   : Flip           ¢Ç");
	setCoordinate(x, y + 9); printf(" ¢Ç    ¢·  ¢¹ : Left / Right   ¢Ç");
	setCoordinate(x, y + 10); printf(" ¢Ç      ¡ä   : Soft Drop      ¢Ç");
	setCoordinate(x, y + 11); printf(" ¢Ç    SPACE  : Hard Drop      ¢Ç");
	setCoordinate(x, y + 12); printf(" ¢Ç      P    : Pause          ¢Ç");
	setCoordinate(x, y + 13); printf(" ¢Ç     ESC   : Quit           ¢Ç");
	setCoordinate(x, y + 14); printf(" ¢Ç                            ¢Ç");
	setCoordinate(x, y + 15); printf(" ¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");

	setCoordinate(x, y + 16); printf("          ESC: Exit game.     ");



	while (_kbhit())
		_getch();
	input_command = _getch();


	if (input_command == ESC) {
		clearScreen();
		exit(0);
	}
	else
		setGameFlow(0); /* Reset the game */

}

void printPauseScreen() { // °ÔÀÓ ÀÏ½ÃÁ¤Áö		Pause game.
	int x = MAP_ADJ_X + 8;
	int y = MAP_ADJ_Y + 5;

	setCoordinate(x, y + 0); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
	setCoordinate(x, y + 1); printf("¢Ç                              ¢Ç");
	setCoordinate(x, y + 2); printf("¢Ç  +-----------------------+   ¢Ç");
	setCoordinate(x, y + 3); printf("¢Ç  |       P A U S E       |   ¢Ç");
	setCoordinate(x, y + 4); printf("¢Ç  +-----------------------+   ¢Ç");
	setCoordinate(x, y + 5); printf("¢Ç   Press any key to resume.   ¢Ç");
	setCoordinate(x, y + 6); printf("¢Ç                              ¢Ç");
	setCoordinate(x, y + 7); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");

	_getch();

	system("cls");
	printGameScreen();
}

void printEndScreen(int endType) {
	int input_command = 0;
	int x = MAP_ADJ_X + 8;

	switch (endType) {
	case 0:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("¢Ç                              ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("¢Ç  +-----------------------+   ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("¢Ç  |  G A M E  O V E R..   |   ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("¢Ç  +-----------------------+   ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("¢Ç   YOUR SCORE: %6d         ¢Ç", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("¢Ç                              ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("¢Ç   Press any key to restart.  ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("¢Ç                              ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 16); printf("      ESC: Back to main menu.     ");

		while (_kbhit()) _getch();
		input_command = _getch();

		if (input_command == ESC) {
			while (_kbhit()) _getch();
			clearScreen();
			goMainMenuScreen();
			return;
		}

		setGameFlow(0); /* Reset the game */
		break;

	case 1:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("¢Ç                              ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("¢Ç  +-----------------------+   ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("¢Ç  |      STAGE CLEAR      |   ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("¢Ç  +-----------------------+   ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("¢Ç   YOUR SCORE: %6d         ¢Ç", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("¢Ç                              ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("¢Ç Press any key to next stage. ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("¢Ç                              ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");

		while (_kbhit()) _getch();
		input_command = _getch();

		setGameFlow(1); /* To next stage */
		break;

	case 2:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("¢Ç                                      ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("¢Ç  +-------------------------------+   ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("¢Ç  | WINNER WINNER CHICKEN DINNER! |   ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("¢Ç  +-------------------------------+   ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("¢Ç         YOUR SCORE: %6d           ¢Ç", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("¢Ç                                      ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("¢Ç         Cleared final stage.         ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("¢Ç                                      ¢Ç");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç¢Ç");

		while (_kbhit()) _getch();
		input_command = _getch();
		setGameFlow(0); /* Reset the game */
		break;

	default:
		clearScreen();
		setCoordinate(x, MAP_ADJ_Y + 13); printf(" @System error: state end condition currupted.");
		exit(0);
	}
}

void clearScreen() {
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			gameScreen[i][j] = 0;
		}
	}
	block.pos_x = 0;
	block.pos_x = 0;
	block.num = 0;
	block.nextnum = 0;
	block.isactive = 0;

	block2.pos_x = 0;
	block2.pos_x = 0;
	block2.num = 0;
	block2.nextnum = 0;
	block2.isactive = 0;

	system("cls");
}
/* @ '¡è¡è¡èDO NOT MODIFY.¡è¡è¡è'ends. */