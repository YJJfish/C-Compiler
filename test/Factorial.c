int printf(char ptr, ...);
int scanf(char ptr, ...);

int Fact(int n);

int main(void){
	int n;
	scanf("%d", &n);
	printf("%d", Fact(n));
	return 0;
}

int Fact(int n){
	if (n)
		return n * Fact(n - 1);
	else
		return 1;
}