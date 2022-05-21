int printf(char ptr, ...);

void main(void)
{
    int i = 0;
    while(1)
    {
        ++i;
        if(i % 2 == 0)
        {
            continue;	//Continue语句
        }
        if(i > 9)
        {
            break;	//Break语句
        }
        printf("%d ", i);
    }
    printf("\n");
}