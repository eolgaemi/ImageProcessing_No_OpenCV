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

////////////////////// -------�Լ����� -------//////////////////////
// ���						// Ȯ��						// ȸ��						
void trans_zoom_out();		void trans_zoom_in();		void trans_rotate();		
//�߸� ���� ������ ȸ��						�߸� ���� ���� ȸ��
void trans_rotate_right_no_cutting();		void trans_rotate_left_no_cutting();
// ���������� �̵�			�������� �̵�				���� �̵�					�Ʒ��� �̵�
void trans_move_r();		void trans_move_l();		void trans_move_u();		void trans_move_d();
// ��,�� �̷���				// ��,�� �̷���
void trans_mirror_h();		void trans_mirror_v();


////////////////////// -------�ܺ��Լ� -------//////////////////////
// ���� 1��					�Ǽ� 1��					���� ���� 1��
extern int getIntValue();	extern float getFValue();	extern int getangleValue();
// �̹��� ���
extern void print_image_after();


// ������
void trans_zoom_out() {

	printf("2�� ��� �ϰڽ��ϴ�.");

	free_outImage_mem();
	int scale = 2;

	out_height = (int)(out_height / scale);
	out_width = (int)(out_width / scale);

	printf("\nOUT_SIZE: %d * %d", out_height, out_width);
	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	// inImage -> outImage�� ����
	for (int h = 0; h < in_height; h++) {
		for (int w = 0; w < in_width; w++) {
			outImage[(int)h / scale][(int)w / scale] = inImage[h][w];
		}
	}
	print_image_after();
}

// ����� = ����ȭ�� ������
void trans_zoom_in() {
	free_outImage_mem();
	printf("2�� Ȯ�� �ϰڽ��ϴ�.");
	int scale = 2;

	out_height = (int)(in_height * scale);
	out_width = (int)(in_width * scale);

	free_outImage_mem();

	printf("\nOUT_SIZE: %d * %d", out_height, out_width);
	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = inImage[(int)h / scale][(int)w / scale];
		}
	}
	print_image_after();
}

// ȸ��
// xd = cos xs - sin ys // yd = sin xs + cos ys
void trans_rotate() {

	int degree = getangleValue();

	double radian = degree * 3.141592 / 180.0;

	int center_height = in_height / 2;
	int center_width = in_width / 2;

	
	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {

			int xd = h;
			int yd = w;

			// ȸ��
			int xs = (cos(radian) * (xd - center_height)) - (sin(radian) * (yd - center_width));
			int ys = (sin(radian) * (xd - center_height)) + (cos(radian) * (yd - center_width));

			xs += center_height;
			ys += center_width;

			if ((0 <= xs && xs < out_height) && (0 <= ys && ys < out_width)) {
				outImage[xd][yd] = inImage[xs][ys];
			}
		}
	}
	print_image_after();
}

// ȸ��
// xd = cos xs - sin ys // yd = sin xs + cos ys
void trans_rotate_right_no_cutting() {
	free_outImage_mem();
	int degree = getangleValue();
	
	double radian = 0.0;
	double radian_90 = 0.0;

	radian = degree * 3.141592 / 180.0;

	if (degree > 0) {
		radian_90 = (90 - degree) * 3.141592 / 180.0;
	}
	else {
		radian_90 = (-90  - degree) * 3.141592 / 180.0;
	}
	// ����� ���̿� �ʺ�
	int background_height = 0;
	int background_width = 0;

	background_height = cos(radian) * in_height + cos(radian_90) * in_width;
	background_width = cos(radian_90) * in_height + cos(radian) * in_width;	

	if (background_height % 2) {
		background_height += 1;
	}

	if (background_width % 2) {
		background_width += 1;
	}

	// ����� ���̿� �ʺ� ���纻�� ���̿� �ʺ�� ����
	out_height = background_height;
	out_width = background_width;

	// ���纻 �̹��� �޸� ���� -> �Ҵ�
	
	malloc_outImage_mem();

	// ��� ������� �ʱ�ȭ
	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = 255;
		}
	}

	// ���� �̹����� �߾��� -> ȸ�� �� ��ǥ ����
	int in_center_height = in_height / 2;
	int in_center_width = in_width / 2;

	// ���纻 �̹����� �߾��� -> ȸ�� �� ��ǥ ����
	int out_center_height = out_height / 2;
	int out_center_width = out_width / 2;

	// ȸ��
	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {

			int xd = h;
			int yd = w;

			int xs = (cos(radian) * (xd - out_center_height)) - (sin(radian) * (yd - out_center_width));
			int ys = (sin(radian) * (xd - out_center_height)) + (cos(radian) * (yd - out_center_width));

			xs += in_center_height;
			ys += in_center_width;

			if ((0 <= xs && xs < in_height) && (0 <= ys && ys < in_width)) {
				outImage[xd][yd] = inImage[xs][ys];
			}
		}
	}
	print_image_after();
}

void trans_rotate_left_no_cutting() {
	free_outImage_mem();
	int degree = getangleValue();

	double radian = 0.0;
	double radian_90 = 0.0;

	radian = -1 * degree * 3.141592 / 180.0;

	if (degree > 0) {
		radian_90 = (90 - degree) * 3.141592 / 180.0;
	}
	else {
		radian_90 = (-90 - degree) * 3.141592 / 180.0;
	}

	// ����� ���̿� �ʺ�
	int background_height = 0;
	int background_width = 0;

	background_height = cos(radian) * in_height + cos(radian_90) * in_width;
	background_width = cos(radian_90) * in_height + cos(radian) * in_width;

	if (background_height % 2) {
		background_height += 1;
	}

	if (background_width % 2) {
		background_width += 1;
	}

	

	// ����� ���̿� �ʺ� ���纻�� ���̿� �ʺ�� ����
	out_height = background_height;
	out_width = background_width;

	// ���纻 �̹��� �޸� ���� -> �Ҵ�
	
	malloc_outImage_mem();

	// ���� �̹����� �߾��� -> ȸ�� �� ��ǥ ����
	int in_center_height = in_height / 2;
	int in_center_width = in_width / 2;

	// ���纻 �̹����� �߾��� -> ȸ�� �� ��ǥ ����
	int out_center_height = out_height / 2;
	int out_center_width = out_width / 2;

	// ��� ������� �ʱ�ȭ
	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = 255;
		}
	}

	// ȸ��
	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {

			int xd = h;
			int yd = w;

			int xs = (cos(radian) * (xd - out_center_height)) - (sin(radian) * (yd - out_center_width));
			int ys = (sin(radian) * (xd - out_center_height)) + (cos(radian) * (yd - out_center_width));

			xs += in_center_height;
			ys += in_center_width;

			if ((0 <= xs && xs < in_height) && (0 <= ys && ys < in_width)) {
				outImage[xd][yd] = inImage[xs][ys];
			}
		}
	}
	print_image_after();
}



void trans_move_r() {

	free_outImage_mem();
	printf("���������� �̵��� �ȼ� ��");
	int value = getIntValue();

	out_height = in_height;
	out_width = in_width;

	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	// outImage �������
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 0;
		}
	}

	// �ȼ��� �� ���� ���ϴ� �̵����� �����ֱ�
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			int pixel_moved = k + value;
			if (pixel_moved < out_width)
				outImage[i][pixel_moved] = inImage[i][k];
		}
	}
	print_image_after();
}

void trans_move_l() {

	free_outImage_mem();
	printf("�������� �̵��� �ȼ� ��");
	int value = getIntValue();

	out_height = in_height;
	out_width = in_width;

	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	// outImage �������
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 0;
		}
	}

	// �ȼ��� �� ���� ���ϴ� �̵����� ���ֱ�
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			int pixel_moved = k - value;
			if (pixel_moved >= 0)
				outImage[i][pixel_moved] = inImage[i][k];
		}
	}
	print_image_after();
}

void trans_move_u() {

	free_outImage_mem();
	printf("�������� �̵��� �ȼ� ��");
	int value = getIntValue();

	out_height = in_height;
	out_width = in_width;

	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	// outImage �������
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 0;
		}
	}

	// �ȼ��� �� ���� ���ϴ� �̵����� �����ֱ�
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			int pixel_moved = i - value;
			if (pixel_moved >= 0)
				outImage[pixel_moved][k] = inImage[i][k];
		}
	}
	print_image_after();
}

void trans_move_d() {

	free_outImage_mem();
	printf("�Ʒ������� �̵��� �ȼ� ��");
	int value = getIntValue();

	out_height = in_height;
	out_width = in_width;

	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	// outImage �������
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 0;
		}
	}

	// �ȼ��� �� ���� ���ϴ� �̵����� �����ֱ�
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			int pixel_moved = i + value;
			if (pixel_moved < out_height)
				outImage[pixel_moved][k] = inImage[i][k];
		}
	}
	print_image_after();
}


void trans_mirror_h() {

	free_outImage_mem();

	out_height = in_height;
	out_width = in_width;

	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	// ���Ʒ� �ٲ�ٲ�
	for (int i = 0; i < out_height / 2; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[out_height - 1 - i][k] = inImage[i][k];
			outImage[i][k] = inImage[out_height - 1 - i][k];
		}
	}

	// �̹��� ���̰� Ȧ�����~
	if (out_height % 2) {
		for (int k = 0; k < out_width; k++) {
			outImage[out_height / 2 + 1][k] = inImage[out_height / 2 + 1][k];
		}
	}
	print_image_after();
}

void trans_mirror_v() {
	free_outImage_mem();

	out_height = in_height;
	out_width = in_width;

	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	// �¿� �ٲ�ٲ�
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width / 2; k++) {
			outImage[i][out_width - 1 - k] = inImage[i][k];
			outImage[i][k] = inImage[i][out_width - 1 - k];
		}
	}

	// �̹��� ���̰� Ȧ�����~
	if (out_width % 2) {
		for (int i = 0; i < out_height; i++) {
			outImage[i][out_width / 2 + 1] = inImage[i][out_width / 2 + 1];
		}
	}
	print_image_after();
}