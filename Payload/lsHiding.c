#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "fileops.h"

/*
* must use the following format:
* ./lshiding <input> <output>
*/

#define MAX_FILENAME_LEN 256

size_t hideme(char *original_file, char *hidden_filename, char *thebintohide, size_t numbytes);
void restoreme(char *hidden_filename, char *original_file, char *bin, size_t numbytes);

void usage(void)
{
    fprintf(stdout, "Usage: ./lsHiding <file-to-hide> <file-to-restore>\n");
    fprintf(stdout, "Example: ./lsHiding myFile.txt restoredfile.txt\n");
    fprintf(stdout, "Options:\n");
    fprintf(stdout, "  -h  Display this help message.\n");
}

void error_usage_and_exit(const char *prog_name)
{
    fprintf(stderr, "Error: Incorrect number of arguments.\n");
    fprintf(stderr, "Usage: %s <file-to-hide> <file-to-restore>\n", prog_name);
    fprintf(stderr, "Try '%s -h' for more information.\n", prog_name);
    exit(EXIT_FAILURE);
}


int verify_existence(const char *filename, const char *status)
{
    struct stat buffer;
    printf("%s for '%s': ", status, filename);
    if (stat(filename, &buffer) == 0) {
        printf("EXISTS (size: %ld bytes)\n", (long)buffer.st_size);
        return 1;
    } else {
        printf("DOES NOT EXIST\n");
        return 0;
    }
}

size_t hideme(char *original_file, char *hidden_filename, char *thebintohide, size_t numbytes)
{
    int fd_write = open(hidden_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
    if (fd_write == -1) {
        perror("Error creating new file");
        return 0;
    }
    ssize_t written_bytes = write(fd_write, thebintohide, numbytes);
    if (written_bytes != (ssize_t)numbytes) {
        perror("Error writing hidden content to new file");
        close(fd_write);
        return 0;
    }
    close(fd_write);
    if (unlink(original_file) != 0) {
        perror("Error deleting the original file");
    } else {
        printf("Hidden content successfully saved to '%s'. Original file '%s' deleted.\n", hidden_filename, original_file);
    }
    
    return written_bytes;
}

void restoreme(char *hidden_filename, char *original_file, char *bin, size_t numbytes){
    int fd_read = open(hidden_filename, O_RDONLY);
    if (fd_read == -1) {
        perror("Error opening hidden file for reading during restore");
        return;
    }
    
    ssize_t bytes_read = read(fd_read, bin, numbytes);
    if (bytes_read != (ssize_t)numbytes) {
        perror("Error reading content from hidden file");
        close(fd_read);
        return;
    }
    close(fd_read);
    int fd_write = open(original_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_write == -1) {
        perror("Error creating original file during restore");
        return;
    }
    
    write(fd_write, bin, numbytes);
    close(fd_write);
    if (unlink(hidden_filename) != 0) {
         perror("Error unlinking/deleting the hidden file");
    } else {
        printf("Content successfully restored to '%s'. Hidden file '%s' deleted.\n", original_file, hidden_filename);
    }
}

int main(int argc, char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "-h") == 0)) {
        usage();
        return EXIT_SUCCESS;
    }

    if (argc != 3) {
        error_usage_and_exit(argv[0]);
    }

    char *file_to_hide = argv[1];      
    char *hidden_file = argv[2];       

    printf("\nHIDING THE FILE\n");
    
    verify_existence(file_to_hide, "Original file status");

    struct stat filestats;
    if (stat(file_to_hide, &filestats) != 0) {
        perror("Error stating file to hide. Does it exist?");
        return EXIT_FAILURE;
    }
    size_t filesize = filestats.st_size;
    
    char *file_content = (char *)malloc(filesize);
    if (file_content == NULL) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }

    int fd_read = open(file_to_hide, O_RDONLY);
    if (fd_read == -1) {
        perror("Error opening file to hide for reading");
        free(file_content);
        return EXIT_FAILURE;
    }
    if (read(fd_read, file_content, filesize) != (ssize_t)filesize) {
        perror("Error reading file content");
        close(fd_read);
        free(file_content);
        return EXIT_FAILURE;
    }
    close(fd_read);
    printf("File '%s' read successfully. Size: %zu bytes.\n", file_to_hide, filesize);

    printf("\nHiding the file\n");
    size_t hidden_bytes = hideme(file_to_hide, hidden_file, file_content, filesize);
    if (hidden_bytes == 0) {
        free(file_content);
        return EXIT_FAILURE;
    }

    printf("\nChecking if file hid correctly.\n");
    verify_existence(file_to_hide, "Original file status");
    verify_existence(hidden_file, "Hidden file status");
    

    free(file_content);

    return 0;
}