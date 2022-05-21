int printf(char ptr, ...);

typedef union{
    int n;
    char ch;
    long m;
}DATA;

void main(void)
{
    DATA data;
    data.n = 97;
    printf("%d %c %ld\n", data.n, data.ch, data.m);
}