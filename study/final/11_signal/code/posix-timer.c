#include <signal.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define SIGTIMER (SIGRTMAX)
#define ONESHOTTIMER (SIGRTMAX-1)

timer_t SetTimer(int signo, int sec, int mode);
void SignalHandler(int signo, siginfo_t * info, void *context);
timer_t timerid,oneshotTimer;

int main()
{
	struct sigaction sigact;

	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = SignalHandler;
	// Set up sigaction to catch 11_signal
	if (sigaction(SIGTIMER, &sigact, NULL) == -1) {
		perror("sigaction failed"); return -1;
	}
	// Set up sigaction to catch 11_signal
	if (sigaction(ONESHOTTIMER, &sigact, NULL) == -1) {
		perror("sigaction failed"); return -1;
	}
	// Establish a handler to catch CTRL+C and use it for exiting
	sigaction(SIGINT, &sigact, NULL);
	timerid = SetTimer(SIGTIMER, 1000, 1);
	oneshotTimer = SetTimer(ONESHOTTIMER, 10000, 0);
	while(1);
	return 0;
}

timer_t SetTimer(int signo, int sec, int mode)
{
	struct sigevent sigev;
	timer_t timerid;
	struct itimerspec itval;
	struct itimerspec oitval;


	// Create the POSIX timer to generate signo
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = signo;
	sigev.sigev_value.sival_ptr = &timerid;
	if (timer_create(CLOCK_REALTIME, &sigev, &timerid) == 0) {
		itval.it_value.tv_sec = sec / 1000;
		itval.it_value.tv_nsec = (long)(sec % 1000) * (1000000L);
		if (mode == 1) {
			itval.it_interval.tv_sec = itval.it_value.tv_sec;
			itval.it_interval.tv_nsec = itval.it_value.tv_nsec;
		} else {
			itval.it_interval.tv_sec = 0;
			itval.it_interval.tv_nsec = 0;
		}
		if (timer_settime(timerid, 0, &itval, &oitval) != 0) {
			perror("time_settime error!");
		}
	} else {
		perror("timer_create error!");
		return (timer_t)-1;
	}
	return timerid;
}

void SignalHandler(int signo, siginfo_t * info, void *context)
{
	if (signo == SIGTIMER) {
		puts("Periodic timer");
	}
	else if (signo == ONESHOTTIMER) {
		puts("One-short timer");
	}
	else if (signo == SIGINT) {
		timer_delete(oneshotTimer);
		timer_delete(timerid);
		perror("Ctrl + C catched!\n");
		exit(1);
	}
}
