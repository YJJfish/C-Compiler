int printf(char ptr, ...);

void main(void)
{
    int i,sum = 0;
    i = 1;
    //Do-While语句
    do{
        sum = sum + i;
        i++;
    }
    while(i <= 100);
    printf("%d\n", sum);
}