#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
#include <string.h>
// �ܺ� ���� ����

// ���� ���� ���,														���� ������ 
extern char parent_dir[200];	 extern char parent_out_dir[200];		extern FILE* rfp, * wfp;

// ���� �̹����� ���� �ȼ� ��		���� �̹����� ���� �ȼ� ��		���纻 �̹����� ���� �ȼ� ��		���纻 �̹����� ���� �ȼ� ��
extern int in_height;				extern int in_width;			extern int out_height;				extern int out_width;

// �����̹����� ��� ����������		���纻�̹����� ��� ����������
extern unsigned char** inImage;		extern unsigned char** outImage;

// ���� �ܺ� (�ܺ�,������) ���� API <- MFC�� ������					���� ���� (����,����) ���� API <- MFC�� ������
extern HWND hwnd;													extern HDC hdc;

//////////////////// ------- �����Լ� ------- ////////////////////
// ���� �ҷ�����	����				���� 1��			�Ǽ� 1��           
void load_image();	void equal_image();	int getIntValue();	float getFValue();	int getangleValue();
// ���� ���		�޴� ���			�̹��� ����
void print_image();	void print_menu();	void save_image();
// After ���� ���
void print_image_after();
// inImage �޸� �Ҵ�			outImage �޸� �Ҵ�
void malloc_inImage_mem();		void malloc_outImage_mem();
// inImage �޸� ����			outImage �޸� ����
void free_inImage_mem();		void free_outImage_mem();

// �ӽ� �̹��� �޸� �Ҵ�								�ӽ� �̹��� �޸� ����
double** malloc_temp_Image_mem(int h, int w);			void free_temp_Image_mem(double** temp_image, int h);

// --------------------- �� ��  �� �� �� --------------------- //
////////////////////////////////////////////////////////////////////
//////////////////// -------�� �� �� �� -------/////////////////////
////////////////////////////////////////////////////////////////////
void load_image() {

	// �̹������� ��μ���
	char full_dir[300];
	char image_size[30];
	char child_file[100];

	printf("pwd ---> %s\n", parent_dir);
	printf("�̹��� ������ ----> ");
	scanf("%s", image_size);
	printf("���� �� ----> ");
	scanf("%s", child_file);
	strcpy(full_dir, parent_dir);
	strcat(full_dir, image_size);
	strcat(full_dir, "/");
	strcat(full_dir, child_file);
	strcat(full_dir, ".raw");

	printf("%s", full_dir);

	// �̹��� ����, �� ���ϱ� //
	// �б� ���� ������
	rfp = fopen(full_dir, "rb");

	// ������ ������ �̵�
	fseek(rfp, 0L, SEEK_END);
	long long fsize = ftell(rfp);
	fclose(rfp);

	// ����, �� ���ϱ�
	in_height = in_width = (int)sqrt(fsize);
	printf("\nIN_SIZE : %d * %d", in_height, in_width);

	// �б� ���� ������
	rfp = fopen(full_dir, "rb");

	// inImage �޸� �Ҵ�			
	malloc_inImage_mem();

	// ���� --> inImage�� ����
	for (int h = 0; h < in_height; h++) {
		fread(inImage[h], sizeof(unsigned char), in_width, rfp);
	}
	fclose(rfp);

	// �ҷ����ڸ��� out_image ����
	equal_image();

	// �ҷ����ڸ��� �ѹ� print
	print_image();

	printf("height == %d", sizeof(inImage));
}

// ����
void equal_image() {
	out_height = in_height;
	out_width = in_width;

	free_outImage_mem();
	printf("\nOUT_SIZE: %d * %d", out_height, out_width);
	// outImage �޸� �Ҵ�
	malloc_outImage_mem();

	// inImage -> outImage�� ����
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
}

// ����ó�� ���� �Է�
int getIntValue() {
	int retValue;
	printf("���� ó�� ���� �Է�(0~255) ---> ");
	scanf("%d", &retValue);
	return retValue;
}

// ����ó�� ���� �Է�
int getangleValue() {
	int retValue;
	printf("��ŭ ȸ���Ͻðھ��?(0~90) ---> ");
	scanf("%d", &retValue);
	return retValue;
}

// ���� ����ó�� ���� �Է�
float getFValue() {
	float retValue;
	printf("���� ó�� ���� �Է�(0~2) ---> ");
	scanf("%f", &retValue);
	return retValue;
}

// in_image ���
void print_image() {
	system("cls");
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			int px = inImage[i][k];
			SetPixel(hdc, k  + 10, i + 20, RGB(px, px, px));
		}
	}
}

// out_image ���
void print_image_after() {
	system("cls");
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + in_width + 20, i + 20, RGB(px, px, px));
		}
	}
	printf(" ������ ���� ---> %d , ������ �ʺ� ---> %d\n\n", out_height, out_width);
}

void print_menu() {
	printf("\n ## GrayScale Image Processing (Beta3)");
	printf("\n ## ----------------------������----------------------");
	printf("\n ## %-12s %-12s %-12s", "0.����", "1.����", "2.����");
	printf("\n ## ----------------------ȭ�������----------------------");
	printf("\n ## %-12s %-12s %-12s", "3.�ʱ�ȭ", "4.���", "5.��Ӱ�");
	printf("\n ## %-12s %-12s %-12s", "6.���", "7.����", "8.����");
	printf("\n ## %-12s %-12s %-12s", "p.������׷�_��Ʈ��ġ", "q.������_������_��Ʈ��ġ", "r.������׷�_��Ȱȭ");
	printf("\n ## ----------------------�����----------------------");
	printf("\n ## %-12s %-12s", "9.���", "a.�߾Ӱ�");
	printf("\n ## %-12s %-12s %-12s", "b.AND ����", "c.OR ����", "d.XOR����");
	printf("\n ## %-12s %-12s", "e.�Ķ󺼶�CAP", "f.�Ķ󺼶�CUP");
	printf("\n ## ----------------------�����б��----------------------");
	printf("\n ## %-12s %-12s %-12s", "g.���", "h.Ȯ��", "i.ȸ��");
	printf("\n ## %-12s %-12s %-12s %-12s", "j.�������̵�", "k.�����̵�", "l.�����̵�", "m.�Ʒ����̵�");
	printf("\n ## %-12s %-12s %-12s %-12s", "n.���Ϲ̷���", "o.�¿�̷���", "v.�߸�����ȸ��(��)", "w.�߸�����ȸ��(��)");
	printf("\n ## ----------------------ȭ�ҿ������----------------------");
	printf("\n ## %-12s %-12s %-12s %-12s", "s.������", "t.����", "u. ������ ������","x.����þ� ������");
	printf("\n ## %-12s %-12s", "z.����1����������", "A.����2��������-����");
	printf("\n ���ϴ� ��� �Է� ------>");
}

void save_image() {
	if (outImage == NULL) {
		printf("������ �̹����� �����ϴ�\n");
		return;
	}
	// �̹������� ��μ���
	char save_file_name[100];
	char full_dir[300];
	printf("������ ������ �̸� ---->");
	scanf("%s", save_file_name);

	strcpy(full_dir, parent_out_dir);
	strcat(full_dir, save_file_name);
	strcat(full_dir, ".raw");

	wfp = fopen(full_dir, "wb");
	// �޸� -> ���� (�� �྿)
	for (int h = 0; h < out_height; h++) {
		fwrite(outImage[h], sizeof(unsigned char), out_width, wfp);
	}
	fclose(wfp);
	MessageBox(hwnd, L"����Ϸ�", L"���� â", NULL);
	printf("\n����Ϸ�");
}

// inImage �޸� �Ҵ�			
void malloc_inImage_mem() {
	// inImage �޸� �Ҵ�
	inImage = (unsigned char**)malloc(sizeof(unsigned char*) * in_height);

	for (int h = 0; h < in_height; h++) {
		inImage[h] = (unsigned char*)malloc(sizeof(unsigned char) * in_width);
	}
}

// outImage �޸� �Ҵ�
void malloc_outImage_mem() {

	if (outImage != NULL) {
		return;
	}
	// outImage �޸� �Ҵ�
	outImage = (unsigned char**)malloc(sizeof(unsigned char*) * out_height);

	for (int h = 0; h < out_height; h++) {
		outImage[h] = (unsigned char*)malloc(sizeof(unsigned char) * out_width);
	}

	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
}

// inImage �޸� ����
void free_inImage_mem() {

	if (inImage == NULL) {
		return;
	}
	for (int h = 0; h < in_height; h++) {
		free(inImage[h]);
	}
	free(inImage);
	inImage = NULL;
}

// outImage �޸� ����
void free_outImage_mem() {

	if (outImage == NULL) {
		return;
	}
	for (int h = 0; h < out_height; h++) {
		free(outImage[h]);
	}
	free(outImage);
	outImage = NULL;
}

double** malloc_temp_Image_mem(int h, int w) {
	double** temp_image = (double**)malloc(sizeof(double*) * h);
	for (int i = 0; i < h; i++) {
		temp_image[i] = (double*)malloc(sizeof(double) * w);
	}

	return temp_image;
}


void free_temp_Image_mem(double** temp_image, int h) {

	if (temp_image == NULL) {
		return;
	}
	for (int i = 0; i < h; i++) {
		free(temp_image[i]);
	}

	free(temp_image);
}