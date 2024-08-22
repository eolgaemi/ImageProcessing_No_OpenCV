#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 외부 전역 변수

// 초기 이미지의 가로 픽셀 수
extern int in_height;
// 초기 이미지의 세로 픽셀 수
extern int in_width;
// 영상 처리 후 가로 픽셀 수
extern int out_height;
// 영상 처리 후 세로 픽셀 수
extern int out_width;

// 원본이미지가 담길 이중포인터
extern unsigned char** inImage;
// 복사본이미지가 담길 이중포인터
extern unsigned char** outImage;

////////////////////// -------함수선언 -------//////////////////////
// 
//밝게 하기             어둡게 하기          흑백 만들기
void trans_brighter();  void trans_darker(); void trans_baw();

//감마 보정             반전 처리
void trans_gamma();     void trans_reverse();

//AND 연산			 OR연산			     XOR연산
void trans_and();    void trans_or();    void trans_xor();

//파라볼라CAP			파라볼라CUP
void trans_para_cap();  void trans_para_cup();

//히스토그램 스트레치			히스토그램 엔드인 스트레치
void trans_hist_stretch();		void trans_hist_endin_stretch();

//히스토그램 평활화
void trans_hist_equal();

////////////////////// -------외부함수 -------//////////////////////
// 정수 1개					실수 1개					이미지 출력
extern int getIntValue();	extern float getFValue();	extern void print_image_after();
// outImage 메모리 할당					outImage 메모리 해제
extern void malloc_outImage_mem();		extern void free_outImage_mem();

////////////////////////////////////////////////////////////////////
////////////////////// -------함수정의 -------//////////////////////
////////////////////////////////////////////////////////////////////
// 밝게
void trans_brighter() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	printf("\n 밝게 ");
	int value = getIntValue();
	printf("\n");
	printf("-------%d 만큼 밝게 해볼게요 -------\n", value);
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			// 클래핑
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
// 어둡게
void trans_darker() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	printf("\n 어둡게 ");
	int value = getIntValue();
	printf("-------%d 만큼 어둡게 해볼게요 -------\n", value);
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			// 클래핑
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
// 흑백
void trans_baw() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	printf("\n 흑백 ");
	int value = getIntValue();
	printf("-------%d를 기준으로 흑백으로 나눌게요 -------\n", value);
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
// 감마 보정
void trans_gamma() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	float gamma = getFValue();
	unsigned char temp = 0;
	printf("\n");
	printf("-------감마보정(%0.2f) 해볼게요 -------\n", gamma);
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
// 반전처리
void trans_reverse() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	printf("-------이미지 반전 처리 -------\n");
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 255 - outImage[i][k];
		}
	}
	print_image_after();
}

// 비트와이즈 AND
void trans_and() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	int filter;
	printf("AND 연산 값 입력 -----> ");
	scanf("%d", &filter);
	printf("\n");
	printf("-------AND 연산(%d) 해볼게요 -------\n", (unsigned char)filter);

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = (outImage[h][w] & filter);
		}
	}
	print_image_after();
}

// 비트와이즈 OR
void trans_or() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	int filter;
	printf("OR 연산 값 입력 -----> ");
	scanf("%d", &filter);
	printf("\n");
	printf("-------OR 연산(%d) 해볼게요 -------\n", (unsigned char)filter);

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = (outImage[h][w] | filter);
		}
	}
	print_image_after();
}

// 비트와이즈 XOR
void trans_xor() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();
	int filter;
	printf("XOR 연산 값 입력 -----> ");
	scanf("%d", &filter);
	printf("\n");
	printf("-------XOR 연산(%d) 해볼게요 -------\n", (unsigned char)filter);

	for (int h = 0; h < out_height; h++) {
		for (int w = 0; w < out_width; w++) {
			outImage[h][w] = (outImage[h][w] ^ filter);
		}
	}
	print_image_after();
}

// 파라볼라 CAP
void trans_para_cap() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();

	printf("\n 파라볼라 CAP  ");
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = 255 - (255 * pow(outImage[i][k] / 127 - 1, 2));
		}
	}
	printf("\n");
	print_image_after();
}

// 파라볼라 CUP
void trans_para_cup() {
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;
	malloc_outImage_mem();

	printf("\n 파라볼라 CUP  ");
	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {
			outImage[i][k] = (unsigned char)-1 * 255 * pow((outImage[i][k] / 127 - 1), 2) + 255;
		}
	}
	printf("\n");
	print_image_after();
}

// 0~255까지 이미 값들이 있으면 일부 손실을 해서 늘려줘야 한다.

void trans_hist_stretch() {
	free_outImage_mem();
	
	out_height = in_height;
	out_width = in_width;

	malloc_outImage_mem();
	printf("\nOUT_SIZE: %d * %d", out_height, out_width);

	// outImage 메모리 할당
	
	// 영상의 첫번째 픽셀의 값으로 초기화 하면 좋다
	/*
	int highest = 0;
	int lowest = 255;
	*/

	// 최대 명도값 , 최소 명도 값 구하기
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
	// 영상 변환 후 저장하기
	int old = inImage[0][0];
	int new = outImage[0][0];

	for (int i = 0; i < out_height; i++) {
		for (int k = 0; k < out_width; k++) {

			old = inImage[i][k];
			new = (int)((double)(old - lowest) / (double)(highest - lowest) * 255.0);
			outImage[i][k] = new;
		}
	}

	// 화면출력
	print_image_after();
}

void trans_hist_endin_stretch() {
	
	free_outImage_mem();
	out_height = in_height;
	out_width = in_width;

	printf("\nOUT_SIZE: %d * %d", out_height, out_width);

	// outImage 메모리 할당
	malloc_outImage_mem();

	// 영상의 첫번째 픽셀의 값으로 초기화 하면 좋다
	/*
	int highest = 0;
	int lowest = 255;
	*/
	// 최대 명도값 , 최소 명도 값 구하기
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
	// 최대 명도값 , 최소 명도 값을 적절히 조정하기
	highest -= 10;
	lowest += 10;


	// 영상 변환 후 저장하기
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
	// 화면 출력 하기
	print_image_after();
}


// 히스토그램 평활화
void trans_hist_equal() {
	free_outImage_mem();
	// 높이 넓이 저장
	out_height = in_height;
	out_width = in_width;
	printf("\nOUT_SIZE: %d * %d", out_height, out_width);
	malloc_outImage_mem();

	// 히스토그램 메모리 할당
	double* histo_gram = (double*)malloc(sizeof(double) * 256);

	// 히스토그램 초기화
	for (int i = 0; i < 256; i++) {
		histo_gram[i] = 0.0;
	}

	// 히스토그램 빈도 카운트
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			histo_gram[inImage[i][k]] += 1.0;
		}
	}

	// 누적합 히스토그램 메모리 할당
	double* sum_histo_gram = (double*)malloc(sizeof(double) * 256);

	// 첫 요소만 복사
	sum_histo_gram[0] = histo_gram[0];

	// 누적합 구현
	for (int i = 1; i < 256; i++) {
		sum_histo_gram[i] = sum_histo_gram[i - 1] + histo_gram[i];
	}

	// 할 일 마친 초기 히스토그램 메모리 해제
	free(histo_gram);

	// 정규화 히스토그램 메모리 할당
	double* normal_histo_gram = (double*)malloc(sizeof(double) * 256);

	// 총 화소 수
	long long pixel_n = in_height * in_width;

	// 최대 명도
	double highest_pixel = inImage[0][0];

	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			if (highest_pixel < inImage[i][k]) {
				highest_pixel = inImage[i][k];
			}
		}
	}

	// 정규화
	for (int i = 0; i < 256; i++) {
		normal_histo_gram[i] = sum_histo_gram[i] * (1.0 / pixel_n) * highest_pixel;
	}

	// 할 일 마친 누적합 히스토그램 메모리 해제
	free(sum_histo_gram);

	// 이미지 생성
	for (int i = 0; i < in_height; i++) {
		for (int k = 0; k < in_width; k++) {
			outImage[i][k] = (unsigned char)normal_histo_gram[inImage[i][k]];
		}
	}

	// 할 일 마친 정규화 히스토그램 메모리 해제
	free(normal_histo_gram);

	// 영상 출력
	print_image_after();
}