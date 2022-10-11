#include <stdio.h>
#include<string>
#include <stdbool.h>
#include<time.h>
#include<math.h>
#include<windows.h>
#include<unordered_map>

using namespace std;

//�O���[�o���ϐ��錾�G���A
unordered_map<long long int, bool> mp;

//���[�J���ϐ� num,count
int number_of_bit1(long long int num) {
	int count = 0;
	while (num != 0) {
		num = num & (num - 1);
		count += 1;
	}
	return count;
}

//���[�J���ϐ�  number_1,first_1,win_condition_1
bool W_f_win( int number_1,int first_1,int* win_condition_1) {
	int i = 0;
	for (i = 0; i < number_1; i++) {
		if (win_condition_1[i] == (first_1 & win_condition_1[i])) {
			return true;
		}
	}
	return false;
}

//���[�J���ϐ�  number_2,second_2,win_condition_2,first_2,max_move
bool W_draw(int number_2, int second_2,int* win_condition_2, int first_2, int max_move_2) {

	long long int x;
	x = first_2;
	if (number_of_bit1(x) > max_move_2 - 1) {
		return true;
	}
	 int i = 0;
	for (i = 0; i < number_2; i++) {
		if ((second_2 & win_condition_2[i]) != 0) {
			;
		}
		else {
			return false;
		}
	}
	return true;
}


//���[�J���ϐ�number_3,boardsize_3,first3,second_3,win_condition_3,turn_3
bool W_judgement(int number_3, int boardsize_3, int first_3, int second_3, int* win_condition_3, bool turn_3, int max_move_3,int depth_3) {

	long long int x;
	x = first_3;
	x = x << boardsize_3;
	x = x | second_3;
	auto itr = mp.find(x);
	if (itr != mp.end()) {
		return mp[x];
	}


	int i = 0;
	bool evaluation = false;
	if (W_f_win(number_3, first_3, win_condition_3)) {
		return true;
	}
	if (W_draw(number_3, second_3, win_condition_3, first_3, max_move_3)) {
		return false;
	}
	if (turn_3) {
		evaluation = false;
	}
	else {
		evaluation = true;
	}

	for (i = 0; i < boardsize_3; i++) {
		if (((first_3 & (1 << i)) | (second_3 & (1 << i))) != 0) {
			continue;
		}
		if (turn_3) {
			first_3 = first_3 | (1 << i);
			evaluation = evaluation || W_judgement(number_3, boardsize_3, first_3, second_3, win_condition_3, !turn_3, max_move_3,depth_3);
		}
		else {
			second_3 = second_3 | (1 << i);
			evaluation = evaluation && W_judgement(number_3, boardsize_3, first_3, second_3, win_condition_3, !turn_3, max_move_3,depth_3);
		}
		

		if ((first_3 & (1 << i)) != 0) {
			first_3 = first_3 - (1 << i);
		}
		if ((second_3 & (1 << i)) != 0) {
			second_3 = second_3 - (1 << i);
		}
		
		if (number_of_bit1(first_3) < depth_3) {
			long long int y;
			y = first_3;
			y = y << boardsize_3;
			y = y | second_3;
			mp[y] = evaluation;
		}
	}
	return evaluation;
}

void f_answer(int number_4, int boardsize_4,int first_4,int second_4,int* win_condition_4, int max_move_4,int depth_4) {
	int i = 0;
	int A = 0;
	for (i = 0; i < boardsize_4; i++) {
		if (((first_4 & (1 << i)) | (second_4 & (1 << i))) != 0) {
			continue;
		}
		A = first_4 | (1 << i);
		if (W_judgement(number_4, boardsize_4, A, second_4, win_condition_4, false, max_move_4,depth_4)) {
			printf("%d�ɑłĂΏ���\n", i);
		}
	}
}

void s_answer(int number_5, int boardsize_5, int first_5, int second_5,int* win_condition_5, int max_move_5,int depth_5) {
	int i = 0;
	int A = 0;
	for (i = 0; i < boardsize_5; i++) {
		if (((first_5 & (1 << i)) | (second_5 & (1 << i))) != 0) {
			continue;
		}
		A = second_5 | (1 << i);
		if (W_judgement(number_5, boardsize_5, first_5, A, win_condition_5, true, max_move_5,depth_5) == 0) {
			printf("%d�ɑłĂΑj�~�\\n", i);
		}
	}
}

//�����܂Ŋ֐��A�����������

int main(void) {
	//���[�J���ϐ��錾�i�j
	clock_t st, ed;
	int boardsize, number, first, second, max_move,depth, i = 0;
	bool a, turn = false; //������

	int win_condition[14] = { 1029,2059,4118,8236,88,177,354,708,1416,2832,5664,11328,6272,12544 };

	number = 14;               //���������̌�
	boardsize = 14;             //�Ֆʂ̐��A�����I�����31
	first = 0;                 //���̋�̈ʒu
	second = 0;                //���̋�̈ʒu
	max_move = 100;            //�ł��؂�萔
	depth = 6;                 //�����ɓ����[��
	turn = true;              //���ݎ��

	st = clock();

	a = W_judgement(number, boardsize, first, second, win_condition, turn, max_move,depth);
	if (a == true) { printf("���̏���\n"); }
	if (a == false) { printf("���̕���\n"); }

	/*for (i = 0; i < 14; i++) {
		first = pow(2, i);
		a = W_judgement(number, boardsize, first, second, win_condition, turn, max_move);
		s_answer(number, boardsize, first, second, win_condition, max_move);//�ǂ��ɑłĂΑj�~�ł���̂��H�@in���ݔՖ�
	}*/


	/*int i = 0;
	for (i =0; i < 14; i++) {
		//if (i==12 ||i ==16 ) { continue; }
		first = pow(2, i);
		a = W_judgement(number, boardsize, first, second, win_condition, turn, max_move);
		printf("���ʂ�%d�A�܂����%s\n", a, win_lose(a));
	} */

    f_answer(number, boardsize, first, second, win_condition, max_move, depth); //�ǂ��ɑłĂΏ��Ă�̂��H�@in���ݔՖ�
	s_answer(number, boardsize, first, second, win_condition, max_move, depth); //�ǂ��ɑłĂΑj�~�ł���̂��H�@in���ݔՖ�
	ed = clock();
	printf("�v�Z���Ԃ�%f\n", (double)(ed - st) / CLOCKS_PER_SEC);


	Beep(440, 1500);
	return 0;
}





//31�}�X(1,2,4) int win_condition[48] = { 139,278,556,1112,2224,4448,8896,17792,35584,71168,142336,284672,569344,1138688,2277376,4554752,9109504,18219008,336438016,72876032,145752064,36438016,72876032,145752064,\
		209,418,836,1672,3344,6688,13376,26752,53504,107008,214016,428032,856064,1712128,3424256,6848512,13697024,27394048,54788096,109576192,219152384,438304768,876609536,1753219072 }; //��������

	//31�}�X(1,2,4,8) int win_condition[32] = {32907,65814,131628,263256,526512,1053024,2106048,4212096,8424192,16848384,33696768,67393536,134787072,269574144,539148288,1078296576,\
        53505,107010,214020,428040,856080,1712160,3424320,6848640,13697280,27394560,54789120,109578240,219156480,438312960,876625720,1753251840};

	//31�}�X�Ж�(1,2,4) int win_condition[24] = {139,278,556,1112,2224,4448,8896,17792,35584,71168,142336,284672,569344,1138688,2277376,4554752,9109504,18219008,336438016,72876032,145752064,36438016,72876032,145752064};

	//31�}�X�Ж�(1,2) int win_condition[28]={ 11,22,44,88,176,352,704,1408,2816,5632,11264,22528,45056,90112,180224,360448,720896,1441792,2883584,5767168,11534336,23068672,46137344,92274688,1842549376,369098752,738197504,1476395008};

	//�敪�Ж�(1,2,4,8)int win_condition[15] = { 139,278,556,1112,2224,4448,8896,17792,2816,5632,11264,22528,2065,4130,8261 };

