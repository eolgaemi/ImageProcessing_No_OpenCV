#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
#include <string.h>
// 외부 전역 변수

// 파일 저장 경로,														파일 포인터 
extern char parent_dir[200];	 extern char parent_out_dir[200];		extern FILE* rfp, * wfp;

// 원본 이미지의 세로 픽셀 수		원본 이미지의 가로 픽셀 수		복사본 이미지의 세로 픽셀 수		복사본 이미지의 세로 픽셀 수
extern int in_height;				extern int in_width;			extern int out_height;				extern int out_width;

// 원본이미지가 담길 이중포인터		복사본이미지가 담길 이중포인터
extern unsigned char** inImage;		extern unsigned char** outImage;

// 윈도 외부 (외부,물리적) 윈도 API <- MFC의 구버전					윈도 내부 (내부,논리적) 윈도 API <- MFC의 구버전
extern HWND hwnd;													extern HDC hdc;

//////////////////// ------- 공통함수 ------- ////////////////////
// 영상 불러오기	동일				정수 1개			실수 1개           
void load_image();	void equal_image();	int getIntValue();	float getFValue();	int getangleValue();
// 영상 출력		메뉴 출력			이미지 저장
void print_image();	void print_menu();	void save_image();
// After 영상 출력
void print_image_after();
// inImage 메모리 할당			outImage 메모리 할당
void malloc_inImage_mem();		void malloc_outImage_mem();
// inImage 메모리 해제			outImage 메모리 해제
void free_inImage_mem();		void free_outImage_mem();

// 임시 이미지 메모리 할당								임시 이미지 메모리 해제
double** malloc_temp_Image_mem(int h, int w);			void free_temp_Image_mem(double** temp_image, int h);

// --------------------- 함 수  정 의 부 --------------------- //
////////////////////////////////////////////////////////////////////
//////////////////// -------공 통 함 수 -------/////////////////////
////////////////////////////////////////////////////////////////////
void load_image() {

	// 이미지파일 경로설정
	char full_dir[300];
	char image_size[30];
	char child_file[100];

	printf("pwd ---> %s\n", parent_dir);
	printf("이미지 사이즈 ----> ");
	scanf("%s", image_size);
	printf("파일 명 ----> ");
	scanf("%s", child_file);
	strcpy(full_dir, parent_dir);
	strcat(full_dir, image_size);
	strcat(full_dir, "/");
	strcat(full_dir, child_file);
	strcat(full_dir, ".raw");

	printf("%s", full_dir);

	// 이미지 높이, 폭 구하기 //
	// 읽기 파일 포인터
	rfp = fopen(full_dir, "rb");

	// 파일의 끝으로 이동
	fseek(rfp, 0L, SEEK_END);
	long long fsize = ftell(rfp);
	fclose(rfp);

	// 높이, 폭 구하기
	in_height = in_width = (int)sqrt(fsize);
	printf("\nIN_SIZE : %d * %d", in_height, in_width);

	// 읽기 파일 포인터
	rfp = fopen(full_dir, "rb");

	// inImage 메모리 할당			
	malloc_inImage_mem();

	// 파일 --> inImage로 저장
	for (int h = 0; h < in_height; h++) {
		fread(inImage[h], sizeof(unsigned char), in_width, rfp);
	}
	fclose(rfp);

	// 불러오자마자 out_image 생성
	equal_image();

	// 불러오자마자 한번 print
	print_image();

	printf("height == %d", sizeof(inImage));
}

// 동일
void equal_image() {
	out_height = in_height;
	out_width = in_width;

	free_outImage_mem();
	printf("\nOUT_SIZE: %d * %d", out_height, out_width);
	// outImage 메모리 할당
	malloc_outImage_mem();

	// inImage -> outImage로 복사
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			outImage[i][k] = inImage[i][k];
		}
	}
}

// 영상처리 강도 입력
int getIntValue() {
	int retValue;
	printf("영상 처리 강도 입력(0~255) ---> ");
	scanf("%d", &retValue);
	return retValue;
}

// 영상처리 강도 입력
int getangleValue() {
	int retValue;
	printf("얼만큼 회전하시겠어요?(0~90) ---> ");
	scanf("%d", &retValue);
	return retValue;
}

// 감마 영상처리 강도 입력
float getFValue() {
	float retValue;
	printf("영상 처리 강도 입력(0~2) ---> ");
	scanf("%f", &retValue);
	return retValue;
}

// in_image 출력
void print_image() {
	system("cls");
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			int px = inImage[i][k];
			SetPixel(hdc, k  + 10, i + 20, RGB(px, px, px));
		}
	}
}

// out_image 출력
void print_image_after() {
	system("cls");
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			int px = outImage[i][k];
			SetPixel(hdc, k + in_width + 20, i + 20, RGB(px, px, px));
		}
	}
	printf(" 영상의 높이 ---> %d , 영상의 너비 ---> %d\n\n", out_height, out_width);
}

void print_menu() {
	printf("\n ## GrayScale Image Processing (Beta3)");
	printf("\n ## ----------------------공통기능----------------------");
	printf("\n ## %-12s %-12s %-12s", "0.열기", "1.저장", "2.종료");
	printf("\n ## ----------------------화소점기능----------------------");
	printf("\n ## %-12s %-12s %-12s", "3.초기화", "4.밝게", "5.어둡게");
	printf("\n ## %-12s %-12s %-12s", "6.흑백", "7.감마", "8.반전");
	printf("\n ## %-12s %-12s %-12s", "p.히스토그램_스트레치", "q.히스토_엔드인_스트레치", "r.히스토그램_평활화");
	printf("\n ## ----------------------통계기능----------------------");
	printf("\n ## %-12s %-12s", "9.평균", "a.중앙값");
	printf("\n ## %-12s %-12s %-12s", "b.AND 연산", "c.OR 연산", "d.XOR연산");
	printf("\n ## %-12s %-12s", "e.파라볼라CAP", "f.파라볼라CUP");
	printf("\n ## ----------------------기하학기능----------------------");
	printf("\n ## %-12s %-12s %-12s", "g.축소", "h.확대", "i.회전");
	printf("\n ## %-12s %-12s %-12s %-12s", "j.오른쪽이동", "k.왼쪽이동", "l.위쪽이동", "m.아래쪽이동");
	printf("\n ## %-12s %-12s %-12s %-12s", "n.상하미러링", "o.좌우미러링", "v.잘림없는회전(우)", "w.잘림없는회전(좌)");
	printf("\n ## ----------------------화소영역기능----------------------");
	printf("\n ## %-12s %-12s %-12s %-12s", "s.엠보싱", "t.블러링", "u. 고주파 샤프닝","x.가우시안 스무딩");
	printf("\n ## %-12s %-12s", "z.에지1차원프리윗", "A.에지2차원가우-라플");
	printf("\n 원하는 기능 입력 ------>");
}

void save_image() {
	if (outImage == NULL) {
		printf("저장할 이미지가 없습니다\n");
		return;
	}
	// 이미지파일 경로설정
	char save_file_name[100];
	char full_dir[300];
	printf("저장할 파일의 이름 ---->");
	scanf("%s", save_file_name);

	strcpy(full_dir, parent_out_dir);
	strcat(full_dir, save_file_name);
	strcat(full_dir, ".raw");

	wfp = fopen(full_dir, "wb");
	// 메모리 -> 파일 (한 행씩)
	for (int h = 0; h < out_height; h++) {
		fwrite(outImage[h], sizeof(unsigned char), out_width, wfp);
	}
	fclose(wfp);
	MessageBox(hwnd, L"저장완료", L"저장 창", NULL);
	printf("\n저장완료");
}

// inImage 메모리 할당			
void malloc_inImage_mem() {
	// inImage 메모리 할당
	inImage = (unsigned char**)malloc(sizeof(unsigned char*) * in_height);

	for (int h = 0; h < in_height; h++) {
		inImage[h] = (unsigned char*)malloc(sizeof(unsigned char) * in_width);
	}
}

// outImage 메모리 할당
void malloc_outImage_mem() {

	if (outImage != NULL) {
		return;
	}
	// outImage 메모리 할당
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

// inImage 메모리 해제
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

// outImage 메모리 해제
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