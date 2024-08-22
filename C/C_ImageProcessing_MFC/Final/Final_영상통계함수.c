#include <stdlib.h>

// 외부 전역변수
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



//////////////////// -------영상통계함수 -------////////////////////
//  평균값 가져오기		중앙값 가져오기
int get_average(void);  int get_median(void);

//////////////////// -------외부정렬함수 -------////////////////////
// 합병 정렬
extern void mergesort(unsigned char arr[], int low, int high);


// 평균
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

// 중앙값
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