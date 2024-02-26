#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INSERT_OP      0x01
#define DELETE_OP      0x02
#define SUBSTITUTE_OP  0x04
#define MATCH_OP       0x08
#define TRANSPOSE_OP   0x10

#define INSERT_COST	1
#define DELETE_COST	1
#define SUBSTITUTE_COST	1
#define TRANSPOSE_COST	1

void print_alignment( char align_str[][8], int level);
// 재귀적으로 연산자 행렬을 순회하며, 두 문자열이 최소편집거리를 갖는 모든 가능한 정렬(alignment) 결과를 출력한다.
// op_matrix : 이전 상태의 연산자 정보가 저장된 행렬 (1차원 배열임에 주의!)
// col_size : op_matrix의 열의 크기
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
// level : 재귀호출의 레벨 (0, 1, 2, ...)
// align_str : 정렬된 문자쌍들의 정보가 저장된 문자열 배열 예) "a - a", "a - b", "* - b", "ab - ba"
static void backtrace_main( int *op_matrix, int col_size, char *str1, char *str2, int n, int m, int level, char align_str[][8], int *align_num)
{
	if(n == 0 && m == 0)
	{
		printf("\n[%d] ==============================\n", ++(*align_num));
		print_alignment(align_str, level - 1);
		return;
	}

	int d = op_matrix[col_size * n + m];
	char info_str[] = {(n == 0 ? '*' : str1[n - 1]), ' ', '-', ' ', (m == 0 ? '*' : str2[m - 1]), '\0'};
	strcpy(align_str[level], info_str);
	if(d & MATCH_OP) 
	{
		backtrace_main(op_matrix, col_size, str1, str2, n - 1, m - 1, level + 1, align_str, align_num);
	}
	if(d & SUBSTITUTE_OP) 
	{
		backtrace_main(op_matrix, col_size, str1, str2, n - 1, m - 1, level + 1, align_str, align_num);
	}
	if(d & INSERT_OP)
	{
		align_str[level][0] = '*';
		backtrace_main(op_matrix, col_size, str1, str2, n, m - 1, level + 1, align_str, align_num);
	}
	if(d & DELETE_OP)
	{
		align_str[level][0] = str1[n - 1];
		align_str[level][4] = '*';
		backtrace_main(op_matrix, col_size, str1, str2, n - 1, m, level + 1, align_str, align_num);
	}
	if(d & TRANSPOSE_OP)
	{
		char trans_str[] = {str1[n - 2], str1[n - 1], ' ', '-', ' ', str2[m - 2], str2[m - 1], '\0'};
		strcpy(align_str[level], trans_str);
		backtrace_main(op_matrix, col_size, str1, str2, n - 2, m - 2, level + 1, align_str, align_num);
	}
}

// 강의 자료의 형식대로 op_matrix를 출력 (좌하단(1,1) -> 우상단(n, m))
// 각 연산자를  괄호 안의 기호로 표시한다. 삽입(I), 삭제(D), 교체(S), 일치(M), 전위(T)
void print_matrix( int *op_matrix, int col_size, int n, int m)
{
	for (int i = n; i >= 1; i--)
	{
		for(int j = 1; j <= m; j++)
		{
			int d = op_matrix[i * col_size + j];

			if(d & MATCH_OP) printf("M");
			if(d & SUBSTITUTE_OP) printf("S");
			if(d & INSERT_OP) printf("I");
			if(d & DELETE_OP) printf("D");
			if(d & TRANSPOSE_OP) printf("T");

			printf("\t");
		}
		printf("\n");
	}
	
}

// 두 문자열 str1과 str2의 최소편집거리를 계산한다.
// return value : 최소편집거리
// 이 함수 내부에서 print_matrix 함수와 backtrace 함수를 호출함
int min_editdistance( char *str1, char *str2);

////////////////////////////////////////////////////////////////////////////////
// 세 정수 중에서 가장 작은 값을 리턴한다.
static int __GetMin3( int a, int b, int c)
{
	int min = a;
	if (b < min)
		min = b;
	if(c < min)
		min = c;
	return min;
}

////////////////////////////////////////////////////////////////////////////////
// 네 정수 중에서 가장 작은 값을 리턴한다.
static int __GetMin4( int a, int b, int c, int d)
{
	int min = __GetMin3( a, b, c);
	return (min > d) ? d : min;
}

////////////////////////////////////////////////////////////////////////////////
// 정렬된 문자쌍들을 출력
void print_alignment( char align_str[][8], int level)
{
	int i;
	
	for (i = level; i >= 0; i--)
	{
		printf( "%s\n", align_str[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
// backtrace_main을 호출하는 wrapper 함수
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	char align_str[n + m][8]; // n+m strings
	for(int i = 0; i < n + m; i++)
		for(int j = 0; j < 8; j++)
			align_str[i][j] = 0;
	
	int align_num = 0;
	backtrace_main( op_matrix, col_size, str1, str2, n, m, 0, align_str, &align_num);
}

////////////////////////////////////////////////////////////////////////////////
int min_editdistance(char *str1, char *str2)
{
	int n = strlen(str1), m = strlen(str2);
	
	int i, j;
	int d[n + 1][m + 1]; // distance
	int op_matrix[(n + 1) * (m + 1)]; // op

	for(int i = 0; i < (n + 1) * (m + 1); i++) op_matrix[i] = 0; // initalize op
	for(int i = 0; i <= n; i++) 
	{
		d[i][0] = i * DELETE_COST;
		op_matrix[i * (m + 1)] |= DELETE_OP;
	}
	for(int j = 0; j <= m; j++)
	{
		d[0][j] = j * INSERT_COST;
		op_matrix[j] |= INSERT_OP;
	}	

	for(int i = 1; i <= n; i++)
	{
		for(int j = 1; j <= m; j++)
		{
			const int INF = 987654321;

			int is_match = (str1[i - 1] == str2[j - 1]);
			int can_trans = (i >= 2 && j >= 2) && (str1[i - 1] == str2[j - 2] && str1[i - 2] == str2[j - 1]);

			int match_d = is_match ? d[i - 1][j - 1] : INF;
			int sub_d = !is_match ? d[i - 1][j - 1] + SUBSTITUTE_COST : INF;
			int ins_d = d[i][j - 1] + INSERT_COST;
			int del_d = d[i - 1][j] + DELETE_COST;
			int trans_d = can_trans ? d[i - 2][j - 2] + TRANSPOSE_COST : INF;
			
			int min_d = __GetMin4(match_d, ins_d, del_d, sub_d);
			min_d = min_d < trans_d ? min_d : trans_d;

			d[i][j] = min_d;
			if(match_d == min_d) op_matrix[i * (m + 1) + j] |= MATCH_OP;
			if(ins_d == min_d) op_matrix[i * (m + 1) + j] |= INSERT_OP;
			if(del_d == min_d) op_matrix[i * (m + 1) + j] |= DELETE_OP;
			if(sub_d == min_d) op_matrix[i * (m + 1) + j] |= SUBSTITUTE_OP;
			if(trans_d == min_d) op_matrix[i * (m + 1) + j] |= TRANSPOSE_OP;
		}
	}
	
	print_matrix(op_matrix, m + 1, n, m);
	backtrace(op_matrix, m + 1, str1, str2, n, m);

	return d[n][m];
}
int main()
{
	char str1[30];
	char str2[30];
	
	int distance;
	
	fprintf( stderr, "INSERT_COST = %d\n", INSERT_COST);
	fprintf( stderr, "DELETE_COST = %d\n", DELETE_COST);
	fprintf( stderr, "SUBSTITUTE_COST = %d\n", SUBSTITUTE_COST);
	fprintf( stderr, "TRANSPOSE_COST = %d\n", TRANSPOSE_COST);
	
	while( fscanf( stdin, "%s\t%s", str1, str2) != EOF)
	{
		printf( "\n==============================\n");
		printf( "%s vs. %s\n", str1, str2);
		printf( "==============================\n");
		
		distance = min_editdistance( str1, str2);
		
		printf( "\nMinEdit(%s, %s) = %d\n", str1, str2, distance);
	}
	return 0;
}

