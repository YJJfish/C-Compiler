/*
Multiply two 3x3 matrices.
*/


int printf(char ptr, ...);
int scanf(char ptr, ...);
void ptr malloc(long);
void free(void ptr);

typedef float array(3) array(3) Matrix3f;
typedef double array(3) array(3) Matrix3d;
typedef float array(4) array(4) Matrix4f;
typedef double array(4) array(4) Matrix4d;

void ScanMatrix3d(Matrix3d In){
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			scanf("%lf", &In[i][j]);
}

void PrintMatrix3d(Matrix3d Out){
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++)
			printf("%g ", Out[i][j]);
		printf("\n");
	}
}

void Matrix3dMul(Matrix3d Left, Matrix3d Right, Matrix3d Out){
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++){
			double Sum = 0.0;
			for (int k = 0; k < 3; k++)
				Sum += Left[i][k] * Right[k][j];
			Out[i][j] = Sum;
		}
}

int main(void){
	Matrix3d M1, M2, M3;
	printf("Input a 3x3 matrix M1:\n");
	ScanMatrix3d(M1);
	printf("Input a 3x3 matrix M2:\n");
	ScanMatrix3d(M2);
	Matrix3dMul(M1, M2, M3);
	printf("M1 * M2:\n");
	PrintMatrix3d(M3);
}