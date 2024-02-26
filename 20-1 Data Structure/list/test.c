#include <stdio.h>

int lower_bound(int[], int, int);

int main()
{
    int arr[10] = {1, 1, 2, 2, 2, 3, 4, 6, 6, 6}; // 데이터들이 크기순으로 졍렬되어 있는 상태이다.
    int target = 1;                               // 이 중 몇 번째 인덱스부터 10 이상이 되는지 찾아보도록 하자.
    scanf("%d", &target);
    int n = 10; // 배열의 크기
    printf("lower bound is arr[%d]\n", lower_bound(arr, target, n));
    return 0;
}

int lower_bound(int arr[], int target, int size)
{
    int mid, start, end;
    start = 0, end = size - 1;

    while (start <= end) // end가 start보다 같거나 작아지면, 그 값이 Lower Bound이므로 반복을 종료한다.
    {
        mid = (start + end) / 2;
        if (arr[mid] < target)
            start = mid + 1;
        else
            end = mid - 1;
    }
    return start;
}