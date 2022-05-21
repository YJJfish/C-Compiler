int printf(char ptr, ...);
typedef struct{
    int a;
    char b;
    double c;
}abc;

void main(void)
{
    abc X;
    X.a = 1;
    X.b = 'b';
    X.c = 1.0;
    printf("%d\n",X.a);
    printf("%c\n",X.b);
    printf("%lf\n",X.c);
}