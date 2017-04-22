#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc,char* argv[])
{
 int i=1,len;
 int fd[5];
 char buf[256];

 if(argc==1)
 {
  gets(buf);
  puts(buf);
 }

 while(--argc>0)
 {
  fd[i]=open(argv[i],O_RDONLY,777);
  len=read(fd[i],buf,256);
  write(1,buf,len);
  close(fd[i]);
 }
 return 0;
}
