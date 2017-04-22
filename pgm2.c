#include<stdio.h>
#include<string.h>

void main()
{
 char cmds[100]="",wh[20];
 char *tok;
 int count=0;

 while(1)
 {
  count=0;
  printf(" >>> ");
  scanf(" %[^\n]s",cmds); 
  printf("%s",cmds);
  if(strcmp(cmds,"exit")==0)
   break;
  else
  {
   tok=strtok(cmds," \t");
   while(tok!=NULL)
   {
    if(*tok=='\"')
    {
     tok++;
     printf("%s",tok);
     strcpy(wh,tok); 
     do
     {     
      strcat(wh," ");
      tok = strtok(NULL," \t");
      
      strcat(wh,tok);
     }while(tok[strlen(tok)-1]!='\"');
     wh[strlen(wh)-1]='\0';
     printf("token[%d]: %s \n",count,wh);
     count++;
    }
    else
    {
     printf("token[%d]: %s \n",count,tok);
     count++;
    }
    tok=strtok(NULL," \t");
   }
  }
 }
}
