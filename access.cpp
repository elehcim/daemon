#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> /* For O_* constants */
#include <iostream>
#include <stdlib.h> //EXIT_FAILURE

static void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
int main(int argc, char const *argv[])
{

    std::string shm_file("test.fits");

    int shm = shm_open(shm_file.c_str(), O_RDONLY, 0666);
    if (shm==-1) error_and_die("Aprendo lo shared segment");

    // char* res=static_cast<char*>(mmap(NULL,20000,PROT_READ,MAP_SHARED,shm,0));

    // get file_size
    const size_t filesize = lseek(shm, 0, SEEK_END);
    lseek(shm, 0, SEEK_SET);

    printf("Opened Shared segment with handle %i of size %lu\n", shm, filesize );

    void* res=mmap(NULL,filesize,PROT_READ,MAP_PRIVATE,shm,0);
    if (res == MAP_FAILED)
        error_and_die("mmap");

    printf("pointer res is: %p\n",res);

    for (int i=0; i<10; ++i)
    {
        printf("%c",*(static_cast<char*>(res)));
        res = static_cast<char*>(res) + 1;
    }
    printf("\n");

    munmap(res, filesize);
    shm_unlink(shm_file.c_str());
    return 0;
}