#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// �ܺ� ���� ����

// �ʱ� �̹����� ���� �ȼ� ��
extern int in_height;
// �ʱ� �̹����� ���� �ȼ� ��
extern int in_width;
// ���� ó�� �� ���� �ȼ� ��
extern int out_height;
// ���� ó�� �� ���� �ȼ� ��
extern int out_width;

// �����̹����� ��� ����������
extern unsigned char** inImage;
// ���纻�̹����� ��� ����������
extern unsigned char** outImage;

////////////////////// -------�Լ����� -------//////////////////////
// 
//��� �ϱ�             ��Ӱ� �ϱ�          ��� �����
void trans_brighter();  void trans_darker(); void trans_baw();

//���� ����             ���� ó��
void trans_gamma();     void trans_reverse();

//AND ����			 OR����			     XOR����
void trans_and();    void trans_or();    void trans_xor();

//�Ķ󺼶�CAP			�Ķ󺼶�CUP
void trans_para_cap();  void trans_para_cup();

//������׷� ��Ʈ��ġ			������׷� ������ ��Ʈ��ġ
void trans_hist_stretch();		void trans_hist_endin_stretch();

//������׷� ��Ȱȭ
void trans_hist_equal();

////////////////////// -------�ܺ��Լ� -------//////////////////////
// ���� 1��					�Ǽ� 1��					�̹��� ���
extern int getIntValue();	extern float getFValue();	extern void print_image_after();
// outImage �޸� �Ҵ�					outImage �޸� ����
extern void malloc_outImage_mem();		extern void free_outImage_mem();

////////////////////////////////////////////////////////////////////
////////////////////// -------�Լ����� -------//////////////////////
////////////////////////////////////////////////////////////////////
// ���
void trans_brighter() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	printf("\n ��� ");
	int value = getIntValue();
	printf("\n");
	printf("-------%d ��ŭ ��� �غ��Կ� -------\n", value);
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			// Ŭ����
			if (inImage[i][k] + value < 255) {
				outImage[i][k] += value;
			}
			else {
				outImage[i][k] = 255;
			}
		}
	}
	print_image_after();
}
// ��Ӱ�
void trans_darker() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	printf("\n ��Ӱ� ");
	int value = getIntValue();
	printf("-------%d ��ŭ ��Ӱ� �غ��Կ� -------\n", value);
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			// Ŭ����
			if (outImage[i][k] - value > 0) {
				outImage[i][k] -= value;
			}
			else {
				outImage[i][k] = 0;
			}
		}
	}
	print_image_after();
}
// ���
void trans_baw() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	printf("\n ��� ");
	int value = getIntValue();
	printf("-------%d�� �������� ������� �����Կ� -------\n", value);
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {

			if (outImage[i][k] > value) {
				outImage[i][k] = 255;
			}
			else {
				outImage[i][k] = 0;
			}
		}
	}
	printf("\n");
	print_image_after();
}
// ���� ����
void trans_gamma() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	float gamma = getFValue();
	unsigned char temp = 0;
	printf("\n");
	printf("-------��������(%0.2f) �غ��Կ� -------\n", gamma);
	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {

			temp = (unsigned char)pow((float)outImage[h][w], 1.0 / gamma);
			
			if (temp < 0) {
				outImage[h][w] = 0;
			}
			else if (temp > 255) {
				outImage[h][w] = 255;
			}
			else {
				outImage[h][w] = temp;
			}
		}
	}
	print_image_after();
}
// ����ó��
void trans_reverse() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	printf("-------�̹��� ���� ó�� -------\n");
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 255 - outImage[i][k];
		}
	}
	print_image_after();
}

// ��Ʈ������ AND
void trans_and() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	int filter;
	printf("AND ���� �� �Է� -----> ");
	scanf("%d", &filter);
	printf("\n");
	printf("-------AND ����(%d) �غ��Կ� -------\n", (unsigned char)filter);

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = (outImage[h][w] & filter);
		}
	}
	print_image_after();
}

// ��Ʈ������ OR
void trans_or() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	int filter;
	printf("OR ���� �� �Է� -----> ");
	scanf("%d", &filter);
	printf("\n");
	printf("-------OR ����(%d) �غ��Կ� -------\n", (unsigned char)filter);

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = (outImage[h][w] | filter);
		}
	}
	print_image_after();
}

// ��Ʈ������ XOR
void trans_xor() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	int filter;
	printf("XOR ���� �� �Է� -----> ");
	scanf("%d", &filter);
	printf("\n");
	printf("-------XOR ����(%d) �غ��Կ� -------\n", (unsigned char)filter);

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = (outImage[h][w] ^ filter);
		}
	}
	print_image_after();
}

// �Ķ󺼶� CAP
void trans_para_cap() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();

	printf("\n �Ķ󺼶� CAP  ");
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 255 - (255 * pow(outImage[i][k] / 127 - 1, 2));
		}
	}
	printf("\n");
	print_image_after();
}

// �Ķ󺼶� CUP
void trans_para_cup() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();

	printf("\n �Ķ󺼶� CUP  ");
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = (unsigned char)-1 * 255 * pow((outImage[i][k] / 127 - 1), 2) + 255;
		}
	}
	printf("\n");
	print_image_after();
}

// 0~255���� �̹� ������ ������ �Ϻ� �ս��� �ؼ� �÷���� �Ѵ�.

void trans_hist_stretch() {
	free_outImage_mem();
	
	out_height = in_height;
	out_width = in_width;

	malloc_outImage_mem();
	printf("\nOUT_SIZE: %d * %d", out_height, out_width);

	// outImage �޸� �Ҵ�
	
	// ������ ù��° �ȼ��� ������ �ʱ�ȭ �ϸ� ����
	/*
	int highest = 0;
	int lowest = 255;
	*/

	// �ִ� ���� , �ּ� �� �� ���ϱ�
	int highest = inImage[0][0];
	int lowest = inImage[0][0];

	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {

			if (highest < inImage[i][k]) {
				highest = inImage[i][k];
			}
			if (lowest > inImage[i][k]) {
				lowest = inImage[i][k];
			}
		}
	}
	// ���� ��ȯ �� �����ϱ�
	int old = inImage[0][0];
	int new = outImage[0][0];

	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {

			old = inImage[i][k];
			new = (int)((double)(old - lowest) / (double)(highest - lowest) * 255.0);
			outImage[i][k] = new;
		}
	}

	// ȭ�����
	print_image_after();
}

void trans_hist_endin_stretch() {
	
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;

	printf("\nOUT_SIZE: %d * %d", out_height, out_width);

	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	// ������ ù��° �ȼ��� ������ �ʱ�ȭ �ϸ� ����
	/*
	int highest = 0;
	int lowest = 255;
	*/
	// �ִ� ���� , �ּ� �� �� ���ϱ�
	int highest = inImage[0][0];
	int lowest = inImage[0][0];

	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {

			if (highest < inImage[i][k]) {
				highest = inImage[i][k];
			}
			if (lowest > inImage[i][k]) {
				lowest = inImage[i][k];
			}
		}
	}
	// �ִ� ���� , �ּ� �� ���� ������ �����ϱ�
	highest -= 10;
	lowest += 10;


	// ���� ��ȯ �� �����ϱ�
	int old = inImage[0][0];
	int new = outImage[0][0];

	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {

			old = inImage[i][k];
			if (old - lowest >= 0 || highest - lowest >= 0) {
				new = (int)((double)(old - lowest) / (double)(highest - lowest) * 255.0);
			}
			outImage[i][k] = new;
		}
	}
	// ȭ�� ��� �ϱ�
	print_image_after();
}


// ������׷� ��Ȱȭ
void trans_hist_equal() {
	free_outImage_mem();
	// ���� ���� ����
	out_height = in_height;
	out_width = in_width;
	printf("\nOUT_SIZE: %d * %d", out_height, out_width);
	malloc_outImage_mem();

	// ������׷� �޸� �Ҵ�
	double* histo_gram = (double*)malloc(sizeof(double) * 256);

	// ������׷� �ʱ�ȭ
	for (int i = 0; i < 256; i++) {
		histo_gram[i] = 0.0;
	}

	// ������׷� �� ī��Ʈ
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			histo_gram[inImage[i][k]] += 1.0;
		}
	}

	// ������ ������׷� �޸� �Ҵ�
	double* sum_histo_gram = (double*)malloc(sizeof(double) * 256);

	// ù ��Ҹ� ����
	sum_histo_gram[0] = histo_gram[0];

	// ������ ����
	for (int i = 1; i < 256; i++) {
		sum_histo_gram[i] = sum_histo_gram[i - 1] + histo_gram[i];
	}

	// �� �� ��ģ �ʱ� ������׷� �޸� ����
	free(histo_gram);

	// ����ȭ ������׷� �޸� �Ҵ�
	double* normal_histo_gram = (double*)malloc(sizeof(double) * 256);

	// �� ȭ�� ��
	long long pixel_n = in_height * in_width;

	// �ִ� ��
	double highest_pixel = inImage[0][0];

	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			if (highest_pixel < inImage[i][k]) {
				highest_pixel = inImage[i][k];
			}
		}
	}

	// ����ȭ
	for (int i = 0; i < 256; i++) {
		normal_histo_gram[i] = sum_histo_gram[i] * (1.0 / pixel_n) * highest_pixel;
	}

	// �� �� ��ģ ������ ������׷� �޸� ����
	free(sum_histo_gram);

	// �̹��� ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			outImage[i][k] = (unsigned char)normal_histo_gram[inImage[i][k]];
		}
	}

	// �� �� ��ģ ����ȭ ������׷� �޸� ����
	free(normal_histo_gram);

	// ���� ���
	print_image_after();
}