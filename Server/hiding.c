// Payload hiding from ls

#include <stdio.h>
#include <unistd.h>     // for read(), write(), close()
#include <fcntl.h>      // for open(), O_RDONLY, O_WRONLY, etc.
#include <sys/types.h>  // for types like off_t, mode_t
#include <sys/stat.h>   // for struct stat, file permissions
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

size_t hideme(char *filename);
void restoreme(char *filename,  char *bin, size_t numbytes);

void main(int argc, char *argv[])
{
     int ppid = atoi(argv[1]);
     printf("SERVER's pid: %d\n", ppid);
    pid_t hbfork = fork();
    if(0 == hbfork)
    {
        while(1)
        {
            printf("sending sigusr1 kill to %d\n", ppid);
            kill(ppid, SIGUSR1);
            
        sleep(1);
        }
    }else{
        struct stat filestats; 
        size_t filesize = 0;

        printf("num bytes:");
        int fd = open("./test.txt", O_RDONLY);
        filesize = filestats.st_size; // .st_size is num of bytes if nextfile
        printf("num bytes: %ld\n", filesize);

        //1. Malloc memory
        
        char *buffer = (char *)malloc(sizeof(char)* filesize);

        //2. call hide me
        read(fd, buffer, filesize); // takes the data and redas it into the heap
        //3. call restore me and name it something different
        // reference the file descriptor  
        hideme("./test.txt");
        sleep(10);
        restoreme("./restored.txt", buffer, filesize);

    }

}

void restoreme(char *filename,  char *bin,  size_t numbytes)
{
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    write(fd, bin, numbytes);
    close(fd);

}

size_t hideme(char *filename)
{
    unlink(filename);
}


