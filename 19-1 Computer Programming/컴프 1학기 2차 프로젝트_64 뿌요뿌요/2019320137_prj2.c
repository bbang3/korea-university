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

/* 'DO NOT MODIFY THESE FUNCTIONS.'		'檜 л熱菟擊 堅纂雖 葆撮蹂.'		 */
void removeCursorFromCMD();
void setCoordinate(int x, int y); //cmd縑憮 醴憮蒂 x, y 嬪纂煎 撮た

								  /* display fuctions */
void goMainMenuScreen(); // 詭檣詭景煎 檜翕 Moving to main menu.
void printGameScreen();  // 啪歜�飛� 轎溘	 Printing game screen.
void printPauseScreen(); // 橾衛 薑雖 �飛�	 Prining pause screen.
void printEndScreen(int endType); //  // 蝶纔檜雖 謙猿 �飛� 轎溘. 蝶纔檜雖贗葬橫, 賅萇蝶纔檜雖贗葬橫, ぬ寡.
								  //Printing result of end of stages including 'cleared the stage', 'cleared whole stages', and 'lost'.
void clearScreen();
/* 'DO NOT MODIFY THESE FUNCTIONS.' ends	'檜 л熱菟擊 堅纂雖 葆撮蹂' 部.	*/

/* @ --- You can modify following functions ----  檜 嬴楚縑氈朝 л熱菟擊 堅纂撮蹂.  */
//Implement these functions below to complete your project.
//檜 л熱菟擊 掘⑷ж敷憮 Щ煎薛お蒂 諫撩ж撮蹂.

/* Block generating functions */
int getRandomNumber();
char getRandomOper();
void newBlock(int);

/* block control and merging functions  */
int takeBlockControl();				//綰煙 褻濛 寞щ 殮溘 嫡擠			Taking input for moving blocks
void flipBlock();					// Fliping function.
void moveBlock(int);		// 殮溘 嫡擎 寞щ戲煎 綰煙 檜翕		Moving blocks to corresponding direction of input key.
int checkAdjacentBlock(int, int, int*);	//Checking adjacent blocks for merging conditions
void checkNumber(int*);			// 綰煙 檣蕾 綰煙璋濠 �挫�			Checking number of adjacent block
int isNumberBlock(int); // п渡 蘊檜 璋濠 綰煙檣雖 �挫�
int isOperatorBlock(int); // п渡 蘊檜 翱骯濠 綰煙檣雖 �挫�

									/* game flow control functions */
int isStageEnd();  // 蝶纔檜雖陛 部陬朝雖 羹觼. (蝶纔檜雖 贗葬橫, 賅萇 蝶纔檜雖 贗葬橫, ぬ寡)         Checking condition for end of the stage. (stage clear, all stage clear, lost)
void setGameFlow(int type); // 陝 蝶纔檜雖縑 蜃朝 啪歜 �秣� 撮た.   Setting up status values for each stages.

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

	int count64 = 0; // 64綰煙檜 虜菟橫霞 �蝦�
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
		if (!block.isactive && !block2.isactive) { // 檜 睡碟擎 雲橫雖朝 綰煙檜 夥款檜釭 棻艇綰煙縑 湊懊朝雖 羹觼м棲棻. This statement is cheking that wether the falling blocks got touched by floor or other blocks.
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

	橾薑 �捕�煎 2, 4, 8 醞 ж釭蒂 儅撩ж罹 斜 高擊 葬欐л.
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

	橾薑 �捕�煎 +, - 醞 ж釭蒂 儅撩ж罹 斜 高擊 葬欐л.
	Generate +, or - with certain percentage and return the number.

	*/

	// You should modify example value for complete this function. 蕨衛高擊 堅纂敷憮 л熱蒂 諫撩ж撮蹂.
	//return '+'; // example retrun value 	
}

void newBlock(int isInit) {
	/*

	檜 л熱朝 儅撩脹 高婁 雲橫雖晦 衛濛ж朝 嬪纂蒂 殮溘п憮 億 綰煙擊 虜蛙棲棻.
	斜葬堅 釭憮, 棻擠 綰煙縑 餌辨腆 億 高擊 儅撩м棲棻.
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

	block.pos_x = 0; // example value of position 蕨衛高
	block.pos_y = rand() % (Y - 1); // example value of position 蕨衛高
	block.isactive = 1;
	gameScreen[block.pos_x][block.pos_y] = block.num;

	block2.pos_x = 0; // example value of position 蕨衛高
	block2.pos_y = block.pos_y + 1; // example value of position 蕨衛高
	block2.isactive = 1;
	gameScreen[block2.pos_x][block2.pos_y] = block2.num;

}

int takeBlockControl() {
	int input_blockControl = 0;

	if (_kbhit()) {
		input_blockControl = _getch();
		if (input_blockControl == 224 && block.isactive && block2.isactive) { //寞щ酈檣唳辦  Cases for direction keys
			do { input_blockControl = _getch(); } while (input_blockControl == 224);//寞щ酈雖衛高擊 幗葡  dropping the value of direction
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
				陝 寞щ酈縑 渠и 唳辦菟擊 掘⑷ж撮蹂

				*/
			}
		}
		else { //寞щ酈陛 嬴棋唳辦  Cases for other keys
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
				ж萄萄奧擊 掘⑷ж撮蹂

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
	while (_kbhit()) _getch(); //酈幗ぷ蒂 綠遺. Emptying key buffer.
	return 0;
}

void flipBlock(void) {//謝辦 奩瞪л熱		Block flipping function
	if (!block.isactive || !block2.isactive) return;

	Block temp;
	temp = block;

	block.num = block2.num;
	gameScreen[block.pos_x][block.pos_y] = block2.num;

	block2.num = temp.num;
	gameScreen[block2.pos_x][block2.pos_y] = temp.num;
	/*

	Fliping blocks when the 'up' butten is pressed.
	嬪煎 幗が檜 揚溜擊唳辦 綰煙擊 夥習棲棻.

	*/
}

void moveBlock(int direction) { // 謝,辦,嬴楚 殮溘衛 遺霜歜 л熱	Moving blocks for left, reight, and down input
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

int checkAdjacentBlock(int x, int y, int* count64) { //Merging 褻勒 �挫� л熱		Checking merging condition

	// 熱ゎ 寞щ 匐餌
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
	// 熱霜 寞щ 匐餌
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

	褻勒�挫恉狤� 翱骯ж堅 Merging
	64 儅撩衛 薄熱 啗骯

	Implement checking condition of Merging.
	If 64 is found, player earns points.

	*/

	return 0; //example value. 0 for none, 1 for mergable.  蕨衛高. 0檜賊 м艦 綰煙檜 橈堅, 1檜賊 氈擠.
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

void setGameFlow(int setGameFlowType) { // Implement this function to set conditions of stages.  蝶纔檜雖曖 褻勒菟擊 撲薑ж晦嬪п 檜 л熱蒂 諫撩ж撮蹂.
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

int isStageEnd() { // 蝶纔檜雖陛 部陬朝雖 羹觼      Checking if stage ended up or not.
	for (int i = X - 1; i > 1; i--) {
		for (int j = 0; j < Y; j++) {
			if (score >= goal) {
				currentStage < 6 ? printEndScreen(1) : printEndScreen(2);
				return 1;
			}
		}
	}
	for (int i = 0; i < Y; i++) {
		if (gameScreen[0][i] != 0) { //ぬ寡			Game over
			printEndScreen(0);
			return 1;
		}
	}
	return 0;
}

/* @ ⊿⊿⊿'DO NOT MODIFY FROM HERE.'⊿⊿⊿ starts		 檜 嬴楚煎朝 熱薑ж雖 葆撮蹂.	*/
void removeCursorFromCMD() { // cmd縑憮 梭緇檜朝 醴憮橈據 Concealing cursor from cmd.
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void setCoordinate(int x, int y) { //cmd縑憮 醴憮 嬪纂蒂 x, y煎 撮た   Setting curser to (x,y).
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
					printf("     氖      ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("-------------");
				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("     氖      ");
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
					printf("忙式式式式式式式式式忖 ");
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
					printf("戌式式式式式式式式式戎 ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);

				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("忙式式式式式式式式式忖  ");
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
					printf("戌式式式式式式式式式戎  ");
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
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 4); printf("  忙式式式式式式式式式忖    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 5); printf("     %4d    ", block.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 6); printf("  戌式式式式式式式式式戎    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 7); printf("  忙式式式式式式式式式忖    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 8); printf("     %4c    ", block2.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 9); printf("  戌式式式式式式式式式戎    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 10); printf("+-- -  -  - --+ ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 11); printf(" YOUR SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 12); printf("        %6d", score);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 13); printf(" BEST SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 14); printf("        %6d", best_score);

	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 15); printf("  ﹦   : Flip");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 16); printf("８  Ⅰ : Left / Right");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 17); printf("  ∩   : Soft Drop ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 18); printf(" SPACE : Hard Drop");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 19); printf("   P   : Pause");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 20); printf("  ESC  : Quit");

}

void goMainMenuScreen() { //衛濛�飛� 轎溘 л熱. Printing out main menu.
	int x = 19;
	int y = 9;
	int input_command = 0;

	setCoordinate(x, y + 2); printf("式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("≧ 式式式式式式式式式式式式式式式式式式式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("o ≧ 式式式式式式式式式式式式式式式式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yo ≧ 式式式式式式式式式式式式式式式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yoPuyo ≧ 式式式式式式式式式式式式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("PuyoPuyo ≧ 式式式式式式式式式式式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式 64 PuyoPuyo ≧式式式式式式式式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式 64 PuyoPuyo ≧ 式式式式式式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式 64 PuyoPuyo ≧ 式式式式式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式  64 PuyoPuyo ≧ 式式式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式式式  64 PuyoPuyo ≧ 式式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式式式式  64 PuyoPuyo ≧ 式式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式式式式式  64 PuyoPuyo ≧ 式式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式式式式式式  64 PuyoPuyo ≧ 式式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式式式式式式式  64 PuyoPuyo ≧ 式式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式式式式式式式式  64 PuyoPuyo ≧ 式式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式式式式式式式式式  64 PuyoPuyo ≧ 式式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式式式式式式式式式式  64 PuyoPuyo ≧ 式式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式式式式式式式式式式式  64 PuyoPuyo ≧ 式式");
	Sleep(30);
	setCoordinate(x, y + 2); printf("式式式式式式式式式式式式式式式式  64 PuyoPuyo ≧ 式");


	setCoordinate(x + 2, y + 4); printf(" Press any key to start");

	setCoordinate(x, y + 6);  printf(" 〥〥〥〥 HOW TO CONTROL 〥〥〥〥");
	setCoordinate(x, y + 7);  printf(" 〥                            〥");
	setCoordinate(x, y + 8);  printf(" 〥      ﹦   : Flip           〥");
	setCoordinate(x, y + 9); printf(" 〥    ８  Ⅰ : Left / Right   〥");
	setCoordinate(x, y + 10); printf(" 〥      ∩   : Soft Drop      〥");
	setCoordinate(x, y + 11); printf(" 〥    SPACE  : Hard Drop      〥");
	setCoordinate(x, y + 12); printf(" 〥      P    : Pause          〥");
	setCoordinate(x, y + 13); printf(" 〥     ESC   : Quit           〥");
	setCoordinate(x, y + 14); printf(" 〥                            〥");
	setCoordinate(x, y + 15); printf(" 〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥");

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

void printPauseScreen() { // 啪歜 橾衛薑雖		Pause game.
	int x = MAP_ADJ_X + 8;
	int y = MAP_ADJ_Y + 5;

	setCoordinate(x, y + 0); printf("〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥");
	setCoordinate(x, y + 1); printf("〥                              〥");
	setCoordinate(x, y + 2); printf("〥  +-----------------------+   〥");
	setCoordinate(x, y + 3); printf("〥  |       P A U S E       |   〥");
	setCoordinate(x, y + 4); printf("〥  +-----------------------+   〥");
	setCoordinate(x, y + 5); printf("〥   Press any key to resume.   〥");
	setCoordinate(x, y + 6); printf("〥                              〥");
	setCoordinate(x, y + 7); printf("〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥");

	_getch();

	system("cls");
	printGameScreen();
}

void printEndScreen(int endType) {
	int input_command = 0;
	int x = MAP_ADJ_X + 8;

	switch (endType) {
	case 0:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("〥                              〥");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("〥  +-----------------------+   〥");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("〥  |  G A M E  O V E R..   |   〥");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("〥  +-----------------------+   〥");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("〥   YOUR SCORE: %6d         〥", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("〥                              〥");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("〥   Press any key to restart.  〥");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("〥                              〥");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥");
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
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("〥                              〥");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("〥  +-----------------------+   〥");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("〥  |      STAGE CLEAR      |   〥");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("〥  +-----------------------+   〥");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("〥   YOUR SCORE: %6d         〥", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("〥                              〥");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("〥 Press any key to next stage. 〥");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("〥                              〥");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥");

		while (_kbhit()) _getch();
		input_command = _getch();

		setGameFlow(1); /* To next stage */
		break;

	case 2:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("〥                                      〥");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("〥  +-------------------------------+   〥");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("〥  | WINNER WINNER CHICKEN DINNER! |   〥");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("〥  +-------------------------------+   〥");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("〥         YOUR SCORE: %6d           〥", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("〥                                      〥");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("〥         Cleared final stage.         〥");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("〥                                      〥");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥〥");

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
/* @ '∟∟∟DO NOT MODIFY.∟∟∟'ends. */