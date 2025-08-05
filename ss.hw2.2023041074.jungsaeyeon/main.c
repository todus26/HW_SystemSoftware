#include <stdio.h>              // ǥ�� ����� �Լ��� ���� ���
#include <stdlib.h>             // strtoul �Լ��� ���� ���
#include <stdint.h>             // uint32_t Ÿ���� ���� ���
#include <math.h>               // ldexpf, INFINITY, NAN�� ���� ���
#include <string.h>             // strcspn �Լ��� ���� ���

// IEEE 754 ������ �ε��Ҽ����� ǥ���ϱ� ���� ����ü
typedef union {                 // �޸� ����� ��ȯ ���� ���� ���� ��Ʈ ������ ����, float ���� ��Ʈ ǥ���� ���� ������ ���� sturct�� �ƴ� union ���
    float f;                    // ���� float ��
    struct {
        uint32_t sign : 1;      // ��ȣ ��Ʈ (1 bit)
        uint32_t exp : 8;       // ���� ��Ʈ (8 bits)
        uint32_t frac : 23;     // ���� ��Ʈ (23 bits)
    } bits;                     // ��Ʈ �ʵ� ����ü
} Float32;

// 32��Ʈ ������ float�� ��ȯ�ϴ� �Լ�
float bitsToFloat(uint32_t bits) {
    // ��Ʈ ������ Float32 ����ü�� ���� (32��Ʈ ������ ������ �ε��Ҽ��� ������ ���� ��ҷ� �����Ͽ� Float32 ����ü �ʱ�ȭ)
    Float32 f = {.bits = {              // bits ��� �ʱ�ȭ
        .sign = (bits >> 31) & 1,       // ��ȣ ��Ʈ ���� (32��Ʈ �� �ֻ��� ��Ʈ�� ���������� shift�Ͽ� ������, &1�� ����ŷ�Ͽ� 0 �Ǵ� 1�� ���� ���⵵�� ��)
        .exp = (bits >> 23) & 0xFF,     // ���� ��Ʈ ���� (23~30�� ��Ʈ�� ���������� shift�Ͽ� ������, &0xFF�� ����ŷ�Ͽ� 8��Ʈ �� ���� 8��Ʈ�� ���⵵�� ��)
        .frac = bits & 0x7FFFFF         // ���� ��Ʈ ���� (0~22�� ��Ʈ�� ���������� shift�Ͽ� ������, &0x7FFFFF�� ����ŷ�Ͽ� 23��Ʈ �� ���� 23��Ʈ�� ���⵵�� ��)
    }};

    // ������ȭ ��(denormalized number) ó��
    if (f.bits.exp == 0) {
        return f.bits.frac == 0 ? (f.bits.sign ? -0.0f : 0.0f)
                                : ldexpf((float)f.bits.frac, -149) * (f.bits.sign ? -1.0f : 1.0f); 
        /*������ 0�� ���� ������ȭ ���̹Ƿ�, 
        frac�� 0�̸� ��ȣ�� ���� +0 �Ǵ� -0, 
        �ƴϸ� ������ȭ���̹Ƿ� (frac * 2^-23) * 2^-126 = frac * 2^-149
        ���⼭ ������ȭ���� 0�� ����� �ſ� ������ ������ �ǹ��ϰ� �� 
        */
    }
    // ���Ѵ�� NaN ó��
    if (f.bits.exp == 0xFF) {
        return f.bits.frac == 0 ? (f.bits.sign ? -INFINITY : INFINITY) : NAN; 
        /*������ ��� 1�̸�(0xFF) ���Ѵ� �Ǵ� NaN
        frac�� 0�̸� (+-)���Ѵ�
        �ƴϸ� NaN
        */
    }
    // ����ȭ ��(normalized number) ó��
    float mantissa = 1.0f + ((float)f.bits.frac / 0x800000);                        // 1 + frac * 2^-23
    return ldexpf(mantissa, f.bits.exp - 127) * (f.bits.sign ? -1.0f : 1.0f);       // mantissa * 2^(exp-127)
    /*����ȭ�� ��� ������ 1.---�� ���¸� ��Ƿ�,
    ���� ��� 1.--- �κ��� ����ؾ� ��
    �̸� ���� ������ [0,1) ������ �Ҽ��� ����ؾ� �ϰ�
    ������ 2^(exp-127)�� �����־�� �Ѵ�.
    */
}

int main(int argc, char *argv[]) {                                  // argc: ����� ������ ����, argv: ����� ������ �迭
    // �Է� �� ��� ���� �̸� ���� (����� ���� �Ǵ� �⺻�� ���)
    const char *infile = argc > 1 ? argv[1] : "input.txt";          // �Է� ���� �̸� (����� ���ڰ� ���α׷� �̸� �ܿ� �߰� ���ڰ� ������ �Է� ���� �̸����� argv[1]��, �ƴϸ� "input.txt"�� ���)
    const char *outfile = argc > 2 ? argv[2] : "output.txt";        // ��� ���� �̸� (����� ���ڰ� ���α׷� �̸� �ܿ� �߰� ���ڰ� ������ ��� ���� �̸����� argv[2]��, �ƴϸ� "output.txt"�� ���)

    // ���� ����
    FILE *in = fopen(infile, "r");                                  // �Է� ������ �б� ���� ����       
    FILE *out = fopen(outfile, "w");                                // ��� ������ ���� ���� ����

    // ���� ���� ���� �� ���� ó��
    if (!in || !out) {                                            // in ���� �����Ͱ� NULL�� ��� or out ���� �����Ͱ� NULL�� ��� �߿� �ϳ��� ���̸� 
        perror("���� �б⿡ �����Ͽ����ϴ�.");                            // ���� �޽��� ���
        return 1;
    }

    char line[33];                                                      // 32��Ʈ ������ ���ڿ� + �� ���� ���ڸ� ���� ����
    // ������ �� ���� �о� ó��
    for (int lineNum = 1; fgets(line, sizeof(line), in); lineNum++) {   // ������ �� ���� �о���� �ݺ��� (fgets(���ڿ� ���� ����, �о���� �ִ� ���� ��, ���� ������))
        line[strcspn(line, "\n")] = 0;                                  // �ٹٲ� ���� ���� (���� �����͸� �����ϱ� ����)
        if (strlen(line) != 32) {                                       // 32��Ʈ�� �ƴ� �Է��� ����
            continue;
        }

        uint32_t bits = strtoul(line, NULL, 2);                         // 2���� ���ڿ��� 32��Ʈ ������ ��ȯ (uint32_t = 'unsigned 32-bit integer type', ���ڿ��� ��ȣ ���� long ������ ��ȯ, strtoul(���ڿ�, ��ȯ�� ���ڿ��� �� �ּ�, ��ȯ�� ����))
        float value = bitsToFloat(bits);                                // 32��Ʈ ������ float�� ��ȯ
        
        fprintf(out, "%.6f\n", value);                                  // ��ȯ�� float ���� ��� ���Ͽ� ����
    }

    // ���� �ݱ�
    fclose(in);
    fclose(out);
    printf("\nfinished\n");  // ó�� �Ϸ� �޽��� ���
    return 0;
}
