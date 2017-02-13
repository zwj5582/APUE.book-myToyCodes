#include <stdio.h>
 2 #include <stdlib.h>
 3 #include <fcntl.h>
 4 #include <sys/types.h>
 5 #include <unistd.h>
 6 #include <sys/types.h>
 7
 8 int main(void)
 9 {
10         int fd, pid, status;
11         char buf[10];
12         if ((fd = open("./test.txt", O_RDONLY)) < 0) {
13                 perror("open");  exit(-1);
14         }
15         if ((pid = fork()) < 0) {
16                 perror("fork");  exit(-1);
17         } else if (pid == 0) {  //child
18                 read(fd, buf, 2);
19                 write(STDOUT_FILENO, buf, 2);
20         } else {  //parent
21                 sleep(2);
23                 lseek(fd, SEEK_CUR, 1);
24                 read(fd, buf, 3);
25                 write(STDOUT_FILENO, buf, 3);
26                 write(STDOUT_FILENO, "\n", 1);
27         }
28         return 0;
29 }
