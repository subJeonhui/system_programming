#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

# define NUM_RECORDS 100

struct record {
	char	name[20];	// account owner
	int		id;	// account number
	int		balance;
};

void get_new_record(struct record *curr);
void display_record(struct record *curr);

int main( int argc, char *argv[])
{
	FILE *fp; struct record current;
	int record_no;
	int fd;
	long pos;
	char yes;

	fd = open( argv[1], O_RDWR | O_CREAT , S_IRUSR | S_IWUSR);

	if(( fp = fdopen( fd, "r+")) == NULL)
	{	perror( argv[1]);
		exit(2);
	}
	printf("enter record number: ");
	scanf("%d",&record_no); 
	getchar();

	while (record_no >= 0 && record_no < NUM_RECORDS) {
		pos = record_no * sizeof(struct record);
		fseek( fp, pos, SEEK_SET);
		fread(&current, sizeof(struct record), 1, fp);
		display_record (&current);
		printf("update records? yes = y\n");
		scanf("%c", &yes); getchar();
		if (yes == 'y') {
			get_new_record(&current);
			fseek( fp, pos, SEEK_SET);
			fwrite(&current, sizeof(struct record), 1, fp);
			printf("update done\n");
		}
		printf("enter record number: ");
		scanf("%d",&record_no); 
		getchar();
	}
	fclose(fp);
}


void get_new_record(struct record *curr)
{
	printf("> id? ");
	scanf("%d", &curr->id);
	printf("> name? ");
	scanf("%s", curr->name);
	printf("> balance? ");
	scanf("%d", &curr->balance);
}

void display_record(struct record *curr)
{
	printf("\n");
	printf("id: %d \n", curr->id);
	printf("name: %s \n", curr->name);
	printf("balance: %d \n", curr->balance);
	printf("\n");
}
