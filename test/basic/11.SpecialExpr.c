int printf(char ptr, ...);

void main(void)
{
    int a = 5;
    int b = 6;
    bool c = (a != b);
    //TernaryCondition类
    a = (c ? 1 : b); //右值
    (!c ? a : b) = 2;//左值

    //CommaExpr类
    int d = (a, b, c, 3);   //右值
    int e;
    (a, b, c, d, e) = 4;    //左值
    
    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", d);  
    printf("%d\n", e);   
}