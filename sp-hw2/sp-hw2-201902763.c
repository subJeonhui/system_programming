//컴퓨터전자시스템공학부 201902763 이전희
//정상동작
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

struct account {
    char acc_no[6];
    char name[10];
    int balance;
};

int find_account(int fd, char *acc_no);

struct account inquiry(int fd, int acc_no);

struct account deposit(int fd, int acc_no, int amount);

struct account withdraw(int fd, int acc_no, int amount);

int acclock(int fd, int recno, int type);

void child(int accountData, long sleepTime) {
    FILE *opData = fopen("./operation.dat", "r");
    char buf[255];

    char *acc_no;
    char optype;
    int amount;

    int acc_idx;
    int pid = getpid();
    struct account res;

    while (fgets(buf, sizeof(buf), opData)) {
        acc_no = strtok(buf, "\t");
        optype = strtok(NULL, "\t")[0];

        if ((acc_idx = find_account(accountData, acc_no)) == -1) {
            printf("\"%s\" Account does not exist. \n", acc_no);
            continue;
        }

        switch (optype) {
            case 'i':
                res = inquiry(accountData, acc_idx);
                printf("pid: %d acc_no: %s %-16s balance: %d\n", pid, res.acc_no, "inquiry", res.balance);
                fflush(stdout);
                break;
            case 'd':
                amount = atoi(strtok(NULL, ""));
                res = deposit(accountData, acc_idx, amount);
                printf("pid: %d acc_no: %s %-9s %-6d balance: %d\n", pid, res.acc_no, "deposit:", amount, res.balance);
                fflush(stdout);
                break;
            case 'w':
                amount = atoi(strtok(NULL, ""));
                res = withdraw(accountData, acc_idx, amount);
                printf("pid: %d acc_no: %s %-9s %-6d balance: %d\n", pid, res.acc_no, "withdraw:", amount,
                       res.balance);
                fflush(stdout);
                break;
            default:
                break;
        }

    }
    fclose(opData);
    usleep(sleepTime);
    exit(0);
}

int main(int argc, char *argv[]) {
    FILE *fp = fopen("./account.dat", "rb+");
    struct account acc;
    int pid[10] = {0,};
    long sleepTime;
    for (int i = 0; i < 10; i++) {
        sleepTime = random() % 1000001;
        if ((pid[i] = fork()) == 0) {
            child(fileno(fp), sleepTime);
        }
    }

    for (int i = 0; i < 10; i++) {
        waitpid(pid[i], NULL, 0);
    }

    lseek(fileno(fp), 0, SEEK_SET);
    while (fread(&acc, sizeof(acc), 1, fp)) {
        printf("acc_no: %s balance: %d\n", acc.acc_no, acc.balance);
    }

    fclose(fp);
    return 0;
}

int acclock(int fd, int recno, int type) {
    struct flock fl;

    switch (type) {
        case F_RDLCK:
        case F_WRLCK:
        case F_UNLCK:
            fl.l_type = type;
            fl.l_whence = SEEK_SET;
            fl.l_start = recno * sizeof(struct account);
            fl.l_len = sizeof(struct account);
            fcntl(fd, F_SETLKW, &fl);
            return 1;
        default:
            return -1;
    }
}

int find_account(int fd, char *acc_no) {
    struct account acc;
    int n = 0;
    while (pread(fd, &acc, sizeof(acc), n * sizeof(struct account))) {
        if (strcmp(acc.acc_no, acc_no) == 0) {
            return n;
        }
        n += 1;
    }
    return -1;
}

struct account inquiry(int fd, int acc_no) {
    int pos;
    struct account acc;
    acclock(fd, acc_no, F_RDLCK);
    pos = acc_no * sizeof(struct account);
    int n = pread(fd, &acc, sizeof(struct account), pos);
    acclock(fd, acc_no, F_UNLCK);
    return acc;
}

struct account deposit(int fd, int acc_no, int amount) {
    int pos;
    struct account acc;
    acclock(fd, acc_no, F_WRLCK);
    pos = acc_no * sizeof(struct account);
    lseek(fd, pos, SEEK_SET);
    int n = pread(fd, &acc, sizeof(struct account), pos);
    acc.balance += amount;
    lseek(fd, pos, SEEK_SET);
    pwrite(fd, &acc, sizeof(struct account), pos);
    acclock(fd, acc_no, F_UNLCK);
    return acc;
}

struct account withdraw(int fd, int acc_no, int amount) {
    int pos;
    struct account acc;
    acclock(fd, acc_no, F_WRLCK);
    pos = acc_no * sizeof(struct account);
    int n = pread(fd, &acc, sizeof(struct account), pos);
    acc.balance -= amount;
    pwrite(fd, &acc, sizeof(struct account), pos);
    acclock(fd, acc_no, F_UNLCK);
    return acc;
}
