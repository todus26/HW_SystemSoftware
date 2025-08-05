#include <stdio.h>              // 표준 입출력 함수를 위한 헤더
#include <stdlib.h>             // strtoul 함수를 위한 헤더
#include <stdint.h>             // uint32_t 타입을 위한 헤더
#include <math.h>               // ldexpf, INFINITY, NAN을 위한 헤더
#include <string.h>             // strcspn 함수를 위한 헤더

// IEEE 754 단정도 부동소수점을 표현하기 위한 공용체
typedef union {                 // 메모리 절약과 변환 과정 없이 직접 비트 조작을 위해, float 값과 비트 표현을 쉽게 오가기 위해 sturct이 아닌 union 사용
    float f;                    // 실제 float 값
    struct {
        uint32_t sign : 1;      // 부호 비트 (1 bit)
        uint32_t exp : 8;       // 지수 비트 (8 bits)
        uint32_t frac : 23;     // 가수 비트 (23 bits)
    } bits;                     // 비트 필드 구조체
} Float32;

// 32비트 정수를 float로 변환하는 함수
float bitsToFloat(uint32_t bits) {
    // 비트 패턴을 Float32 구조체에 매핑 (32비트 정수를 단정도 부동소수점 형식의 구성 요소로 분해하여 Float32 공용체 초기화)
    Float32 f = {.bits = {              // bits 멤버 초기화
        .sign = (bits >> 31) & 1,       // 부호 비트 추출 (32비트 중 최상위 비트를 오른쪽으로 shift하여 가져옴, &1로 마스킹하여 0 또는 1의 값만 남기도록 함)
        .exp = (bits >> 23) & 0xFF,     // 지수 비트 추출 (23~30번 비트를 오른쪽으로 shift하여 가져옴, &0xFF로 마스킹하여 8비트 중 하위 8비트만 남기도록 함)
        .frac = bits & 0x7FFFFF         // 가수 비트 추출 (0~22번 비트를 오른쪽으로 shift하여 가져옴, &0x7FFFFF로 마스킹하여 23비트 중 하위 23비트만 남기도록 함)
    }};

    // 비정규화 수(denormalized number) 처리
    if (f.bits.exp == 0) {
        return f.bits.frac == 0 ? (f.bits.sign ? -0.0f : 0.0f)
                                : ldexpf((float)f.bits.frac, -149) * (f.bits.sign ? -1.0f : 1.0f); 
        /*지수가 0일 때는 비정규화 수이므로, 
        frac가 0이면 부호에 따라 +0 또는 -0, 
        아니면 비정규화수이므로 (frac * 2^-23) * 2^-126 = frac * 2^-149
        여기서 비정규화수는 0에 가까운 매우 세밀한 수들을 의미하게 됨 
        */
    }
    // 무한대와 NaN 처리
    if (f.bits.exp == 0xFF) {
        return f.bits.frac == 0 ? (f.bits.sign ? -INFINITY : INFINITY) : NAN; 
        /*지수가 모두 1이면(0xFF) 무한대 또는 NaN
        frac가 0이면 (+-)무한대
        아니면 NaN
        */
    }
    // 정규화 수(normalized number) 처리
    float mantissa = 1.0f + ((float)f.bits.frac / 0x800000);                        // 1 + frac * 2^-23
    return ldexpf(mantissa, f.bits.exp - 127) * (f.bits.sign ? -1.0f : 1.0f);       // mantissa * 2^(exp-127)
    /*정규화인 경우 가수는 1.---의 형태를 띄므로,
    이의 경우 1.--- 부분을 계산해야 함
    이를 위해 가수를 [0,1) 범위의 소수로 계산해야 하고
    가수에 2^(exp-127)를 곱해주어야 한다.
    */
}

int main(int argc, char *argv[]) {                                  // argc: 명령줄 인자의 개수, argv: 명령줄 인자의 배열
    // 입력 및 출력 파일 이름 설정 (명령줄 인자 또는 기본값 사용)
    const char *infile = argc > 1 ? argv[1] : "input.txt";          // 입력 파일 이름 (명령줄 인자가 프로그램 이름 외에 추가 인자가 있으면 입력 파일 이름으로 argv[1]을, 아니면 "input.txt"를 사용)
    const char *outfile = argc > 2 ? argv[2] : "output.txt";        // 출력 파일 이름 (명령줄 인자가 프로그램 이름 외에 추가 인자가 있으면 출력 파일 이름으로 argv[2]를, 아니면 "output.txt"를 사용)

    // 파일 열기
    FILE *in = fopen(infile, "r");                                  // 입력 파일을 읽기 모드로 열기       
    FILE *out = fopen(outfile, "w");                                // 출력 파일을 쓰기 모드로 열기

    // 파일 열기 실패 시 에러 처리
    if (!in || !out) {                                            // in 파일 포인터가 NULL인 경우 or out 파일 포인터가 NULL인 경우 중에 하나라도 참이면 
        perror("파일 읽기에 실패하였습니다.");                            // 에러 메시지 출력
        return 1;
    }

    char line[33];                                                      // 32비트 이진수 문자열 + 널 종료 문자를 위한 버퍼
    // 파일의 각 줄을 읽어 처리
    for (int lineNum = 1; fgets(line, sizeof(line), in); lineNum++) {   // 파일의 각 줄을 읽어오는 반복문 (fgets(문자열 저장 버퍼, 읽어들일 최대 문자 수, 파일 포인터))
        line[strcspn(line, "\n")] = 0;                                  // 줄바꿈 문자 제거 (실제 데이터만 추출하기 위함)
        if (strlen(line) != 32) {                                       // 32비트가 아닌 입력은 무시
            continue;
        }

        uint32_t bits = strtoul(line, NULL, 2);                         // 2진수 문자열을 32비트 정수로 변환 (uint32_t = 'unsigned 32-bit integer type', 문자열을 부호 없는 long 정수로 변환, strtoul(문자열, 변환된 문자열의 끝 주소, 변환할 진법))
        float value = bitsToFloat(bits);                                // 32비트 정수를 float로 변환
        
        fprintf(out, "%.6f\n", value);                                  // 변환된 float 값을 출력 파일에 쓰기
    }

    // 파일 닫기
    fclose(in);
    fclose(out);
    printf("\nfinished\n");  // 처리 완료 메시지 출력
    return 0;
}