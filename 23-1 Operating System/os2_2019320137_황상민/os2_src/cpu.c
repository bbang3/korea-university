#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <time.h>
#include <sched.h>
#include <syscall.h>
#include <linux/kernel.h>
#define ROW (100)
#define COL ROW

int count = 0;
long long total_duration_ms = 0;
int process_idx;
struct sched_attr
{
	uint32_t size;			 /* Size of this structure */
	uint32_t sched_policy;	 /* Policy (SCHED_*) */
	uint64_t sched_flags;	 /* Flags */
	int32_t sched_nice;		 /* Nice value (SCHED_OTHER,
			   SCHED_BATCH) */
	uint32_t sched_priority; /* Static priority (SCHED_FIFO,
			  SCHED_RR) */
							 /* Remaining fields are for SCHED_DEADLINE */
	uint64_t sched_runtime;
	uint64_t sched_deadline;
	uint64_t sched_period;
};

static int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags)
{
	return syscall(SYS_sched_setattr, pid, attr, flags);
}

int calc()
{
	int matrixA[ROW][COL];
	int matrixB[ROW][COL];
	int matrixC[ROW][COL];

	int i, j, k;
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			for (k = 0; k < COL; k++)
			{
				matrixC[i][j] += matrixA[i][j] * matrixB[i][j];
			}
		}
	}
	// 연산 횟수 증가 기준점
	count++;
}

const long long NANO = 1000000000LL;
const long long MSEC = 1000000LL;
void run_process(struct timespec base, int execution_sec)
{
	int epoch_duration = 100;
	struct timespec begin, end;
	long long prev_epoch_ms = 0;
	clock_gettime(CLOCK_MONOTONIC, &begin);

	while (1)
	{
		calc();
		clock_gettime(CLOCK_MONOTONIC, &end);

		total_duration_ms = (NANO * (end.tv_sec - base.tv_sec) + (end.tv_nsec - base.tv_nsec)) / MSEC;
		long long cur_epoch_ms = (NANO * (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec)) / MSEC;

		long long epoch_diff = (cur_epoch_ms - prev_epoch_ms) / epoch_duration;
		if (epoch_diff > 0)
		{
			printf("PROCESS #%02d count = %d %lld\n", process_idx, count, cur_epoch_ms - prev_epoch_ms);
			prev_epoch_ms = cur_epoch_ms;
		}

		if (total_duration_ms >= execution_sec * 1000)
			break;
	}
	
	printf("DONE!! PROCESS #%02d: %d %lld / pid: %d\n", process_idx, count, total_duration_ms, getpid());
}

void signal_handler(int signal)
{
	printf("DONE!! PROCESS #%02d: %d %lld / pid: %d\n", process_idx, count, total_duration_ms, getpid());
	exit(0);
}

int main(int argc, char *argv[])
{
	int num_process = atoi(argv[1]);
	int execution_time = atoi(argv[2]);

	int ret;

	struct sched_attr attr;
	memset(&attr, 0, sizeof(attr));
	attr.size = sizeof(struct sched_attr);
	attr.sched_priority = 10;
	attr.sched_policy = SCHED_RR;

	struct timespec base;
	for (int i = 0; i < num_process; i++)
	{
		printf("Creating Process: #%02d\n", i);
		process_idx = i;

		ret = sched_setattr(getpid(), &attr, 0);
		if (ret == -1)
		{
			printf("Set RR Scheduling failed\n");
		}
		pid_t pid = fork();
		if (pid < 0)
		{
			printf("fork failed\n");
		}
		else if (pid == 0)
		{
			signal(SIGINT, signal_handler);
			clock_gettime(CLOCK_MONOTONIC, &base);
			run_process(base, execution_time);
			exit(0);
		}
	}
	while (wait(NULL) > 0)
	{
	}

	return 0;
}

