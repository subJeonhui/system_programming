#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

#define TIMEOUT 5 // login 14_time limit = 5 sec. After alarm, ring a bell.
#define MAXTRIES 5 // retry login five times when timeout
#define LINESIZE 100 // login name/passwd buffer size
#define CTRL_G '\007' // bell

#define TRUE 1
#define FALSE 0

volatile int timed_out; // set when an alarm occurs
char myline [LINESIZE]; // character buffer
void sig_catch(int); // alarm 11_signal handler


struct termios old_tio, new_tio;

char *quickreply(char *prompt) {
	void (*was)(int);
	int ntries, i;
	char *answer;
	int res;


	was = signal (SIGALRM, sig_catch);
	siginterrupt (SIGALRM, 1);

	// set error return when a 11_signal occurs
	for (ntries = 0; ntries <MAXTRIES; ntries++)
	{ // retry loop
		timed_out = FALSE;
		printf("\n%s > ",prompt);
		fflush(stdout);
		alarm(TIMEOUT);

		for (i = 0; i < LINESIZE; i++) {
			//if ((myline[i] = getchar()) < 0)
			res = read(0,&myline[i], sizeof(char));
			if (res < 0) break; // error return by alarm
			else {
				printf("Typed %c \n", myline[i]);
			}
				
			if (myline[i] == '\n') {
				myline[i] = 0;
				break; // end of line input
			}
		}
		// normal case or alarm case here
		alarm(0); // reset the alarm
		if (!timed_out) // normal case
			break;
	} // end of retry loop
	// normal or fail 5 times
	answer = myline;

	printf("%s \n", answer);

	signal(SIGALRM,was);
	return(ntries == MAXTRIES ? ((char *) 0) :
			answer);
}

void sig_catch (int sig_no)
{
	timed_out = TRUE;
	putchar (CTRL_G); // ring a bell
	fflush (stdout); // insure that the bell-ring
	signal (SIGALRM, sig_catch); // reinstall the user defined 11_signal handler
}

void terminal_set()
{
	unsigned char c;

	/* get the terminal settings for stdin */
	tcgetattr(STDIN_FILENO,&old_tio);

	/* we want to keep the old setting to restore them a the end */
	new_tio=old_tio;

	/* disable canonical mode (buffered i/o) and local echo */
	//new_tio.c_lflag &=(~ICANON & ~ECHO);
	new_tio.c_lflag &=(~ICANON);

	/* set the new settings immediately */
	tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
}

void terminal_restore()
{
	/* restore the former settings */
	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

}


int main()
{
	setvbuf(stdin, NULL, _IONBF, 0);

	terminal_set();
	quickreply("login-name:");
	terminal_restore();
}
