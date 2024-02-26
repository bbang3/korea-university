#include <stdio.h>
#include <stdlib.h>

#define PEASANT 0x08
#define WOLF 0x04
#define GOAT 0x02
#define CABBAGE 0x01

static void print_statename(FILE *fp, int state);
static void get_pwgc(int state, int *p, int *w, int *g, int *c);
static int is_dead_end(int state);
static int is_possible_transition(int state1, int state2);
static int changeP(int state);
static int changePW(int state);
static int changePG(int state);
static int changePC(int state);
static int is_visited(int visited[], int level, int state);
static void print_states(int visited[], int count);
static void dfs_main(int state, int goal_state, int level, int visited[]);
void make_adjacency_matrix(int graph[][16]);
void print_graph(int graph[][16], int num);
void save_graph(char *filename, int graph[][16], int num);
void depth_first_search(int init_state, int goal_state);

// 주어진 상태 state의 이름(마지막 4비트)을 화면에 출력
// 예) state가 7(0111)일 때, "<0111>"을 출력
static void print_statename(FILE *fp, int state)
{
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);
	fprintf(fp, "<%d%d%d%d>", p, w, g, c);
}

// 주어진 상태 state에서 농부, 늑대, 염소, 양배추의 상태를 각각 추출하여 p, w, g, c에 저장
// 예) state가 7(0111)일 때, p = 0, w = 1, g = 1, c = 1
static void get_pwgc(int state, int *p, int *w, int *g, int *c)
{
	*p = (state & PEASANT) > 0;
	*w = (state & WOLF) > 0;
	*g = (state & GOAT) > 0;
	*c = (state & CABBAGE) > 0;
}

// 허용되지 않는 상태인지 검사
// 예) 농부없이 늑대와 염소가 같이 있는 경우 / 농부없이 염소와 양배추가 같이 있는 경우
// return value: 1 허용되지 않는 상태인 경우, 0 허용되는 상태인 경우
static int is_dead_end(int state)
{
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);

	if (p != w && w == g)
		return 1;
	if (p != g && g == c)
		return 1;

	return 0;
}

// state1 상태에서 state2 상태로의 전이 가능성 점검
// 농부 또는 농부와 다른 하나의 아이템이 강 반대편으로 이동할 수 있는 상태만 허용
// 허용되지 않는 상태(dead-end)로의 전이인지 검사
// return value: 1 전이 가능한 경우, 0 전이 불이가능한 경우
static int is_possible_transition(int state1, int state2)
{
	if (is_dead_end(state2))
		return 0;

	int next_state[] = {changeP(state1), changePW(state1), changePG(state1), changePC(state1)};
	for (int i = 0; i < 4; i++)
	{
		if (next_state[i] == state2)
			return 1;
	}

	return 0;
}

// 상태 변경: 농부 이동
// return value : 새로운 상태
static int changeP(int state)
{
	return state ^ PEASANT;
}

// 상태 변경: 농부, 늑대 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePW(int state)
{
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);

	if (p != w)
		return -1;

	return state ^ PEASANT ^ WOLF;
}

// 상태 변경: 농부, 염소 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePG(int state)
{
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);

	if (p != g)
		return -1;

	return state ^ PEASANT ^ GOAT;
}

// 상태 변경: 농부, 양배추 이동
// return value : 새로운 상태, 상태 변경이 불가능한 경우: -1
static int changePC(int state)
{
	int p, w, g, c;
	get_pwgc(state, &p, &w, &g, &c);

	if (p != c)
		return -1;

	return state ^ PEASANT ^ CABBAGE;
}

// 주어진 state가 이미 방문한 상태인지 검사
// return value : 1 visited, 0 not visited
static int is_visited(int visited[], int level, int state)
{
	for (int i = 0; i <= level; i++)
	{
		if (visited[i] == state)
			return 1;
	}

	return 0;
}

// 방문한 상태들을 차례로 화면에 출력
static void print_states(int visited[], int count)
{
	for (int i = 0; i <= count; i++)
	{
		int state = visited[i];
		print_statename(stdout, state);
		printf("\n");
	}
}

// recursive function
static void dfs_main(int state, int goal_state, int level, int visited[])
{
	visited[level] = state;
	printf("cur state is ");
	print_statename(stdout, state);
	printf(" (level %d)\n", level);

	if (state == goal_state)
	{
		printf("Goal state found!\n");
		print_states(visited, level);
		printf("\n");
		return;
	}

	int next_states[] = {changeP(state), changePW(state), changePG(state), changePC(state)};
	for (int i = 0; i < 4; i++)
	{
		int next_state = next_states[i];
		if (next_state == -1)
			continue;

		if (is_visited(visited, level, next_state))
		{
			printf("\tnext state ");
			print_statename(stdout, next_state);
			printf(" has been visited\n");
			continue;
		}
		if (!is_possible_transition(state, next_state))
		{
			printf("\tnext state ");
			print_statename(stdout, next_state);
			printf(" is dead-end\n");
			continue;
		}
		dfs_main(next_state, goal_state, level + 1, visited);

		printf("back to ");
		print_statename(stdout, state);
		printf(" (level %d)\n", level);
	}
}

////////////////////////////////////////////////////////////////////////////////
// 상태들의 인접 행렬을 구하여 graph에 저장
// 상태간 전이 가능성 점검
// 허용되지 않는 상태인지 점검
void make_adjacency_matrix(int graph[][16])
{
	for (int src = 0; src < 16; src++)
		for (int dest = 0; dest < 16; dest++)
		{
			int w = !is_dead_end(src) && is_possible_transition(src, dest);
			graph[src][dest] = w;
		}
}

// 인접행렬로 표현된 graph를 화면에 출력
void print_graph(int graph[][16], int num)
{
	for (int src = 0; src < 16; src++)
	{
		for (int dest = 0; dest < 16; dest++)
		{
			printf("%10d", graph[src][dest]);
		}
		printf("\n");
	}
}

// 주어진 그래프(graph)를 .net 파일로 저장
// pgwc.net 참조
void save_graph(char *filename, int graph[][16], int num)
{
	FILE *fp = fopen(filename, "wt");

	fprintf(fp, "*Vertices 16\n");
	for (int state = 0; state < 16; state++)
	{
		fprintf(fp, "%d \"", state + 1);
		print_statename(fp, state);
		fprintf(fp, "\"\n");
	}

	fprintf(fp, "*Edges\n");
	for (int src = 0; src < 16; src++)
		for (int dest = src + 1; dest < 16; dest++)
			if (graph[src][dest])
			{
				fprintf(fp, "%3d %3d\n", src + 1, dest + 1);
			}

	fclose(fp);
}

////////////////////////////////////////////////////////////////////////////////
// 깊이 우선 탐색 (초기 상태 -> 목적 상태)
void depth_first_search(int init_state, int goal_state)
{
	int level = 0;
	int visited[16] = {
		0,
	}; // 방문한 정점을 저장

	dfs_main(init_state, goal_state, level, visited);
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	int graph[16][16] = {
		0,
	};

	// 인접 행렬 만들기
	make_adjacency_matrix(graph);

	// 인접 행렬 출력 (only for debugging)
	// print_graph(graph, 16);

	// .net 파일 만들기
	save_graph("pwgc.net", graph, 16);

	// 깊이 우선 탐색
	depth_first_search(0, 15); // initial state, goal state

	return 0;
}
