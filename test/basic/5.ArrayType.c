int printf(char ptr, ...);
int array(5) a;    //一维数组
double array(10) array(10) b;    //二维数组

void main(void)
{
    a[0] = 1;
    b[1][1] = 2.0;
    printf("%d\n",a[0]);
    printf("%lf\n",b[1][1]);
}