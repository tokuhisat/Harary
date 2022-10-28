#include <stdio.h>
#include <string>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <unordered_map>

/*変数の意味あい
number……勝利条件の個数(達成すれば先手の勝ちとなる集合の個数)
first……先手の石の位置(2進数表記)
second……後手の石の位置(2進数表記)
win_condition……勝利条件全体の集合
max_move……先手が打てる石の数の上限(オプション)
depth……先手の石が〇個までの結果を辞書に格納する*/

//グローバル変数宣言エリア
std::unordered_map<long long int, bool> dict;

int number_of_bit1(long long int num) {    //盤上に存在する石の個数を数える関数
	int count = 0;
	while (num != 0) {
		num = num & (num - 1);
		count += 1;
	}
	return count;
}

bool W_f_win( int number_1,int first_1,int* win_condition_1) {       //先手が勝利条件を満たしているかを確認する関数
	int i = 0;
	for (i = 0; i < number_1; i++) {
		if (win_condition_1[i] == (first_1 & win_condition_1[i])) {      //ある勝利条件の全てのマスに先手の石があるなら
			return true;
		}
	}
	return false;
}

bool W_draw(int number_2, int second_2,int* win_condition_2, int first_2, int max_move_2) {      //以降、先手が勝利することができないことを確認する関数

	long long int x;
	x = first_2;
	if (number_of_bit1(x) > max_move_2 - 1) {    //許される手数を全て使い切ったが勝利条件を満たしていない
		return true;
	}
	 int i = 0;
	for (i = 0; i < number_2; i++) {
		if ((second_2 & win_condition_2[i]) != 0) {
			;
		}
		else {                                   //全ての空きマスに先手の石を置いた時、先手が勝利するならFalse
			return false;
		}
	}
	return true;
}

bool W_judgement(int number_3, int boardsize_3, int first_3, int second_3, int* win_condition_3, bool turn_3, int max_move_3,int depth_3) {

	long long int x;
	x = first_3;
	x = x << boardsize_3;
	x = x | second_3;                   //xに先手と後手の石の位置情報を入れる
	auto itr = dict.find(x);
	if (itr != dict.end()) {
		return dict[x];                 //辞書にあればその結果を持ってくる
	}

	int i = 0;
	bool evaluation = false;    //初期化、Trueは先手勝ちを、Falseは先手勝ちでないを表す
	if (W_f_win(number_3, first_3, win_condition_3)) {     //先手が勝利条件を満たしているなら
		return true;
	}
	if (W_draw(number_3, second_3, win_condition_3, first_3, max_move_3)) {  //先手がどうやっても勝利条件を満たせないなら
		return false;
	}
	if (turn_3) {                                     //事前設定、先手番ならFalse、後手番ならTrueにしておく
		evaluation = false;
	}
	else {
		evaluation = true;
	}

	for (i = 0; i < boardsize_3; i++) {                //全てのマスについて
		if (((first_3 & (1 << i)) | (second_3 & (1 << i))) != 0) {         //既に先手か後手の石が打ってあればskip
			continue;
		}
		if (turn_3) {                   //手番が先手なら、先手の石を打って手番を変えて再評価する
			first_3 = first_3 | (1 << i);
			evaluation = evaluation || W_judgement(number_3, boardsize_3, first_3, second_3, win_condition_3, !turn_3, max_move_3,depth_3);
		}  //1つでもTrueがあれば先手勝ち(ミニマックス手続きを真偽値とand,orを使って行っている)
		else {
			second_3 = second_3 | (1 << i);
			evaluation = evaluation && W_judgement(number_3, boardsize_3, first_3, second_3, win_condition_3, !turn_3, max_move_3,depth_3);
		}  //1つでもFalseがあれば先手勝てない
		

		if ((first_3 & (1 << i)) != 0) {     //打った手を戻して、前の盤面に戻る
			first_3 = first_3 - (1 << i);
		}
		if ((second_3 & (1 << i)) != 0) {
			second_3 = second_3 - (1 << i);
		}
		
		if (number_of_bit1(first_3) < depth_3) { //先手の石の数がdepth未満なら辞書に盤面と結果を格納する
			long long int y;
			y = first_3;
			y = y << boardsize_3;
			y = y | second_3;
			dict[y] = evaluation;
		}
	}
	return evaluation;
}

void f_answer(int number_4, int boardsize_4,int first_4,int second_4,int* win_condition_4, int max_move_4,int depth_4) {  //現在の盤面で先手から石を打ち始める時、先手が勝利できる場所を列挙する関数
	int i = 0;
	int A = 0;
	for (i = 0; i < boardsize_4; i++) {
		if (((first_4 & (1 << i)) | (second_4 & (1 << i))) != 0) {
			continue;
		}
		A = first_4 | (1 << i);
		if (W_judgement(number_4, boardsize_4, A, second_4, win_condition_4, false, max_move_4,depth_4)) {  //先手が1つ石を打った上で後手から打ち始めた時に、先手必勝となる場所の列挙
			printf("%dに打てば勝利\n", i);
		}
	}
}

void s_answer(int number_5, int boardsize_5, int first_5, int second_5,int* win_condition_5, int max_move_5,int depth_5) {  //現在の盤面で後手から石を打ち始める時、先手の勝利を阻止できる場所を列挙する関数
	int i = 0;
	int A = 0;
	for (i = 0; i < boardsize_5; i++) {
		if (((first_5 & (1 << i)) | (second_5 & (1 << i))) != 0) {
			continue;
		}
		A = second_5 | (1 << i);
		if (W_judgement(number_5, boardsize_5, first_5, A, win_condition_5, true, max_move_5,depth_5) == 0) {
			printf("%dに打てば阻止可能\n", i);
		}
	}
}

//ここまで関数定義、ここから入力

int main(void) {
	//ローカル変数宣言（）
	clock_t st, ed;
	int boardsize, number, first, second, max_move,depth, i = 0;
	bool a, turn = false; //初期化

	int win_condition[14] = { 1029,2059,4118,8236,88,177,354,708,1416,2832,5664,11328,6272,12544 };

	number = 14;                //勝利条件の個数
	boardsize = 14;             //盤面の数、物理的上限は31
	first = 0;                  //先手の駒の位置
	second = 0;                 //後手の駒の位置
	max_move = 100;             //打ち切り手数
	depth = 7;                  //辞書に入れる深さ
	turn = true;                //現在手番

	st = clock();

	a = W_judgement(number, boardsize, first, second, win_condition, turn, max_move,depth);
	if (a == true) { printf("先手の勝ち\n"); }
	if (a == false) { printf("先手の負け\n"); }

    //f_answer(number, boardsize, first, second, win_condition, max_move, depth); //入力盤面で手番が先手なら、先手はどこに打てば勝てるのか？
	//as_answer(number, boardsize, first, second, win_condition, max_move, depth); //入力盤面で手番が後手なら、どこに打てば先手の勝利を阻止できるのか？
	ed = clock();
	printf("計算時間は%f\n", (double)(ed - st) / CLOCKS_PER_SEC);

	return 0;
}