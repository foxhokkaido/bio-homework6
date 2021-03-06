#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct s_i {
	int *s;
	int i;
};

//раскодируем нуклеотиды
string cod(int* a, int l) {
	string word;
	for (int i = 0; i < l; i++) {
		switch (a[i])
		{
		case 0: word += 'A'; break;
		case 1: word += 'C'; break;
		case 2: word += 'G'; break;
		case 3: word += 'T'; break;
		default:
			break;
		}
	}
	return word;
}

int* Next_Leaf(int* a, int l, int k) {
	for (int i = l - 1; i >= 0; i--) {
		if (a[i] < k - 1) {
			a[i]++;
			return a;
		}
		a[i] = 0;
	}
	return a;
}

void All_Leaves(int l, int k) {
	int *s = new int[l];
	for (int i = 0; i < l; i++) {
		s[i] = 0;
	}
	for (;;) {
		for (int i = 0; i < l; i++) {
			cout << s[i];
		}
		cout << endl;
		s = Next_Leaf(s, l, k);
		int n;
		for (n = 0; n < l && s[n] == 0; n++) {
			;
		}
		if (n == l) return;

	}
}

struct s_i NextVertex(int* a, int i, int l, int k) {
	struct s_i a_i;
	a_i.s = new int[l];
	a_i.i = i;
	for (int h = 0; h < l; h++) {
		a_i.s[h] = a[h];
	}
	if (a_i.i < l) {
		a_i.s[a_i.i + 1] = 0;
		a_i.i++;
		return a_i;
	}
	else {
		for (int j = l-1; j >= 0; j--) {
			if (a_i.s[j] < k-1 ) {
				a_i.s[j] = a_i.s[j] + 1;
				a_i.i = j;
				return a_i;
			}
		}
	}
	a_i.i = -1;
	return a_i;
}

struct s_i ByPass(int* a, int i, int l, int k) {
	struct s_i a_i;
	a_i.s = new int[l];
	a_i.i = i;
	for (int h = 0; h < l; h++) {
		a_i.s[h] = a[h];
	}
	for (int j = l - 1; j >= 0; j--) {
		if (a_i.s[j] < k-1 ) {
			a_i.s[j] = a_i.s[j] + 1;
			a_i.i = j;
			return a_i;
		}
	}
	a_i.i = -1;
	return a_i;
}

int Total_Distance(string word, string* dna, int n, int l, int t) {
	int Hmg = 0;
	int *h = new int[n - l + 1];
	int *h_min = new int[t];
	for (int i = 0; i < t; i++) {
		h_min[i] = 10000000;
	}
	for (int i = 0; i < t; i++) { //считаем минимальное расстояние Хэмминга для всех строк
		string buff = dna[i];
		for (int c = 0; c < n - l + 1; c++) {
			h[c] = 0;
		}
		for (int j = 0; j < (n - l + 1); j++) {//скользящее окно
			int m; int k = j;
			for (m = 0; m < l; m++) {//считаем число различных позиций				
				if (word[m] != buff[k]) {
					h[j]++;//считаем расстояние Хэмминга для каждой стартовой позиции
				}
				k++;
			}
		}
		//найдем минимальное и перейдем на новую строку
		for (int g = 0; g < n - l + 1; g++) {
			if (h[g] < h_min[i]) {
				h_min[i] = h[g];
			}
		}
	}
	//считаем полное расснояние Хэмминга
	for (int i = 0; i < t; i++) {
		Hmg = Hmg + h_min[i];
	}
	return Hmg;
}

string BranchAndBoundMedianSearch(string* dna, int t, int n, int l) {
	struct s_i si;
	si.s = new int[l];//код паттерна
	si.i = 0;
	for (int i = 0; i < l; i++) {
		si.s[i] = 0;
	}
	string word;
	string best_word;
	string prefix;

	int best_distance = 100000000; 

	int optimistic_distance;

	while (si.i > -1) {
		if (si.i < l) {
			word = cod(si.s, l);
			prefix = word.substr(0, si.i+1);
			optimistic_distance = Total_Distance(prefix, dna, n, si.i+1, t);
			if (optimistic_distance > best_distance) {
				si = ByPass(si.s, si.i, l, 4);
				
			}
			else {
				si = NextVertex(si.s, si.i, l, 4);
				
			}

		}
		else {
			word = cod(si.s, l);
			if (Total_Distance(word, dna, n, l, t) <= best_distance) {
				best_distance = Total_Distance(word, dna, n, l, t);
				best_word = word;
			}
			si = NextVertex(si.s, si.i, l, 4);
		}
	}
	return best_word;

}


int main() {
	ifstream fin("input.txt");

	if (!fin.is_open())
		cout << "Файл не может быть открыт!\n";
	else
	{
		int l;//размер шаблона

		fin >> l;

		string buff[200];

		int i = 0;
		while (!fin.eof()) {
			fin >> buff[i];
			i++;
		}

		int t;//число последовательностей
		t = i;

		string* arr_str = new string[t];

		for (int j = 0; j < t; j++) {
			arr_str[j] = buff[j];
		}

		int n = arr_str[0].length();//длина одной последовательности

		string median_string;

		median_string = BranchAndBoundMedianSearch(arr_str, t, n, l);

		ofstream fout("output.txt");

		fout << median_string;

		fin.close();

		fout.close();

	}
	return 0;
}
