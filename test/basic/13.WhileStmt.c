int printf(char ptr, ...);
int getchar(void);

void main(void)
{
    int n = 0;
    printf("input a string: ");
    //While语句
    while(getchar() != '\n') n++;
    printf("number of characters: %d\n",n);
}