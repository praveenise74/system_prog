#include<stdio.h>
void main()
{
 system("rm log.txt");
 char cmd[20]="",cmd1[20]="";
 char res[20]="",res1[20]="";
 while(1)
 {
  printf("\n Enter the command \n hello>> ");
  scanf(" %[^\n]s",cmd);
  if(strcmp(cmd,"exit")==0)
   break;
  else
  {
   if(strcmp(cmd,"myscript")==0)
   {
    printf(" Myscript started, file is log.txt \n");
    system("echo -n \"Myscript started on \" >> log.txt ; date >> log.txt");
    while(1)
    {
     printf("\n Enter the command \n welcome$ ");
     scanf(" %[^\n]s",cmd1);
     sprintf(res,"echo %s >> log.txt",cmd1);
     system(res);
     if(strcmp(cmd1,"exit")==0)
     {
      printf(" Myscript done, file is log.txt \n");  
      system("echo -n \"Myscript done on \" >> log.txt;date >> log.txt");
      break;
     }
     else
     {
      system(cmd1);
      sprintf(res1,"%s >> log.txt",cmd1);
      system(res1);
     }
    }
   }
   else
    system(cmd);
  }
 }
}
     
     
    
 
