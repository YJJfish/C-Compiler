/*

Write a nonrecursive procedure to reverse a singly linked list in O(N) time using constant extra space.

The function Reverse is supposed to return the reverse linked list of L, with a dummy header.

Sample Input:
5
1 3 4 5 2

Sample Output:
2 5 4 3 1
2 5 4 3 1

*/
int printf(char ptr, ...);
int scanf(char ptr, ...);
void ptr malloc(long);
void free(void ptr);

typedef int ElementType;
typedef struct {
    ElementType Element;
    Node ptr Next;
} Node;
typedef Node ptr PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

List Read();
void Print(List L);
List Reverse(List L);

int main(){
    List L1, L2;
    L1 = Read();
    L2 = Reverse(L1);
    Print(L1);
    Print(L2);
    return 0;
}

List Read(){
	int N;
	scanf("%d", &N);
	if (N == 0)
		return 0;
	//Create a dummy header
	List L = (List)malloc(sizeof(Node));
	L->Next = 0;
	//Read the input data
	Position pListNode = L;
	for (int i = 0; i < N; i++){
		pListNode = pListNode->Next = (List)malloc(sizeof(Node));
		pListNode->Next = 0;
		scanf("%d", &pListNode->Element);
	}
	//Return
	return L;
}

List Reverse(List L) {
	Position p, q, r;
	if (L == 0 || L->Next == 0) return L;
	p = L->Next;
	q = L->Next->Next;
	p->Next = 0;
	while (q) {
		r = q->Next;
		q->Next = p;
		p = q;
		q = r;
	}
	L->Next = p;
	return L;
}

void Print(List L){
	L = L->Next;
	while (L){
		printf("%d ", L->Element);
		L = L->Next;
	}
	printf("\n");
}