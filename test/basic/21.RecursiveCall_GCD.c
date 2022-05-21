int printf(char ptr, ...);
int scanf(char ptr, ...);

int GCD(int A, int B){
	if (A % B)
		return GCD(B, A % B);
	else
		return B;
}

int main(void){
	int A, B;
	scanf("%d%d", &A, &B);
	printf("%d\n", GCD(A, B));
	return 0;
}