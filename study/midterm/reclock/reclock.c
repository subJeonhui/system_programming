#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TRUE	1
#define FALSE	0

#define NUM_RECORDS 100

struct record{
	int id; // account umber
	char name[20]; // account owner
	int balance; // balance
};

int reclock (int fd, int recno, int len, int type);
void display(struct record *curr);

int main()
{
	struct record current;
	int record_no;
	int fd, pos, i,n;
	char yes;
	char operation;
	int amount ;
	char buffer[100];
	int quit=FALSE;


	fd = open("./Account_File", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	while (!quit) {
		printf("enter account number (0~99): ");
		scanf("%d", &record_no);
		fgets(buffer, 100, stdin); // to remove the new line character in the stdin buffer

		if (record_no < 0 && record_no >= NUM_RECORDS) break;
		printf("enter operation name (c/r/d/q): ");
		scanf("%c", &operation);
		fgets(buffer, 100, stdin); // to remove the new line character in the stdin buffer

		switch (operation) {
			case 'c': // create
				reclock(fd, record_no, sizeof(struct record), F_WRLCK);
				pos = record_no * sizeof(struct record);
				lseek(fd, pos, SEEK_SET);
				printf("> id ? ");
				scanf("%d", &current.id);
				fgets(buffer, 100, stdin);	// to remove the new line character in the stdin buffer

				printf("> name ? ");
				fgets(current.name, sizeof(current.name), stdin);

				current.balance = 0 ;
				n = write(fd, &current, sizeof(struct record));
				display (&current);
				reclock(fd, record_no, sizeof(struct record), F_UNLCK);
				break;

			case 'r': // inquiry
				reclock(fd, record_no, sizeof(struct record), F_RDLCK);
				pos = record_no * sizeof(struct record);
				lseek(fd, pos, SEEK_SET);
				n = read(fd, &current, sizeof(struct record));
				display (&current);
				reclock(fd, record_no, sizeof(struct record), F_UNLCK);
				break;

			case 'd' : // deposit
				reclock(fd, record_no, sizeof(struct record), F_WRLCK);
				pos = record_no * sizeof(struct record);
				lseek(fd, pos, SEEK_SET);
				n = read(fd, &current, sizeof(struct record));
				display (&current);
				printf("enter amount: ");
				scanf("%d", &amount);
				fgets(buffer, 100, stdin);	// to remove the new line character in the stdin buffer

				current.balance += amount;
				lseek(fd, pos, SEEK_SET);
				write(fd, &current, sizeof(struct record));
				reclock(fd, record_no, sizeof(struct record), F_UNLCK);
				break;
			case 'q' :
				quit = TRUE ;
				break ;

			default :
				printf("illegal input\n");
				continue;
		};
	}
	fflush(NULL);
	close(fd);
}

int reclock (int fd, int recno, int len, int type)
{
	struct flock fl;

	switch (type) {
		case F_RDLCK: 
		case F_WRLCK:
		case F_UNLCK:
			fl.l_type = type;
			fl.l_whence = SEEK_SET;
			fl.l_start = recno * len;
			fl.l_len = len;
			fcntl (fd, F_SETLKW, &fl);
			return 1;
		default: 
			return -1;
	};
}


void display(struct record *curr)
{
	printf("\n");
	printf("id: %d \n", curr->id);
	printf("name: %s \n", curr->name);
	printf("balance: %d \n", curr->balance);
	printf("\n");
}
