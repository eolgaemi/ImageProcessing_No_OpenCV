#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 외부 전역 변수

// 초기 이미지의 가로 픽셀 수		초기 이미지의 세로 픽셀 수
extern int in_height;				extern int in_width;

// 영상 처리 후 가로 픽셀 수		영상 처리 후 세로 픽셀 수
extern int out_height;				extern int out_width;

// 원본이미지가 담길 이중포인터		복사본이미지가 담길 이중포인터
extern unsigned char** inImage;		extern unsigned char** outImage;

////////////////////// -------함수선언 -------//////////////////////
// 축소						// 확대						// 회전						
void trans_zoom_out();		void trans_zoom_in();		void trans_rotate();		
//잘림 없는 오른쪽 회전						잘림 없는 왼쪽 회전
void trans_rotate_right_no_cutting();		void trans_rotate_left_no_cutting();
// 오른쪽으로 이동			왼쪽으로 이동				위로 이동					아래로 이동
void trans_move_r();		void trans_move_l();		void trans_move_u();		void trans_move_d();
// 상,하 미러링				// 좌,우 미러링
void trans_mirror_h();		void trans_mirror_v();


////////////////////// -------외부함수 -------//////////////////////
// 정수 1개					실수 1개					각도 정수 1개
extern int getIntValue();	extern float getFValue();	extern int getangleValue();
// 이미지 출력
extern void print_image_after();


// 포워딩
void trans_zoom_out() {

	printf("2배 축소 하겠습니다.");

	free_outImage_mem();
	int scale = 2;

	out_height = (int)(out_height / scale);
	out_width = (int)(out_width / scale);

	printf("\nOUT_SIZE: %d * %d", out_height, out_width);
	// outImage 메모리 할당
	malloc_outImage_mem();

	// inImage -> outImage로 복사
	for (int h = 0; h < in_height; h++) {
		for (int w = 0; w < in_width; w++) {
			outImage[(int)h / scale][(int)w / scale] = inImage[h][w];
		}
	}
	print_image_after();
}

// 백워딩 = 인접화소 보간법
void trans_zoom_in() {
	free_outImage_mem();
	printf("2배 확대 하겠습니다.");
	int scale = 2;

	out_height = (int)(in_height * scale);
	out_width = (int)(in_width * scale);

	free_outImage_mem();

	printf("\nOUT_SIZE: %d * %d", out_height, out_width);
	// outImage 메모리 할당
	malloc_outImage_mem();

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = inImage[(int)h / scale][(int)w / scale];
		}
	}
	print_image_after();
}

// 회전
// xd = cos xs - sin ys // yd = sin xs + cos ys
void trans_rotate() {

	int degree = getangleValue();

	double radian = degree * 3.141592 / 180.0;

	int center_height = in_height / 2;
	int center_width = in_width / 2;

	
	// outImage 메모리 할당
	malloc_outImage_mem();

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {

			int xd = h;
			int yd = w;

			// 회전
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

// 회전
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
	// 배경의 높이와 너비
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

	// 배경의 높이와 너비를 복사본의 높이와 너비로 설정
	out_height = background_height;
	out_width = background_width;

	// 복사본 이미지 메모리 해제 -> 할당
	
	malloc_outImage_mem();

	// 배경 흰색으로 초기화
	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = 255;
		}
	}

	// 원본 이미지의 중앙점 -> 회전 후 좌표 보정
	int in_center_height = in_height / 2;
	int in_center_width = in_width / 2;

	// 복사본 이미지의 중앙점 -> 회전 후 좌표 보정
	int out_center_height = out_height / 2;
	int out_center_width = out_width / 2;

	// 회전
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

	// 배경의 높이와 너비
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

	

	// 배경의 높이와 너비를 복사본의 높이와 너비로 설정
	out_height = background_height;
	out_width = background_width;

	// 복사본 이미지 메모리 해제 -> 할당
	
	malloc_outImage_mem();

	// 원본 이미지의 중앙점 -> 회전 후 좌표 보정
	int in_center_height = in_height / 2;
	int in_center_width = in_width / 2;

	// 복사본 이미지의 중앙점 -> 회전 후 좌표 보정
	int out_center_height = out_height / 2;
	int out_center_width = out_width / 2;

	// 배경 흰색으로 초기화
	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = 255;
		}
	}

	// 회전
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
	printf("오른쪽으로 이동할 픽셀 수");
	int value = getIntValue();

	out_height = in_height;
	out_width = in_width;

	// outImage 메모리 할당
	malloc_outImage_mem();

	// outImage 검은배경
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 0;
		}
	}

	// 픽셀의 열 값에 원하는 이동범위 더해주기
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
	printf("왼쪽으로 이동할 픽셀 수");
	int value = getIntValue();

	out_height = in_height;
	out_width = in_width;

	// outImage 메모리 할당
	malloc_outImage_mem();

	// outImage 검은배경
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 0;
		}
	}

	// 픽셀의 열 값에 원하는 이동범위 빼주기
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
	printf("위쪽으로 이동할 픽셀 수");
	int value = getIntValue();

	out_height = in_height;
	out_width = in_width;

	// outImage 메모리 할당
	malloc_outImage_mem();

	// outImage 검은배경
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 0;
		}
	}

	// 픽셀의 열 값에 원하는 이동범위 더해주기
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
	printf("아래쪽으로 이동할 픽셀 수");
	int value = getIntValue();

	out_height = in_height;
	out_width = in_width;

	// outImage 메모리 할당
	malloc_outImage_mem();

	// outImage 검은배경
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 0;
		}
	}

	// 픽셀의 열 값에 원하는 이동범위 더해주기
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

	// outImage 메모리 할당
	malloc_outImage_mem();

	// 위아래 바꿔바꿔
	for (int i = 0; i < out_height / 2; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[out_height - 1 - i][k] = inImage[i][k];
			outImage[i][k] = inImage[out_height - 1 - i][k];
		}
	}

	// 이미지 높이가 홀수라면~
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

	// outImage 메모리 할당
	malloc_outImage_mem();

	// 좌우 바꿔바꿔
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width / 2; k++) {
			outImage[i][out_width - 1 - k] = inImage[i][k];
			outImage[i][k] = inImage[i][out_width - 1 - k];
		}
	}

	// 이미지 넓이가 홀수라면~
	if (out_width % 2) {
		for (int i = 0; i < out_height; i++) {
			outImage[i][out_width / 2 + 1] = inImage[i][out_width / 2 + 1];
		}
	}
	print_image_after();
}