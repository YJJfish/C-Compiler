int printf(char ptr, ...);
typedef enum
{
      MON=1, TUE, WED=3, THU, FRI, SAT, SUN
}DAY;

void main(void)
{
    DAY day = WED;
    printf("%d\n", day);
    int dday = SUN;
	printf("%d\n", dday);
}