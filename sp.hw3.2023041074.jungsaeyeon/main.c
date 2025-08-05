#include <stdio.h>
#include <stdlib.h>

// func2: �� ������ ���Ͽ� ù ��° ���ڰ� �� ũ�� 1, �ƴϸ� 0�� ��ȯ
int func2(int a, int b)
{
	__asm__ __volatile__
  	(
		"movl 16(%ebp),%edx \n\t" // ù ��° ���� a�� edx �������ͷ� �̵�
		"movl 24(%ebp),%eax \n\t" // �� ��° ���� b�� eax �������ͷ� �̵�
		"cmpl %eax,%edx \n\t"     // edx�� eax �� (a�� b ��)
		"jle L0 \n\t"             // a�� b���� �۰ų� ������ L0�� ����
		"movl $1,%eax \n\t"       // a�� b���� ũ�� 1�� ��ȯ
		"jmp L1 \n\t"             // L1���� ����
	"L0: \n\t"
		"movl $0,%eax \n\t"       // a�� b���� �۰ų� ������ 0�� ��ȯ
	"L1: \n\t"
	);
}

// func1: �� ������ �����͸� �޾� �ش� ������ ���� ��ȯ
void func1(int *a, int *b)
{
	__asm__ __volatile__
  	(
		"movl 24(%ebp),%ecx \n\t" // �� ��° �����͸� ecx �������ͷ� �̵�
		"movl 16(%ebp),%edx \n\t" // ù ��° �����͸� edx �������ͷ� �̵�
		"movl (%ecx),%eax \n\t"   // ecx�� ����Ű�� ���� eax�� �̵�
		"movl (%edx),%ebx \n\t"   // edx�� ����Ű�� ���� ebx�� �̵�
		"movl %eax,(%edx) \n\t"   // eax�� ���� ù ��° ������ ��ġ�� ����
		"movl %ebx,(%ecx) \n\t"   // ebx�� ���� �� ��° ������ ��ġ�� ����
	);
}

int main(int argc, char *argv[]) {
	 
	// ������ �迭 �ʱ�ȭ
    int arr[] = { 1, 9, 5, 6, 3, 2, 9, 4, 1, 3 };
    
    // �迭�� ũ�� ���
    int size = sizeof(arr) / sizeof(arr[0]);
    
    // ���� ���� ����
    // i�� ���� ������ ������ ��ġ
    for(int i = 1; i < size; i++) {
        // j�� ������ ���Ұ� ���ĵ� �κ��� �������� Ž���ϸ� �̵��ϴ� �ε���
        for(int j = i; j > 0; j--) {
            // ���� ��ġ(j)�� ���Ҹ� ���ĵ� �κ��� ������ ��ġ�� �̵�
            // arr[j-1]�� arr[j]���� ũ�� ��ȯ�� �ʿ�
            if(func2(arr[j-1], arr[j])) {
                func1(&arr[j-1], &arr[j]);  // ���ҵ��� ��ȯ�Ͽ� ���� ��ġ ����
            }
            // ������ ���� ��ġ�� ã������ �� �̻� ���� �ʿ� ����
            else {
                break;
            }
        }
    }
    
    // ���ĵ� �迭 ���
    for(int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    
    printf("\n");  

        
    return 0;
}
