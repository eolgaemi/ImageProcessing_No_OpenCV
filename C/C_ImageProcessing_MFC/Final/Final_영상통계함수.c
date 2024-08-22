#include <stdlib.h>

// �ܺ� ��������
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



//////////////////// -------��������Լ� -------////////////////////
//  ��հ� ��������		�߾Ӱ� ��������
int get_average(void);  int get_median(void);

//////////////////// -------�ܺ������Լ� -------////////////////////
// �պ� ����
extern void mergesort(unsigned char arr[], int low, int high);


// ���
int get_average(void) {
	int avg = 0;

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			avg += outImage[h][w];
		}
	}

	avg /= out_height * out_width;

	return avg;
}

// �߾Ӱ�
int get_median(void) {

	unsigned char* image_flat = (unsigned char*)malloc(sizeof(unsigned char) * out_height * out_width + 1);

	int median = 0;
	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			image_flat[(h * out_height) + w] = outImage[h][w];
		}
	}
	mergesort(image_flat, 0, out_height * out_width);

	median = image_flat[(int)(out_height * out_width) / 2];

	free(image_flat);

	return median;
}