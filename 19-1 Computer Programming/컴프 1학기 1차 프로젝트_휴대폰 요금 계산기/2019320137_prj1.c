#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BASIC 0
#define MOREDATA 1

void displayMenu();
void displayPlan();
void setPastInfoRandomly(int[], int[], int[], double[]);
void displayPastUsage(int, int[], int[], double[]);
void setThisMonthUsage(int[], int[], double[]);
void printthisMonthUsage(int[], int[], double[]);
void calculateFee(int, int[], int[], double[], double[], int[]);
void displayBillings(int, int[], int[], double[], double[], int[]);
void recommendBestPlan(int, int[], int[], double[]);

int main()
{
	srand((unsigned int)time(NULL));
	int command;
	char ch = 'Y';
	int plan[2] = { 0, }, voiceUsage[3] = { 0, }, textUsage[3] = { 0, }, fee[3] = { 0, };
	double dataUsage[3] = { 0, }, extraData[3] = { 0, };

	setPastInfoRandomly(plan, voiceUsage, textUsage, dataUsage);
	while (1)
	{
		if (ch == 'Y')
		{
			ch = -1;
			system("cls");
			displayMenu();
			scanf("%d", &command);
		}
		system("cls");

		switch (command)
		{
		case 1:
			displayPlan();
			break;
		case 2:
			displayPastUsage(plan[0], voiceUsage, textUsage, dataUsage);
			break;
		case 3:
			setThisMonthUsage(voiceUsage, textUsage, dataUsage);
			break;
		case 4:
			calculateFee(plan[0], voiceUsage, textUsage, dataUsage, extraData, fee);
			displayBillings(plan[0], voiceUsage, textUsage, dataUsage, extraData, fee);
			break;
		case 5:
			recommendBestPlan(plan[0], voiceUsage, textUsage, dataUsage);
			break;
		case 6:
			return 0;
			break;
		default:
			printf("Invalid Input\n");
			break;
		}
		
		printf("Back to Main Menu (Y/N) : ");
		scanf(" %c", &ch);

		if (ch != 'Y' && command == 3)
		{
			system("cls");
			while (ch != 'Y')
			{
				printthisMonthUsage(voiceUsage, textUsage, dataUsage);
				printf("Back to Main Menu (Y/N) : ");
				scanf("\n%c", &ch);
				system("cls");
			}
		}
	}

	return 0;
}

void displayMenu()
{
	printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖\n");
	printf("弛           KU Mobile Phone Bill Calculator\t\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛\t1 : Display Plans\t\t\t\t弛\n");
	printf("弛\t2 : Usage information for past two months\t弛\n");
	printf("弛\t3 : Input this month usage\t\t\t弛\n");
	printf("弛\t4 : Toal fee for past 3 months\t\t\t弛\n");
	printf("弛\t5 : Best plan recommendation\t\t\t弛\n");
	printf("弛\t6 : Exit\t\t\t\t\t弛\n");
	printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n");
}

void displayPlan()
{
	printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖\n");
	printf("弛                      Basic Plan\t\t\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Included in plan\t\t弛 Additional usages\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Voice\t\t弛 500m free\t弛 \\ 50 / 1m\t\t弛\n");
	printf("弛 Text\t\t弛 100t free\t弛 \\ 10 / 1t\t\t弛\n");
	printf("弛 Data\t\t弛 2.00GB free\t弛 \\ 1000 / 0.1GB\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Basic fee\t弛 \\ 20000\t\t\t\t弛\n");
	printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n");
	printf("\n");

	printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖\n");
	printf("弛                    More Data Plan\t\t\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Included in plan\t\t弛 Additional usages\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Voice\t\t弛 300m free\t弛 \\ 10 / 1m\t\t弛\n");
	printf("弛 Text\t\t弛 100t free\t弛 \\ 30 / 1t\t\t弛\n");
	printf("弛 Data\t\t弛 30.00GB free\t弛 \\ 500 / 0.1GB\t\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Basic fee\t弛 \\ 45000\t\t\t\t弛\n");
	printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n");
}

void setPastInfoRandomly(int plan[], int voiceUsage[], int textUsage[], double dataUsage[])
{
	int i;

	plan[0] = rand() % 2;

	for (i = 0; i <= 1; i++)
	{
		voiceUsage[i] = rand() % 700 + 1;
		textUsage[i] = rand() % 301;
		dataUsage[i] = (rand() / (double)RAND_MAX) * (149.5) + 0.5;
		dataUsage[i] = (int)(dataUsage[i] * 100) / 100.0;
		//dataUsage[i] = (rand() % 14951)/ 100.0 + 0.5;
	}

	return;
}

void displayPastUsage(int currentPlan, int voiceUsage[], int textUsage[], double dataUsage[])
{
	printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖\n");
	printf("弛          Last two months usage\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 My Plan : ");
	currentPlan == BASIC ? printf("Basic Plan\t") : printf("More Data Plan");
	printf(" \t\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛\t弛    February\t弛      March\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Voice\t弛   %6d min\t弛   %6d min\t弛\n",voiceUsage[0], voiceUsage[1]);
	printf("弛 Text\t弛   %6d text\t弛   %6d text\t弛\n", textUsage[0], textUsage[1]);
	printf("弛 Data\t弛   %6.2lf GB\t弛   %6.2lf GB\t弛\n", dataUsage[0], dataUsage[1]);
	printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n");

}

void setThisMonthUsage(int voiceUsage[], int textUsage[], double dataUsage[])
{
	printf("========================================\n");
	printf("Input your usages of voice : ");
	scanf("%d", &voiceUsage[2]);
	printf("Input your usages of text : ");
	scanf("%d", &textUsage[2]);
	printf("Input your usages of data : ");
	scanf("%lf", &dataUsage[2]);

	return;
}

void printthisMonthUsage(int voiceUsage[], int textUsage[], double dataUsage[])
{
	printf("========================================\n");
	printf("Input your usages of voice : %d\n", voiceUsage[2]);
	printf("Input your usages of text : %d\n", textUsage[2]);
	printf("Input your usages of data : %lf\n", dataUsage[2]);
}
void calculateFee(int currentPlan, int voiceUsage[], int textUsage[], double dataUsage[], double extraData[], int fee[])
{
	double additionalDataUsage;

	for (int i = 0; i <= 2; i++)
	{
		if (currentPlan == BASIC)
		{
			fee[i] = 20000;
			if (voiceUsage[i] > 500)
				fee[i] += (voiceUsage[i] - 500) * 50;
			if (textUsage[i] > 100)
				fee[i] += (textUsage[i] - 100) * 10;
			if (2.0 - dataUsage[i] > 0)
				extraData[i] = (2.0 - dataUsage[i]) > 0.2 ? 0.2 : (2.0 - dataUsage[i]);
			else extraData[i] = 0;

			if (i == 0)
				additionalDataUsage = dataUsage[i] > 2.0 ? dataUsage[i] - 2.0 : 0;
			else
				additionalDataUsage = dataUsage[i] - extraData[i - 1] - 2.0 > 0 ? dataUsage[i] - extraData[i - 1] - 2.0 : 0;

			fee[i] += (int)(additionalDataUsage * 10000);
		}
		else
		{
			fee[i] = 45000;
			if (voiceUsage[i] > 300)
				fee[i] += (voiceUsage[i] - 300) * 10;
			if (textUsage[i] > 100)
				fee[i] += (textUsage[i] - 100) * 30;
			if (30.0 - dataUsage[i] > 0)
				extraData[i] = (30.0 - dataUsage[i]) > 3.0 ? 3.0 : (30.0 - dataUsage[i]);
			else extraData[i] = 0;


			if (i == 0)
				additionalDataUsage = dataUsage[i] > 30.0 ? dataUsage[i] - 30.0 : 0;
			else
				additionalDataUsage = dataUsage[i] - extraData[i - 1] - 30.0 > 0 ? dataUsage[i] - extraData[i - 1] - 30.0 : 0;

			fee[i] += (int)(additionalDataUsage * 5000);
		}
	}
}

void displayBillings(int currentPlan, int voiceUsage[], int textUsage[], double dataUsage[], double extraData[], int fee[])
{
	printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖\n");
	printf("弛                      My three months usage\t\t\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 My Plan : ");
	currentPlan == BASIC ? printf("Basic Plan\t") : printf("More Data Plan");
	printf(" \t\t\t\t\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 \t\t弛    February\t弛      March\t弛      April\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Voice\t\t弛   %6d min\t弛   %6d min\t弛   %6d min\t弛\n", voiceUsage[0], voiceUsage[1], voiceUsage[2]);
	printf("弛 Text\t\t弛   %6d text\t弛   %6d text\t弛   %6d text\t弛\n", textUsage[0], textUsage[1], textUsage[2]);
	printf("弛 Data\t\t弛   %6.2lf GB\t弛   %6.2lf GB\t弛   %6.2lf GB\t弛\n", dataUsage[0], dataUsage[1], dataUsage[2]);
	printf("弛 Extra Data\t弛   %6.2lf GB\t弛   %6.2lf GB\t弛   %6.2lf GB\t弛\n", extraData[0], extraData[1], extraData[2]);
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Total Fee\t弛 \\ %7d\t弛 \\ %7d\t弛 \\ %7d\t弛\n", fee[0], fee[1], fee[2]);
	printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n");
}

void recommendBestPlan(int currentPlan, int voiceUsage[], int textUsage[], double dataUsage[])
{
	int avgVoice = 0, avgText = 0;
	double avgData = 0;
	int basicPlanFee = 20000, moreDataPlanFee = 45000;
	int i;

	for (i = 0; i <= 2; i++)
	{
		avgVoice += voiceUsage[i];
		avgText += textUsage[i];
		avgData += dataUsage[i];
	}

	avgVoice /= 3;
	avgText /= 3;
	avgData = (int)((avgData / 3) * 100) / 100.0;
	basicPlanFee += avgVoice > 500 ? (avgVoice - 500) * 50 : 0;
	basicPlanFee += avgText > 100 ? (avgText - 100) * 10 : 0;
	basicPlanFee += avgData > 2.0 ? (int)((avgData - 2.0) * 10000) : 0;

	moreDataPlanFee += avgVoice > 300 ? (avgVoice - 300) * 10 : 0;
	moreDataPlanFee += avgText > 100 ? (avgText - 100) * 30 : 0;
	moreDataPlanFee += avgData > 30.0 ? (int)((avgData - 30.0) * 5000) : 0;

	printf("忙式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式忖\n");
	printf("弛           Average usage of 3 months\t\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 My Plan : ");
	currentPlan == BASIC ? printf("Basic Plan\t") : printf("More Data Plan");
	printf(" \t\t\t弛\n");
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Average usage of voice : %5d\t\t弛\n", avgVoice);
	printf("弛 Average usage of text : %6d\t\t弛\n", avgText);
	printf("弛 Average usage of data : %6.2lf\t\t弛\n", avgData);
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 Basic Plan Fee :\t \\%7d\t\t弛\n", basicPlanFee);
	printf("弛 More Data Plan Fee :\t \\%7d\t\t弛\n", moreDataPlanFee);
	printf("戍式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式扣\n");
	printf("弛 We recommend to use ");
	basicPlanFee < moreDataPlanFee ? printf("Basic Plan\t") : printf("More Data Plan");
	printf(" \t\t弛\n");
	printf("戌式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式戎\n");
	return;
}