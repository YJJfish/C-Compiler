int printf(char ptr, ...);
int ptr pointer;    //指针变量
int array(3) array(5) a;
int array(5) ptr b;    //数组指针
char ptr array(10) c;    //指针数组
typedef struct{
    char a;
}def;

typedef def ptr pdef;    //结构指针

void main(void)
{
    int x = 1;
    pdef y;
    pointer = &x;
    a[0][0] = 2;
    a[0][1] = 3;
    b = a;
    c[0] = "Hello";
    y->a = 'a';
    printf("%d\n",*pointer);
    printf("%d\n",b[0]);
    printf("%d\n",b[1]);
    printf("%s\n",c[0]);
    printf("%c\n",y->a);
}