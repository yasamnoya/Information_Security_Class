#include<iostream>
#include"DES.h"
int main(int argc,char** argv) {
	std::cout << "0x" << DES::encrypt(argv[2] + 2, argv[1] + 2);
	return 0;
}