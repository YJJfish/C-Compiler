int printf(char ptr, ...);

void main(void)
{
    int i, j, k;
    //For语句
    for (i=0; i<2; i++)
        for(j=0; j<2; j++)
            for(k=0; k<2; k++)
                printf("%d %d %d\n", i, j, k);
}