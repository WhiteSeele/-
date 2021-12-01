#pragma once
#ifndef MATRIX_H
#define MATRIX_H

struct MatrixNode {
	int value;
	int row;
	int col;
	MatrixNode* right;
	MatrixNode* down;
	MatrixNode() {
		value = -1;
		row = -1;
		col = -1;
		right = nullptr;
		down = nullptr;
	}
	MatrixNode(int Value, int Row, int Col) :value(Value), row(Row), col(Col) {
		right = nullptr;
		down = nullptr;
	}
};

class LinkMatrix {
public:
	LinkMatrix(int Row, int Col);
	void Row_push_back(int Value, int Row, int Col); //将结点链入行链表,测试数据设计时需按列号大小来排
	void ColLinkSet();
	LinkMatrix operator+(LinkMatrix& LM);
	LinkMatrix operator*(LinkMatrix& LM);
	void Print();
private:
	MatrixNode* head;
	MatrixNode* tail;
};
#endif
