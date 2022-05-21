int printf(char ptr, ...);
void ptr malloc(long);

typedef struct {
	Node ptr Left, Right;    //自定义类型
	int Value, AVL;
} Node;

typedef Node ptr pNode;

void main(void)
{
    pNode T;    //自定义类型
    T = (pNode)malloc(sizeof(Node));
    T -> Value = 1;
    printf("%d\n",T -> Value);
}