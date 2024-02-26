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
	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("��           KU Mobile Phone Bill Calculator\t\t��\n");
	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("��\t1 : Display Plans\t\t\t\t��\n");
	printf("��\t2 : Usage information for past two months\t��\n");
	printf("��\t3 : Input this month usage\t\t\t��\n");
	printf("��\t4 : Toal fee for past 3 months\t\t\t��\n");
	printf("��\t5 : Best plan recommendation\t\t\t��\n");
	printf("��\t6 : Exit\t\t\t\t\t��\n");
	printf("������������������������������������������������������������������������������������������������������������������\n");
}

void displayPlan()
{
	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("��                      Basic Plan\t\t\t��\n");
	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("�� Included in plan\t\t�� Additional usages\t��\n");
	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("�� Voice\t\t�� 500m free\t�� \\ 50 / 1m\t\t��\n");
	printf("�� Text\t\t�� 100t free\t�� \\ 10 / 1t\t\t��\n");
	printf("�� Data\t\t�� 2.00GB free\t�� \\ 1000 / 0.1GB\t��\n");
	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("�� Basic fee\t�� \\ 20000\t\t\t\t��\n");
	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("\n");

	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("��                    More Data Plan\t\t\t��\n");
	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("�� Included in plan\t\t�� Additional usages\t��\n");
	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("�� Voice\t\t�� 300m free\t�� \\ 10 / 1m\t\t��\n");
	printf("�� Text\t\t�� 100t free\t�� \\ 30 / 1t\t\t��\n");
	printf("�� Data\t\t�� 30.00GB free\t�� \\ 500 / 0.1GB\t\t��\n");
	printf("������������������������������������������������������������������������������������������������������������������\n");
	printf("�� Basic fee\t�� \\ 45000\t\t\t\t��\n");
	printf("������������������������������������������������������������������������������������������������������������������\n");
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
	printf("����������������������������������������������������������������������������������\n");
	printf("��          Last two months usage\t��\n");
	printf("����������������������������������������������������������������������������������\n");
	printf("�� My Plan : ");
	currentPlan == BASIC ? printf("Basic Plan\t") : printf("More Data Plan");
	printf(" \t\t��\n");
	printf("����������������������������������������������������������������������������������\n");
	printf("��\t��    February\t��      March\t��\n");
	printf("����������������������������������������������������������������������������������\n");
	printf("�� Voice\t��   %6d min\t��   %6d min\t��\n",voiceUsage[0], voiceUsage[1]);
	printf("�� Text\t��   %6d text\t��   %6d text\t��\n", textUsage[0], textUsage[1]);
	printf("�� Data\t��   %6.2lf GB\t��   %6.2lf GB\t��\n", dataUsage[0], dataUsage[1]);
	printf("����������������������������������������������������������������������������������\n");

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
	printf("����������������������������������������������������������������������������������������������������������������������������������\n");
	printf("��                      My three months usage\t\t\t��\n");
	printf("����������������������������������������������������������������������������������������������������������������������������������\n");
	printf("�� My Plan : ");
	currentPlan == BASIC ? printf("Basic Plan\t") : printf("More Data Plan");
	printf(" \t\t\t\t\t��\n");
	printf("����������������������������������������������������������������������������������������������������������������������������������\n");
	printf("�� \t\t��    February\t��      March\t��      April\t��\n");
	printf("����������������������������������������������������������������������������������������������������������������������������������\n");
	printf("�� Voice\t\t��   %6d min\t��   %6d min\t��   %6d min\t��\n", voiceUsage[0], voiceUsage[1], voiceUsage[2]);
	printf("�� Text\t\t��   %6d text\t��   %6d text\t��   %6d text\t��\n", textUsage[0], textUsage[1], textUsage[2]);
	printf("�� Data\t\t��   %6.2lf GB\t��   %6.2lf GB\t��   %6.2lf GB\t��\n", dataUsage[0], dataUsage[1], dataUsage[2]);
	printf("�� Extra Data\t��   %6.2lf GB\t��   %6.2lf GB\t��   %6.2lf GB\t��\n", extraData[0], extraData[1], extraData[2]);
	printf("����������������������������������������������������������������������������������������������������������������������������������\n");
	printf("�� Total Fee\t�� \\ %7d\t�� \\ %7d\t�� \\ %7d\t��\n", fee[0], fee[1], fee[2]);
	printf("����������������������������������������������������������������������������������������������������������������������������������\n");
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

	printf("��������������������������������������������������������������������������������������������������\n");
	printf("��           Average usage of 3 months\t\t��\n");
	printf("��������������������������������������������������������������������������������������������������\n");
	printf("�� My Plan : ");
	currentPlan == BASIC ? printf("Basic Plan\t") : printf("More Data Plan");
	printf(" \t\t\t��\n");
	printf("��������������������������������������������������������������������������������������������������\n");
	printf("�� Average usage of voice : %5d\t\t��\n", avgVoice);
	printf("�� Average usage of text : %6d\t\t��\n", avgText);
	printf("�� Average usage of data : %6.2lf\t\t��\n", avgData);
	printf("��������������������������������������������������������������������������������������������������\n");
	printf("�� Basic Plan Fee :\t \\%7d\t\t��\n", basicPlanFee);
	printf("�� More Data Plan Fee :\t \\%7d\t\t��\n", moreDataPlanFee);
	printf("��������������������������������������������������������������������������������������������������\n");
	printf("�� We recommend to use ");
	basicPlanFee < moreDataPlanFee ? printf("Basic Plan\t") : printf("More Data Plan");
	printf(" \t\t��\n");
	printf("��������������������������������������������������������������������������������������������������\n");
	return;
}