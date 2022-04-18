// 컴퓨터전자시스템공학부 201902763 이전희
// 완성
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
	int fdC1[2];
	int fdC2[2];

	int pid1;
	int pid2;

	pipe(fdC1);
	pipe(fdC2);

	char buf[255];

	if((pid1 = fork()) == 0){
		int lineOdd = 0;
		int wordOdd = 0;
		close(fdC1[1]);
		close(fdC2[0]);
		close(fdC2[1]);

		FILE *c1rd = fdopen(fdC1[0], "r");
		FILE *c1Out = fopen("c1-out.txt", "w");

		while(fgets(buf, 255, c1rd) && !feof(c1rd)){
			lineOdd += 1;
			for(int i=0; i < strlen(buf); i++){
				if('a' <= buf[i] && buf[i] <= 'z')
					buf[i] -= 32;
				if(i != 0 && (buf[i-1] != ' ' && (buf[i] == ' ' || buf[i] == '\n')))
					wordOdd += 1;
			}
			fputs(buf, c1Out);
		}
		
		fprintf(c1Out, "line: %d	word: %d\n", lineOdd, wordOdd);

		close(fdC1[0]);
		fclose(c1Out);
		printf("Goodbye from C1\n");
		exit(0);
	}
	if(pid1 != 0 && (pid2 = fork()) == 0){
		int lineEven = 0;
		int wordEven = 0;
		close(fdC1[0]);
		close(fdC1[1]);
		close(fdC2[1]);

		FILE *c2rd = fdopen(fdC2[0],"r");
		FILE *c2Out = fopen("c2-out.txt","w");

		while(fgets(buf, 255, c2rd) != 0 && !feof(c2rd)){
			lineEven += 1;
			for(int i = 0; i < strlen(buf); i++){
				if('A' <= buf[i] && buf[i] <= 'Z')
					buf[i] += 32;
				if(i !=0 && (buf[i-1] != ' ' && (buf[i] == ' ' || buf[i] == '\n')))
					wordEven += 1;
			}
			fputs(buf, c2Out);
		}
		fprintf(c2Out, "line: %d	word: %d\n", lineEven, wordEven);

		close(fdC2[0]);
		fclose(c2Out);
		printf("Goodbye from C2\n");
		exit(0);
	}
	if(pid1 !=0 && pid2 != 0){
		int stat;
		int line = 0;

		FILE *inputFile = fopen(argv[1], "r");
		FILE *c1wr = fdopen(fdC1[1], "w");
		FILE *c2wr = fdopen(fdC2[1], "w");
		
		close(fdC1[0]);
		close(fdC2[0]);
		
		while(fgets(buf, 255, inputFile) != 0 && !feof(inputFile)){
			line += 1;
			if(line % 2 == 1){
				fputs(buf, c1wr);
				fflush(c1wr);
			}else{
				fputs(buf, c2wr);
				fflush(c2wr);
			}
		}	

		close(fdC1[1]);
		close(fdC2[0]);

		fclose(inputFile);
		fclose(c1wr);
		fclose(c2wr);

		waitpid(pid1, &stat, 0);
		waitpid(pid2, &stat, 0);
		printf("Goodbye from P\n");
	}
	

	return 0;
}

