#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
  int fd;
  int value = 0;
  printf("demo test\r\n");

  fd = open("/dev/demo_driver", O_RDWR);

  while (1) {
    read(fd, &value, 4);
    sleep(1);
    write(fd, &value, 4);
    sleep(1);

    printf("demo run\r\n");
  }
  
  return 0;
}
