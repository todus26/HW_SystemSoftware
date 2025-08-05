// 시스템소프트웨어 과제 1 (2023041074 정새연)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int fun_A(int a) {
    // 소수 판별
    /*소수 판별 시, a의 제곱근까지만 확인하면 충분하기 때문에 i^2로 for문을 돌림
    -> a가 소수가 아닐 경우, 다음과 같이 표현이 가능함. a=x*y 이때 x나 y는 둘 중 하나라도 제곱근 a보다 작음*/
    if (a <= 1) return 0;                   // 1은 소수가 아니므로 0 반환 
    for (int i = 2; i * i <= a; i++) {      // 2부터 a의 제곱근까지만 확인
        if (a % i == 0) return 0;           // a가 i로 나누어 떨어지면 소수가 아님
    }                                       // 소수가 아닌 경우 0 반환 
    return 1;                               // 소수인 경우 1 반환
}

int fun_B(int a) {
    // 각 자릿 수의 합이 7
    int sum = 0;                            // 자릿수의 합을 저장할 변수
    while (a > 0) {                         // a가 0보다 클 때까지 반복
        sum += a % 10;                      // a를 10으로 나눈 나머지(a의 일의 자리 수)를 sum에 더함      
        a /= 10;                            // a를 10으로 나누어 일의 자리 수를 없앰(다음 자릿수를 더하기 위해)
    }
    return (sum == 7);                      // 자릿수 합이 7이면 1 반환, 아니면 0 반환
}

int fun_C(int a) {
    // 짝수
    return (a % 2 == 0);                    // 2로 나누어떨어지면(짝수이면) 1 반환, 아니면 0 반환
}

int fun_D(int a) {
    // 1000보다 작은 수
    return (a < 1000);                      // 1000보다 작으면 1 반환, 아니면 0 반환
}

void do_something() {                       // 아무것도 하지 않는 함수 (시물레이션용)
    volatile int temp = 0;                  // 최적화 방지용 변수
    for (int i = 0; i < 100; i++) {         // 0부터 99까지 반복
        temp += i;                          // temp에 i를 더함
    }
}

// 소수이면서 자리수 합이 7인 가장 큰 숫자를 찾는 함수
int find_max_num1(FILE *file, int *count_num1) {    
    int max_num = 0;                                // 가장 큰 숫자를 저장할 변수
    int num;                                        // 파일에서 읽어온 숫자를 저장할 변수         
    *count_num1 = 0;                                // 소수이면서 자리수 합이 7인 숫자의 개수를 저장할 변수

    while (fscanf(file, "%d", &num) != EOF) {       // 파일에서 숫자를 읽어오는 반복문 
    /*(입력받을 파일 포인터, 입력 받을 데이터의 형식, 데이터 형식에 따라 데이터 저장할 변수들의 주소)
    성공적으로 읽은 항목의 수를 반환하게 되고, 파일의 끝에 도달하는 경우 EOF
    즉, file에서 정수형 데이터를 읽어서 num에 저장, 파일 끝에 도달할 때까지 반복하게 됨*/
        if (fun_A(num) && fun_B(num)) {             // 소수이면서 자리수 합이 7인지 확인
            (*count_num1)++;                        // 참이면 1 증가
            if (num > max_num) {                    // 현재 숫자가 가장 큰 숫자보다 크면
                max_num = num;                      // 가장 큰 숫자를 현재 숫자로 변경    
            }
        }
    }
    
    return max_num;                                 // 가장 큰 숫자 반환
}

// 짝수이면서 1000보다 작은 가장 큰 숫자를 찾는 함수
int find_max_num2(FILE *file, int *count_num2) {
    int max_num = 0;                                // 가장 큰 숫자를 저장할 변수
    int num;                                        // 파일에서 읽어온 숫자를 저장할 변수               
    *count_num2 = 0;                                // 짝수이면서 1000보다 작은 숫자의 개수를 저장할 변수
    
    while (fscanf(file, "%d", &num) != EOF) {       // 파일에서 숫자를 읽어오는 반복문
        if (fun_C(num) && fun_D(num)) {             // 짝수이면서 1000보다 작은지 확인
            (*count_num2)++;                        // 참이면 1 증가
            if (num > max_num) {                    // 현재 숫자가 가장 큰 숫자보다 크면
                max_num = num;                      // 가장 큰 숫자를 현재 숫자로 변경
            }
        }
    }
    
    return max_num;                                 // 가장 큰 숫자 반환
}

int main() 
{
    clock_t start, end;                             // 시간 측정을 위한 변수
    FILE *file = fopen("numbers.txt", "r");         // 파일을 읽기 모드로 열기
    if (file == NULL) {                             // 파일 열기에 실패하면  
        printf("파일을 열 수 없습니다.\n");           // 메세지 출력 후
        return 1;                                   // 프로그램 종료
    }

    int count_num1 = 0, count_num2 = 0;             // 소수이면서 자리수 합이 7인 숫자의 개수, 짝수이면서 1000보다 작은 숫자의 개수를 저장할 변수
    int num1, num2;                                 // 소수이면서 자리수 합이 7인 가장 큰 숫자, 짝수이면서 1000보다 작은 가장 큰 숫자를 저장할 변수


    num1 = find_max_num1(file, &count_num1);        // 소수이면서 자리수 합이 7인 가장 큰 숫자를 찾는 함수 호출
    rewind(file);                                   // stream의 파일 위치 지시자를 시작 위치로 이동         


    num2 = find_max_num2(file, &count_num2);        // 짝수이면서 100보다 작은 가장 큰 숫자를 찾는 함수 호출


    fclose(file);                                   // 파일 닫기

    // 결과 출력
    printf("소수이면서 각 자리수의 합이 7인 숫자의 개수: %d\n, 가장 큰 숫자: %d\n\n", count_num1, num1);
    printf("짝수이면서 1000보다 작은 숫자의 개수: %d\n, 가장 큰 숫자: %d\n\n", count_num2, num2);

    
    int count = 0;                                  // do_something 함수가 실행된 횟수를 저장할 변수
    start = clock();                                // 시간 측정 시작

    for (int i = 0; i < 1000000; i++) {             // 0부터 999999까지 반복
        if ((fun_C(num2) && fun_D(num2)) || (fun_B(num1) && fun_A(num1))){
            // i가 소수이면서 자리수 합이 7인 숫자이거나 짝수이면서 1000보다 작은 숫자이거나
            // num1이 짝수이거나 1000보다 작은 숫자이거나 num2가 소수이거나 자리수 합이 7인 숫자이면
            do_something();                         // do_something 함수 호출
            count++;                                // do_something 함수가 실행된 횟수 증가
        }
    }

    end = clock();                                              // 시간 측정 종료
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC; // 수행 시간 계산

    printf("count = %d\n", count);                              // do_something 함수가 실행된 횟수 출력
    printf("execution time: %f seconds\n", time_taken);         // 수행 시간 출력

    return 0;
}

