#include <stdio.h>
#include <string>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <unordered_map>

/*�ϐ��̈Ӗ�����
number�c�c���������̌�(�B������ΐ��̏����ƂȂ�W���̌�)
first�c�c���̐΂̈ʒu(2�i���\�L)
second�c�c���̐΂̈ʒu(2�i���\�L)
win_condition�c�c���������S�̂̏W��
max_move�c�c��肪�łĂ�΂̐��̏��(�I�v�V����)
depth�c�c���̐΂��Z�܂ł̌��ʂ������Ɋi�[����*/

//�O���[�o���ϐ��錾�G���A
std::unordered_map<long long int, bool> dict;

int number_of_bit1(long long int num) {    //�Տ�ɑ��݂���΂̌��𐔂���֐�
	int count = 0;
	while (num != 0) {
		num = num & (num - 1);
		count += 1;
	}
	return count;
}

bool W_f_win( int number_1,int first_1,int* win_condition_1) {       //��肪���������𖞂����Ă��邩���m�F����֐�
	int i = 0;
	for (i = 0; i < number_1; i++) {
		if (win_condition_1[i] == (first_1 & win_condition_1[i])) {      //���鏟�������̑S�Ẵ}�X�ɐ��̐΂�����Ȃ�
			return true;
		}
	}
	return false;
}

bool W_draw(int number_2, int second_2,int* win_condition_2, int first_2, int max_move_2) {      //�ȍ~�A��肪�������邱�Ƃ��ł��Ȃ����Ƃ��m�F����֐�

	long long int x;
	x = first_2;
	if (number_of_bit1(x) > max_move_2 - 1) {    //�������萔��S�Ďg���؂��������������𖞂����Ă��Ȃ�
		return true;
	}
	 int i = 0;
	for (i = 0; i < number_2; i++) {
		if ((second_2 & win_condition_2[i]) != 0) {
			;
		}
		else {                                   //�S�Ă̋󂫃}�X�ɐ��̐΂�u�������A��肪��������Ȃ�False
			return false;
		}
	}
	return true;
}

bool W_judgement(int number_3, int boardsize_3, int first_3, int second_3, int* win_condition_3, bool turn_3, int max_move_3,int depth_3) {

	long long int x;
	x = first_3;
	x = x << boardsize_3;
	x = x | second_3;                   //x�ɐ��ƌ��̐΂̈ʒu��������
	auto itr = dict.find(x);
	if (itr != dict.end()) {
		return dict[x];                 //�����ɂ���΂��̌��ʂ������Ă���
	}

	int i = 0;
	bool evaluation = false;    //�������ATrue�͐�菟�����AFalse�͐�菟���łȂ���\��
	if (W_f_win(number_3, first_3, win_condition_3)) {     //��肪���������𖞂����Ă���Ȃ�
		return true;
	}
	if (W_draw(number_3, second_3, win_condition_3, first_3, max_move_3)) {  //��肪�ǂ�����Ă����������𖞂����Ȃ��Ȃ�
		return false;
	}
	if (turn_3) {                                     //���O�ݒ�A���ԂȂ�False�A���ԂȂ�True�ɂ��Ă���
		evaluation = false;
	}
	else {
		evaluation = true;
	}

	for (i = 0; i < boardsize_3; i++) {                //�S�Ẵ}�X�ɂ���
		if (((first_3 & (1 << i)) | (second_3 & (1 << i))) != 0) {         //���ɐ�肩���̐΂��ł��Ă����skip
			continue;
		}
		if (turn_3) {                   //��Ԃ����Ȃ�A���̐΂�ł��Ď�Ԃ�ς��čĕ]������
			first_3 = first_3 | (1 << i);
			evaluation = evaluation || W_judgement(number_3, boardsize_3, first_3, second_3, win_condition_3, !turn_3, max_move_3,depth_3);
		}  //1�ł�True������ΐ�菟��(�~�j�}�b�N�X�葱����^�U�l��and,or���g���čs���Ă���)
		else {
			second_3 = second_3 | (1 << i);
			evaluation = evaluation && W_judgement(number_3, boardsize_3, first_3, second_3, win_condition_3, !turn_3, max_move_3,depth_3);
		}  //1�ł�False������ΐ�菟�ĂȂ�
		

		if ((first_3 & (1 << i)) != 0) {     //�ł������߂��āA�O�̔Ֆʂɖ߂�
			first_3 = first_3 - (1 << i);
		}
		if ((second_3 & (1 << i)) != 0) {
			second_3 = second_3 - (1 << i);
		}
		
		if (number_of_bit1(first_3) < depth_3) { //���̐΂̐���depth�����Ȃ玫���ɔՖʂƌ��ʂ��i�[����
			long long int y;
			y = first_3;
			y = y << boardsize_3;
			y = y | second_3;
			dict[y] = evaluation;
		}
	}
	return evaluation;
}

void f_answer(int number_4, int boardsize_4,int first_4,int second_4,int* win_condition_4, int max_move_4,int depth_4) {  //���݂̔ՖʂŐ�肩��΂�ł��n�߂鎞�A��肪�����ł���ꏊ��񋓂���֐�
	int i = 0;
	int A = 0;
	for (i = 0; i < boardsize_4; i++) {
		if (((first_4 & (1 << i)) | (second_4 & (1 << i))) != 0) {
			continue;
		}
		A = first_4 | (1 << i);
		if (W_judgement(number_4, boardsize_4, A, second_4, win_condition_4, false, max_move_4,depth_4)) {  //��肪1�΂�ł�����Ō�肩��ł��n�߂����ɁA���K���ƂȂ�ꏊ�̗�
			printf("%d�ɑłĂΏ���\n", i);
		}
	}
}

void s_answer(int number_5, int boardsize_5, int first_5, int second_5,int* win_condition_5, int max_move_5,int depth_5) {  //���݂̔ՖʂŌ�肩��΂�ł��n�߂鎞�A���̏�����j�~�ł���ꏊ��񋓂���֐�
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

//�����܂Ŋ֐���`�A�����������

int main(void) {
	//���[�J���ϐ��錾�i�j
	clock_t st, ed;
	int boardsize, number, first, second, max_move,depth, i = 0;
	bool a, turn = false; //������

	int win_condition[14] = { 1029,2059,4118,8236,88,177,354,708,1416,2832,5664,11328,6272,12544 };

	number = 14;                //���������̌�
	boardsize = 14;             //�Ֆʂ̐��A�����I�����31
	first = 0;                  //���̋�̈ʒu
	second = 0;                 //���̋�̈ʒu
	max_move = 100;             //�ł��؂�萔
	depth = 7;                  //�����ɓ����[��
	turn = true;                //���ݎ��

	st = clock();

	a = W_judgement(number, boardsize, first, second, win_condition, turn, max_move,depth);
	if (a == true) { printf("���̏���\n"); }
	if (a == false) { printf("���̕���\n"); }

    //f_answer(number, boardsize, first, second, win_condition, max_move, depth); //���͔ՖʂŎ�Ԃ����Ȃ�A���͂ǂ��ɑłĂΏ��Ă�̂��H
	//as_answer(number, boardsize, first, second, win_condition, max_move, depth); //���͔ՖʂŎ�Ԃ����Ȃ�A�ǂ��ɑłĂΐ��̏�����j�~�ł���̂��H
	ed = clock();
	printf("�v�Z���Ԃ�%f\n", (double)(ed - st) / CLOCKS_PER_SEC);

	return 0;
}