int printf(char ptr, ...);

int add(int a, int b)
{
    return a + b;
}

void main(void)
{
    int a = -2;    //Constant类
    int b = 2;
    char ptr c = "Global String";    //GlobalString类
    int d = add(a, b);    //FunctionCall类
    double e = 3.0;
    int f = (int)e;    //TypeCast类
    int g = sizeof(a);    //Sizeof类
    a++;    //PostfixInc类
    b--;    //PostfixDec类
    int ptr h = &a;    //Addressof类
    
    //其他运算符，每类各选取几种代表的运算符
    //一元+，一元-
    int unaryPlus = +a;
    int unaryMinus = -b;
    //位运算
    int bitwiseOR = a | b;
    int bitwiseAnd = a & b;
    //逻辑运算
    int logicOR = a || b;
    int logicAnd = a && b;
    //比较运算
    int logicGT = (a > b);
    int logicLE = (a <= b);
    //算术运算
    int add = a + b;
    int sub = a - b;
    int mul = a * b;
    int div = a / b;
    int mod = a % b;
    //移位运算符
    int SHL = a << 1;
    int SHR = b >> 1;
    
    printf("%d\n", a);    
    printf("%d\n", b); 
    printf("%s\n", c);
    printf("%d\n", d);
    printf("%lf\n", e);
    printf("%d\n", f);
    printf("%d\n", g);
    printf("%d\n", unaryPlus);
    printf("%d\n", unaryMinus);
    printf("%d\n", bitwiseOR);
    printf("%d\n", bitwiseAnd);
    printf("%d\n", logicOR);
    printf("%d\n", logicAnd);
    printf("%d\n", logicOR);
    printf("%d\n", logicGT);
    printf("%d\n", logicLE);
    printf("%d\n", add);
    printf("%d\n", sub);
    printf("%d\n", mul);
    printf("%d\n", div);
    printf("%d\n", mod);
    printf("%d\n", SHL);
    printf("%d\n", SHR);
}