#include <iostream>
#include <fstream>
#include <string>
#include "seq_Y86_simulator.h"
using namespace std;

int main() {
	
	Y86 y86;
	string file_name;
	cout << "模拟CPU\n\n";
	cout << "请输入源代码文件名:";
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