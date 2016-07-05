#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> /* For O_* constants */
#include <stdlib.h>
#include <string>
#include <iostream>
#include <string.h> //memcpy

void error_and_die(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
using namespace std;
int main(int argc, char const *argv[])
{
    const int how_much = 300;
    // const size_t filesize = 5;
    std::string shm_file("test.fits");
    cout << "memorizing: "<< shm_file <<endl;
    cout << "for " << how_much << " seconds"<<endl;
    // int shm = shm_open(shm_file.c_str(), O_CREAT|O_RDONLY, 0666);
    int shm = shm_open(shm_file.c_str(), O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
    if (shm==-1) error_and_die("Aprendo lo shared segment");

    // get the file size
    int f = open(shm_file.c_str(),O_RDONLY);    
    const size_t filesize = lseek(f, 0, SEEK_END);
    lseek(f, 0, SEEK_SET);
    
    // allocate
    void *buf = malloc(filesize);
    read(f, buf, filesize);
    close(f);

    // truncate shared memory to file size
    ftruncate(shm, filesize);
    printf("Opened Shared segment with handle %i of size %lu\n", shm, filesize );
 
    // mmap shared memory
    void* res=mmap(NULL,filesize,PROT_WRITE | PROT_READ,MAP_SHARED,shm,0);
    if (res == MAP_FAILED)
        error_and_die("mmap");

    // copy to res pointer
    memcpy(res, buf, filesize);
    
    // free memory
    if (buf) free(buf);

    //show res
    printf("pointer res is: %p\n",res);
    printf("Content of res is\n");
    for (int i=0; i<60; ++i)
    {
        printf("%c",*(static_cast<char*>(res)));
        res = static_cast<char*>(res) + 1;
    }

    printf("...\n");
    
    sleep(how_much);
    
    // TODO getch

    munmap(res, filesize);
    shm_unlink(shm_file.c_str());
    return 0;
}