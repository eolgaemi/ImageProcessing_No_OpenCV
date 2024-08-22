#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>


// ---------------------  �� �� �� �� --------------------- // 
// �ʱ� �̹����� ���� �ȼ� ��		�ʱ� �̹����� ���� �ȼ� ��
int in_height = 0;					int in_width = 0;

// ���� ó�� �� ���� �ȼ� ��		���� ó�� �� ���� �ȼ� ��
int out_height = 0;					int out_width = 0;


// ������ ���� ��� ���� ��� ����
//char parent_dir[200] = "D:/YongJae/RAW_IMAGE/Etc_Raw/";
char parent_dir[200] = "D:/C_git_repo/C_learning/Etc_Raw/";
char parent_out_dir[200] = "D:/YongJae/OUT_IMAGE/";

//���� ������ rfp�� �б� wfp�� ����
FILE* rfp, * wfp;

// ���� �ܺ� (�ܺ�,������) ���� API <- MFC�� ������		���� ���� (����,����) ���� API <- MFC�� ������
HWND hwnd;												HDC hdc;

// �����̹����� ��� ����������		���纻�̹����� ��� ����������
unsigned char** inImage = NULL;		unsigned char** outImage = NULL;

// 
// --------------------- �� ��  �� �� �� --------------------- //
// 

//////////////////// ------- �����Լ� ------- ////////////////////
// ���� �ҷ�����			����					
extern void load_image();	extern void equal_image();	
// ���� ���				�޴� ���					�̹��� ����
extern void print_image();	extern void print_menu();	extern void save_image();

// inImage �޸� �Ҵ�				outImage �޸� �Ҵ�
extern void malloc_inImage_mem();	extern void malloc_outImage_mem();
// inImage �޸� ����				outImage �޸� ����
extern void free_inImage_mem();		extern void free_outImage_mem();

//////////////////// -------ȭ����ó���Լ� -------////////////////////
//��� �ϱ�						��Ӱ� �ϱ�					��� �����
extern void trans_brighter();	extern void trans_darker();	extern void trans_baw();
//���� ����					���� ó��
extern void trans_gamma();	extern void trans_reverse();
// AND ����					OR����					XOR����
extern void trans_and();	extern void trans_or();	extern void trans_xor();
//�Ķ󺼶�CAP					�Ķ󺼶�CUP
extern void trans_para_cap();	extern void trans_para_cup();
// ������׷� ��Ʈ��ġ			������׷� ������ ��Ʈ��ġ
extern void trans_hist_stretch();	extern void trans_hist_endin_stretch();
// ������׷� ��Ȱȭ
extern void trans_hist_equal();

//////////////////// -------������ó���Լ� -------////////////////////
// ���							Ȯ��
extern void trans_zoom_out();	extern void trans_zoom_in();
// ȸ��							�߸� ���� ������ ȸ��								�߸� ���� ���� ȸ��
extern void trans_rotate();		extern void trans_rotate_right_no_cutting();		extern void trans_rotate_left_no_cutting();
// ���������� �̵�				�������� �̵�					���� �̵�						�Ʒ��� �̵�
extern void trans_move_r();		extern void trans_move_l();		extern void trans_move_u();		extern void trans_move_d();
// ���� �̷���					�¿� �̷���
extern void trans_mirror_h();	extern void trans_mirror_v();

//////////////////// -------ȭ�ҿ���ó���Լ� -------////////////////////
// ������						����
extern void trans_embossing();	extern void trans_blurring();
// ������ ������							����þ� ������
extern void trans_high_freq_sharpening();	extern void trans_gausian_smoothing();
// �������� 1���� ������					���� ���� 2����
extern void trans_edge_1d_prewitt();		extern void trans_edge_2d_LoG();


//////////////////// -------��������Լ� -------////////////////////
//  ��հ� ��������				�߾Ӱ� ��������
extern int get_average(void);	extern int get_median(void);

// �����Լ�
int main(void) {

	// Windows ��¿� ������ �ʱ�ȭ
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(NULL); // Windows 11
	//hdc = GetWindowDC(hwnd);
	// 1. ���� �ҷ�����
	load_image();

	// 2. �޴� ����
	char inKey = 0;
	while (inKey != '2') {
		// �޴� ���
		print_menu();

		// �޴� ����
		inKey = _getch();

		// ��ɾ� ����
		system("cls");

		switch (inKey) {
		case '0': {
			// ���ο� ���� ���� ����

			// inImage �޸� ����
			free_inImage_mem();
			// outImage �޸� ����
			free_outImage_mem();
			// ���� ���� ����
			load_image();
			break;
		}
		case '1': save_image(); break; // �������� ����
		case '2': printf("���α׷��� ���� �Ǿ����ϴ�."); break; // ���α׷� ����
		case '3': {
			// outImage �޸� ����
			free_outImage_mem();
			equal_image();
			print_image();
			break; // ���� �ٽ� �����ؿ���(�ʱ�ȭ)
		}
		case '4': trans_brighter(); break;	// ���
		case '5': trans_darker(); break;	// ��Ӱ�
		case '6': trans_baw(); break;		// ���
		case '7': trans_gamma(); break;		// ��������
		case '8': trans_reverse(); break;	// ����
		case '9': {
			int avg = get_average();
			printf("\n�� �̹����� ��հ��� --->%d �Դϴ�\n", avg);
			break;							// ������ ��հ� ���
		}
		case 'a': {
			int median = get_median();
			printf("\n�� �̹����� �߾Ӱ��� --->%d �Դϴ�\n", median);
			break;							// ������ �߾Ӱ� ���
		}
		case 'b': trans_and(); break;		// AND ����
		case 'c': trans_or(); break;		// OR ����
		case 'd': trans_xor(); break;		// XOR ����
		case 'e': trans_para_cap(); break;	// �Ķ󺼶� CAP
		case 'f': trans_para_cup(); break;	// �Ķ󺼶� CUP
		case 'g': trans_zoom_out(); break;	// ���� ���
		case 'h': trans_zoom_in(); break;	// ���� Ȯ��
		case 'i': trans_rotate(); break;	// ȸ��(���� �߸�)
		case 'j': trans_move_r(); break;	// �̵�(������)(���� �߸�)
		case 'k': trans_move_l(); break;	// �̵�(��  ��)(���� �߸�)
		case 'l': trans_move_u(); break;	// �̵�(��	��)(���� �߸�)
		case 'm': trans_move_d(); break;	// �̵�(�Ʒ���)(���� �߸�)
		case 'n': trans_mirror_h(); break;	// ���Ϲ���
		case 'o': trans_mirror_v(); break;	// �¿����
		case 'p': trans_hist_stretch(); break;					// ������׷� ��Ʈ��ġ
		case 'q': trans_hist_endin_stretch(); break;			// ������׷� ������ ��Ʈ��ġ
		case 'r': trans_hist_equal(); break;					// ������׷� ��Ȱȭ
		case 's': trans_embossing(); break;						// ������
		case 't': trans_blurring(); break;						// ����
		case 'u': trans_high_freq_sharpening(); break;			// ������ ������
		case 'v': trans_rotate_right_no_cutting(); break;		// ȸ��(������)(���� �� �߸�)
		case 'w': trans_rotate_left_no_cutting(); break;		// ȸ��(��	��)(���� �� �߸�)
		case 'x': trans_gausian_smoothing(); break;				// ����þ� ������
		case 'z': trans_edge_1d_prewitt(); break;				// �������� 1���� ������
		case 'A': trans_edge_2d_LoG(); break;					// ����þ� ������ + ���ö�þ� 2d ���� ����
			
		default: printf("�ش� ����� �����ϴ�. �ٽü����� �ּ���\n");  break;
		}
	}
	// inImage �޸� ����
	free_inImage_mem();

	// outImage �޸� ����
	free_outImage_mem();

	return 0;
}