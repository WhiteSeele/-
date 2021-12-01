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
	MatrixNode** First = new MatrixNode * [head->col];   //First[i]�ǵ�i�еĵ�һ��Ԫ��
	MatrixNode** Last = new MatrixNode * [head->col];   //Last[i]�ǵ�i�е����һ��Ԫ��
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
	for (int i = 0; i < head->col; i++) {  //���������������
		if (First[temp1->col] == NULL) {    //temp1�ǵ�ǰ���ϵĵ�һ��Ԫ��
			First[temp1->col] = temp1;
			Place[temp1->col] = 1;
		}
		else {    //temp1��ǰ����Ԫ��������ͬһ�У����кű�temp1С
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
	for (int i = 0; i < head->col; i++) { //��ȡ������ӵ�˳��
		if (Place[i] == 1) {   //��i����Ԫ��
			Sign[Length] = i;  //����һ��ת����һ�е�ӳ���ϵ
			Length++;
		}
	}
	if (Sign[0] == -1)
		return;
	for (int i = 0; i < Length - 1; i++)
		Last[Sign[i]]->down = First[Sign[i + 1]];   //����ͬ�е���������һ��������
	head->down = First[Sign[0]];
	Last[Sign[Length - 1]]->down = head;
	delete[]First;
	delete[]Last;
	delete[]Place;
	delete[]Sign;
}

LinkMatrix LinkMatrix::operator+(LinkMatrix& LM) { //��������ʽ����ʽ�ļӷ�
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
		val = 0;         //ĳһ����ĳһ������˷��������㣻
		r = Thisp->row;    //���˾���ǰ���������
		c = LMp->col;      //�˾���ǰ���������
		Start = Thisp;       //��¼���˾����ÿһ�еĵ�һ������Ԫ��
		n = nplace;
		while (r == Thisp->row || c == LMp->col) {   //���˾���ĵ�r�г��Եڳ˾���ĵ�c��
			if (r != Thisp->row) {   //���˾����r�е������Ѿ������ָ꣬���Ѿ���������һ�У����˾���ĵ�c�к�������Ԫ�ش��ڣ�����ЩԪ����ȫ����������Ϊ���ǵ���������0��
				LMp = LMp->down;
				LMn++;
			}
			else
				if (c != LMp->col) {   //�˾����c�е������Ѿ������ָ꣬���Ѿ���������һ�У������˾���ĵ�r������Ԫ�ش��ڣ�����ЩԪ����ȫ����������Ϊ���ǵ���������0��
					Thisp = Thisp->right;
					n++;
				}
				else
					if (Thisp->col == LMp->row) {     //���Ͼ��������������Ԫ����ˣ�
						val += Thisp->value * LMp->value;
						Thisp = Thisp->right;
						LMp = LMp->down;
						n++; LMn++;
					}
					else
						if (Thisp->col > LMp->row) {        //���˾���Thispָ���Ԫ�����������˳˾���LMp��ָ��Ԫ�ص��������൱��Thispָ���Ѿ�������LMpָ���ǰ�棬�����轫LMpָ�����
							LMp = LMp->down;
							LMn++;
						}
						else
							if (Thisp->col < LMp->row) {    //�˾����LMpָ���Ԫ�ص����������˱��˾���Thisp��ָ���Ԫ�ص��������൱��LMpָ��������Thispָ���ǰ�棬������Ҫ��Thispָ�����
								Thisp = Thisp->right;
								n++;
							}
		}
		if (val!=0)      //����Ԫ�ش������
			Result.Row_push_back(val, r, c);
		if (n == head->value && LMn == LM.head->value)   //�˷��������������������Ԫ�ض�������
			return Result;
		if (LMn == LM.head->value) {  //���˾���ĵ�i���Ѿ��ͳ˾�����������������㣬�������ĵ�i���Ѿ��ó���
			nplace=n;
			LMp = LM.head->down;
			LMn = 0;
		}
		else                          //���˾���ָ��Thisp�������ͳ˾�����������ѱ䣬�����ڳ˾����л���Ԫ��δ����֤�����˾���ĵ�r��Ԫ�صĳ˷���û�����꣬�践�ص�r�еĵ�һ������Ԫ�ؼ�������
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