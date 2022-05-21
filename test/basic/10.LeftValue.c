int printf(char ptr, ...);
void ptr malloc(long);

typedef struct{
    int a;
}B;

typedef B ptr C;

void main(void)
{
    int array(5) a;
    a[0] = 1;   //Subscript类
    B b;
    C c = (C)malloc(sizeof(C));
    b.a = 2;    //StructReference类
    c->a = 3;   //StructDeference类
    int d = 4;
    int ptr e = &d;
    *e = 5; //Indirection类
    ++a[0]; //PrefixInc类
    --a[0]; //PrefixDec类
    ((a[1] = 1) += 2) %= 3;    //DirectAssign类
    
    printf("%d\n", a[0]);
    printf("%d\n", a[1]);     
    printf("%d\n", b.a); 
    printf("%d\n", c->a);
    printf("%d\n", d);
}