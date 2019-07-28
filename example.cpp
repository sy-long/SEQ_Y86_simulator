#include <iostream>
#include <fstream>
#include <string>
#include "seq_Y86_simulator.h"
using namespace std;

int main() {
	
	Y86 y86;
	string file_name;
	cout << "ģ��CPU\n\n";
	cout << "������Դ�����ļ���:";
	cin >> file_name;
	vector<unsigned char> test;
	ifstream fp;
	fp.open(file_name);
	string ch;
	while (!fp.eof()) {
		fp >> ch;
		test.push_back(stoi(ch));
	}
	y86.set_i_mem(test);
	y86.run();
	cout << y86.get_result();
}