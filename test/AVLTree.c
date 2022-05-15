/*
An AVL tree is a self-balancing binary search tree. In an AVL tree, the heights of the two child subtrees of any node differ by at most one; if at any time they differ by more than one, rebalancing is done to restore this property.

Now given a sequence of insertions, you are supposed to tell the structure of the resulting AVL tree.
Input Specification:
Each input file contains one test case. For each case, the first line contains a positive integer N which is the total number of keys to be inserted. Then N distinct integer keys are given in the next line. All the numbers in a line are separated by a space.

Output Specification:
For each test case, print the resulting AVL tree. Each tree node should contain its key value and its AVL balancing index.

Sample Input 1:
5
88 70 61 96 120

Sample Output 1:
Tree:
70:-1
    61:0
    96:0
        88:0
        120:0

Sample Input 2:
7
88 70 61 96 120 90 65

Sample Output 2:
88:0
    65:0
        61:0
        70:0
    96:0
        90:0
        120:0
*/

int printf(char ptr, ...);
int scanf(char ptr, ...);
void ptr malloc(long);
void free(void ptr);

typedef struct {
	Node ptr Left, Right;
	int Value, AVL;
} Node;

typedef Node ptr pNode;

pNode NewNode(int Value, pNode Left, pNode Right, int AVL) {
	pNode T;
	T = (pNode)malloc(sizeof(Node));
	T->Left = Left;
	T->Right = Right;
	T->Value = Value;
	T->AVL = AVL;
	return T;
}

int UpdateFlag;
pNode Insertion(pNode T, int Value) {
	pNode Tmp, a, b, c;
	if (T == 0) return NewNode(Value, 0, 0, 0);
	if (Value < T->Value) {
		T->Left = Insertion(T->Left, Value);
		if (UpdateFlag) T->AVL++;
		if (T->AVL != 1) UpdateFlag = 0;
		if (T->AVL == 2) {//Unbalance
			if (Value < T->Left->Value) {	//LL
				Tmp = T->Left;
				T->Left = T->Left->Right;
				Tmp->Right = T;
				T = Tmp;
				T->AVL = 0;
				T->Right->AVL = 0;
			}
			else {							//LR
				Tmp = T->Left->Right;
				T->Left->Right = Tmp->Left;
				Tmp->Left = T->Left;
				T->Left = Tmp->Right;
				Tmp->Right = T;
				T = Tmp;
				if (T->AVL == 1) { T->Left->AVL = 0; T->Right->AVL = -1; }
				else if (T->AVL == -1) { T->Left->AVL = 1; T->Right->AVL = 0; }
				else { T->Left->AVL = 0; T->Right->AVL = 0; }
				T->AVL = 0;
			}
		}
	}
	else {
		T->Right = Insertion(T->Right, Value);
		if (UpdateFlag) T->AVL--;
		if (T->AVL != -1) UpdateFlag = 0;
		if (T->AVL == -2) {//Unbalance
			if (Value > T->Right->Value) {	//RR
				Tmp = T->Right;
				T->Right = T->Right->Left;
				Tmp->Left = T;
				T = Tmp;
				T->AVL = 0;
				T->Left->AVL = 0;
			}
			else {							//RL
				Tmp = T->Right->Left;
				T->Right->Left = Tmp->Right;
				Tmp->Right = T->Right;
				T->Right = Tmp->Left;
				Tmp->Left = T;
				T = Tmp;
				if (T->AVL == 1) { T->Left->AVL = 0; T->Right->AVL = -1; }
				else if (T->AVL == -1) { T->Left->AVL = 1; T->Right->AVL = 0; }
				else { T->Left->AVL = 0; T->Right->AVL = 0; }
				T->AVL = 0;
			}
		}
	}
	return T;
}

void PrintTree(pNode T, int Depth) {
	for (int i = 0; i < Depth; i++) printf("    ");
	if (!T)
		return;
	printf("%d:%d\n", T->Value, T->AVL);
	PrintTree(T->Left, Depth + 1);
	PrintTree(T->Right, Depth + 1);
}


int main() {
	int N, Value;
	pNode T = 0;
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &Value);
		UpdateFlag = 1;
		T = Insertion(T, Value);
	}
	printf("Tree:\n");
	PrintTree(T, 0);
}