#include<iostream>
#include"Matrix.h"

int main() {
	LinkMatrix M(4, 4);
	LinkMatrix N(4, 2);
	M.Row_push_back(1, 0, 0);
	M.Row_push_back(2, 0, 1);
	M.Row_push_back(3, 1, 0);
	M.Row_push_back(1, 3, 3);
	N.Row_push_back(2, 1, 0);
	N.Row_push_back(3, 3, 1);
	M.ColLinkSet();
	N.ColLinkSet();
	M.Print();
	std::cout << std::endl;
	(M + M).Print();
	std::cout << std::endl;
	(M * M).Print();
	std::cout << std::endl;
	(M * N).Print();
	return 0;
}