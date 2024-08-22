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

////////////////////// -------외부함수 -------//////////////////////
// 정수 1개					실수 1개					이미지 출력
extern int getIntValue();	extern float getFValue();	extern void print_image_after();

// outImage 메모리 할당					outImage 메모리 해제
extern void malloc_outImage_mem();		extern void free_outImage_mem();	extern int get_median(void);

// temp 이미지 메모리 할당									temp 이미지 메모리 해제
extern double** malloc_temp_Image_mem(int h, int w);		extern void free_temp_Image_mem(double** temp_image, int h);

////////////////////// -------함수선언 -------//////////////////////
// 엠보싱					블러링
void trans_embossing();		void trans_blurring();

// 고주파필터 샤프닝				가우시안 스무딩
void trans_high_freq_sharpening();	void trans_gausian_smoothing();

// 에지검출 1차원 프리윗			가우시안 스무딩 + 라플라시안 2d 에지 검출
void trans_edge_1d_prewitt();		void trans_edge_2d_LoG();

// 엠보싱
void trans_embossing() {

	// 복사본 이미지 메모리 해제
	free_outImage_mem();

	// 복사본 이미지 크기 설정
	out_height = in_height;
	out_width = in_width;

	// 복사본 이미지 메모리 할당
	malloc_outImage_mem();

	// 마스크
	double mask[3][3] = { {-1.0,0.0,0.0},
						  {0.0,0.0,0.0},
						  {0.0,0.0,1.0} };

	// 임시메모리 temp_Image 실수형으로 선언

	double** temp_in_image = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image = malloc_temp_Image_mem(out_height, out_width);

	// tempinImage 127로 초기화
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image[i][k] = 127.0;
		}
	}

	// inImage 복사
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image[i + 1][k + 1] = inImage[i][k];
		}
	}

	// 연산 1 싸이클의 결과를 저장 할 변수
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

	// 후처리 마스크 값의 합계에 따라서(0 or 1)
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			temp_out_image[i][k] += 127.0;
		}
	}

	// outImage에 복사하기
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
	// 메모리 해제
	free_temp_Image_mem(temp_in_image, in_height + 2);
	free_temp_Image_mem(temp_out_image, out_height);

	// 영상 출력
	print_image_after();
}

// 블러링
void trans_blurring() {

	// 복사본 이미지 메모리 해제
	free_outImage_mem();

	// 복사본 이미지 크기 설정
	out_height = in_height;
	out_width = in_width;

	// 복사본 이미지 메모리 할당
	malloc_outImage_mem();

	// 마스크
	double blur = 1.0 / 9;

	// 마스크
	double mask[3][3] = { {blur,blur,blur},
						  {blur,blur,blur},
						  {blur,blur,blur} };

	// 임시 이미지 메모리 실수형으로 할당

	double** temp_in_image = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image = malloc_temp_Image_mem(out_height, out_width);

	// 임시 이미지 회색으로 초기화
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image[i][k] = 127.0;
		}
	}

	// 임시 이미지에 원본 이미지 복사
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image[i + 1][k + 1] = inImage[i][k];
		}
	}

	// 마스킹 연산 1 싸이클의 결과를 저장 할 임시변수
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

	// 복사본 이미지에 임시 이미지 복사하기
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

	// 메모리 해제
	free_temp_Image_mem(temp_in_image, in_height + 2);
	free_temp_Image_mem(temp_out_image, out_height);

	// 영상 출력
	print_image_after();
}

// 고주파 필터 샤프닝
void trans_high_freq_sharpening() {

	// 복사본 이미지 메모리 해제
	free_outImage_mem();

	// 복사본 이미지 크기 설정
	out_height = in_height;
	out_width = in_width;

	// 복사본 이미지 메모리 할당
	malloc_outImage_mem();

	// 마스크
	double boundary = -1.0 / 9;
	double center = 8.0 / 9;

	
	double mask[3][3] = { {boundary,boundary,boundary},
						  {boundary,center,boundary},
						  {boundary,boundary,boundary} };

	// 임시 이미지 메모리 실수형으로 할당

	double** temp_in_image = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image = malloc_temp_Image_mem(out_height, out_width);

	// 임시 이미지 회색으로 초기화
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image[i][k] = 127.0;
		}
	}

	// 임시 이미지에 원본 이미지 복사
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image[i + 1][k + 1] = inImage[i][k];
		}
	}

	// 마스킹 연산 1 싸이클의 결과를 저장 할 임시변수
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

	// 복사본 이미지에 임시 이미지 복사하기
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
	// 메모리 해제
	free_temp_Image_mem(temp_in_image, in_height + 2);
	free_temp_Image_mem(temp_out_image, out_height);

	// 영상 출력
	print_image_after();
}

void trans_gausian_smoothing() {

	// 복사본 이미지 메모리 해제
	free_outImage_mem();

	// 복사본 이미지 크기 설정
	out_height = in_height;
	out_width = in_width;

	// 복사본 이미지 메모리 할당
	malloc_outImage_mem();

	// 마스크
	double mask[3][3] = { {1.0/16,1.0/8,1.0/16},
						  {1.0/8,1.0/4,1.0/8},
						  {1.0/16,1.0/8,1.0/16} };

	// 임시 이미지 메모리 실수형으로 할당
	double** temp_in_image = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image = malloc_temp_Image_mem(out_height, out_width);

	// 임시 이미지 회색으로 초기화
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image[i][k] = 127.0;
		}
	}

	// 임시 이미지에 원본 이미지 복사
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image[i + 1][k + 1] = inImage[i][k];
		}
	}

	// 마스킹 연산 1 싸이클의 결과를 저장 할 임시변수
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

	// 복사본 이미지에 임시 이미지 복사하기
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

	// 메모리 해제
	free_temp_Image_mem(temp_in_image, in_height + 2);
	free_temp_Image_mem(temp_out_image, out_height);

	// 영상 출력
	print_image_after();
}


// 1차 미분 에지 검출 (프리윗)
void trans_edge_1d_prewitt() {

	// 복사본 이미지 메모리 해제
	free_outImage_mem();

	// 복사본 이미지 크기 설정
	out_height = in_height;
	out_width = in_width;

	// 복사본 이미지 메모리 할당
	malloc_outImage_mem();

	// 행 검출 마스크
	double mask_col[3][3] =		{{-1.0,-1.0,-1.0},
								{0.0,0.0,0.0},
								{1.0,1.0,1.0}};

	// 열 검출 마스크
	double mask_row[3][3] = {	{1.0,0.0,-1.0},
								{1.0,0.0,-1.0},
								{1.0,0.0,-1.0} };

	// 1. 행 검출


	// 임시메모리 실수형으로 선언(행 검출용)
	double** temp_in_image_col= malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image_col = malloc_temp_Image_mem(out_height, out_width);

	// 임시메모리 127로 초기화
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image_col[i][k] = 127.0;
		}
	}

	// 원본 이미지를 임시메모리(행 검출용)에 복사
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image_col[i + 1][k + 1] = inImage[i][k];
		}
	}

	// 연산 1 싸이클의 결과를 저장 할 변수
	double temp = 0.0;

	// 행 검출 연산 수행
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
	
	// 행 검출 후 outImage에 복사하기
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

	// 메모리 해제(행 검출 용)
	free_temp_Image_mem(temp_in_image_col, in_height + 2);
	free_temp_Image_mem(temp_out_image_col, out_height);

	// 2. 열 검출

	// 임시메모리 실수형으로 선언(열 검출용)
	double** temp_in_image_row = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image_row = malloc_temp_Image_mem(out_height, out_width);

	
	// 임시메모리 127로 초기화
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image_row[i][k] = 127.0;
		}
	}
	
	// 원본 이미지를 임시메모리(열 검출용)에 복사
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image_row[i + 1][k + 1] = inImage[i][k];
		}
	}

	// 연산 1 싸이클의 결과를 저장 할 변수
	temp = 0.0;

	// 열 검출 연산 수행
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

	// 열 검출 후 outImage에 더하기
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
	// 메모리 해제
	free_temp_Image_mem(temp_in_image_row, in_height + 2);
	free_temp_Image_mem(temp_out_image_row, out_height);

	// 영상 출력
	print_image_after();
	
}

// 가우시안 스무딩 + 라플라시안 2d 에지 검출
void trans_edge_2d_LoG() {

	// 복사본 이미지 메모리 해제
	free_outImage_mem();

	// 복사본 이미지 크기 설정
	out_height = in_height;
	out_width = in_width;

	// 복사본 이미지 메모리 할당
	malloc_outImage_mem();

	// 라플라시안 마스크
	double mask_lap[3][3]={{-1.0,-1.0,-1.0},
						  {-1.0,8.0,-1.0},
						  {-1.0,-1.0,-1.0}};

	// 가우시안 마스크
	double mask_gau[3][3]={ {1.0 / 16,1.0 / 8,1.0 / 16},
						  {1.0 / 8,1.0 / 4,1.0 / 8},
						  {1.0 / 16,1.0 / 8,1.0 / 16} };

	// 임시메모리 temp_Image 실수형으로 선언

	double** temp_in_image = malloc_temp_Image_mem(in_height + 2, in_width + 2);
	double** temp_out_image = malloc_temp_Image_mem(out_height, out_width);

	// tempinImage 127로 초기화
	for (int i = 0; i < in_height + 2; i++) {
		for (int k = 0; k < in_width + 2; k++) {
			temp_in_image[i][k] = 127.0;
		}
	}

	// inImage 복사
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			temp_in_image[i + 1][k + 1] = inImage[i][k];
		}
	}

	// 연산 1 싸이클의 결과를 저장 할 변수
	double temp = 0.0;

	// 가우시안 스무딩
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

	// 라플라시안 처리
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

	// outImage에 복사하기
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
	// 메모리 해제
	free_temp_Image_mem(temp_in_image, in_height + 2);
	free_temp_Image_mem(temp_out_image, out_height);

	// 영상 출력
	print_image_after();
}