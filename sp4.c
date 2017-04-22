#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

void parse(char *line, char **argv)
{ 
 while(*line!='\0')
 {
  while(*line==' ' || *line=='\t' || *line=='\n')
      *line++='\0';
  *argv++ = line;
  
  while(*line!='\0' && *line!=' ' && *line!='\t' && *line!='\n')
       line++;
 }
 *argv='\0';
}

void execute(char **argv)
{
 pid_t pid;
 pid_t wpid;
 int status;

 printf("Forking a child.. \n");
 pid=fork();
 printf("common line procees id= %d \n",pid);
 if(pid<0)
  printf("Error Forking a child.. \n");
 else 
  if(pid==0)
  {
   printf("Executing the command %s in child \n",argv[0]);
   if((execvp(argv[0],argv))==-1)
   {
    perror("Error: exec failed");
    exit(1);
   }
  }
  else
  {
   do
   {
    printf("In parent.. waiting for child for the execution \n");
    wpid=waitpid(pid,&status,WUNTRACED);
   }while(!WIFEXITED(status)&&!WIFSIGNALED(status));
  }  
}

void main()
{
 char line[1024]="";
 char *argv[64];
 while(1)
 {
  printf(">>>");
  //gets(line);
  scanf(" %[^\n]s",line);
  parse(line,argv);
  if(strcmp(argv[0],"exit")==0)
   exit(0);
  execute(argv);
 }
}  
  

