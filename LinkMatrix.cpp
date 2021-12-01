#include"Matrix.h"
#include<iostream>

LinkMatrix::LinkMatrix(int Row, int Col) {
	head = tail = new MatrixNode(0, Row, Col);
	head->right = head->down = head;
}

void LinkMatrix::Row_push_back(int Value, int Row, int Col) {
	tail->right = new MatrixNode(Value, Row, Col);
	tail = tail->right;
	tail->right = head;
	head->value++;
}

void LinkMatrix::ColLinkSet() {
	MatrixNode** First = new MatrixNode * [head->col];   //First[i]是第i列的第一个元素
	MatrixNode** Last = new MatrixNode * [head->col];   //Last[i]是第i列的最后一个元素
	int* Place = new int[head->col];
	int* Sign = new int[head->col];
	int Length = 0;
	for (int i = 0; i < head->col; i++) {
		First[i] = NULL;
		Last[i] = NULL;
		Place[i] = 0;
		Sign[i] = -1;
	}
	MatrixNode* temp1 = head->right;
	for (int i = 0; i < head->col; i++) {  //按行链表遍历矩阵
		if (First[temp1->col] == NULL) {    //temp1是当前列上的第一个元素
			First[temp1->col] = temp1;
			Place[temp1->col] = 1;
		}
		else {    //temp1的前面有元素与它在同一列，且行号比temp1小
			MatrixNode* temp2 = First[temp1->col];
			while (1) {
				if (temp2->down == NULL) {
					temp2->down = temp1;
					break;
				}
				temp2 = temp2->down;
			}
		}
		Last[temp1->col] = temp1;    //
		temp1 = temp1->right;
	}
	for (int i = 0; i < head->col; i++) { //获取最后连接的顺序
		if (Place[i] == 1) {   //第i列有元素
			Sign[Length] = i;  //由上一列转到下一列的映射关系
			Length++;
		}
	}
	if (Sign[0] == -1)
		return;
	for (int i = 0; i < Length - 1; i++)
		Last[Sign[i]]->down = First[Sign[i + 1]];   //将不同列的链表连成一整个链表
	head->down = First[Sign[0]];
	Last[Sign[Length - 1]]->down = head;
	delete[]First;
	delete[]Last;
	delete[]Place;
	delete[]Sign;
}

LinkMatrix LinkMatrix::operator+(LinkMatrix& LM) { //类似于链式多项式的加法
	if (this->head->col != LM.head->col && this->head->row != LM.head->row)
		exit(1);
	LinkMatrix Result(head->row, head->col);
	int i = head->value;
	int j = LM.head->value;
	MatrixNode* p1 = head->right;
	MatrixNode* p2 = LM.head->right;
	while (i && j) {
		if (p1->row == p2->row && p1->col == p2->col) {
			Result.Row_push_back(p1->value + p2->value, p1->row, p1->col);
			p1 = p1->right;
			p2 = p2->right;
			i--;
			j--;
		}
		if (p1->row < p2->row || p1->row == p2->row && p1->col < p2->col) {
			Result.Row_push_back(p1->value, p1->row, p1->col);
			p1 = p1->right;
			i--;
		}
		if (p2->row < p1->row || p1->row == p2->row && p2->col < p1->col) {
			Result.Row_push_back(p2->value, p2->row, p2->col);
			p2 = p2->right;
			j--;
		}
	}
	if (i == 0) {
		while (j--) {
			Result.Row_push_back(p2->value, p2->row, p2->col);
			p2 = p2->right;
		}
	}
	if (j == 0) {
		while (i--) {
			Result.Row_push_back(p1->value, p1->row, p1->col);
			p1 = p1->right;
		}
	}
	Result.ColLinkSet();
	return Result;
}

LinkMatrix LinkMatrix::operator*(LinkMatrix& LM) {
	if (this->head->col != LM.head->row)
		exit(1);
	int r, c, val;
	int nplace, n, LMn;
	val = nplace = n = LMn = 0;
	LinkMatrix Result(head->row, LM.head->col);
	MatrixNode* Thisp = head->right;
	MatrixNode* LMp = LM.head->down;
	MatrixNode* Start = head->right;
	while (1) {
		val = 0;         //某一行与某一列做完乘法后结果清零；
		r = Thisp->row;    //被乘矩阵当前运算的行数
		c = LMp->col;      //乘矩阵当前运算的列数
		Start = Thisp;       //记录被乘矩阵的每一行的第一个非零元素
		n = nplace;
		while (r == Thisp->row || c == LMp->col) {   //被乘矩阵的第r行乘以第乘矩阵的第c列
			if (r != Thisp->row) {   //被乘矩阵第r行的数据已经处理完，指针已经跳到了下一行，若乘矩阵的第c列后续仍有元素存在，则这些元素需全部跳过，因为它们的运算结果是0；
				LMp = LMp->down;
				LMn++;
			}
			else
				if (c != LMp->col) {   //乘矩阵第c列的数据已经处理完，指针已经跳到了下一列，若被乘矩阵的第r行仍有元素存在，则这些元素需全部跳过，因为它们的运算结果是0；
					Thisp = Thisp->right;
					n++;
				}
				else
					if (Thisp->col == LMp->row) {     //符合矩阵相乘条件的两元素相乘；
						val += Thisp->value * LMp->value;
						Thisp = Thisp->right;
						LMp = LMp->down;
						n++; LMn++;
					}
					else
						if (Thisp->col > LMp->row) {        //被乘矩阵Thisp指向的元素列数超过了乘矩阵LMp所指向元素的行数，相当于Thisp指针已经走在了LMp指针的前面，所以需将LMp指针后移
							LMp = LMp->down;
							LMn++;
						}
						else
							if (Thisp->col < LMp->row) {    //乘矩阵的LMp指向的元素的行数超过了被乘矩阵Thisp所指向的元素的列数，相当于LMp指针走在了Thisp指针的前面，所以需要将Thisp指针后移
								Thisp = Thisp->right;
								n++;
							}
		}
		if (val!=0)      //非零元素存入矩阵
			Result.Row_push_back(val, r, c);
		if (n == head->value && LMn == LM.head->value)   //乘法结束，两个矩阵的所有元素都已运算
			return Result;
		if (LMn == LM.head->value) {  //被乘矩阵的第i行已经和乘矩阵的所有列做完运算，结果矩阵的第i行已经得出；
			nplace=n;
			LMp = LM.head->down;
			LMn = 0;
		}
		else                          //被乘矩阵指针Thisp的行数和乘矩阵的列数都已变，但是在乘矩阵中还有元素未处理，证明被乘矩阵的第r行元素的乘法还没操作完，需返回第r行的第一个非零元素继续处理；
			Thisp = Start;   
	}
}

void LinkMatrix::Print() {
	int row, col;
	row = col = 0;
	MatrixNode* Temp = head->right;
	for (int i = 0; i < head->value; i++) {
		std::cout << "(Val: " << Temp->value << " R: " << Temp->row << " C: " << Temp->col << ")";
		Temp = Temp->right;
	}
}