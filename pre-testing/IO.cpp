#include <iostream>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  short fd1;
  short fd2;
  int r;
  char* ch = new char[12];
  char* ch2 = new char[12];
  fd1 = open("t2.txt", O_RDONLY);
  fd2 = open("test1.txt", O_RDONLY);
  std::cout<<read(fd1, ch, 10)<<std::endl;
  std::cout<<read(fd2, ch2, 10)<<std::endl;
  std::cout<<ch<<" "<<ch2<<std::endl;
  close(fd1);
  close(fd2);
  std::cout<<read(0, ch, 1)<<std::endl;
  std::cout<<ch<<std::endl;
  delete[] ch;
  delete[] ch2;
}