/*
In this project, you are supposed to implement a B+ tree of order 3, with the following operations: initialize, insert (with splitting) and search. The B+ tree should be able to print out itself.

Input Specification:
Each input file contains one test case. For each case, the first line contains a positive number N (<=10^4), the number of integer keys to be inserted. Then a line of the N positive integer keys follows. All the numbers in a line are separated by spaces.

Output Specification:
For each test case, insert the keys into an initially empty B+ tree of order 3 according to the given order. Print in a line Key X is duplicated where X already exists when being inserted. After all the insertions, print out the B+ tree in a top-down lever-order format as shown by the samples.

Sample Input 1:
6
7 8 9 10 7 4

Sample Output 1:
Key 7 is duplicated
[9]
[4,7,8][9,10]

Sample Input 2:
10
3 1 4 5 9 2 6 8 7 0

Sample Output 2:
[6]
[2,4][8]
[0,1][2,3][4,5][6,7][8,9]

Sample Input 3:
3
1 2 3

Sample Output 3:
[1,2,3]

*/

int printf(char ptr, ...);
int scanf(char ptr, ...);
void ptr malloc(long);
void free(void ptr);

typedef struct {
	int array(4) v;
	int sum;
	int IsLeaf;
	Node ptr array(4) child;
} Node;

typedef Node ptr pNode;

bool AddValue(pNode NodePtr, int value) {
	//return true if successful, false if duplicated
	int i;
	for (i = 0; i < NodePtr->sum; i++)
		if (NodePtr->v[i] == value)
			return false;
	for (i = NodePtr->sum - 1; i >= 0; i--)
		if (NodePtr->v[i] > value) NodePtr->v[i + 1] = NodePtr->v[i];
		else break;
	NodePtr->v[i + 1] = value;
	NodePtr->sum++;
	return true;
}

pNode NewNode(int IsLeaf) {
	pNode NodePtr;
	NodePtr = (pNode)malloc(sizeof(Node));
	NodePtr->IsLeaf = IsLeaf;
	NodePtr->sum = 0;
	return NodePtr;
}

pNode Tree;

bool Insert(pNode F, pNode T, int v) {
	//return true if successful, false if duplicated 
	int flag, i;
	if (T->IsLeaf) {//Leaf Node
		if (!AddValue(T, v)) return 0;
		if (T->sum == 4) {
			if (F) {//T is not the root
				for (i = F->sum++; F->child[i - 1] != T; F->child[i] = F->child[i - 1], F->v[i - 1] = F->v[i - 2], i--);//Shift
				F->v[i - 1] = T->v[2];	//Divide value = min(T->v[2],T->v[3])
				F->child[i] = NewNode(1); F->child[i]->sum = 2; F->child[i]->v[0] = T->v[2]; F->child[i]->v[1] = T->v[3];
				T->sum = 2;
			}
			else {//T is the root
				Tree = NewNode(0); Tree->sum = 2; Tree->v[0] = T->v[2];
				Tree->child[0] = T; T->sum = 2;
				Tree->child[1] = NewNode(1); Tree->child[1]->sum = 2; Tree->child[1]->v[0] = T->v[2]; Tree->child[1]->v[1] = T->v[3];
			}
		}
	}
	else {//Nonleaf node
		for (i = T->sum - 2; i >= 0 && T->v[i] > v; i--);//v[0] divides child[0] / child[1], v[1] divides child[1] / child[2]
		if (!Insert(T, T->child[i + 1], v)) return 0;//Recursively insertion
		if (T->sum == 4) {
			if (F) {//T is not the root
				for (i = F->sum++; F->child[i - 1] != T; F->child[i] = F->child[i - 1], F->v[i - 1] = F->v[i - 2], i--);//Shift
				F->v[i - 1] = T->v[1];
				F->child[i] = NewNode(0); F->child[i]->sum = 2; F->child[i]->child[0] = T->child[2]; F->child[i]->child[1] = T->child[3]; F->child[i]->v[0] = T->v[2];
				T->sum = 2;
			}
			else {//T is the root
				Tree = NewNode(0); Tree->sum = 2; Tree->v[0] = T->v[1];
				Tree->child[0] = T; T->sum = 2;
				Tree->child[1] = NewNode(0); Tree->child[1]->sum = 2; Tree->child[1]->v[0] = T->v[2]; Tree->child[1]->child[0] = T->child[2]; Tree->child[1]->child[1] = T->child[3];
			}
		}
	}
	return 1;
}

void PrintTree(pNode T) {
	pNode ptr q = malloc(sizeof(pNode) * 10000);
	int ptr level = malloc(sizeof(int) * 10000);
	int f, r, i, lastlevel = 0;
	f = 0; r = 1; q[0] = T; level[0] = 0;
	while (f < r) {
		T = q[f];
		if (level[f] > lastlevel) printf("\n");
		lastlevel = level[f];
		f++;
		printf("[%d", T->v[0]);
		for (i = 1; i < ((T->IsLeaf) ? (T->sum) : (T->sum - 1)); i++) printf(",%d", T->v[i]);
		printf("]");
		if (T->IsLeaf == 0) for (i = 0; i < T->sum; i++, r++) {
			q[r] = T->child[i];
			level[r] = lastlevel + 1;
		}
	}
	free(q);
	free(level);
}

int main() {
	int n, v;
	Tree = NewNode(1);
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &v);
		if (!Insert(0, Tree, v)) printf("Key %d is duplicated\n", v);
	}
	PrintTree(Tree);
}