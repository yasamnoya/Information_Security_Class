#include<iostream>
#include"DES.h"
int main(int argc,char** argv) {
	std::cout << "0x" << DES::decrypt(argv[2] + 2, argv[1] + 2);
	return 0;
}