#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>


// ---------------------  전 역 변 수 --------------------- // 
// 초기 이미지의 가로 픽셀 수		초기 이미지의 세로 픽셀 수
int in_height = 0;					int in_width = 0;

// 영상 처리 후 가로 픽셀 수		영상 처리 후 세로 픽셀 수
int out_height = 0;					int out_width = 0;


// 파일의 최종 경로 명이 담길 변수
//char parent_dir[200] = "D:/YongJae/RAW_IMAGE/Etc_Raw/";
char parent_dir[200] = "D:/C_git_repo/C_learning/Etc_Raw/";
char parent_out_dir[200] = "D:/YongJae/OUT_IMAGE/";

//파일 포인터 rfp는 읽기 wfp는 쓰기
FILE* rfp, * wfp;

// 윈도 외부 (외부,물리적) 윈도 API <- MFC의 구버전		윈도 내부 (내부,논리적) 윈도 API <- MFC의 구버전
HWND hwnd;												HDC hdc;

// 원본이미지가 담길 이중포인터		복사본이미지가 담길 이중포인터
unsigned char** inImage = NULL;		unsigned char** outImage = NULL;

// 
// --------------------- 함 수  선 언 부 --------------------- //
// 

//////////////////// ------- 공통함수 ------- ////////////////////
// 영상 불러오기			동일					
extern void load_image();	extern void equal_image();	
// 영상 출력				메뉴 출력					이미지 저장
extern void print_image();	extern void print_menu();	extern void save_image();

// inImage 메모리 할당				outImage 메모리 할당
extern void malloc_inImage_mem();	extern void malloc_outImage_mem();
// inImage 메모리 해제				outImage 메모리 해제
extern void free_inImage_mem();		extern void free_outImage_mem();

//////////////////// -------화소점처리함수 -------////////////////////
//밝게 하기						어둡게 하기					흑백 만들기
extern void trans_brighter();	extern void trans_darker();	extern void trans_baw();
//감마 보정					반전 처리
extern void trans_gamma();	extern void trans_reverse();
// AND 연산					OR연산					XOR연산
extern void trans_and();	extern void trans_or();	extern void trans_xor();
//파라볼라CAP					파라볼라CUP
extern void trans_para_cap();	extern void trans_para_cup();
// 히스토그램 스트레치			히스토그램 엔드인 스트레치
extern void trans_hist_stretch();	extern void trans_hist_endin_stretch();
// 히스토그램 평활화
extern void trans_hist_equal();

//////////////////// -------기하학처리함수 -------////////////////////
// 축소							확대
extern void trans_zoom_out();	extern void trans_zoom_in();
// 회전							잘림 없는 오른쪽 회전								잘림 없는 왼쪽 회전
extern void trans_rotate();		extern void trans_rotate_right_no_cutting();		extern void trans_rotate_left_no_cutting();
// 오른쪽으로 이동				왼쪽으로 이동					위로 이동						아래로 이동
extern void trans_move_r();		extern void trans_move_l();		extern void trans_move_u();		extern void trans_move_d();
// 상하 미러링					좌우 미러링
extern void trans_mirror_h();	extern void trans_mirror_v();

//////////////////// -------화소영역처리함수 -------////////////////////
// 엠보싱						블러링
extern void trans_embossing();	extern void trans_blurring();
// 고주파 샤프닝							가우시안 스무딩
extern void trans_high_freq_sharpening();	extern void trans_gausian_smoothing();
// 에지검출 1차원 프리윗					에지 검출 2차원
extern void trans_edge_1d_prewitt();		extern void trans_edge_2d_LoG();


//////////////////// -------영상통계함수 -------////////////////////
//  평균값 가져오기				중앙값 가져오기
extern int get_average(void);	extern int get_median(void);

// 메인함수
int main(void) {

	// Windows 출력용 윈도우 초기화
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(NULL); // Windows 11
	//hdc = GetWindowDC(hwnd);
	// 1. 영상 불러오기
	load_image();

	// 2. 메뉴 선택
	char inKey = 0;
	while (inKey != '2') {
		// 메뉴 출력
		print_menu();

		// 메뉴 선택
		inKey = _getch();

		// 명령어 비우기
		system("cls");

		switch (inKey) {
		case '0': {
			// 새로운 영상 파일 열기

			// inImage 메모리 해제
			free_inImage_mem();
			// outImage 메모리 해제
			free_outImage_mem();
			// 영상 파일 열기
			load_image();
			break;
		}
		case '1': save_image(); break; // 영상파일 저장
		case '2': printf("프로그램이 종료 되었습니다."); break; // 프로그램 종료
		case '3': {
			// outImage 메모리 해제
			free_outImage_mem();
			equal_image();
			print_image();
			break; // 영상 다시 복사해오기(초기화)
		}
		case '4': trans_brighter(); break;	// 밝게
		case '5': trans_darker(); break;	// 어둡게
		case '6': trans_baw(); break;		// 흑백
		case '7': trans_gamma(); break;		// 감마보정
		case '8': trans_reverse(); break;	// 반전
		case '9': {
			int avg = get_average();
			printf("\n이 이미지의 평균값은 --->%d 입니다\n", avg);
			break;							// 영상의 평균값 출력
		}
		case 'a': {
			int median = get_median();
			printf("\n이 이미지의 중앙값은 --->%d 입니다\n", median);
			break;							// 영상의 중앙값 출력
		}
		case 'b': trans_and(); break;		// AND 연산
		case 'c': trans_or(); break;		// OR 연산
		case 'd': trans_xor(); break;		// XOR 연산
		case 'e': trans_para_cap(); break;	// 파라볼라 CAP
		case 'f': trans_para_cup(); break;	// 파라볼라 CUP
		case 'g': trans_zoom_out(); break;	// 영상 축소
		case 'h': trans_zoom_in(); break;	// 영상 확대
		case 'i': trans_rotate(); break;	// 회전(영상 잘림)
		case 'j': trans_move_r(); break;	// 이동(오른쪽)(영상 잘림)
		case 'k': trans_move_l(); break;	// 이동(왼  쪽)(영상 잘림)
		case 'l': trans_move_u(); break;	// 이동(위	쪽)(영상 잘림)
		case 'm': trans_move_d(); break;	// 이동(아래쪽)(영상 잘림)
		case 'n': trans_mirror_h(); break;	// 상하반전
		case 'o': trans_mirror_v(); break;	// 좌우반전
		case 'p': trans_hist_stretch(); break;					// 히스토그램 스트레치
		case 'q': trans_hist_endin_stretch(); break;			// 히스토그램 엔드인 스트레치
		case 'r': trans_hist_equal(); break;					// 히스토그램 평활화
		case 's': trans_embossing(); break;						// 엠보싱
		case 't': trans_blurring(); break;						// 블러링
		case 'u': trans_high_freq_sharpening(); break;			// 고주파 샤프닝
		case 'v': trans_rotate_right_no_cutting(); break;		// 회전(오른쪽)(영상 안 잘림)
		case 'w': trans_rotate_left_no_cutting(); break;		// 회전(왼	쪽)(영상 안 잘림)
		case 'x': trans_gausian_smoothing(); break;				// 가우시안 스무딩
		case 'z': trans_edge_1d_prewitt(); break;				// 에지검출 1차원 프리윗
		case 'A': trans_edge_2d_LoG(); break;					// 가우시안 스무딩 + 라플라시안 2d 에지 검출
			
		default: printf("해당 기능이 없습니다. 다시선택해 주세요\n");  break;
		}
	}
	// inImage 메모리 해제
	free_inImage_mem();

	// outImage 메모리 해제
	free_outImage_mem();

	return 0;
}