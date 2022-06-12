#include<stdio.h>
#include<unistd.h>
//#include<errno.h>
#include<signal.h>

void wakeup()
{
 printf("Hello\n");
 printf("-----------\n");
}

int main()
{
 int i,j;
 struct sigaction action;
 action.sa_handler=(void(*)(int))wakeup;
 action.sa_flags=SA_RESTART;
 sigemptyset(&action.sa_mask);
 if(sigaction(SIGALRM,&action,0)==-1)
  perror("sigaction");
 
 while(1)
 {
  printf("Waiting for alarm\n");
  alarm(2);
  pause();
  printf("\n");
 }
 return 0;
}
