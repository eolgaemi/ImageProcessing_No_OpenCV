#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// �ܺ� ���� ����
// �ʱ� �̹����� ���� �ȼ� ��		�ʱ� �̹����� ���� �ȼ� ��
extern int in_height;				extern int in_width;

// ���� ó�� �� ���� �ȼ� ��		���� ó�� �� ���� �ȼ� ��
extern int out_height;				extern int out_width;

// �����̹����� ��� ����������		���纻�̹����� ��� ����������
extern unsigned char** inImage;		extern unsigned char** outImage;

////////////////////// -------�ܺ��Լ� -------//////////////////////
// ���� 1��					�Ǽ� 1��					�̹��� ���
extern int getIntValue();	extern float getFValue();	extern void print_image_after();

// outImage �޸� �Ҵ�					outImage �޸� ����
extern void malloc_outImage_mem();		extern void free_outImage_mem();	extern int get_median(void);

// temp �̹��� �޸� �Ҵ�									temp �̹��� �޸� ����
extern double** malloc_temp_Image_mem(int h, int w);		extern void free_temp_Image_mem(double** temp_image, int h);

////////////////////// -------�Լ����� -------//////////////////////
// ������					����
void trans_embossing();		void trans_blurring();

// ���������� ������				����þ� ������
void trans_high_freq_sharpening();	void trans_gausian_smoothing();

// �������� 1���� ������			����þ� ������ + ���ö�þ� 2d ���� ����
void trans_edge_1d_prewitt();		void trans_edge_2d_LoG();

// ������
void trans_embossing() {

	// ���纻 �̹��� �޸� ����
	free_outImage_mem();

	// ���纻 �̹��� ũ�� ����
	out_height = in_height;
	out_width = in_width;

	// ���纻 �̹��� �޸� �Ҵ�
	malloc_outImage_mem();

	// ����ũ
	double mask[3][3] = { {-1.0,0.0,0.0},
						  {0.0,0.0,0.0},
						  {0.0,0.0,1.0} };

	// �ӽø޸� temp_Image �Ǽ������� ����

	double** temp_in_image = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image = malloc_temp_Image_mem(out_height, out_width);

	// tempinImage 127�� �ʱ�ȭ
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image[i][k] = 127.0;
		}
	}

	// inImage ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image[i + 1][k + 1] = inImage[i][k];
		}
	}

	// ���� 1 ����Ŭ�� ����� ���� �� ����
	double temp = 0.0;

	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp = 0.0;
			for (int s = 0; s < 3; s++) {
				for (int g = 0; g < 3; g++) {
					temp += temp_in_image[i + s][k + g] * mask[s][g];
				}
			}
			temp_out_image[i][k] = temp;
		}
	}

	// ��ó�� ����ũ ���� �հ迡 ����(0 or 1)
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			temp_out_image[i][k] += 127.0;
		}
	}

	// outImage�� �����ϱ�
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {

			if (temp_out_image[i][k] < 0.0) {
				outImage[i][k] = 0;
			}
			else if (temp_out_image[i][k] > 255.0) {
				outImage[i][k] = 255;
			}
			else {
				outImage[i][k] = (unsigned char)temp_out_image[i][k];
			}
		}
	}
	// �޸� ����
	free_temp_Image_mem(temp_in_image, in_height + 2);
	free_temp_Image_mem(temp_out_image, out_height);

	// ���� ���
	print_image_after();
}

// ����
void trans_blurring() {

	// ���纻 �̹��� �޸� ����
	free_outImage_mem();

	// ���纻 �̹��� ũ�� ����
	out_height = in_height;
	out_width = in_width;

	// ���纻 �̹��� �޸� �Ҵ�
	malloc_outImage_mem();

	// ����ũ
	double blur = 1.0 / 9;

	// ����ũ
	double mask[3][3] = { {blur,blur,blur},
						  {blur,blur,blur},
						  {blur,blur,blur} };

	// �ӽ� �̹��� �޸� �Ǽ������� �Ҵ�

	double** temp_in_image = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image = malloc_temp_Image_mem(out_height, out_width);

	// �ӽ� �̹��� ȸ������ �ʱ�ȭ
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image[i][k] = 127.0;
		}
	}

	// �ӽ� �̹����� ���� �̹��� ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image[i + 1][k + 1] = inImage[i][k];
		}
	}

	// ����ŷ ���� 1 ����Ŭ�� ����� ���� �� �ӽú���
	double temp = 0.0;

	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp = 0.0;
			for (int s = 0; s < 3; s++) {
				for (int g = 0; g < 3; g++) {
					temp += temp_in_image[i + s][k + g] * mask[s][g];
				}
			}
			temp_out_image[i][k] = temp;
		}
	}

	// ���纻 �̹����� �ӽ� �̹��� �����ϱ�
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {

			if (temp_out_image[i][k] < 0.0) {
				outImage[i][k] = 0;
			}
			else if (temp_out_image[i][k] > 255.0) {
				outImage[i][k] = 255;
			}
			else {
				outImage[i][k] = (unsigned char)temp_out_image[i][k];
			}
		}
	}

	// �޸� ����
	free_temp_Image_mem(temp_in_image, in_height + 2);
	free_temp_Image_mem(temp_out_image, out_height);

	// ���� ���
	print_image_after();
}

// ������ ���� ������
void trans_high_freq_sharpening() {

	// ���纻 �̹��� �޸� ����
	free_outImage_mem();

	// ���纻 �̹��� ũ�� ����
	out_height = in_height;
	out_width = in_width;

	// ���纻 �̹��� �޸� �Ҵ�
	malloc_outImage_mem();

	// ����ũ
	double boundary = -1.0 / 9;
	double center = 8.0 / 9;

	
	double mask[3][3] = { {boundary,boundary,boundary},
						  {boundary,center,boundary},
						  {boundary,boundary,boundary} };

	// �ӽ� �̹��� �޸� �Ǽ������� �Ҵ�

	double** temp_in_image = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image = malloc_temp_Image_mem(out_height, out_width);

	// �ӽ� �̹��� ȸ������ �ʱ�ȭ
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image[i][k] = 127.0;
		}
	}

	// �ӽ� �̹����� ���� �̹��� ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image[i + 1][k + 1] = inImage[i][k];
		}
	}

	// ����ŷ ���� 1 ����Ŭ�� ����� ���� �� �ӽú���
	double temp = 0.0;

	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp = 0.0;
			for (int s = 0; s < 3; s++) {
				for (int g = 0; g < 3; g++) {
					temp += temp_in_image[i + s][k + g] * mask[s][g];
				}
			}
			temp_out_image[i][k] = temp;
		}
	}

	// ���纻 �̹����� �ӽ� �̹��� �����ϱ�
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {

			if (temp_out_image[i][k] < 0.0) {
				outImage[i][k] = 0;
			}
			else if (temp_out_image[i][k] > 255.0) {
				outImage[i][k] = 255;
			}
			else {
				outImage[i][k] = (unsigned char)temp_out_image[i][k];
			}
		}
	}
	// �޸� ����
	free_temp_Image_mem(temp_in_image, in_height + 2);
	free_temp_Image_mem(temp_out_image, out_height);

	// ���� ���
	print_image_after();
}

void trans_gausian_smoothing() {

	// ���纻 �̹��� �޸� ����
	free_outImage_mem();

	// ���纻 �̹��� ũ�� ����
	out_height = in_height;
	out_width = in_width;

	// ���纻 �̹��� �޸� �Ҵ�
	malloc_outImage_mem();

	// ����ũ
	double mask[3][3] = { {1.0/16,1.0/8,1.0/16},
						  {1.0/8,1.0/4,1.0/8},
						  {1.0/16,1.0/8,1.0/16} };

	// �ӽ� �̹��� �޸� �Ǽ������� �Ҵ�
	double** temp_in_image = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image = malloc_temp_Image_mem(out_height, out_width);

	// �ӽ� �̹��� ȸ������ �ʱ�ȭ
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image[i][k] = 127.0;
		}
	}

	// �ӽ� �̹����� ���� �̹��� ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image[i + 1][k + 1] = inImage[i][k];
		}
	}

	// ����ŷ ���� 1 ����Ŭ�� ����� ���� �� �ӽú���
	double temp = 0.0;

	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp = 0.0;
			for (int s = 0; s < 3; s++) {
				for (int g = 0; g < 3; g++) {
					temp += temp_in_image[i + s][k + g] * mask[s][g];
				}
			}
			temp_out_image[i][k] = temp;
		}
	}

	// ���纻 �̹����� �ӽ� �̹��� �����ϱ�
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {

			if (temp_out_image[i][k] < 0.0) {
				outImage[i][k] = 0;
			}
			else if (temp_out_image[i][k] > 255.0) {
				outImage[i][k] = 255;
			}
			else {
				outImage[i][k] = (unsigned char)temp_out_image[i][k];
			}
		}
	}

	// �޸� ����
	free_temp_Image_mem(temp_in_image, in_height + 2);
	free_temp_Image_mem(temp_out_image, out_height);

	// ���� ���
	print_image_after();
}


// 1�� �̺� ���� ���� (������)
void trans_edge_1d_prewitt() {

	// ���纻 �̹��� �޸� ����
	free_outImage_mem();

	// ���纻 �̹��� ũ�� ����
	out_height = in_height;
	out_width = in_width;

	// ���纻 �̹��� �޸� �Ҵ�
	malloc_outImage_mem();

	// �� ���� ����ũ
	double mask_col[3][3] =		{{-1.0,-1.0,-1.0},
								{0.0,0.0,0.0},
								{1.0,1.0,1.0}};

	// �� ���� ����ũ
	double mask_row[3][3] = {	{1.0,0.0,-1.0},
								{1.0,0.0,-1.0},
								{1.0,0.0,-1.0} };

	// 1. �� ����


	// �ӽø޸� �Ǽ������� ����(�� �����)
	double** temp_in_image_col= malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image_col = malloc_temp_Image_mem(out_height, out_width);

	// �ӽø޸� 127�� �ʱ�ȭ
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image_col[i][k] = 127.0;
		}
	}

	// ���� �̹����� �ӽø޸�(�� �����)�� ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image_col[i + 1][k + 1] = inImage[i][k];
		}
	}

	// ���� 1 ����Ŭ�� ����� ���� �� ����
	double temp = 0.0;

	// �� ���� ���� ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp = 0.0;
			for (int s = 0; s < 3; s++) {
				for (int g = 0; g < 3; g++) {
					temp += temp_in_image_col[i + s][k + g] * mask_col[s][g];
				}
			}
			temp_out_image_col[i][k] = temp;
		}
	}
	
	// �� ���� �� outImage�� �����ϱ�
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			temp = 0.0;
			temp = temp_out_image_col[i][k];
			if (temp_out_image_col[i][k] < 0.0) {
				outImage[i][k] = 0;
			}
			else if (temp_out_image_col[i][k] > 255.0) {
				outImage[i][k] = 255;
			}
			else {
				outImage[i][k] = (unsigned char)temp_out_image_col[i][k];
			}
		}
	}

	// �޸� ����(�� ���� ��)
	free_temp_Image_mem(temp_in_image_col, in_height + 2);
	free_temp_Image_mem(temp_out_image_col, out_height);

	// 2. �� ����

	// �ӽø޸� �Ǽ������� ����(�� �����)
	double** temp_in_image_row = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image_row = malloc_temp_Image_mem(out_height, out_width);

	
	// �ӽø޸� 127�� �ʱ�ȭ
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image_row[i][k] = 127.0;
		}
	}
	
	// ���� �̹����� �ӽø޸�(�� �����)�� ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image_row[i + 1][k + 1] = inImage[i][k];
		}
	}

	// ���� 1 ����Ŭ�� ����� ���� �� ����
	temp = 0.0;

	// �� ���� ���� ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp = 0.0;
			for (int s = 0; s < 3; s++) {
				for (int g = 0; g < 3; g++) {
					temp += temp_in_image_row[i + s][k + g] * mask_row[s][g];
				}
			}
			temp_out_image_row[i][k] = temp;
		}
	}

	// �� ���� �� outImage�� ���ϱ�
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			temp = 0.0;
			temp += outImage[i][k];
			temp += temp_out_image_row[i][k];

			if (temp > 255) {
				outImage[i][k] = 255;
			}
			else if(temp >= 0){
				outImage[i][k] += (unsigned char)temp;
			}
			else {
				outImage[i][k] = 0;
			}
		}
	}
	// �޸� ����
	free_temp_Image_mem(temp_in_image_row, in_height + 2);
	free_temp_Image_mem(temp_out_image_row, out_height);

	// ���� ���
	print_image_after();
	
}

// ����þ� ������ + ���ö�þ� 2d ���� ����
void trans_edge_2d_LoG() {

	// ���纻 �̹��� �޸� ����
	free_outImage_mem();

	// ���纻 �̹��� ũ�� ����
	out_height = in_height;
	out_width = in_width;

	// ���纻 �̹��� �޸� �Ҵ�
	malloc_outImage_mem();

	// ���ö�þ� ����ũ
	double mask_lap[3][3]={{-1.0,-1.0,-1.0},
						  {-1.0,8.0,-1.0},
						  {-1.0,-1.0,-1.0}};

	// ����þ� ����ũ
	double mask_gau[3][3]={ {1.0 / 16,1.0 / 8,1.0 / 16},
						  {1.0 / 8,1.0 / 4,1.0 / 8},
						  {1.0 / 16,1.0 / 8,1.0 / 16} };

	// �ӽø޸� temp_Image �Ǽ������� ����

	double** temp_in_image = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image = malloc_temp_Image_mem(out_height, out_width);

	// tempinImage 127�� �ʱ�ȭ
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image[i][k] = 127.0;
		}
	}

	// inImage ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image[i + 1][k + 1] = inImage[i][k];
		}
	}

	// ���� 1 ����Ŭ�� ����� ���� �� ����
	double temp = 0.0;

	// ����þ� ������
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp = 0.0;
			for (int s = 0; s < 3; s++) {
				for (int g = 0; g < 3; g++) {
					temp += temp_in_image[i + s][k + g] * mask_gau[s][g];
				}
			}
			temp_in_image[i + 1][k + 1] = temp;
		}
	}

	// ���ö�þ� ó��
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp = 0.0;
			for (int s = 0; s < 3; s++) {
				for (int g = 0; g < 3; g++) {
					temp += temp_in_image[i + s][k + g] * mask_lap[s][g];
				}
			}
			temp_out_image[i][k] = temp;
		}
	}

	// outImage�� �����ϱ�
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {

			if (temp_out_image[i][k] < 0.0) {
				outImage[i][k] = 0;
			}
			else if (temp_out_image[i][k] > 255.0) {
				outImage[i][k] = 255;
			}
			else {
				outImage[i][k] = (unsigned char)temp_out_image[i][k];
			}
		}
	}
	// �޸� ����
	free_temp_Image_mem(temp_in_image, in_height + 2);
	free_temp_Image_mem(temp_out_image, out_height);

	// ���� ���
	print_image_after();
}