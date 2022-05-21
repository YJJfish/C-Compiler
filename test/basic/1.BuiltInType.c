int printf(char ptr, ...);
bool a = true;
short b = 12;
int c = -2;
long d = 123456789;
char e = 'a';
float f = 1.0;
double g =2.1;
void main(void)
{
    printf("%d\n",a);
    printf("%hd\n",b);
    printf("%d\n",c);
    printf("%ld\n",d);
    printf("%c\n",e);
    printf("%f\n",f);
    printf("%lf\n",g);
}