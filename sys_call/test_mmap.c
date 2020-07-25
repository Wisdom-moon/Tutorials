#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char **argv) {

  int prot = PROT_READ|PROT_WRITE;
  int flags = MAP_SHARED;

  int fd = open(argv[1], O_RDWR);

  void * mapaddr1 = mmap(NULL, 8, prot, flags, fd, 0);
  void * mapaddr2 = mmap(NULL, 8, prot, flags, fd, 0);
  void * mapaddr3 = mmap(NULL, 8, prot, flags, fd, 0);

  if (mapaddr1 == MAP_FAILED) {
    perror ("mmap error");
    return -1;
  }

  *(long *) mapaddr1 = 0x1341;
//  *(long *) mapaddr2 = 0x2222;
//  *(long *) mapaddr3 = 0x3333;

  printf("%lx\n", *(long *) mapaddr2);
  printf("%lx\n", *(long *) mapaddr3);

  printf("%lx\n",  mapaddr2);
  printf("%lx\n",  mapaddr3);

  close(fd);

  munmap(mapaddr1, 8);
  munmap(mapaddr2, 8);
  munmap(mapaddr3, 8);

  printf("%lx\n",  mapaddr2);
  printf("%lx\n",  mapaddr3);
  
  return 0;
}
