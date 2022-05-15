/*
Quick sort. Input the number of elements N and N integers. Output the sorted numbers in non-descent order.

Sample Input:
5
10 8 5 3 7

Sample Output:
3 5 7 8 10
*/

int printf(char ptr, ...);
int scanf(char ptr, ...);
void ptr malloc(long);
void free(void ptr);


void QuickSort(int ptr Arr, int L, int R){
	int i = L,
		j = R,
		Mid = Arr[(i + j) / 2];
	while (i <= j){
		while (Arr[i] < Mid) i++;
		while (Mid < Arr[j]) j--;
		if (i <= j){
			int Tmp = Arr[i];
			Arr[i] = Arr[j];
			Arr[j] = Tmp;
			i++; j--;
		}
	}
	if (L < j) QuickSort(Arr, L, j);
	if (i < R) QuickSort(Arr, i, R);
}

int main(void){
	int N;
	int ptr Arr;
	printf("Input the number of elements:\n");
	scanf("%d", &N);
	Arr = malloc(N * sizeof(int));
	printf("Input %d integers:\n", N);
	for (int i = 0; i < N; i++)
		scanf("%d", &Arr[i]);
	QuickSort(Arr, 0, N);
	printf("QuickSort result:\n");
	for (int i = 0; i < N; i++)
		printf("%d ", Arr[i]);
	free(Arr);
	return 0;
}