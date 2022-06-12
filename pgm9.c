#include<stdio.h>
#include<string.h>
#include<sys/time.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
 struct timeval tv;

 char str[20],f11[20],f12[20];
 char *f1,*f2;

 while(1)
 {  
  printf(">>>");
  scanf(" %[^\n]s",str);
   
  if(!strcmp(str,"rename"))
  { 
   printf("Source file");
   scanf("%s",f11);
   
   
   printf("Destination file");
   scanf("%s",f12);

   f1=f11;
   f2=f12;

   link(f1,f2);
   unlink(f1);
  }
  else if(!strcmp(str,"sleep"))
  {
   printf("system entered sleep mode for 4.5 \n");
   fflush(stdout);
   tv.tv_sec=4;
   tv.tv_usec=500000;
   select(0,NULL,NULL,NULL,&tv);
   printf("Exiting sleep mode\n");
  }
  else if(!strcmp(str,"exit"))
   exit(0);
  else
   system(str);
  }
  return 0;
}
  
