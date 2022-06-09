/* File : myProg.c */
#include <stdio.h>

struct book{
	int page;
	int price;
} c_prog = {300, 10000};
int global_value = 0;
char *color[3] = {"blue", "white", "red" };

func1(int a)
{
	return a % 2;
}

int main()
{
	int i, local_value = 10;
	char *local_str = "c programming";
	struct book *ptr = &c_prog;

	for(i = 0; i < 4; i++) {
		printf("color[i]=%s\n", color[i] );
		if( func1(i) ) {
			local_value = local_value / global_value;
			printf("local_value = %d\n",
					local_value);
		}
	}
	return 0;
}
