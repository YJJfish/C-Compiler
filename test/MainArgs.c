int printf(char ptr, ...);

int main(int argc, char ptr array() argv) {
	printf("argc = %d\n", argc);
	for (int i = 0; i <= argc; i++){
		printf("argv[%d] = \"%s\"\n", i, argv[i]);
	}
}