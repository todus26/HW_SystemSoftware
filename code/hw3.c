#include <stdio.h>
#include <stdlib.h>

// func2: 두 정수를 비교하여 첫 번째 인자가 더 크면 1, 아니면 0을 반환
int func2(int a, int b)
{
    asm volatile
    (
        "movl 8(%ebp),%edx \n\t"    // 첫 번째 인자 a를 edx 레지스터로 이동
        "movl 12(%ebp),%eax \n\t"   // 두 번째 인자 b를 eax 레지스터로 이동
        "cmpl %eax,%edx \n\t"       // edx와 eax 비교 (a와 b 비교)
        "jle L0 \n\t"               // a가 b보다 작거나 같으면 L0로 점프
        "movl $1,%eax \n\t"         // a가 b보다 크면 1을 반환
        "jmp L1 \n\t"               // L1으로 점프
    "L0: \n\t"
        "movl $0,%eax \n\t"         // a가 b보다 작거나 같으면 0을 반환
    "L1: \n\t"
    );
}

// func1: 두 정수의 포인터를 받아 해당 값들을 서로 교환
void func1(int *a, int *b)
{
    asm volatile
    (
        "movl 12(%ebp),%ecx \n\t"   // 두 번째 포인터를 ecx 레지스터로 이동
        "movl 8(%ebp),%edx \n\t"    // 첫 번째 포인터를 edx 레지스터로 이동
        "movl (%ecx),%eax \n\t"     // ecx가 가리키는 값을 eax로 이동
        "movl (%edx),%ebx \n\t"     // edx가 가리키는 값을 ebx로 이동
        "movl %eax,(%edx) \n\t"     // eax의 값을 첫 번째 포인터 위치에 저장
        "movl %ebx,(%ecx) \n\t"     // ebx의 값을 두 번째 포인터 위치에 저장
    );
}

int main(int argc, char *argv[]) {
    // 정렬할 배열 초기화
    int arr[] = { 1, 9, 5, 6, 3, 2, 9, 4, 1, 3 };
    
    // 배열의 크기 계산
    int size = sizeof(arr) / sizeof(arr[0]);
    
    // 삽입 정렬 구현
    // i는 현재 삽입할 원소의 위치
    for(int i = 1; i < size; i++) {
        // j는 삽입할 원소가 정렬된 부분을 왼쪽으로 탐색하며 이동하는 인덱스
        for(int j = i; j > 0; j--) {
            // 현재 위치(j)의 원소를 정렬된 부분의 적절한 위치로 이동
            // arr[j-1]이 arr[j]보다 크면 교환이 필요
            if(func2(arr[j-1], arr[j])) {
                func1(&arr[j-1], &arr[j]);  // 원소들을 교환하여 삽입 위치 조정
            }
            // 적절한 삽입 위치를 찾았으면 더 이상 비교할 필요 없음
            else {
                break;
            }
        }
    }
    
    // 정렬된 배열 출력
    for(int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
        
    return 0;
}