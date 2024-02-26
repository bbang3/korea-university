#include <stdio.h>

int main()
{
	int yr, w, d, l;
	int totalw = 0, totald = 0, totall = 0;
	FILE *fp = fopen("Text.txt", "r");

	while (1)
	{
		fscanf(fp, "%d", &yr);
		if (yr == -1) break;
		fscanf(fp, "%d %d %d", &w, &d, &l);
		if (w > l) totalw++;
		else if (w < l) totall++;
		else totald++;
	}
	fclose(fp);

	printf("Total win: %d\n", totalw);
	printf("Total draw: %d\n", totald);
	printf("Total loss: %d\n", totall);

	return 0;
}