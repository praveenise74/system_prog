#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

struct my_msgbuf {
        long mtype;
        int mnum;
};

int initializeMessageQueue()
{
        key_t key;


        int msgqid;
        if ((msgqid = msgget(key, 0666 | IPC_CREAT)) == -1) {
                perror("msgget");
                exit(1);
        }
        return msgqid;
}

void killMessageQueue(int msgqid)
{
        if (msgctl(msgqid, IPC_RMID, NULL) == -1) {
                perror("msgctl");
                exit(1);
  }
}

void int_send(int num, int msgqid, int type)
{
        printf("Converting num %d to string \n", num);

        struct my_msgbuf buf;

        buf.mtype = type;
        buf.mnum = num;

        size_t size = sizeof(int);

        if (msgsnd(msgqid, &buf, size, 0) == -1)
                perror("msgsnd");

}

int int_recv(int msgqid, int type)
{
        struct my_msgbuf buf;

        if (msgrcv(msgqid, &buf, sizeof (int), type, 0) == -1) {
                perror("msgrcv");
                exit(1);
        }

        printf("num retrieved from queue: \"%d\"\n", buf.mnum);
        return buf.mnum;
}
int main(void)
{
        int msgqid = initializeMessageQueue();
        printf("Created a queue with msqid: %d\n", msgqid);

        int status;
        pid_t pid, wpid;

        pid = fork();

        switch (pid) {
                case -1:
                        printf("fork failed");
                        exit(-1);

                case 0:

                        int_recv(msgqid, getpid());

                        sleep(5);

                        exit(0);

                default:

                        do {

                                printf("Parent: pid=%d ", getpid());
                                printf("my child is %d \n", pid);

                                int num;
                                printf("Enter a number :");

                                 scanf("%d",&num);

                                int_send(num, msgqid, pid);

                                wpid = waitpid(pid, &status, WUNTRACED);
                        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

                        printf("\nExiting ... killing message queue \n");
                        killMessageQueue(msgqid);
        }
}
