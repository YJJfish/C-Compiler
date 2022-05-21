int printf(char ptr, ...);

void main(void)
{
    char a = 'a';
    double b = 1.0;
    printf("%d\n", a);    //类型升级
    printf("%d\n", (int)b);    //强制类型转换
}